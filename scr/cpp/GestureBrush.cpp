#include "../h/GestureBrush.h"
#include "../h/Board.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMouseEvent>
#include <QPainterPath>

#include <cmath>
#include <iostream>

// ═══════════════════════════════════════════════════════════
//  ★ 轮廓构建：中心线 + 宽度数组 → 闭合贝塞尔多边形
// ═══════════════════════════════════════════════════════════
static QPainterPath buildOutline(const QVector<QPointF>& pts, const QVector<double>& widths)
{
    const qsizetype n = pts.size();
    if (n == 0) return {};

    if (n == 1) {
        QPainterPath p;
        const double r = widths[0] / 2.0;
        p.addEllipse(pts[0], r, r);
        p.setFillRule(Qt::WindingFill);
        return p;
    }

    QVector<QPointF> left, right, dirs;
    left.reserve(n);
    right.reserve(n);
    dirs.reserve(n);

    for (qsizetype i = 0; i < n; ++i) {
        QPointF dir;
        if (i == 0)          dir = pts[1] - pts[0];
        else if (i == n - 1) dir = pts[n - 1] - pts[n - 2];
        else                 dir = pts[i + 1] - pts[i - 1];
        dirs.append(dir);

        const double len = std::hypot(dir.x(), dir.y());
        if (len < 1e-6) {
            if (!left.isEmpty()) {
                left.append(left.last());
                right.append(right.last());
            }
            continue;
        }

        const double nx = -dir.y() / len;
        const double ny =  dir.x() / len;
        const double halfW = widths[i] / 2.0;

        left.append(QPointF(pts[i].x() + nx * halfW, pts[i].y() + ny * halfW));
        right.append(QPointF(pts[i].x() - nx * halfW, pts[i].y() - ny * halfW));
    }

    QPainterPath path;
    path.moveTo(left.first());

    // 左侧贝塞尔平滑
    for (qsizetype i = 1; i < left.size() - 1; ++i) {
        const QPointF mid((left[i].x() + left[i + 1].x()) / 2.0,
                          (left[i].y() + left[i + 1].y()) / 2.0);
        path.quadTo(left[i], mid);
    }
    if (left.size() > 1) path.lineTo(left.last());

    // ★ 末端圆头：用 quadTo 逼近半圆，控制点 = 圆心 + 前进方向单位向量 * radius * k
    // k = 4/3 ≈ 1.333 是二次贝塞尔逼近半圆的最佳系数（实际略大，用 1.3 视觉更好）
    {
        const QPointF& endL = left.last();
        const QPointF& endR = right.last();
        const QPointF center((endL.x() + endR.x()) / 2.0, (endL.y() + endR.y()) / 2.0);
        if (const double radius = std::hypot(endL.x() - endR.x(), endL.y() - endR.y()) / 2.0; radius > 1e-6) {
            // 前进方向单位向量
            const QPointF& d = dirs.last();
            const double dlen = std::hypot(d.x(), d.y());
            const QPointF fwd(d.x() / dlen, d.y() / dlen);
            // 控制点：圆心沿前进方向偏移
            const QPointF ctrl(center.x() + fwd.x() * radius * 1.3,
                               center.y() + fwd.y() * radius * 1.3);
            path.quadTo(ctrl, endR);
        } else {
            path.lineTo(endR);
        }
    }

    // 右侧贝塞尔平滑（逆序）
    for (qsizetype i = right.size() - 2; i > 0; --i) {
        const QPointF mid((right[i].x() + right[i - 1].x()) / 2.0,
                          (right[i].y() + right[i - 1].y()) / 2.0);
        path.quadTo(right[i], mid);
    }
    if (right.size() > 1) path.lineTo(right.first());

    // ★ 首端圆头：控制点 = 圆心 - 前进方向单位向量 * radius * 1.3（向后凸）
    {
        const QPointF& startR = right.first();
        const QPointF& startL = left.first();
        const QPointF center((startR.x() + startL.x()) / 2.0, (startR.y() + startL.y()) / 2.0);
        if (const double radius = std::hypot(startR.x() - startL.x(), startR.y() - startL.y()) / 2.0; radius > 1e-6) {
            const QPointF& d = dirs.first();
            const double dlen = std::hypot(d.x(), d.y());
            const QPointF fwd(d.x() / dlen, d.y() / dlen);
            // 控制点：圆心沿前进方向的反方向偏移（向后凸出）
            const QPointF ctrl(center.x() - fwd.x() * radius * 1.3,
                               center.y() - fwd.y() * radius * 1.3);
            path.quadTo(ctrl, startL);
        } else {
            path.lineTo(startL);
        }
    }

    path.closeSubpath();
    path.setFillRule(Qt::WindingFill);
    return path;
}

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

    // 加载橡皮擦图像
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
        if (const QJsonDocument doc = QJsonDocument::fromJson(file.readAll()); doc.isObject())
            m_penSettings = doc.object();
    }
    m_penWidthMode = m_penSettings.value("width_mode").toString(QStringLiteral("freeze"));
    m_penWriteMode = m_penSettings.value("write_mode").toString(QStringLiteral("pen"));
    m_thicknessDeltaStep = m_penSettings.value("thicknessDeltaStep").toDouble(0.5);
    m_pointsThreshold = m_penSettings.value("points_threshold").toInt(20);
    m_fixed_points = m_penSettings.value("fixed_points").toInt(3);

    std::cout << "m_penWriteMode: " << m_penWriteMode.toStdString() << std::endl;
}

