#ifndef MAP_H
#define MAP_H
#include <cmath>
#include <fstream>
#include <QMainWindow>
#include <QPropertyAnimation>
//#include <opencv2/opencv.hpp>

namespace Ui {
class map;
}

class map : public QMainWindow
{
    Q_OBJECT

public:

    explicit map(QWidget *parent = nullptr);
    int action;          //动作(放大,缩小,移动...)
    int pixW;            //图片宽
    int pixH;            //图片高
    float ratio;         //比例
    QPoint offset;       //一次的图片偏移值
    QPoint Alloffset;    //总偏移
    int tp=0;//控制符
    int lineNum=0,pointNum=0;
    double matrix[51][51]={};
    bool reach[51]={};
    enum  Type {
            None= 0,
            Amplification ,
            Shrink,
            Lift,
            Right,
            Up,
            Down,
            Move
        };
    bool inrect(int x,int y );
    void DFS(int p1,int p2);
    bool isin(QPoint x,QPoint y,int n=10);
    double dist(QPoint a,QPoint b);
    void clear();
    ~map();


private slots:

    void on_addPathBt_clicked();
    bool event(QEvent * event);
    void paintEvent(QPaintEvent * );//绘图函数
    void mousePressEvent(QMouseEvent* event);//主要功能实现
    void keyPressEvent(QKeyEvent* event);//wasd控制移动
    void wheelEvent(QWheelEvent* event);//滑轮放大缩小
    void on_addPointBt_clicked();
    void on_allandshortestBt_clicked();
    void on_changePathlenBt_clicked();
    void on_changePnameBt_clicked();
    void on_delPathBt_clicked();
    void on_delPointBt_clicked();
    void on_showPathlenBt_clicked();
    void on_changeBackBt_clicked();
    void on_changeTitleBt_clicked();
    void on_clearTitleBt_clicked();
    void on_saveMapBt_clicked();
    void on_installMapBt_clicked();

private:
    Ui::map *ui;
    void initValue();//初始化上栏
};

#endif // MAP_H
