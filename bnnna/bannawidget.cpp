/*****************************************
 * 功能：轮播图窗口
 * ***************************************/
#include "bannawidget.h"
#include "ui_bannawidget.h"

// 构造函数
BannaWidget::BannaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BannaWidget)
{
    ui->setupUi(this);
}

// 析构函数
BannaWidget::~BannaWidget()
{
    delete ui;
}


void BannaWidget::on_pushButton_clicked()
{
    window = new map;
    window->showNormal();
    this->close();
}

void BannaWidget::keyPressEvent(QKeyEvent* event)
{

    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
               // 当按下 Enter 键时模拟按钮被点击
               ui->pushButton->click();
    }
}