// ═══════════════════════════════════════════════════════════
double GestureBrush::curScreenWidth() const
{
    if (m_type == ERASER_BTN)
        return m_eraserWidth;          // ★ 加这一行

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
//  ★ 离屏缓存
// ═══════════════════════════════════════════════════════════
void GestureBrush::rebuildCache()
{
    const qreal dpr = devicePixelRatioF();
    m_cache = QPixmap(size() * dpr);
    m_cache.setDevicePixelRatio(dpr);
    m_cache.fill(Qt::transparent);

    QPainter p(&m_cache);
    p.setRenderHint(QPainter::RenderHint::Antialiasing, true);

    for (int i = 0; i < m_strokes.size(); ++i) {
        if (i == m_curIndex) continue;
        drawStroke(p, m_strokes[i]);
    }
    p.end();
    m_cacheDirty = false;
}

// ═══════════════════════════════════════════════════════════
//  绘制入口
// ═══════════════════════════════════════════════════════════
void GestureBrush::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    if (!m_standaloneMode) return;

    QPainter p(this);

    if (m_cacheDirty) rebuildCache();
    p.drawPixmap(0, 0, m_cache);

    if (m_curIndex >= 0 && m_curIndex < m_strokes.size()) {
        p.setRenderHint(QPainter::RenderHint::Antialiasing, true);
        drawStroke(p, m_strokes[m_curIndex]);
    }
    p.end();
}

void GestureBrush::paintStrokes(QPainter& painter)
{
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    for (Stroke& st : m_strokes)
        drawStroke(painter, st);
}

void GestureBrush::addTail(Stroke &st) {
    // 处理尖尾逻辑
    if (const int size = static_cast<int>(st.ww.size()); size > 0) {
        // std::cout << "size: " << size << std::endl;
        int need_to_deal_size;
        if (size > m_pointsThreshold) {
            // 此时只能使用固定点位数，否则画笔会异常尖尾
            need_to_deal_size = m_fixed_points;
        }
        else {
            need_to_deal_size = static_cast<int>(size * 0.2);
        }
        if (need_to_deal_size == 0) { need_to_deal_size = 1; }
        for (int i = 0; i < size; ++i) {
            if (i >= size - need_to_deal_size) {
                // j = 0, 1, 2, ..., need_to_deal_size - 1
                const int j = i - (size - need_to_deal_size);
                // f 从 1.0 线性递减到接近 0
                const double f = 1.0 - static_cast<double>(j) / need_to_deal_size;
                st.ww[i] = st.ww[i] * f;
            }
        }
    }
}

// ═══════════════════════════════════════════════════════════
//  ★ 核心绘制：全部使用填充多边形
// ═══════════════════════════════════════════════════════════
void GestureBrush::drawStroke(QPainter& painter, Stroke& st) const
{
    const qsizetype n = st.pts.size();
    if (n == 0) return;

    // ★★★★★ 橡皮擦 ★★★★★
    if (st.type == ERASER_BTN) {
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing, false);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);

        const bool isActive = (m_curIndex >= 0
                               && m_curIndex < static_cast<int>(m_strokes.size())
                               && &st == &m_strokes[m_curIndex]);

        QVector<double> eraserWidths;
        eraserWidths.reserve(n);
        if (isActive) {
            const double worldW = m_eraserWidth / st.freezeScale;
            for (qsizetype i = 0; i < n; ++i) eraserWidths.append(worldW);
        } else {
            eraserWidths = st.ww;
        }

        painter.drawPath(buildOutline(st.pts, eraserWidths));

        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
        return;
    }

    // ===== 普通画笔 =====
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setRenderHint(QPainter::RenderHint::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(st.color);

    painter.drawPath(buildOutline(st.pts, st.ww));
}

