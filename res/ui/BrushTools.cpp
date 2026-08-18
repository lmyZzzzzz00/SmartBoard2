#include "BrushTools.h"
#include "BrushToolsUI.h"
#include <iostream>
#include "../../scr/h/ToolButtons.h"

BrushTools::BrushTools(ToolButtons *parent)
    : QWidget(parent->parentWidget()),   // 注意：这里需要将父窗口设置为祖父窗口(Board)
    ui(new Ui::BrushToolsUI),
    m_parent(parent)   // 但是实际调用的接口父窗口是ToolButtons
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;"); // 设置背景颜色为白色
    // ✅ 修改这里：使用 Popup 或 Tool 代替 WindowStaysOnTopHint
    // Qt::Popup: 点击外部自动隐藏，且必定随父窗口销毁
    // Qt::Tool | Qt::FramelessWindowHint: 浮动工具栏行为，绑定父窗口生命周期
    this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint); // 设置窗口标志
    // 重新链接图片
    ui->pencil_btn->setPixmap(QPixmap("../res/btn_image/PencilIcon.png"));
    ui->pen_btn->setPixmap(QPixmap("../res/btn_image/PenIcon.png"));
    ui->brush_btn->setPixmap(QPixmap("../res/btn_image/BrushIcon.png"));
    ui->ThinBrushBtn->setPixmap(QPixmap("../res/btn_image/ThinBrushBtn.png"));
    ui->MiddleBrushBtn->setPixmap(QPixmap("../res/btn_image/MiddleBrushBtn.png"));
    ui->ThickBrushBtn->setPixmap(QPixmap("../res/btn_image/ThickBrushBtn.png"));
    std::cout << "BrushTools created" << std::endl;
}

BrushTools::~BrushTools(){
    delete ui;
}

void BrushTools::OnBtnClickedEvent(){
    if (m_parent->m_now_btn_type == BRUSH_BTN) {
        if (m_first_clicked) {
            // 获取按钮在全局屏幕中的位置
            QPoint btnGlobalPos = m_parent->m_brush_btn->mapToGlobal(QPoint(0, 0));
            int off_x = (this->width() - m_parent->m_brush_btn->width()) / 2;
            int off_y = 10 + this->height();
            move(btnGlobalPos.x() - off_x, btnGlobalPos.y() - off_y);
            show();
            m_first_clicked = false;
        }
        else {
            hide();
            m_first_clicked = true;
        }
    }
    else {
        m_first_clicked = false;
    }
    // TODO: 在此添加代码，完善功能
}

void BrushTools::OnBtnHidedEvent(){
    // 这个函数用于外部点击（失去焦点，直接在画笔模式下单击画布）的时候隐藏
    std::cout << "OnBtnHidedEvent" << std::endl;
    if (m_parent->m_now_btn_type == BRUSH_BTN) {
        m_first_clicked = true;
    }
    else {
        m_first_clicked = false;
    }
    hide();
}
