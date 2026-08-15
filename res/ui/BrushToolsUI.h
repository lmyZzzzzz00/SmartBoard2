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

class Ui_MainVL
{
public:
    QVBoxLayout *verticalLayout_3;
    QGridLayout *BrushesAndThicknessForm;
    QLabel *brush_btn;
    QLabel *pen_btn;
    QLabel *pencil_btn;
    QLabel *ThinBrushBtn;
    QLabel *MiddleBrushBtn;
    QLabel *ThickBrushBtn;
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

    void setupUi(QWidget *MainVL)
    {
        if (MainVL->objectName().isEmpty())
            MainVL->setObjectName("MainVL");
        MainVL->resize(217, 150);
        MainVL->setStyleSheet(QString::fromUtf8("#MainVL{\n"
"	background-color: rgb(225, 225, 225);\n"
"}"));
        verticalLayout_3 = new QVBoxLayout(MainVL);
        verticalLayout_3->setObjectName("verticalLayout_3");
        BrushesAndThicknessForm = new QGridLayout();
        BrushesAndThicknessForm->setObjectName("BrushesAndThicknessForm");
        BrushesAndThicknessForm->setContentsMargins(0, -1, 71, -1);
        brush_btn = new QLabel(MainVL);
        brush_btn->setObjectName("brush_btn");
        brush_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        brush_btn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/BrushIcon.png")));

        BrushesAndThicknessForm->addWidget(brush_btn, 5, 2, 1, 1);

        pen_btn = new QLabel(MainVL);
        pen_btn->setObjectName("pen_btn");
        pen_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pen_btn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/PenIcon.png")));

        BrushesAndThicknessForm->addWidget(pen_btn, 5, 1, 1, 1);

        pencil_btn = new QLabel(MainVL);
        pencil_btn->setObjectName("pencil_btn");
        pencil_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pencil_btn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/PencilIcon.png")));

        BrushesAndThicknessForm->addWidget(pencil_btn, 5, 0, 1, 1);

        ThinBrushBtn = new QLabel(MainVL);
        ThinBrushBtn->setObjectName("ThinBrushBtn");
        ThinBrushBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        ThinBrushBtn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/ThinBrushBtn.png")));

        BrushesAndThicknessForm->addWidget(ThinBrushBtn, 6, 0, 1, 1);

        MiddleBrushBtn = new QLabel(MainVL);
        MiddleBrushBtn->setObjectName("MiddleBrushBtn");
        MiddleBrushBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        MiddleBrushBtn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/MiddleBrushBtn.png")));

        BrushesAndThicknessForm->addWidget(MiddleBrushBtn, 6, 1, 1, 1);

        ThickBrushBtn = new QLabel(MainVL);
        ThickBrushBtn->setObjectName("ThickBrushBtn");
        ThickBrushBtn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        ThickBrushBtn->setPixmap(QPixmap(QString::fromUtf8("../btn_image/ThickBrushBtn.png")));

        BrushesAndThicknessForm->addWidget(ThickBrushBtn, 6, 2, 1, 1);


        verticalLayout_3->addLayout(BrushesAndThicknessForm);

        ColorsForm = new QGridLayout();
        ColorsForm->setObjectName("ColorsForm");
        orange_btn = new QLabel(MainVL);
        orange_btn->setObjectName("orange_btn");
        orange_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        orange_btn->setStyleSheet(QString::fromUtf8("#orange_btn{\n"
"	background-color: rgb(255, 170, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(orange_btn, 0, 1, 1, 1);

        blue_btn = new QLabel(MainVL);
        blue_btn->setObjectName("blue_btn");
        blue_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        blue_btn->setStyleSheet(QString::fromUtf8("#blue_btn{\n"
"	background-color: rgb(92, 108, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(blue_btn, 0, 5, 1, 1);

        light_blue_btn = new QLabel(MainVL);
        light_blue_btn->setObjectName("light_blue_btn");
        light_blue_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        light_blue_btn->setStyleSheet(QString::fromUtf8("#light_blue_btn{\n"
"	background-color: rgb(0, 255, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(light_blue_btn, 0, 4, 1, 1);

        yellow_btn = new QLabel(MainVL);
        yellow_btn->setObjectName("yellow_btn");
        yellow_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        yellow_btn->setStyleSheet(QString::fromUtf8("#yellow_btn{\n"
"	background-color: rgb(255, 255, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(yellow_btn, 0, 2, 1, 1);

        green_btn = new QLabel(MainVL);
        green_btn->setObjectName("green_btn");
        green_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        green_btn->setStyleSheet(QString::fromUtf8("#green_btn{\n"
"	background-color: rgb(0, 170, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(green_btn, 0, 3, 1, 1);

        red_btn = new QLabel(MainVL);
        red_btn->setObjectName("red_btn");
        red_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        red_btn->setStyleSheet(QString::fromUtf8("#red_btn{\n"
"	background-color: rgb(255, 0, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(red_btn, 0, 0, 1, 1);

        purple_btn = new QLabel(MainVL);
        purple_btn->setObjectName("purple_btn");
        purple_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        purple_btn->setStyleSheet(QString::fromUtf8("#purple_btn{\n"
"	background-color: rgb(170, 85, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(purple_btn, 1, 0, 1, 1);

        black_btn = new QLabel(MainVL);
        black_btn->setObjectName("black_btn");
        black_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        black_btn->setStyleSheet(QString::fromUtf8("#black_btn{\n"
"	background-color: rgb(0, 0, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(black_btn, 1, 4, 1, 1);

        white_btn = new QLabel(MainVL);
        white_btn->setObjectName("white_btn");
        white_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        white_btn->setStyleSheet(QString::fromUtf8("#white_btn{\n"
"	background-color: rgb(255, 255, 255);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(white_btn, 1, 5, 1, 1);

        dark_green_btn = new QLabel(MainVL);
        dark_green_btn->setObjectName("dark_green_btn");
        dark_green_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        dark_green_btn->setStyleSheet(QString::fromUtf8("#dark_green_btn{\n"
"	background-color: rgb(0, 104, 0);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(dark_green_btn, 1, 1, 1, 1);

        pink_btn = new QLabel(MainVL);
        pink_btn->setObjectName("pink_btn");
        pink_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        pink_btn->setStyleSheet(QString::fromUtf8("#pink_btn{\n"
"	background-color: rgb(255, 0, 127);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(pink_btn, 1, 2, 1, 1);

        grey_btn = new QLabel(MainVL);
        grey_btn->setObjectName("grey_btn");
        grey_btn->setCursor(QCursor(Qt::CursorShape::PointingHandCursor));
        grey_btn->setStyleSheet(QString::fromUtf8("#grey_btn{\n"
"	background-color: rgb(170, 170, 170);\n"
"	border-radius: 5px;\n"
"}"));

        ColorsForm->addWidget(grey_btn, 1, 3, 1, 1);


        verticalLayout_3->addLayout(ColorsForm);


        retranslateUi(MainVL);

        QMetaObject::connectSlotsByName(MainVL);
    } // setupUi

    void retranslateUi(QWidget *MainVL)
    {
        MainVL->setWindowTitle(QCoreApplication::translate("MainVL", "Form", nullptr));
        brush_btn->setText(QString());
        pen_btn->setText(QString());
        pencil_btn->setText(QString());
        ThinBrushBtn->setText(QString());
        MiddleBrushBtn->setText(QString());
        ThickBrushBtn->setText(QString());
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
    class MainVL: public Ui_MainVL {};
} // namespace Ui

QT_END_NAMESPACE

#endif // BRUSHTOOLSUI_H
