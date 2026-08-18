#pragma once

#include <QWidget>
#include <QColor>
#include <QPoint>
#include <QTimer>
#include <QLabel>
#include <QGestureEvent>

#include "BtnTypes.h"
#include "../h/Background.h"
#include "../h/ToolButtons.h"


class GestureBrush;   // 前向声明，避免循环包含

class ZoomableImageViewer;

class ToolButtons;

class Board : public QWidget
{
    Q_OBJECT

public:
    explicit Board(const QString& imgPath, int width = 800, int height = 600,
        QWidget* parent = nullptr);

    // ★ GestureBrush 需要访问这些（坐标换算 & 落笔 scale）
    ZoomableImageViewer m_viewer;  // 子组件
    [[nodiscard]] double posX()    const { return m_posX; }
    [[nodiscard]] double posY()    const { return m_posY; }
    [[nodiscard]] double scale()   const { return m_scale; }
    [[nodiscard]] int    imgW()    const { return m_viewer.imgW(); }
    [[nodiscard]] int    imgH()    const { return m_viewer.imgH(); }

    // 运行模式
    QJsonObject m_runMode;
    QString m_window_mode;

    GestureBrush* m_gestureBrush = nullptr;
    ToolButtons* m_toolButtons = nullptr;

    int rotation() const { return m_rotation; }

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    bool event(QEvent* event) override;   // 手势事件分发

private:
    void startAnim() const;
    void animate();
    void rotate_animate();
    void setZoomAnchor(double cx, double cy);
    bool handleGesture(const QGestureEvent* event);

    // ─── 配置 ───
    QJsonObject m_penSettings;
    QJsonObject m_bgSettings;
    QColor      m_bgColor;
    double      m_penThickness = 1.5;
    double      m_maxScale = 500.0;
    double      m_minScale = 0.1;

    // ─── 变换状态 ───
    double m_scale = 1.0;
    double m_targetScale = 1.0;
    double m_lastScale = 1.0;
    double m_posX = 0.0;
    double m_posY = 0.0;

    // ─── 缩放锚点 ───
    double m_zoomCx = 0.0;
    double m_zoomCy = 0.0;
    double m_zoomStartScale = 1.0;
    double m_zoomStartPosX = 0.0;
    double m_zoomStartPosY = 0.0;

    // ─── 拖拽 ───
    bool  m_dragging = false;
    QPoint m_lastMousePos{ 0, 0 };

    // ─── 动画定时器 ───
    QTimer* m_timer = nullptr;
    QTimer* m_rotate_timer = nullptr;

    bool m_showGestureBrush = true;

    bool m_is_first_scaled = false;     // 是否初始化缩放（还原100%缩放）
    qreal m_cacheScale = 0.0;   // 记录当前缓存对应的缩放级别

    // 旋转配置
    double m_rotate_start_angle = 0.0;  // 动画起始角度
    double m_rotate_target_angle = 90.0; // 目标旋转角度
    double m_rotate_step = 0.3;
    double m_rotation = 0.0; // 记录当前旋转角度（0, 90, 180, 270）

    // 缩放提示QLabel
    QLabel* m_scale_info_label = nullptr;
    QTimer* m_scale_info_timer;
    void HideScaleInfoLabel() const;

signals:
    void resized(int w, int h);
    void scaleChangedSignal(double scale);
    void changeToCursorModeSignal();
    void BrushToolsHideSignal();

public slots:
    void OnChangeBtnModeEvent(BtnType type);
    void OnChangeScaleEvent(bool is_bigger);
    void OnRotateEvent(); // 处理旋转按钮的槽函数
};