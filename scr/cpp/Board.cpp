#include "../h/Board.h"
#include "../h/GestureBrush.h"

#include <QGestureEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QTransform>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>   // ← 加这一行

#include <cmath>
#include <algorithm>
#include <iostream>
#include <QApplication>
#include "../h/BtnTypes.h"
#include "../h/SingleToolBtn.h"

// ═══════════════════════════════════════════════════════════
Board::Board(const QString& imgPath, const int width, const int height, QWidget* parent)
    : QWidget(parent)
    , m_viewer(imgPath)
{
    resize(width, height);

    // ─── 加载配置 ───
    {
        if (QFile f(QDir::currentPath() + "/../data/pen_settings.json"); f.open(QIODevice::ReadOnly)) {
            m_penSettings = QJsonDocument::fromJson(f.readAll()).object();
        }
    }
    {
        if (QFile f(QDir::currentPath() + "/../data/background.json"); f.open(QIODevice::ReadOnly)) {
            m_bgSettings = QJsonDocument::fromJson(f.readAll()).object();
        }
    }
    {
        if (QFile f(QDir::currentPath() + "/../data/run_mode.json"); f.open(QIODevice::ReadOnly)) {
            m_runMode = QJsonDocument::fromJson(f.readAll()).object();
            m_window_mode = m_runMode.value("window_mode").toString(QString("full"));
        }
    }

    m_penThickness = m_penSettings.value("thickness").toDouble(1.5);
    m_maxScale = m_bgSettings.value("max_scale").toDouble(500.0);

    if (const auto bgArr = m_bgSettings.value("bg_color").toArray(); bgArr.size() >= 3) {
        m_bgColor = QColor(bgArr[0].toInt(), bgArr[1].toInt(), bgArr[2].toInt());
    }
    else {
        m_bgColor = QColor(50, 50, 50);
    }

    // ─── 创建 GestureBrush 子组件 ───
    m_gestureBrush = new GestureBrush(
        width, height,
        m_bgColor, QColor(255, 0, 0),
        m_penThickness, 0.4, 1.5,
        this   // ★ parent = this
    );
    m_gestureBrush->show();
    m_gestureBrush->move(0, 0);
    m_gestureBrush->resize(this->width(), this->height());

    // 缩放提示标签
    m_scale_info_label = new QLabel(this);
    m_scale_info_label->setAlignment(Qt::AlignCenter);
    m_scale_info_label->setStyleSheet("background-color: rgba(0, 0, 0, 160);color: rgb(255, 255, 255);border-radius: 10px;");
    m_scale_info_label->setFont(QFont("Microsoft YaHei", 12));
    m_scale_info_label->setText("缩放：100%");
    m_scale_info_label->setMargin(10);
    m_scale_info_label->move(this->width() / 2 - m_scale_info_label->width() / 2, this->height() / 2 - m_scale_info_label->height() / 2);
    m_scale_info_label->hide();
    m_scale_info_timer = new QTimer(this);
    connect(m_scale_info_timer, &QTimer::timeout, this, &Board::HideScaleInfoLabel);

    // ─── 动画定时器 ───
    m_timer = new QTimer(this);
    m_timer->setInterval(16);   // ≈60 fps
    connect(m_timer, &QTimer::timeout, this, &Board::animate);

    m_rotate_timer = new QTimer(this);
    m_rotate_timer->setInterval(16);   // ≈60 fps
    connect(m_rotate_timer, &QTimer::timeout, this, &Board::rotate_animate);

    // ─── 启用手势 ───
    grabGesture(Qt::GestureType::PinchGesture);

    // 信号与槽
    connect(this, &Board::scaleChangedSignal, m_gestureBrush, &GestureBrush::OnChangeScaleEvent);
}

