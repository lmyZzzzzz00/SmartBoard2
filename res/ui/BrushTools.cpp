#include "BrushTools.h"
#include "BrushToolsUI.h"
#include <iostream>
#include "../../scr/h/ToolButtons.h"

BrushTools::BrushTools(ToolButtons *parent)
    : QWidget(parent->parentWidget()),   // 注意：这里需要将父窗口设置为祖父窗口(Board)
    ui(new Ui::BrushToolsUI),
    m_parent(parent),   // 但是实际调用的接口父窗口是ToolButtons
    m_timer(new QTimer(this))
{
    ui->setupUi(this);
    // ✅ 核心修复：无边框 + 置顶于父窗口 + 无任务栏条目
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowDoesNotAcceptFocus);
    // ✅ 关键属性：确保真正无边框（去除 Windows DWM 残留边框）
    setAttribute(Qt::WA_ShowWithoutActivating);       // 显示时不抢焦点
    // ✅ 关键：这两行让窗口的透明区域真正透明（而非被系统背景填充）
    setAttribute(Qt::WA_TranslucentBackground);   // 允许窗口背景透明
    setAttribute(Qt::WA_NoSystemBackground);       // 禁止系统绘制默认背景
    ui->bg_label->setStyleSheet("background-color: white; border-radius: 10px; border: 1px solid gray;");
    // 重新链接图片
    ui->pencil_btn->setPixmap(QPixmap("../res/btn_image/PencilIcon.png"));
    ui->pen_btn->setPixmap(QPixmap("../res/btn_image/PenIcon.png"));
    ui->brush_btn->setPixmap(QPixmap("../res/btn_image/BrushIcon.png"));
    ui->ThinBrushBtn->setPixmap(QPixmap("../res/btn_image/ThinBrushBtn.png"));
    ui->MiddleBrushBtn->setPixmap(QPixmap("../res/btn_image/MiddleBrushBtn.png"));
    ui->ThickBrushBtn->setPixmap(QPixmap("../res/btn_image/ThickBrushBtn.png"));
    std::cout << "BrushTools created" << std::endl;

    setWindowOpacity(0);
    show();

    // 定时器
    m_timer->setInterval(16);
    connect(m_timer, &QTimer::timeout, this, &BrushTools::anim);
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
            m_alpha_direction = 1;   // 增加透明度
            m_first_clicked = false;
        }
        else {
            m_alpha_direction = -1;   // 减少透明度
            m_first_clicked = true;
        }
    }
    else {
        m_alpha_direction = -1;   // 减少透明度
        m_first_clicked = false;
    }
    m_timer->start();
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
    m_alpha_direction = -1;   // 减少透明度
    m_timer->start();
}

void BrushTools::anim() {
    const double now_alpha = windowOpacity();
    double new_alpha;

    constexpr double EPSILON = 1e-6; // 浮点比较容差

    if (m_alpha_direction == 1) {
        new_alpha = now_alpha + (1.0 - now_alpha) * m_alpha_speed;
        // ✅ 用容差代替精确比较
        if (new_alpha >= 1.0 - EPSILON) {
            new_alpha = 1.0;       // 强制钳位到精确值
            m_timer->stop();
        }
    } else {
        new_alpha = now_alpha - now_alpha * m_alpha_speed;
        if (new_alpha <= EPSILON) {
            new_alpha = 0.0;
            m_timer->stop();
        }
    }

    setWindowOpacity(new_alpha);
}