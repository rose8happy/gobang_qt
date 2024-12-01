#include <QApplication>
#include "GobangBoard.h"

int main(int argc, char *argv[])
{
    // 创建QApplication对象，必须在每个Qt应用程序中
    QApplication app(argc, argv);

    GobangBoard board;  // 创建五子棋棋盘窗口
    board.setWindowTitle("五子棋");  // 设置窗口标题
    board.show();  // 显示窗口

    // 进入Qt的事件循环，等待用户操作
    return app.exec();
}