// ═══════════════════════════════════════════════════════════
void Board::resizeEvent(QResizeEvent* event)
{
    const QSize oldSize = event->oldSize();
    const QSize newSize = event->size();

    const double iw = m_viewer.imgW() * m_scale;
    const double ih = m_viewer.imgH() * m_scale;

    if (oldSize.width() > 0 && oldSize.height() > 0) {
        const double cx = m_posX + iw / 2.0;
        const double cy = m_posY + ih / 2.0;
        const double ratioX = cx / oldSize.width();
        const double ratioY = cy / oldSize.height();
        m_posX = ratioX * newSize.width() - iw / 2.0;
        m_posY = ratioY * newSize.height() - ih / 2.0;
    }
    else {
        m_posX = (width() - m_viewer.imgW()) / 2.0;
        m_posY = (height() - m_viewer.imgH()) / 2.0;
    }

    m_gestureBrush->resize(width(), height());
    m_scale_info_label->move(this->width() / 2 - m_scale_info_label->width() / 2, this->height() / 2 - m_scale_info_label->height() / 2);

    // 发射信号
    emit resized(width(), height());

    QWidget::resizeEvent(event);
}

// ═══════════════════════════════════════════════════════════
void Board::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        m_dragging = true;
        m_lastMousePos = event->pos();
        setCursor(Qt::CursorShape::ClosedHandCursor);
    }
}

void Board::mouseMoveEvent(QMouseEvent* event)
{
    if (m_dragging) {
        const QPoint delta = event->pos() - m_lastMousePos;
        m_posX += delta.x();
        m_posY += delta.y();
        m_lastMousePos = event->pos();
        update();
    }
}

void Board::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        m_dragging = false;
        setCursor(Qt::CursorShape::ArrowCursor);
    }
}

// ═══════════════════════════════════════════════════════════
void Board::startAnim() const
{
    if (!m_timer->isActive())
        m_timer->start();
}

void Board::animate()
{
    if (const double diff = m_targetScale - m_scale; std::abs(diff) < 0.001) {
        m_scale = m_targetScale;
        m_timer->stop();
    }
    else {
        m_scale += diff * 0.35;
    }

    if (m_zoomStartScale > 0.0) {
        m_posX = m_zoomCx - (m_zoomCx - m_zoomStartPosX) * m_scale / m_zoomStartScale;
        m_posY = m_zoomCy - (m_zoomCy - m_zoomStartPosY) * m_scale / m_zoomStartScale;
    }

    // 缩放提示
    // std::cout << "Scaled: " << m_scale << std::endl;
    m_scale_info_label->setText(QString("缩放：") + QString::number(m_scale * 100, 'f', 0) + QString("%"));
    m_scale_info_label->adjustSize(); // 根据文本内容自适应尺寸
    m_scale_info_label->move(this->width() / 2 - m_scale_info_label->width() / 2, this->height() / 2 - m_scale_info_label->height() / 2); // 重新居中
    m_scale_info_label->show();
    m_scale_info_label->update();

    // 先取消m_scale_info_timer定时器的所有定时，再启动定时器
    m_scale_info_timer->stop();
    m_scale_info_timer->start(2000); // 2秒后隐藏缩放提示

    emit scaleChangedSignal(m_scale);

    update();
}

void Board::setZoomAnchor(const double cx, const double cy)
{
    m_zoomCx = cx;
    m_zoomCy = cy;
    m_zoomStartScale = m_scale;
    m_zoomStartPosX = m_posX;
    m_zoomStartPosY = m_posY;
}

// ═══════════════════════════════════════════════════════════
void Board::wheelEvent(QWheelEvent* event)
{
    if (event->modifiers() & Qt::KeyboardModifier::ControlModifier) {
        // Qt 6: pixelDelta() / angleDelta() 返回 QPoint
        double factor;
        if (!event->pixelDelta().isNull()) {
            factor = std::exp(event->pixelDelta().y() * 0.002);
        }
        else {
            factor = std::exp(event->angleDelta().y() * 0.001);
        }

        setZoomAnchor(event->position().x(), event->position().y());
        m_targetScale = std::clamp(m_targetScale * factor, m_minScale, m_maxScale);
        startAnim();
        event->accept();
    }
    else {
        QWidget::wheelEvent(event);
    }
}

