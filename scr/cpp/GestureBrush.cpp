#include "../h/GestureBrush.h"
#include "../h/Board.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMouseEvent>

#include <cmath>
#include <iostream>

// ═══════════════════════════════════════════════════════════
GestureBrush::GestureBrush(
    const int width, const int height,
    const QColor bgColor, const QColor penColor,
    const double thickness, const double smoothAlpha, const double deadZone,
    QWidget* parent)
    : QWidget(parent)
    , m_bgColor(bgColor)
    , m_penColor(penColor)
    , m_thickness(thickness)
    , m_originalThickness(thickness)
    , m_smoothAlpha(smoothAlpha)
    , m_deadZone(deadZone)
{
    resize(width, height);

    setAttribute(Qt::WidgetAttribute::WA_TranslucentBackground, true);
    setAttribute(Qt::WidgetAttribute::WA_NoSystemBackground, true);
    setMouseTracking(true);

    m_standaloneMode = (parent == nullptr);
    loadPenSettings();

    // 加载图像
    m_eraserPixmap = QPixmap(QStringLiteral("../res/btn_image/eraser.png"));
    const qreal dpi_radio = devicePixelRatioF();
    const int eraser_w = static_cast<int>(m_eraserWidth * dpi_radio);
    const int eraser_h = static_cast<int>(m_eraserWidth * dpi_radio);
    m_eraserPixmap.setDevicePixelRatio(dpi_radio);
    m_eraserPixmap = m_eraserPixmap.scaled(eraser_w, eraser_h, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    m_eraserLabel = new QLabel(this);
    m_eraserLabel->setPixmap(m_eraserPixmap);
    m_eraserLabel->update();
    m_eraserLabel->hide();
}

// ═══════════════════════════════════════════════════════════
void GestureBrush::loadPenSettings()
{
    const QString path = QDir::currentPath() + "/../data/pen_settings.json";
    if (QFile file(path); file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (const QJsonDocument doc = QJsonDocument::fromJson(file.readAll()); doc.isObject()) m_penSettings = doc.object();
    }
    m_penWidthMode = m_penSettings.value("width_mode").toString(QStringLiteral("freeze"));
    m_penWriteMode = m_penSettings.value("write_mode").toString(QStringLiteral("pen"));
    m_thicknessDeltaStep = m_penSettings.value(u"m_thicknessDeltaStep").toDouble(0.5);

    std::cout << "m_penWriteMode: " << m_penWriteMode.toStdString() << std::endl;
}

// ═══════════════════════════════════════════════════════════

double GestureBrush::curScreenWidth() const
{
    if (m_penWriteMode == "pen")
        return std::max(m_thickness + m_thicknessDelta, 0.5);
    return m_thickness;
}

// ═══════════════════════════════════════════════════════════
double GestureBrush::distance(const QPointF& p1, const QPointF& p2)
{
    return std::hypot(p1.x() - p2.x(), p1.y() - p2.y());
}

std::optional<QPointF> GestureBrush::applyDeadZone(const QPointF& raw) const
{
    if (!m_smoothedPos.has_value()) return raw;
    if (distance(raw, *m_smoothedPos) < m_deadZone) return std::nullopt;
    return raw;
}

QPointF GestureBrush::applyEma(const QPointF& raw)
{
    if (!m_smoothedPos.has_value()) {
        m_smoothedPos = raw;
        return raw;
    }
    const double a = m_smoothAlpha;
    const QPointF prev = *m_smoothedPos;
    m_smoothedPos = QPointF(a * raw.x() + (1.0 - a) * prev.x(),
        a * raw.y() + (1.0 - a) * prev.y());
    return *m_smoothedPos;
}

// ═══════════════════════════════════════════════════════════
//  ★ 离屏缓存：把已完成的笔迹一次性渲染到 QPixmap
// ═══════════════════════════════════════════════════════════
void GestureBrush::rebuildCache()
{
    // 按设备像素比创建缓存，保证清晰度
    const qreal dpr = devicePixelRatioF();
    m_cache = QPixmap(size() * dpr);
    m_cache.setDevicePixelRatio(dpr);
    m_cache.fill(Qt::transparent);

    QPainter p(&m_cache);
    p.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    p.setBrush(Qt::BrushStyle::NoBrush);

    // 只渲染已完成的笔迹（不含当前正在画的那一笔）
    for (int i = 0; i < m_strokes.size(); ++i) {
        if (i == m_curIndex) continue;
        drawStroke(p, m_strokes[i]);
    }
    p.end();
    m_cacheDirty = false;
}

// ═══════════════════════════════════════════════════════════
//  绘制
// ═══════════════════════════════════════════════════════════
void GestureBrush::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (!m_standaloneMode) return;   // 非独立模式由 Board 调用 paintStrokes

    QPainter p(this);

    // ★ 先画缓存（一次 drawPixmap，O(1)）
    if (m_cacheDirty) rebuildCache();
    p.drawPixmap(0, 0, m_cache);

    // ★ 再画当前正在进行的笔迹（只有这一笔，点数很少）
    if (m_curIndex >= 0 && m_curIndex < m_strokes.size()) {
        p.setRenderHint(QPainter::RenderHint::Antialiasing, true);
        p.setBrush(Qt::BrushStyle::NoBrush);
        drawStroke(p, m_strokes[m_curIndex]);
    }
    p.end();
}

