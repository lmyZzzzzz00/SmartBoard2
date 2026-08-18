#pragma once

#include <QLabel>

#include "../h/BtnTypes.h"


class SingleToolBtn: public QLabel {
    Q_OBJECT
public:
    qreal m_dpi_ratio;
    QImage m_normal_img;
    QImage m_clicked_img;
    QPixmap m_pixmap;
    QLabel *m_label{};
    QSize m_logical_size;
    QSize m_physical_size;
    int m_x;
    int m_y;
    BtnType m_type;

    /*
     img_path 填 QDir::currentPath() 之后的部分就可以，
     length 为按钮的长度，单位为像素，是逻辑像素
     */
    explicit SingleToolBtn(QWidget *parent, const std::string& img_name, int length, qreal dpi_ratio, int x, int y);
    ~SingleToolBtn() override;

    void update_label(int x, int y);
    void set_type(const std::string& type);
    void set_image(const std::string& img_mode);   // img_mode 为 normal 或 clicked
    static void exit_app();
protected:
    void mousePressEvent(QMouseEvent *event) override;
signals:
    void changeChosenSignal(BtnType type);
    void changeScaleSignal(bool is_bigger);     // is_bigger: 是否放大
    void BtnClickedSignal(bool is_brush_btn);   // 这个信号是专门给BrushTools用的
public slots:
    void OnChangeToCursorModeEvent();
};