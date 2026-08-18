#pragma once

#include <QWidget>

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

public slots:
    void OnBtnClickedEvent(); 
    void OnBtnHidedEvent();   
};