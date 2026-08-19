#pragma once

#include <QWidget>
#include <QTimer>
#include <QColor>
#include <QVector>
#include <QObject>
#include <QLabel>

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

    void change_pen_color(QColor color, QObject *obj);   // 改变画笔颜色

    bool eventFilter(QObject *obj, QEvent *event) override;   // 事件过滤器，用于处理鼠标事件

    QVector<QLabel*> m_labels;   // 存储画笔工具按钮的指针
    QVector<QString> m_base_sytles;   // 存储画笔工具按钮的初始样式

signals:
    void changePenColorSignal(QColor color);   // 信号，用于通知主窗口改变画笔颜色

public slots:
    void OnBtnClickedEvent(); 
    void OnBtnHidedEvent();   
};