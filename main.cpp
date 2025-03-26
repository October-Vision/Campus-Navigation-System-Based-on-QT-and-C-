#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置全局样式为windowxp默认样式
    QApplication::setStyle("windowsxp");
    MainWindow w;
    w.show();
    return a.exec();
}
