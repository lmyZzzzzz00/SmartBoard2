#pragma once

#include <QWidget>
#include <QTimer>

namespace Ui {
    class BrushToolsUI;   // 向前声明
}

class ToolButtons;   // 向前声明

class BrushTools : public QWidget{
    Q_OBJECT
public:
    explicit BrushTools(ToolButtons *parent = nullptr);
    ~BrushTools() noexcept;

private:
    Ui::BrushToolsUI *ui;
    bool m_first_clicked = true;   // 第一次点击画笔工具按钮，默认选中的就是画笔按钮
    ToolButtons *m_parent;

    QTimer *m_timer;   // 定时器，用于动画效果
    double m_alpha = 1.0;   // 透明度
    double m_alpha_speed = 0.2;   // 透明度变化速度
    int    m_alpha_direction = 1;   // 透明度变化方向，1表示增加，-1表示减少
    void anim();   // 动画效果

public slots:
    void OnBtnClickedEvent(); 
    void OnBtnHidedEvent();   
};