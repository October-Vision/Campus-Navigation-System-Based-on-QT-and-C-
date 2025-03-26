/*****************************************
 * 功能：轮播图窗口
 * ***************************************/
#ifndef BANNAWIDGET_H
#define BANNAWIDGET_H

#include <QWidget>
#include "map.h"
#include <QKeyEvent>

namespace Ui {
class BannaWidget;
}

class BannaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BannaWidget(QWidget *parent = 0);
    ~BannaWidget();

private slots:
    void on_pushButton_clicked();
    void keyPressEvent(QKeyEvent* event);//enter进入

private:
    Ui::BannaWidget *ui;
    map *window;
};

#endif // BANNAWIDGET_H