void GestureBrush::paintStrokes(QPainter& painter)
{
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    painter.setBrush(Qt::BrushStyle::NoBrush);
    for (const Stroke& st : m_strokes)
        drawStroke(painter, st);
}

void GestureBrush::drawStroke(QPainter& painter, const Stroke& st) const
{
    const auto n = st.pts.size();
    if (n == 0) return;

    // ★★★★★ 橡皮擦单独处理 ★★★★★
    if (st.type == ERASER_BTN) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing, false);

        // ★ 判断是否为当前正在绘制的笔画
        const bool isActive = (m_curIndex >= 0
                               && m_curIndex < m_strokes.size()
                               && &st == &m_strokes[m_curIndex]);

        if (isActive) {
            // ── 正在拖拽：屏幕恒定大小（cosmetic pen）──
            QPen eraserPen(Qt::black, m_eraserWidth,
                           Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            eraserPen.setCosmetic(true);   // ★ 关键：屏幕像素恒定
            painter.setPen(eraserPen);
        } else {
            // ── 已完成笔画：世界坐标宽度，随画布缩放 ──
            const double worldWidth = m_eraserWidth / st.freezeScale;
            QPen eraserPen(Qt::black, worldWidth,
                           Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);
            eraserPen.setCosmetic(false);  // ★ 关键：随 transform 缩放
            painter.setPen(eraserPen);
        }

        if (n == 1) {
            QPainterPath dot;
            dot.moveTo(st.pts.at(0));
            dot.lineTo(st.pts.at(0));
            painter.drawPath(dot);
        } else {
            QPainterPath path;
            path.moveTo(st.pts.first());
            for (int i = 1; i < n; ++i)
                path.lineTo(st.pts[i]);
            painter.drawPath(path);
        }

        // 恢复默认状态
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
        return;
    }

    // ===== 以下仅处理普通画笔 =====
    const bool cosmetic = (m_penWidthMode == u"screen_constant");
    const QVector<double>& widths = cosmetic ? st.sw : st.ww;

    QPen pen;
    pen.setStyle(Qt::PenStyle::SolidLine);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);
    pen.setCosmetic(cosmetic);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    pen.setColor(st.color);

    if (n == 1) {
        pen.setWidthF(widths.at(0));
        painter.setPen(pen);
        QPainterPath dot;
        dot.moveTo(st.pts.at(0));
        dot.lineTo(st.pts.at(0));
        painter.drawPath(dot);
        return;
    }

    QVector<QPointF> anchors;
    anchors.reserve(n);
    anchors.append(st.pts.first());
    for (int i = 1; i < n - 1; ++i)
        anchors.append(QPointF((st.pts[i].x() + st.pts[i + 1].x()) / 2.0,
                               (st.pts[i].y() + st.pts[i + 1].y()) / 2.0));
    anchors.append(st.pts.last());

    int j = 0;
    while (j < n - 1) {
        const double w = widths.at(j + 1);
        pen.setWidthF(w);
        painter.setPen(pen);

        QPainterPath seg;
        seg.moveTo(anchors[j]);
        seg.quadTo(st.pts[j + 1], anchors[j + 1]);

        int k = j + 1;
        while (k < n - 1 && std::abs(widths.at(k + 1) - w) < 0.01) {
            seg.quadTo(st.pts[k + 1], anchors[k + 1]);
            ++k;
        }

        painter.drawPath(seg);
        j = k;
    }
}