// ═══════════════════════════════════════════════════════════
bool Board::handleGesture(const QGestureEvent* event)
{
    const auto* pinch = dynamic_cast<QPinchGesture*>(
        event->gesture(Qt::GestureType::PinchGesture));
    if (!pinch) return false;

    if (const Qt::GestureState state = pinch->state(); state == Qt::GestureState::GestureStarted) {
        m_lastScale = m_scale;
        setZoomAnchor(width() / 2.0, height() / 2.0);
    }
    else if (state == Qt::GestureState::GestureUpdated ||
        state == Qt::GestureState::GestureFinished) {
        m_targetScale = std::clamp(m_lastScale * pinch->scaleFactor(), 0.1, m_maxScale);
        startAnim();
    }
    else if (state == Qt::GestureState::GestureCanceled) {
        m_targetScale = m_lastScale;
        startAnim();
    }
    return true;
}

bool Board::event(QEvent* event)
{
    if (event->type() == QEvent::Type::Gesture) {
        return handleGesture(dynamic_cast<QGestureEvent*>(event));
    }
    return QWidget::event(event);
}

// ═══════════════════════════════════════════════════════════
void Board::paintEvent(QPaintEvent* event)
{
    if (!m_is_first_scaled) {
        // --- 还原大小 ---
        // 确保 ratio 不为 0，防止除零异常（虽然正常系统不会为0，但作为防御性编程是好习惯）
        if (const qreal ratio = QApplication::primaryScreen()->devicePixelRatio(); ratio > 0.0) {
            // 计算中心点
            const double cx = this->width()/ 2.0;
            const double cy = this->height() / 2.0;
            m_scale = 1.0 / ratio;
            const double iw = m_viewer.m_imgW * m_scale;
            const double ih = m_viewer.m_imgH * m_scale;
            m_posX = cx - iw / 2.0;
            m_posY = cy - ih / 2.0;
        } else {
            m_scale = 1.0;
        }
        m_is_first_scaled = true;
    }

    Q_UNUSED(event);
    QPainter screen_painter(this);
    screen_painter.setRenderHint(QPainter::Antialiasing, true);
    screen_painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 【第1层】直接在 Widget 上绘制背景和底图（不需要透明）
    QTransform bg_transform;
    bg_transform.translate(m_posX, m_posY);
    bg_transform.scale(m_scale, m_scale);
    // ★ 新增：以图像中心为原点旋转
    const double cx = m_viewer.imgW() / 2.0;
    const double cy = m_viewer.imgH() / 2.0;
    bg_transform.translate(cx, cy);
    bg_transform.rotate(m_rotation);
    bg_transform.translate(-cx, -cy);
    screen_painter.setTransform(bg_transform);

    screen_painter.fillRect(QRect(0, 0, m_viewer.imgW(), m_viewer.imgH()), m_bgColor);
    screen_painter.drawImage(0, 0, m_viewer.originalImage());

    // 【第2层】创建视口大小的透明缓冲（★ 关键修正）
    // 尺寸等于当前 Widget 可见区域，保证橡皮擦 Clear 后有真实的 Alpha=0
    const QRect viewport = event->rect();
    QImage stroke_buffer(viewport.size(), QImage::Format_ARGB32_Premultiplied);
    stroke_buffer.fill(Qt::transparent); // 必须初始化为全透明

    QPainter buffer_painter(&stroke_buffer);
    buffer_painter.setRenderHint(QPainter::Antialiasing, true);

    // 将世界坐标变换应用到缓冲区 painter
    // 注意：因为 buffer 是从 (0,0) 开始的，需要加上 viewport 的偏移补偿
    QTransform stroke_transform;
    stroke_transform.translate(m_posX - viewport.x(), m_posY - viewport.y());
    stroke_transform.scale(m_scale, m_scale);
    // ★ 新增：与背景相同的旋转
    stroke_transform.translate(cx, cy);
    stroke_transform.rotate(m_rotation);
    stroke_transform.translate(-cx, -cy);
    buffer_painter.setTransform(stroke_transform);

    // 在透明缓冲上矢量绘制所有笔迹（包括橡皮擦）
    // CompositionMode_Clear 会正确地将 buffer 的像素设为 (0,0,0,0)
    m_gestureBrush->paintStrokes(buffer_painter);
    buffer_painter.end();

    // 【第3层】将带有真实透明通道的笔迹层合成到屏幕上
    screen_painter.resetTransform();
    screen_painter.drawImage(viewport.topLeft(), stroke_buffer);
    screen_painter.end();
    // 不要在此update()!!!
}