// ═══════════════════════════════════════════════════════════
void GestureBrush::requestRepaint()
{
    if (m_standaloneMode) {
        update();
    } else {
        if (auto* board = qobject_cast<Board*>(parentWidget()))
            board->update();
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

    // 获取落笔时缩放比
    if (!m_standaloneMode) {
        const auto* board = qobject_cast<const Board*>(parentWidget());
        m_s0 = board ? board->scale() : 1.0;
    } else {
        m_s0 = m_scale;
    }

    const double sw = curScreenWidth();
    const QPointF w = toImgCoord(raw);
    const double ww = sw / m_s0; // ★ 统一转为世界坐标线宽

    Stroke s;
    s.pts         = { w };
    s.ww          = { ww };
    s.color       = m_penColor;
    s.type        = m_type;
    s.freezeScale = m_s0;
    m_pressPos    = raw;

    m_strokes.append(std::move(s));
    m_curIndex = static_cast<int>(m_strokes.size()) - 1;

    event->accept();
    requestRepaint();
}

void GestureBrush::mouseMoveEvent(QMouseEvent* event)
{
    if (!m_isDragging) return;

    const QPointF raw = event->position();

    // 橡皮擦图标跟随
    if (m_type == ERASER_BTN) {
        if (!m_eraserLabel->isVisible()) m_eraserLabel->show();
        const double cx = raw.x() - m_eraserLabel->width()  / 2.0;
        const double cy = raw.y() - m_eraserLabel->height() / 2.0;
        m_eraserLabel->move(qRound(cx), qRound(cy));
    }

    const auto filtered = applyDeadZone(raw);
    if (!filtered) return;
    const QPointF smoothed = applyEma(*filtered);

    const double speed = distance(raw, smoothed) / m_thickness;
    // 压感/速度动态笔宽
    if (m_penWriteMode.toStdString() == "pen") {
        if (speed > m_lastSpeed) {
            m_thicknessDelta -= m_thicknessDeltaStep;
            if (m_thicknessDelta < -m_thickness + m_minThickness)
                m_thicknessDelta = -m_thickness + m_minThickness;
        } else if (speed < m_lastSpeed) {
            m_thicknessDelta += m_thicknessDeltaStep;
            if (m_thicknessDelta > 0.0) m_thicknessDelta = 0.0;
        }
        m_lastSpeed = speed;
    }

    const double sw = curScreenWidth();
    const QPointF w = toImgCoord(smoothed);
    const double ww = sw / m_s0; // ★ 世界坐标线宽

    Stroke& cur = m_strokes[m_curIndex];
    if (m_curIndex >= 0 && m_curIndex < m_strokes.size()) {
        cur.pts.append(w);
        cur.ww.append(ww);
    }

    emit strokesChangedSignal();
    requestRepaint();
}

// ═══════════════════════════════════════════════════════════
void GestureBrush::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_isDragging) {
        const QPointF releasePos = event->position();
        const bool isClick = (distance(m_pressPos, releasePos) < m_clickThreshold);

        // ★ 先处理笔画数据，再重置状态！
        if (m_curIndex >= 0 && m_curIndex < m_strokes.size()) {
            Stroke& cur = m_strokes[m_curIndex];

            if (isClick) {
                const double sw = curScreenWidth();
                const QPointF centerImg = toImgCoord(m_pressPos);
                makeCircleStroke(cur, centerImg, sw / 2.0);
            } else if (m_type == BRUSH_BTN) {
                // ★ 在 m_curIndex 有效时调用 addTail
                addTail(cur);
            }
        }

        // ★ 数据修改完毕后，再重置状态并刷新
        m_smoothedPos.reset();
        m_curIndex = -1;
        m_isDragging = false;
        m_cacheDirty = true;  // ★ 关键：尖尾修改了ww，必须重建缓存
        event->accept();
        requestRepaint();     // ★ 关键：触发重绘
    } else {
        QWidget::mouseReleaseEvent(event);
    }

    if (m_type == ERASER_BTN)
        m_eraserLabel->hide();
}

void GestureBrush::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    m_cacheDirty = true;
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

void GestureBrush::OnChangeBtnModeEvent(const BtnType type)
{
    m_type = type;
}

void GestureBrush::OnChangeScaleEvent(const double scale)
{
    m_scale = scale;
}

// ═══════════════════════════════════════════════════════════
//  ★ 点击画圆：生成单点+直径，buildOutline 自动处理为实心圆
// ═══════════════════════════════════════════════════════════
void GestureBrush::makeCircleStroke(Stroke& stroke, const QPointF& centerImg, const double radiusScreen)
{
    const double worldRadius = radiusScreen / stroke.freezeScale;
    const double diameter = worldRadius * 2.0;

    stroke.pts.clear();
    stroke.ww.clear();

    stroke.pts.append(centerImg);
    stroke.ww.append(diameter);
}