void GestureBrush::requestRepaint()
{
    if (m_standaloneMode) {
        update();
    }
    else {
        // ★ 补全：通知 Board 重绘（笔迹画在 Board 的 paintEvent 里）
        if (auto* board = qobject_cast<Board*>(parentWidget())) {
            board->update();
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  鼠标事件
// ═══════════════════════════════════════════════════════════
void GestureBrush::mousePressEvent(QMouseEvent* event)
{
    const QPointF raw = event->position();
    m_smoothedPos = raw;
    m_isDragging = true;
    m_thicknessDelta = -m_originalThickness;
    m_lastSpeed = 0.0;

    // 从父 Board 获取当前 scale
    if (!m_standaloneMode) {
        const auto* board = qobject_cast<const Board*>(parentWidget());
        m_s0 = board ? board->scale() : 1.0;
    } else {
        m_s0 = m_scale;
    }

    const double sw = curScreenWidth();
    const QPointF w = toImgCoord(raw);

    Stroke s;
    s.pts   = { w };
    s.sw    = { sw };
    s.ww    = { sw / m_s0 };
    s.color = m_penColor;
    s.type  = m_type;
    s.freezeScale = m_s0;          // ★ 新增：记录落笔时的缩放比

    m_strokes.append(std::move(s));
    m_curIndex = static_cast<int>(m_strokes.size()) - 1;

    event->accept();
    requestRepaint();
}

void GestureBrush::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_isDragging) return;

    const QPointF raw = event->position();

    // 绘制橡皮
    if (m_type == ERASER_BTN) {
        if (!m_eraserLabel->isVisible()) {m_eraserLabel->show();}
        const double cx = raw.x() - m_eraserLabel->width()  / 2.0;
        const double cy = raw.y() - m_eraserLabel->height() / 2.0;
        m_eraserLabel->move(qRound(cx), qRound(cy));
    }

    const auto filtered = applyDeadZone(raw);
    if (!filtered) return;
    const QPointF smoothed = applyEma(*filtered);

    if (m_penWriteMode.toStdString() == "pen") {
        const double speed = distance(raw, smoothed) / m_thickness;
        if (speed > m_lastSpeed) {
            m_thicknessDelta -= m_thicknessDeltaStep;
            if (m_thicknessDelta < -m_thickness + m_minThickness)
                m_thicknessDelta = -m_thickness + m_minThickness;
        }
        else if (speed < m_lastSpeed) {
            m_thicknessDelta += m_thicknessDeltaStep;
            if (m_thicknessDelta > 0.0) m_thicknessDelta = 0.0;
        }
        m_lastSpeed = speed;
    }

    const double sw = curScreenWidth();
    const QPointF w = toImgCoord(smoothed);

    if (m_curIndex >= 0 && m_curIndex < m_strokes.size()) {
        Stroke& cur = m_strokes[m_curIndex];
        cur.pts.append(w);
        cur.sw.append(sw);
        cur.ww.append(sw / m_s0);
    }

    emit strokesChangedSignal();

    requestRepaint();
}

void GestureBrush::mouseReleaseEvent(QMouseEvent* event) {
    if (m_isDragging) { // 只有拖拽了才处理
        m_smoothedPos.reset();
        m_curIndex = -1;
        m_isDragging = false;
        m_cacheDirty = true;
        event->accept();
        requestRepaint();
    } else {
        QWidget::mouseReleaseEvent(event);
    }

    if (m_type == ERASER_BTN) {
        m_eraserLabel->hide();
    }
}

void GestureBrush::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    m_cacheDirty = true;   // ★ 尺寸变了，缓存也要重建
}

QPointF GestureBrush::toImgCoord(const QPointF& windowPos) const
{
    if (!m_standaloneMode) {
        if (const auto* board = qobject_cast<const Board*>(parentWidget())) {
            const double posX = board->posX();
            const double posY = board->posY();
            const double scale = board->scale();
            const int rotation = board->rotation();
            const double cx = board->m_viewer.imgW() / 2.0;
            const double cy = board->m_viewer.imgH() / 2.0;

            // ★ 正向变换: img→screen 的逆变换 screen→img
            // 正向: translate(posX,posY) → scale(s) → translate(cx,cy) → rotate(r) → translate(-cx,-cy)
            // 逆向: translate(cx,cy) → rotate(-r) → translate(-cx,-cy) → scale(1/s) → translate(-posX,-posY)
            QTransform inv;
            inv.translate(cx, cy);
            inv.rotate(-rotation);
            inv.translate(-cx, -cy);
            inv.scale(1.0 / scale, 1.0 / scale);
            inv.translate(-posX, -posY);

            return inv.map(windowPos);
        }
    }
    return windowPos;
}

void GestureBrush::OnChangeBtnModeEvent(const BtnType type) {
    m_type = type;
}

void GestureBrush::OnChangeScaleEvent(const double scale) {
    m_scale = scale;
}
