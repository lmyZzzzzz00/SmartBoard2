#include "../h/SingleToolBtn.h"
#include <QDir>
#include <QLabel>

#include <iostream>
#include <QApplication>
#include "../h/BtnTypes.h"


SingleToolBtn::SingleToolBtn(
    QWidget *parent, const std::string& img_name, const int length,
    const qreal dpi_ratio, const int x, const int y):
    QLabel(parent),
    m_dpi_ratio(dpi_ratio),
    m_logical_size(QSize(length, length)),
    m_x(x),
    m_y(y),
    m_type(EXIT_BTN)
{
    const QString normal_img_path = QDir::currentPath() + "/../res/btn_image/" + QString::fromStdString(img_name) + ".png";
    m_normal_img = QImage(normal_img_path);
    const QString clicked_img_path = QDir::currentPath() + "/../res/btn_image/" + QString::fromStdString(img_name) + "_clicked.png";
    m_clicked_img = QImage(clicked_img_path);

    m_physical_size = m_logical_size * m_dpi_ratio;
    m_pixmap = QPixmap::fromImage(m_normal_img.scaled(m_physical_size,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_pixmap.setDevicePixelRatio(m_dpi_ratio);

    this->setFixedSize(m_logical_size);
    this->setPixmap(m_pixmap);
    this->setCursor(Qt::PointingHandCursor);
    this->move(m_x, m_y);
    this->show();
}

SingleToolBtn::~SingleToolBtn() {
    delete m_label;
}

void SingleToolBtn::update_label(const int x, const int y) {
    this->move(x, y);
    this->update();
}

void SingleToolBtn::set_type(const std::string& type) {
    if (type == "exit_btn") {
        m_type = EXIT_BTN;
    } else if (type == "cursor_btn") {
        m_type = CURSOR_BTN;
    } else if (type == "brush_btn") {
        m_type = BRUSH_BTN;
    } else if (type == "eraser_btn") {
        m_type = ERASER_BTN;
    } else if (type == "rotate_btn") {
        m_type = ROTATE_BTN;
    }else if (type == "bigger_btn") {
        m_type = BIGGER_BTN;
    } else if (type == "smaller_btn") {
        m_type = SMALLER_BTN;
    }
}

void SingleToolBtn::mousePressEvent(QMouseEvent *event) {
    std::cout << "SingleToolBtn::mousePressEvent, type:" << m_type << std::endl;
    if (m_type == EXIT_BTN) {
        emit changeChosenSignal(EXIT_BTN);
        exit_app();
    } else if (m_type == CURSOR_BTN) {
        emit changeChosenSignal(CURSOR_BTN);
    } else if (m_type == BRUSH_BTN) {
        emit changeChosenSignal(BRUSH_BTN);
    } else if (m_type == ERASER_BTN) {
        emit changeChosenSignal(ERASER_BTN);
    } else if (m_type == ROTATE_BTN) {
        emit changeChosenSignal(ROTATE_BTN);
    } else if (m_type == BIGGER_BTN) {
        emit changeChosenSignal(BIGGER_BTN);
        emit changeScaleSignal(true);
        std::cout << "放大" << std::endl;
    } else if (m_type == SMALLER_BTN) {
        emit changeChosenSignal(SMALLER_BTN);
        emit changeScaleSignal(false);
        std::cout << "缩小" << std::endl;
    }
}

void SingleToolBtn::set_image(const std::string &img_mode) {
    if (img_mode == "normal") {
        m_pixmap = QPixmap::fromImage(m_normal_img.scaled(m_physical_size,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else if (img_mode == "clicked") {
        m_pixmap = QPixmap::fromImage(m_clicked_img.scaled(m_physical_size,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    m_pixmap.setDevicePixelRatio(m_dpi_ratio);
    this->setPixmap(m_pixmap);
    this->update();
}

void SingleToolBtn::exit_app() {
    QApplication::quit();
}

void SingleToolBtn::OnChangeToCursorModeEvent() {
    emit changeChosenSignal(CURSOR_BTN);
}
