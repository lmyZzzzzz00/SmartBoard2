#include "../h/ToolButtons.h"
#include "../h/SingleToolBtn.h"
#include "../../res/ui/BrushTools.h"
#include <QWidget>
#include <QDir>

#include <iostream>
#include "../h/BtnTypes.h"


// 这里的m_buttons_num-1是因为第一个m_exit_btn不算
ToolButtons::ToolButtons(QWidget *parent) : 
    m_btn_array{}, 
    m_buttons_num(BtnCount - 1), 
    m_brush_tools(new BrushTools(this))
{
    m_window_width = parent->width();
    m_window_height = parent->height();
    m_dpi_ratio = parent->devicePixelRatioF();

    m_exit_btn = new SingleToolBtn(parent, "exit_btn",
                                   m_target_length, m_dpi_ratio, 0, m_window_height - m_target_length);
    m_exit_btn->set_type("exit_btn");
    m_cursor_btn = new SingleToolBtn(parent, "cursor_btn",
                                     m_target_length, m_dpi_ratio, return_target_x(1),
                                     m_window_height - m_target_length);
    m_cursor_btn->set_type("cursor_btn");
    m_brush_btn = new SingleToolBtn(parent, "brush_btn",
                                    m_target_length, m_dpi_ratio, return_target_x(2),
                                    m_window_height - m_target_length);
    m_brush_btn->set_type("brush_btn");
    m_eraser_btn = new SingleToolBtn(parent, "eraser_btn",
                                    m_target_length, m_dpi_ratio, return_target_x(3),
                                    m_window_height - m_target_length);
    m_eraser_btn->set_type("eraser_btn");
    m_rotate_btn = new SingleToolBtn(parent, "rotate_btn",
                                    m_target_length, m_dpi_ratio, return_target_x(4),
                                    m_window_height - m_target_length);
    m_rotate_btn->set_type("rotate_btn");
    m_bigger_btn = new SingleToolBtn(parent, "bigger_btn",
                                    m_target_length, m_dpi_ratio, return_target_x(5),
                                    m_window_height - m_target_length);
    m_bigger_btn->set_type("bigger_btn");
    m_smaller_btn = new SingleToolBtn(parent, "smaller_btn",
                                    m_target_length, m_dpi_ratio, return_target_x(6),
                                    m_window_height - m_target_length);
    m_smaller_btn->set_type("smaller_btn");

    m_btn_array[0] = m_exit_btn;
    m_btn_array[1] = m_cursor_btn;
    m_btn_array[2] = m_brush_btn;
    m_btn_array[3] = m_eraser_btn;
    m_btn_array[4] = m_rotate_btn;
    m_btn_array[5] = m_bigger_btn;
    m_btn_array[6] = m_smaller_btn;

    // 绑定信号与槽
    for (int i=0; i < m_buttons_num+1; ++i) {
        connect(m_btn_array[i], &SingleToolBtn::changeChosenSignal, this, &ToolButtons::OnChangeChosenEvent);
        connect(m_btn_array[i], &SingleToolBtn::BtnClickedSignal, m_brush_tools, &BrushTools::OnBtnClickedEvent);
    }

    updateButtons();

    parent->update();

    // 默认模式是画笔
    OnChangeChosenEvent(BRUSH_BTN);
    m_now_btn_type = BRUSH_BTN;
}

ToolButtons::~ToolButtons() {
    if (m_exit_btn != nullptr && m_exit_btn->m_label != nullptr) {
        m_exit_btn->m_label->deleteLater();
        delete m_exit_btn;
    }
}

int ToolButtons::return_target_x(const int &index) const{
    const int target_x = (m_window_width - m_buttons_num * m_target_length) / 2 + (index - 1) * m_target_length;
    return target_x;
}

void ToolButtons::updateButtons() const {
    if (m_exit_btn != nullptr) {
        m_exit_btn->update_label(0, m_window_height - m_target_length);
    }
    for (int i = 1; i < m_buttons_num+1; ++i) {
        if (m_btn_array[i] != nullptr) {
            m_btn_array[i]->update_label(return_target_x(i), m_window_height - m_target_length);
        }
    }
}

void ToolButtons::OnResizeEvent(const int w, const int h) {
    // std::cout << "ToolButtons::OnResizeEvent，收到w,h分别为："<< w << ", " << h << std::endl;
    m_window_width = w;
    m_window_height = h;
    updateButtons();
}

void ToolButtons::OnChangeChosenEvent(const BtnType type) {
    m_now_btn_type = type;

    emit changeBtnModeSignal(m_now_btn_type);

    std::cout << "ToolButtons::OnChangeChosenEvent，收到type为：" << type << std::endl;

    for (int i = 1; i < m_buttons_num+1; ++i) {
        if (m_now_btn_type == i) {
            m_btn_array[i]->set_image("clicked");
        }
        else {
            m_btn_array[i]->set_image("normal");
        }
    }
}
