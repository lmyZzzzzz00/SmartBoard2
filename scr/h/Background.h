#pragma once

#include <QImage>
#include <QJsonObject>
#include <QPointF>

/**
 * @brief 基于 CPU + QPainter 的图片缩放/平移查看器
 *
 * 由 PyQt5 的 Background.py 重构而来。
 * 仅依赖 Qt6 的 Core 与 Gui 模块（不涉及 Widgets）。
 */
class ZoomableImageViewer
{
public:
    explicit ZoomableImageViewer(const QString& imgPath);

    // ─── 图片数据 ───
    QImage      m_originalImage;   // 统一为 RGBA8888 格式的原图
    int         m_imgW = 0;
    int         m_imgH = 0;

    // ─── 变换状态（对外可读写，对应 Python 中的公开属性）───
    double      m_scale = 1.0;
    double      m_posX = 0.0;
    double      m_posY = 0.0;
    double      m_maxScale = 500.0;   // 默认值，会被 background.json 覆盖

    QJsonObject m_backgroundSettings;  // ../../data/background.json 的内容

    [[nodiscard]] const QImage& originalImage() const { return m_originalImage; }
    [[nodiscard]] int imgW() const { return m_imgW; }
    [[nodiscard]] int imgH() const { return m_imgH; }

private:
    // ─── 缩放动画/插值状态 ───
    double m_targetScale = 1.0;
    double m_lastScale = 1.0;

    // ─── 缩放锚点 ───
    double m_zoomCx = 0.0;
    double m_zoomCy = 0.0;
    double m_zoomStartScale = 1.0;
    double m_zoomStartPosX = 0.0;
    double m_zoomStartPosY = 0.0;

    // ─── 鼠标拖拽 ───
    bool   m_dragging = false;
    QPointF m_lastMousePos{ 0.0, 0.0 };
};