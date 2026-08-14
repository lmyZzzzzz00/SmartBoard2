#include <QApplication>
#include <QGuiApplication>
#include <QDebug>
#include <QDir>

#include "../h/Board.h"
#include "../h/ToolButtons.h"
#include "../h/BtnTypes.h"
#include "../h/GestureBrush.h"

#include <iostream>

#ifdef Q_OS_WIN
#include <windows.h>
#endif  // Q_OS_WIN

int main(int argc, char* argv[])
{

#ifdef Q_OS_WIN
    SetConsoleOutputCP(65001);  // 输出用 UTF-8
    SetConsoleCP(65001);        // 输入也用 UTF-8
#endif // Q_OS_WIN

    QApplication app(argc, argv);

    std::cout << "argc: " << argc << std::endl;
    std::cout << "*argv: " << *argv << std::endl;
    if (argc > 1) {
        std::cout << "检测到其余参数！" << std::endl;
        //  ##################### 日后补全图片加载！ #####################
    }

    // ── 图片路径（替换为你的实际图片） ──
    const QString imgPath = QDir::currentPath() + "/../a.png";
    std::cout << "imgPath: " << imgPath.toStdString() << std::endl;

    Board board(imgPath, 800, 600);
    board.setWindowTitle(QStringLiteral("SmartBoard"));
    if (board.m_window_mode == QString("full")) {
        // 全屏
        board.showFullScreen();
    }
    else {
        board.resize(800, 600);
        board.move(150, 20);
    }
    board.show();

    ToolButtons toolButtons(&board);

    // 绑定信号与槽
    QObject::connect(&board, &Board::resized, &toolButtons, &ToolButtons::OnResizeEvent);
    QObject::connect(&toolButtons, &ToolButtons::changeBtnModeSignal, &board, &Board::OnChangeBtnModeEvent);
    QObject::connect(toolButtons.m_rotate_btn, &SingleToolBtn::changeChosenSignal,
                 [&board](const BtnType type) {
                     if (type == ROTATE_BTN) board.OnRotateEvent();
                 });
    for (int i = 0; i < BtnCount; ++i) {
        QObject::connect(toolButtons.m_btn_array[i], &SingleToolBtn::changeChosenSignal, board.m_gestureBrush, &GestureBrush::OnChangeBtnModeEvent);
    }

    QObject::connect(toolButtons.m_bigger_btn, &SingleToolBtn::changeScaleSignal, &board, &Board::OnChangeScaleEvent);
    QObject::connect(toolButtons.m_smaller_btn, &SingleToolBtn::changeScaleSignal, &board, &Board::OnChangeScaleEvent);
    QObject::connect(&board, &Board::changeToCursorModeSignal, toolButtons.m_bigger_btn, &SingleToolBtn::OnChangeToCursorModeEvent);
    QObject::connect(&board, &Board::changeToCursorModeSignal, toolButtons.m_smaller_btn, &SingleToolBtn::OnChangeToCursorModeEvent);
    QObject::connect(&board, &Board::changeToCursorModeSignal, toolButtons.m_rotate_btn, &SingleToolBtn::OnChangeToCursorModeEvent);

    board.update();

    return QApplication::exec();   // Qt 6: exec()，不是 exec_()
}
