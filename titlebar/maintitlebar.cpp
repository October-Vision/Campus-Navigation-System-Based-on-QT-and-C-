/*****************************************
 * 功能：主标题栏，内含有窗体最大化，最小化，关闭
 * 以及窗口的Title，以及名称
 * ***************************************/
#include "maintitlebar.h"
#include "ui_maintitlebar.h"
#include <QDesktopServices>

static bool press = false;


MainTitleBar::MainTitleBar(QWidget *parent) :
    TitleBar(parent),
    ui(new Ui::MainTitleBar)
{
    ui->setupUi(this);
    ui->pushButtonNormalMax->setStyleSheet("QPushButton{border-image: url(:/images/images/normal_normal.png);}"
                                           "QPushButton:hover{border-image: url(:/images/images/normal_hover.png);}");
}

MainTitleBar::~MainTitleBar()
{
    delete ui;
}

void MainTitleBar::on_pushButtonClose_clicked()
{
    if(parentWidget)
    {
        parentWidget->close();
    }
}

void MainTitleBar::on_pushButtonMin_clicked()
{
    if(parentWidget)
    {
        parentWidget->showMinimized();
    }
}


void MainTitleBar::on_pushButtonNormalMax_clicked()
{
    if(parentWidget == nullptr) return;
    if (parentWidget->isMaximized())
    {
        parentWidget->showNormal();
        ui->pushButtonNormalMax->setStyleSheet("QPushButton{border-image: url(:/images/images/normal_normal.png);}"
                                               "QPushButton:hover{border-image: url(:/images/images/normal_hover.png);}");
    }
    else
    {
        parentWidget->showMaximized();
        ui->pushButtonNormalMax->setStyleSheet("QPushButton{border-image: url(:/images/images/max_normal.png);}"
                                               "QPushButton:hover{border-image: url(:/images/images/max_hover.png);}");
    }
}

void MainTitleBar::mousePressEvent(QMouseEvent *event)
{
    int x = ui->labelImage->pos().x();
    int y = ui->labelImage->pos().y();
    int w = ui->labelImage->width();
    int h = ui->labelImage->height();

    // 判断鼠标是否是左键按下,且鼠标位置是否在图片区域
    if (event->button() == Qt::LeftButton && event->pos().x() >= x && event->pos().x() <= x + w && event->pos().y() > y && event->pos().y() <= y + h)
    {
        press = true;
    }
}

void MainTitleBar::mouseReleaseEvent(QMouseEvent *event)
{

    int x = ui->labelImage->pos().x();
    int y = ui->labelImage->pos().y();
    int w = ui->labelImage->width();
    int h = ui->labelImage->height();
    // 判断鼠标是否是左键释放,且之前是在绘画区域
    if (event->button() == Qt::LeftButton && press && event->pos().x() >= x && event->pos().x() <= x + w && event->pos().y() > y && event->pos().y() <= y + h)
    {
        QDesktopServices::openUrl(QUrl("https://map.baidu.com/@13541529,3472914,16z"));
    }
    press = false;
}
