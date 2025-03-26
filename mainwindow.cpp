#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QPainter>
#include <QProgressBar>
#include <QThread>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initValue();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);//设置电脑指针为漏斗状
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    button->deleteLater();
    if (button) {
                // 创建进度条
                QProgressBar *progressBar = new QProgressBar(this);
                progressBar->setStyleSheet("QProgressBar::chunk"
                                            "{"
                                            "    border-radius:11px;"
                                            "    background:qlineargradient(spread:pad,x1:0,y1:0,x2:1,y2:0,stop:0 #01FAFF,stop:1  #26B4FF);"
                                            "}"
                                            "QProgressBar"
                                            "{"
                                                "height:22px;"
                                               " text-align:center;/*文本位置*/"
                                               " font-size:14px;"
                                               " color:white;"
                                                "border-radius:11px;"
                                               " background: #1D5573 ;"
                                            "}");
                progressBar->setGeometry(button->pos().x()-50, button->pos().y(), 200, 30);
                progressBar->setMinimumSize(100,30);
                progressBar->setTextVisible(true);
                progressBar->setFormat("%p%");
                progressBar->setRange(0, 100); // 设置进度条范围
                progressBar->setValue(0); // 初始值为0

                // 将进度条放置在原按钮的位置
                progressBar->setGeometry(button->geometry());
                progressBar->show();

                // 删除按钮
                button->deleteLater();
                // 模拟进度更新
                for (int i = 0; i <= 100; ++i) {
                    progressBar->setValue(i);
                    QCoreApplication::processEvents(); // 更新界面
                    if(i!=100){
                        QThread::msleep(10); // 等待一小段时间，模拟进度更新
                    }
                }
     }
    //关闭当前页面
    this->close();
    window = new BannaWidget;
    window->showNormal();

    QApplication::restoreOverrideCursor(); //还原电脑指针
}

// 初始化
void MainWindow::initValue()
{
    ui->widgetTitle->setParentWidget(this);
    ui->widgetTitle->setStyleSheet("color:white");
    this->setWindowFlags(Qt::FramelessWindowHint);
}


