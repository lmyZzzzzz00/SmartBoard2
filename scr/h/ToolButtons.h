#pragma once

#include <QTimer>

#include "../h/SingleToolBtn.h"
#include "../h/BtnTypes.h"


class BrushTools;


class ToolButtons: public QWidget
{
    Q_OBJECT
public:
    explicit ToolButtons(QWidget *parent = nullptr);
    ~ToolButtons() override;

    QImage m_exit_image;
    QPixmap m_exit_pixmap;
    SingleToolBtn* m_exit_btn;
    SingleToolBtn* m_cursor_btn;
    SingleToolBtn* m_brush_btn;
    SingleToolBtn* m_eraser_btn;
    SingleToolBtn* m_rotate_btn;
    SingleToolBtn* m_bigger_btn;
    SingleToolBtn* m_smaller_btn;

    SingleToolBtn *m_btn_array[BtnCount];

    BrushTools* m_brush_tools;

    BtnType m_now_btn_type;
    int m_buttons_num;

private:

    int m_target_length = 60;   // 按钮目标缩放的目标宽度（逻辑坐标）
    int m_window_width;
    int m_window_height;
    qreal m_dpi_ratio; // 屏幕DPI
    QSize m_logical_size;
    QSize m_physical_size;

    [[nodiscard]] int return_target_x(const int &index) const;   // index: 当前按钮图标（只算居中的）的个数序列号，从1开始数
    void updateButtons() const;

signals:
    void changeBtnModeSignal(BtnType type);

public slots:
    void OnResizeEvent(int w, int h);
    void OnChangeChosenEvent(BtnType type);
};