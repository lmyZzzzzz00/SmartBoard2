/********************************************************************************
** Form generated from reading UI file 'BrushToolsUI.ui'
**
** Created by: Qt User Interface Compiler version 6.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef BRUSHTOOLSUI_H
#define BRUSHTOOLSUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BrushToolsUI
{
public:
    QLabel *bg_label;
    QWidget *verticalWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *BrushesAndThicknessForm;
    QLabel *pencil_btn;
    QLabel *ThickBrushBtn;
    QLabel *brush_btn;
    QLabel *pen_btn;
    QLabel *ThinBrushBtn;
    QLabel *MiddleBrushBtn;
    QGridLayout *ColorsForm;
    QLabel *orange_btn;
    QLabel *blue_btn;
    QLabel *light_blue_btn;
    QLabel *yellow_btn;
    QLabel *green_btn;
    QLabel *red_btn;
    QLabel *purple_btn;
    QLabel *black_btn;
    QLabel *white_btn;
    QLabel *dark_green_btn;
    QLabel *pink_btn;
    QLabel *grey_btn;

    void setupUi(QWidget *BrushToolsUI)
    {
        if (BrushToolsUI->objectName().isEmpty())
            BrushToolsUI->setObjectName("BrushToolsUI");
        BrushToolsUI->resize(217, 150);
        bg_label = new QLabel(BrushToolsUI);
        bg_label->setObjectName("bg_label");
        bg_label->setGeometry(QRect(0, 0, 217, 150));
        bg_label->setStyleSheet(QString::fromUtf8("#bg_label{\n"
"	background-color: rgb(255, 255, 255);\n"
"	border-radius: 5px;\n"
"}"));
        verticalWidget = new QWidget(BrushToolsUI);
        verticalWidget->setObjectName("verticalWidget");
        verticalWidget->setGeometry(QRect(0, 0, 217, 150));
        verticalLayout = new QVBoxLayout(verticalWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(10, 10, 10, 10);
        BrushesAndThicknessForm = new QGridLayout();
        BrushesAndThicknessForm->setObjectName("BrushesAndThicknessForm");
        BrushesAndThicknessForm->setContentsMargins(0, -1, 71, -1);
        pencil_btn = new QLabel(verticalWidget);
        pencil_btn->setObjectName("pencil_btn");
        pencil_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pencil_btn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/PencilIcon.png")));

        BrushesAndThicknessForm->addWidget(pencil_btn, 5, 0, 1, 1);

        ThickBrushBtn = new QLabel(verticalWidget);
        ThickBrushBtn->setObjectName("ThickBrushBtn");
        ThickBrushBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        ThickBrushBtn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/ThickBrushBtn.png")));

        BrushesAndThicknessForm->addWidget(ThickBrushBtn, 6, 2, 1, 1);

        brush_btn = new QLabel(verticalWidget);
        brush_btn->setObjectName("brush_btn");
        brush_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        brush_btn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/BrushIcon.png")));

        BrushesAndThicknessForm->addWidget(brush_btn, 5, 2, 1, 1);

        pen_btn = new QLabel(verticalWidget);
        pen_btn->setObjectName("pen_btn");
        pen_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pen_btn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/PenIcon.png")));

        BrushesAndThicknessForm->addWidget(pen_btn, 5, 1, 1, 1);

        ThinBrushBtn = new QLabel(verticalWidget);
        ThinBrushBtn->setObjectName("ThinBrushBtn");
        ThinBrushBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        ThinBrushBtn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/ThinBrushBtn.png")));

        BrushesAndThicknessForm->addWidget(ThinBrushBtn, 6, 0, 1, 1);

        MiddleBrushBtn = new QLabel(verticalWidget);
        MiddleBrushBtn->setObjectName("MiddleBrushBtn");
        MiddleBrushBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        MiddleBrushBtn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/MiddleBrushBtn.png")));

        BrushesAndThicknessForm->addWidget(MiddleBrushBtn, 6, 1, 1, 1);


        verticalLayout->addLayout(BrushesAndThicknessForm);

        ColorsForm = new QGridLayout();
        ColorsForm->setObjectName("ColorsForm");
        orange_btn = new QLabel(verticalWidget);
        orange_btn->setObjectName("orange_btn");
        orange_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        orange_btn->setStyleSheet(QString::fromUtf8("#orange_btn{\n"
"	background-color: rgb(255, 170, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(orange_btn, 0, 1, 1, 1);

        blue_btn = new QLabel(verticalWidget);
        blue_btn->setObjectName("blue_btn");
        blue_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        blue_btn->setStyleSheet(QString::fromUtf8("#blue_btn{\n"
"	background-color: rgb(92, 108, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(blue_btn, 0, 5, 1, 1);

        light_blue_btn = new QLabel(verticalWidget);
        light_blue_btn->setObjectName("light_blue_btn");
        light_blue_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        light_blue_btn->setStyleSheet(QString::fromUtf8("#light_blue_btn{\n"
"	background-color: rgb(0, 255, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(light_blue_btn, 0, 4, 1, 1);

        yellow_btn = new QLabel(verticalWidget);
        yellow_btn->setObjectName("yellow_btn");
        yellow_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        yellow_btn->setStyleSheet(QString::fromUtf8("#yellow_btn{\n"
"	background-color: rgb(255, 255, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(yellow_btn, 0, 2, 1, 1);

        green_btn = new QLabel(verticalWidget);
        green_btn->setObjectName("green_btn");
        green_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        green_btn->setStyleSheet(QString::fromUtf8("#green_btn{\n"
"	background-color: rgb(0, 170, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(green_btn, 0, 3, 1, 1);

        red_btn = new QLabel(verticalWidget);
        red_btn->setObjectName("red_btn");
        red_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        red_btn->setStyleSheet(QString::fromUtf8("#red_btn{\n"
"	background-color: rgb(255, 0, 0);\n"
"	border-radius: 5px;\n"
"	border: 2px dashed black;\n"
"}"));

        ColorsForm->addWidget(red_btn, 0, 0, 1, 1);

        purple_btn = new QLabel(verticalWidget);
        purple_btn->setObjectName("purple_btn");
        purple_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        purple_btn->setStyleSheet(QString::fromUtf8("#purple_btn{\n"
"	background-color: rgb(170, 85, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(purple_btn, 1, 0, 1, 1);

        black_btn = new QLabel(verticalWidget);
        black_btn->setObjectName("black_btn");
        black_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        black_btn->setStyleSheet(QString::fromUtf8("#black_btn{\n"
"	background-color: rgb(0, 0, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(black_btn, 1, 4, 1, 1);

        white_btn = new QLabel(verticalWidget);
        white_btn->setObjectName("white_btn");
        white_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        white_btn->setStyleSheet(QString::fromUtf8("#white_btn{\n"
"	background-color: rgb(255, 255, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(white_btn, 1, 5, 1, 1);

        dark_green_btn = new QLabel(verticalWidget);
        dark_green_btn->setObjectName("dark_green_btn");
        dark_green_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        dark_green_btn->setStyleSheet(QString::fromUtf8("#dark_green_btn{\n"
"	background-color: rgb(0, 104, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(dark_green_btn, 1, 1, 1, 1);

        pink_btn = new QLabel(verticalWidget);
        pink_btn->setObjectName("pink_btn");
        pink_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pink_btn->setStyleSheet(QString::fromUtf8("#pink_btn{\n"
"	background-color: rgb(255, 0, 127);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(pink_btn, 1, 2, 1, 1);

        grey_btn = new QLabel(verticalWidget);
        grey_btn->setObjectName("grey_btn");
        grey_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        grey_btn->setStyleSheet(QString::fromUtf8("#grey_btn{\n"
"	background-color: rgb(170, 170, 170);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(grey_btn, 1, 3, 1, 1);


        verticalLayout->addLayout(ColorsForm);


        retranslateUi(BrushToolsUI);

        QMetaObject::connectSlotsByName(BrushToolsUI);
    } // setupUi

    void retranslateUi(QWidget *BrushToolsUI)
    {
        BrushToolsUI->setWindowTitle(QCoreApplication::translate("BrushToolsUI", "Form", nullptr));
        bg_label->setText(QString());
        pencil_btn->setText(QString());
        ThickBrushBtn->setText(QString());
        brush_btn->setText(QString());
        pen_btn->setText(QString());
        ThinBrushBtn->setText(QString());
        MiddleBrushBtn->setText(QString());
        orange_btn->setText(QString());
        blue_btn->setText(QString());
        light_blue_btn->setText(QString());
        yellow_btn->setText(QString());
        green_btn->setText(QString());
        red_btn->setText(QString());
        purple_btn->setText(QString());
        black_btn->setText(QString());
        white_btn->setText(QString());
        dark_green_btn->setText(QString());
        pink_btn->setText(QString());
        grey_btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BrushToolsUI: public Ui_BrushToolsUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BRUSHTOOLSUI_H