// ═══════════════════════════════════════════════════════════
void Board::keyPressEvent(QKeyEvent* event)
{
    // if (event->key() == Qt::Key::Key_Escape) {
    //     m_showGestureBrush = !m_showGestureBrush;
    //     m_gestureBrush->setVisible(m_showGestureBrush);
    //     std::cout << "[DEBUG] 切换画笔模式：" << (m_showGestureBrush ? "开" : "关") << std::endl;
    // }
    if (event->key() == Qt::Key::Key_Space) {
        // 调试信息
        int nSeg = 0;
        for (const auto& s : m_gestureBrush->strokes())
            nSeg += std::max<int>(0, static_cast<int>(s.pts.size()) - 1);
        std::cout << "[DEBUG]strokes:" << m_gestureBrush->strokes().size() << " segments:" << nSeg << std::endl;
        std::cout << "[DEBUG] pos: " << m_posX << ", " << m_posY << " scale: " << m_scale << std::endl;
        std::cout << "[DEBUG] mode: " << m_gestureBrush->penWidthMode().toStdString() << std::endl;
    }
    else {
        QWidget::keyPressEvent(event);
    }
}

void Board::OnChangeBtnModeEvent(const BtnType type) {
    if (type == BRUSH_BTN || type == ERASER_BTN) {
        m_showGestureBrush = true;
    } else {
        m_showGestureBrush = false;
    }
    m_gestureBrush->setVisible(m_showGestureBrush);
    std::cout << "切换画笔模式：" << (m_showGestureBrush ? "开" : "关") << std::endl;
}

void Board::OnChangeScaleEvent(const bool is_bigger) {
    m_targetScale = m_scale;
    if (is_bigger) {m_targetScale *= 1.35;} else {m_targetScale /= 1.35;}
    if (m_targetScale > m_maxScale) {m_targetScale = m_maxScale;} else if (m_targetScale < m_minScale) {m_targetScale = m_minScale;}
    // 用动画过渡缩放
    // 计算中点坐标，就是屏幕的最中间坐标
    const double cx = this->width() / 2.0;
    const double cy = this->height() / 2.0;
    setZoomAnchor(cx, cy);
    startAnim();
    // 切换回画笔模式
    emit changeToCursorModeSignal();
}

void Board::OnRotateEvent() {
    // ✅ 第一步：将当前角度强制对齐到最近的 90° 倍数（消除历史累积误差）
    const double aligned_current = std::round(m_rotation / 90.0) * 90.0;

    // ✅ 第二步：直接加90°，不做fmod，保持与m_rotation同一量级
    m_rotate_target_angle = aligned_current + 90.0;

    // ✅ 记录动画起始角度（用于插值，而非直接用 m_rotation）
    m_rotate_start_angle = aligned_current;
    m_rotation = aligned_current; // 立即对齐，避免动画首帧跳变

    m_rotate_timer->start();
    emit changeToCursorModeSignal();
}

void Board::rotate_animate() {
    constexpr double MIN_STEP = 0.5;
    const double diff = m_rotate_target_angle - m_rotation;

    if (std::abs(diff) <= MIN_STEP) {
        // 停止时对齐到90°倍数
        m_rotation = std::round(m_rotate_target_angle / 90.0) * 90.0;
        m_rotate_timer->stop();
        update();
        return;
    }

    double step = diff * m_rotate_step;
    if (std::abs(step) < MIN_STEP)
        step = (step > 0 ? MIN_STEP : -MIN_STEP);

    m_rotation += step;  // ✅ 不再截断为int
    update();
}

void Board::HideScaleInfoLabel() {
    m_scale_info_label->hide();
}