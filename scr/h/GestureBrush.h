#pragma once

#include <QJsonObject>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>
#include <QPointF>
#include <QVector>
#include <QLabel>

#include <optional>

#include "BtnTypes.h"


class Board;

struct Stroke {
    QVector<QPointF> pts;
    QVector<double>  ww;
    QColor           color;
    BtnType          type = BRUSH_BTN;
    double           freezeScale = 1.0;  // ★ 新增：记录该笔画创建时的画布缩放比
};

class GestureBrush : public QWidget
{
    Q_OBJECT

public:
    explicit GestureBrush(
        int     width = 800,
        int     height = 600,
        QColor  bgColor = QColor(50, 50, 50),
        QColor  penColor = QColor(255, 0, 0),
        double  thickness = 1.5,
        double  smoothAlpha = 0.4,
        double  deadZone = 1.5,
        QWidget* parent = nullptr);

    void paintStrokes(QPainter& painter);

    [[nodiscard]] const QVector<Stroke>& strokes() const { return m_strokes; }
    [[nodiscard]] const QString& penWidthMode()    const { return m_penWidthMode; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    [[nodiscard]] QPointF toImgCoord(const QPointF& windowPos) const;
    [[nodiscard]] double  curScreenWidth() const;

    static double distance(const QPointF& p1, const QPointF& p2);
    [[nodiscard]] std::optional<QPointF> applyDeadZone(const QPointF& raw) const;
    QPointF                applyEma(const QPointF& raw);

    void drawStroke(QPainter& painter, const Stroke& st) const;
    // void drawStrokeSegment(QPainter& painter, const Stroke& st, int fromIdx);
    void requestRepaint();
    void loadPenSettings();

    // ★ 离屏缓存：已完成的笔迹渲染到这里，不再每帧重算
    void rebuildCache();

    bool    m_standaloneMode = false;

    QColor  m_bgColor;
    QColor  m_penColor;
    double  m_thickness;
    double  m_originalThickness;

    double  m_smoothAlpha;
    double  m_deadZone;
    std::optional<QPointF> m_smoothedPos;

    QString     m_penWidthMode;
    QString     m_penWriteMode;
    QJsonObject m_penSettings;

    QVector<Stroke> m_strokes;
    int             m_curIndex = -1;
    double          m_s0 = 1.0;

    // ★ 离屏缓存
    QPixmap m_cache;          // 已完成笔迹的渲染结果
    bool    m_cacheDirty = true;

    double m_thicknessDelta = 0.0;
    double m_lastSpeed = 0.0;
    double m_thicknessDeltaStep = 0.5;
    double m_minThickness = 1.0;

    bool   m_isDragging = false;
    double m_scale = 1.0;

    // 现在的模式
    BtnType m_type = BRUSH_BTN;   // BRUSH_BTN 代表笔，ERASER_BTN 代表橡皮

    double m_eraserWidth = 60.0;

    // 橡皮
    QPixmap m_eraserPixmap;
    QLabel *m_eraserLabel = nullptr;

    QPointF m_pressPos;           // ★ 新增：记录鼠标按下时的屏幕坐标
    double m_clickThreshold = 5.0; // ★ 新增：点击判定阈值（像素），小于此值视为原地点击
    
    // ★ 新增：将当前笔画替换为圆形
    static void makeCircleStroke(Stroke& stroke, const QPointF& centerImg, double radiusScreen);

public slots:
    void OnChangeBtnModeEvent(BtnType type);
    void OnChangeScaleEvent(double scale);

signals:
    void strokesChangedSignal();
};