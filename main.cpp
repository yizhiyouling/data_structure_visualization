#include "MainWindow.h"
#include <QApplication>

// 程序入口：创建 QApplication 对象并启动主窗口
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // 初始化 QApplication
    MainWindow w;                // 创建主窗口对象
    w.show();                    // 显示主窗口
    return a.exec();             // 进入应用事件循环
}
