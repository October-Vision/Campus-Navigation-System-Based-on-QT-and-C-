#include "map.h"
#include "ui_map.h"
#include <QPainter>
#include <string.h>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QMessageBox>
#include <QFont>
#include <QFileDialog>
#include <QInputDialog>
#include <QDebug>
#include <QSequentialAnimationGroup>

//全局变量
int nowx,nowy;//现在图片显示的位置
QPoint P1,P2;//临时点,分别标记第一个选中点和第二个选中点
QPoint P[51];//地点数组
QString Pname[51],pic=":/images/绘图版";//名称数组，图片路径
int ways;//路径数量
double dis,min;//当前路径长度，最短路径长度
bool showlen = false;//是否显示路径长度
struct lines //线段结构体
{
    int a,b;//两点下标
    bool f=false;//是否是最短路，最短路红色显示，其他路线蓝色显示
} temp1,line[101];//当前线段，所有线段

//栈
class stack
{
public:
    int a[51]={};//点下标
    int num=0;//点个数
public:

    void push(int n){ a[++num]=n; }//入栈

    void pop(){ a[num--]=0; }//出栈

    QString getstr()//显示路径
    {
        QString str ="";
        for(int i=1;i<num;i++)
        {
            if(Pname[a[i]]!=" ")
            {
                str+=Pname[a[i]]+"->";
            }
        }
        if(Pname[a[num]]!=" ")
        {
            str+=Pname[a[num]];
        }
        return str;
    }

    void showline()//标红最短路径
    {
        for(int i=1;i<num;i++)//遍历点
        {
            for(int j=1;line[j].a && j<=100;j++)//遍历边且不超过边范围
            {
                if( (line[j].a==a[i] && line[j].b==a[i+1]) || (line[j].b==a[i] && line[j].a==a[i+1]))//如果线当中点的下标与最短路径栈中存储的点下标相同
                {
                    line[j].f=true;
                    break;
                }
            }
        }
    }

}stack,temp2;//temp存储最短路径点

int count=0;//添加点

double map::dist(QPoint a, QPoint b)//计算两点距离
{
    return sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
}

bool map::isin(QPoint x, QPoint y, int n)//是否选中
{
    int bpointnowx,bpointnowy;//当前图点大小
    bpointnowx = (y.x()-451)*ratio+nowx;//计算现在点的坐标
    bpointnowy= (y.y()-210)*ratio+nowy;
    QPoint y1 = QPoint(bpointnowx,bpointnowy);
    if (dist(x, y1) <= n)
    {
        return true;
    }
    return false;
}

void map::DFS(int p1, int p2)
{
    reach[p1] = true;//起点到达
    stack.push(p1);//将起点压入栈内
    if (p1 == p2)//若已经到终点
    {
        if (ways <= 200000) ui->messagelist->addItem(stack.getstr() + "，长度为：" + QString::number(dis)), ways++;
        if (min == 0 || dis < min)//不断更新最短路径
        {
            min = dis;
            temp2 = stack;
        }
    }
    else//未到达终点
    {
        for (int t = 1; t <=pointNum ; t++)
            if (matrix[p1][t] && !reach[t])//路径存在，且未曾达到
            {
                dis += matrix[p1][t];//当前路径长度＋目前点到下一个点的距离
                DFS(t, p2);//继续走
                dis -= matrix[p1][t];//更换新路径，将此前路径长度减去
            }
    }
    stack.pop();//将最新的点弹出
    reach[p1] = false;//因为此点弹出，故未达到
}

void map::clear()//清除右侧显示栏的显示信息
{
    ui->PointLabel->setText("");//清除点标签信息
    ui->lineLabel->setText("");//清除路径标签信息
    ui->messagelist->clear();//清除显示路径栏信息
    for (int i = 1; i <= lineNum; i++)
    {
        if (line[i].f)
        {
            line[i].f = false;//红线变回蓝线
        }
    }
    update();//调用paintEvent函数
}

void map::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    int width = this->width();
    int height = this->height();
    int cutoutHeight = 950;
    int cutoutWidth = 1520;
    int cutoutX = 451;
    int cutoutY = 210;
    QString backgroundpic = ":/images/主页面背景";
    /*QLinearGradient gradient(0, 0, width, height);
    gradient.setColorAt(0, QColor(210, 180, 140,210));
    gradient.setColorAt(1, QColor(245, 245, 220,210));*/
    painter.fillRect(0, 0, cutoutX, height, QBrush(QColor(255, 255, 255)));  // 左侧留白
    painter.fillRect(cutoutX + cutoutWidth, 0, width - cutoutX - cutoutWidth, height, QBrush(QColor(255, 255, 255)));  // 右侧留白
    painter.fillRect(cutoutX, 0, cutoutWidth, cutoutY, QBrush(QColor(255, 255, 255)));  // 上方留白
    painter.fillRect(cutoutX, cutoutY + cutoutHeight, cutoutWidth, height - cutoutY - cutoutHeight, QBrush(QColor(255, 255, 255)));  // 下方留白
    /*painter.fillRect(0, 0, cutoutX, height, gradient);
    painter.fillRect(cutoutX + cutoutWidth, 0, width - cutoutX - cutoutWidth, height, gradient);
    painter.fillRect(cutoutX, 0, cutoutWidth, cutoutY, gradient);
    painter.fillRect(cutoutX, cutoutY + cutoutHeight, cutoutWidth, height - cutoutY - cutoutHeight, gradient);*/
    painter.drawPixmap(0, 0, width, height, QPixmap(backgroundpic));//载入背景图片并显示出来

    int NowW = ratio*1520;
    int NowH = ratio*950;
    QRect clippingRect(451, 210, 1520, 950); //画布大小
    painter.setClipRect(clippingRect);
    if(action==map::Shrink)           //缩小
    {
        ratio -= 0.1*ratio;
        if(ratio<0.18)
           ratio = 0.1;
        NowW = ratio *pixW;
        NowH = ratio *pixH;
        action=map::None;
    }
    else  if(action==map::Amplification)           //放大
    {
       ratio += 0.1*ratio;
       if(ratio>4.5)
          ratio = 5.000;
       NowW = ratio *pixW;
       NowH = ratio *pixH;
       action=map::None;
    }
    if (action == map::Move)
    {
        int offsetx = Alloffset.x()+offset.x();
        Alloffset.setX(offsetx);
        int offsety = Alloffset.y()+offset.y();
        Alloffset.setY(offsety);
        action = map::None;
    }

    nowx = 451+(1520-NowW)/2+Alloffset.x();
    nowy = 210+(950-NowH)/2+Alloffset.y();
    painter.drawPixmap(nowx, nowy, NowW,NowH, QPixmap(pic));//载入背景图片并显示出来
    QFont font1("Microsoft YaHei", 10);//字体说明
    QFont font2("Microsoft YaHei", 15);
    painter.drawRect(451, 210, 1520, 950);//矩形大小，窗口大小
    painter.setFont(font1);
    // 创建一个QPen对象，并设置颜色为蓝色
    QPen bluepen(Qt::blue);
    // 设置画笔的宽度
    bluepen.setWidth(2);
    // 创建一个QPen对象，并设置颜色为红色
    QPen redpen(Qt::red);
    // 设置画笔的宽度
    redpen.setWidth(2);
    painter.setRenderHint(QPainter::Antialiasing, true);//使接下来的绘图光滑
    int apointnowx,apointnowy,bpointnowx,bpointnowy;//当前图点大小
    for (int i = 1; i <= lineNum; i++)
    {
        apointnowx = (P[line[i].a].x()-451)*ratio+nowx;//计算现在点的坐标
        apointnowy= (P[line[i].a].y()-210)*ratio+nowy;
        bpointnowx = (P[line[i].b].x()-451)*ratio+nowx;//计算现在点的坐标
        bpointnowy= (P[line[i].b].y()-210)*ratio+nowy;
        if (!line[i].f) painter.setPen(bluepen);//设置画笔颜色为蓝色
        else painter.setPen(redpen);
        if(inrect(apointnowx,apointnowy) && inrect(bpointnowx,bpointnowy))//只显示画布内的线
        {
            painter.drawLine(apointnowx,apointnowy, bpointnowx,bpointnowy);//两点连线
        }
    }
    painter.setPen(Qt::darkMagenta);
    if (showlen)//如果显示路径
    {
        for (int i = 1; i < pointNum; i++)
        {
            for (int j = i + 1; j <= pointNum; j++)
            {
                if (matrix[i][j])//若路径存在
                {
                    int x1, y1;
                    apointnowx = (P[i].x()-451)*ratio+nowx;//计算现在点的坐标
                    apointnowy= (P[i].y()-210)*ratio+nowy;
                    bpointnowx = (P[j].x()-451)*ratio+nowx;//计算现在点的坐标
                    bpointnowy= (P[j].x()-210)*ratio+nowy;
                    x1 = (apointnowx + bpointnowx) / 2 - 10*ratio;//路径中央偏左
                    y1 = (apointnowy+bpointnowy) / 2 + 4*ratio;//路径中央偏下
                    //bug：x1y1坐标算法错误，但找不到正确算法
                    if(inrect(x1,y1))//只显示画布内的路径长度
                    {
                        painter.drawText(QPoint(x1, y1), QString::number(matrix[i][j]));//路径长度显示在（x1,y1）位置处
                    }

                }
            }
        }
    }
    painter.setPen(Qt::black);
    painter.setBrush(Qt::yellow);
    painter.setFont(font2);
    for (int i = 1; i <= pointNum; i++)
    {
        int pointnowx = (P[i].x()-451)*ratio+nowx;//计算现在点的坐标
        int pointnowy= (P[i].y()-210)*ratio+nowy;
        if(inrect(pointnowx,pointnowy)){
            painter.drawEllipse(pointnowx,pointnowy, 4, 4);//把点画出来
            painter.drawText(QPoint(pointnowx+ 5, pointnowy + 6), Pname[i]);//画出点的标签
        }
    }
    ui->PointLabel->setText("点数：" + QString::number(pointNum));
    ui->lineLabel->setText("路径数：" + QString::number(lineNum));

}

void map::keyPressEvent(QKeyEvent* event)
{
    int x = ui->human->x();
    int y = ui->human->y();
    if(x<451||x>1971||y<90||y>1060)
    {
        QMessageBox::warning(this, "警告", "人物超出边界");
    }
    QPoint p;
    p.setX(x+30);
    p.setY(y+121);
    for(int i=1;i<=pointNum;i++)
    {
        if(isin(p,P[i]))
        {
            if(Pname[i]!=" ")
            {
                ui->label->setText("我来到了"+Pname[i]);
            }
        }
    }
    switch (event->key())
    {
        case Qt::Key_W : ui->human->move(x, y-10); break;
        case Qt::Key_S : ui->human->move(x, y+10); break;
        case Qt::Key_A : ui->human->move(x-10, y); break;
        case Qt::Key_D : ui->human->move(x+10, y); break;
        case Qt::Key_Z : ui->human->move(x-10, y+10); break;
        case Qt::Key_C : ui->human->move(x+10, y+10); break;
        case Qt::Key_Q : ui->human->move(x-10, y-10); break;
        case Qt::Key_E : ui->human->move(x+10, y-10); break;
     }
}

void map::mousePressEvent(QMouseEvent* event)//功能
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint p = event->pos();
        switch(tp)
        {
             case 1://选择添加路线第一个点
                if(lineNum==100){
                    QMessageBox::warning(this,"警告","路径数量已满");
                }
                else{
                    for (int i = 1; i <= pointNum; i++)//遍历点集合
                    {
                        if (isin(p, P[i]))//选中其中一点
                        {
                            P1 = P[i]; //
                            line[lineNum + 1].a = i; //新线段a点设置
                            tp = 2; //转到第二个
                            ui->underLabel->setText("请选择第二个点");
                            break;
                        }

                    }
                }
            break;

             case 2://选择添加路线第二个点
                    for(int i=1;i<=pointNum;i++)
                    {
                        if(P[i]!=P1 && isin(p,P[i]))//选中点与第一个不同
                        {
                            int t = lineNum++;//线数量+1
                            P2 = P[i]; line[lineNum].b = i; tp = 1;//P2信息录入，完善line[num1]信息，tp回归1.
                            if (line[lineNum].a > line[lineNum].b)//保证线的第一个点下标比第二个小
                            {
                                int t1 = line[lineNum].a; line[lineNum].a = line[lineNum].b; line[lineNum].b = t1;
                            }
                            for (int j = 1; j < lineNum; j++)//判断是否路线已经存在
                            {
                                if (line[lineNum].a == line[j].a && line[lineNum].b == line[j].b)
                                {
                                    line[lineNum--] = line[0];
                                    QMessageBox::warning(this, "警告", "两条路径重合");
                                    break;
                                }
                            }
                            if (t != lineNum)//将两点间的像素距离赋值给两点间的路径长度（默认）
                                matrix[line[lineNum].a][line[lineNum].b] = matrix[line[lineNum].b][line[lineNum].a] = dist(P[line[lineNum].a], P[line[lineNum].b]);
                            ui->underLabel->setText("请选择第一个点");
                            break;
                        }
                    }
                    update();//画线
            break;

             case 3://添加地点
                    if (pointNum < 50 && p.x() >= 451 && p.x() <= 1971 && p.y()>210 && p.y() <= 1160)//判断所加的点是否在窗口范围内
                    {
                        int t = pointNum++;
                        for (int i = 1; i < pointNum; i++)
                            if (isin(p, P[i], 20))//判断两点是否太近，故两点距离需要大于选中距离半径两倍
                            {
                                pointNum--;
                                QMessageBox::warning(this, "警告", "两个点靠太近");
                            }
                        if (t == pointNum) break;
                        P[pointNum] = event->pos();//当前位置赋给最新的P点
                        Pname[pointNum] = QString::number(++count);//创建默认标签
                        update();//画点
                     }
                    else if (pointNum == 50) QMessageBox::warning(this, "警告", "点数已达上限");
                    else QMessageBox::warning(this, "警告", "点超出边界");
            break;

              case 4://删除地点
                    if(pointNum==0){
                        QMessageBox::warning(this,"警告","没有可供删除的地点！");
                    }
                    else{
                        for(int i=1;i<=pointNum;i++)
                        {
                            if(isin(p,P[i]))
                            {
                                //分别删除名字数组、点数组、和邻接矩阵中对应位置
                                for(int j=i;j<pointNum;j++){
                                    P[j]=P[j+1];
                                    Pname[j]=Pname[j+1];
                                    for(int k=1;k<=pointNum;k++)
                                    {
                                        matrix[j][k]=matrix[j+1][k];
                                    }
                                }
                                for(int j=i;j<pointNum;j++)
                                {
                                    for(int k=1;k<=pointNum;k++)
                                    {
                                        matrix[k][j]=matrix[k][j+1];
                                    }
                                }
                                for(int j=1;j<=pointNum;j++)
                                {
                                     matrix[j][pointNum]=matrix[pointNum][j]=0;
                                }
                                P[pointNum]=P[0];
                                Pname[pointNum--]=Pname[0];
                                //删除对应线
                                for(int j=1;j<=lineNum;j++){
                                    if(i==line[j].a||i==line[j].b){
                                        for(int k=j;k<=lineNum;k++){
                                            line[k]=line[k+1];
                                        }
                                        line[lineNum--]=line[0];
                                        j--;//保证全部检查
                                    }
                                    else{//其他线如果点数大于i，需要前移一位
                                        if(line[j].a>i){
                                            line[j].a--;
                                        }
                                        if(line[j].b>i){
                                            line[j].b--;
                                        }
                                    }
                                }

                            }
                        }
                        update();
                        break;
                    }
            break;

               case 5://选择删除路线第一个点
                    if(lineNum==0){
                        QMessageBox::warning(this, "警告", "无路径");
                    }
                    else{
                        for (int i = 1; i <= pointNum; i++)
                        {
                            if (isin(p, P[i]))//判断选中
                            {
                                P1 = P[i]; temp1.a = i; tp = 6; ui->underLabel->setText("请选择要删除路径的第二个点");
                            }
                            break;
                        }
                    }
            break;

               case 6://选择删除路线第二个点
                    for(int i=1;i<=pointNum;i++){
                        if(P1!=P[i] && isin(p,P[i])){
                            P2=P[i];
                            temp1.b=i;
                            tp = 5;
                            if(temp1.a>temp1.b){
                                int a = temp1.a;
                                temp1.a=temp1.b;
                                temp1.b = a;
                            }
                            int t3 = lineNum;//保存路径数量
                            for (int j = 1; j <= lineNum; j++)
                            {
                                if (temp1.a == line[j].a && temp1.b == line[j].b)
                                {
                                    matrix[line[j].a][line[j].b] = matrix[line[j].b][line[j].a] = 0;//点之间路径长度删除
                                    for (int k = j; k < lineNum; k++)
                                    {
                                        line[k] = line[k + 1];//路径（线）下标前移
                                    }
                                    line[lineNum--] = line[0];
                                    break;
                                }
                            }
                            ui->underLabel->setText("请选择要删除路径的第一个点");
                            if (lineNum == t3) QMessageBox::warning(this, "警告", "路径不存在");
                            else break;
                        }
                    }
                    update();
            break;

               case 7://编辑点的标签
                    if(pointNum==0){
                        QMessageBox::warning(this, "警告", "无可编辑的地点");
                    }
                    else
                    {
                        for (int i = 1; i <= pointNum; i++)
                        {
                            if (isin(p, P[i]))
                            {
                                QString newPointName = QInputDialog::getText(this, "编辑标签", "输入文本(最多13个字)");
                                if (newPointName != "") Pname[i] = newPointName.left(13);
                                break;
                            }
                        }
                    }
            break;

               case 8://选择起点
                    for (int i = 1; i <= pointNum; i++)
                    {
                        if (isin(p, P[i]))
                        {
                            P1 = P[i]; tp = 9; ui->underLabel->setText("请选择终点");
                            temp1.a = i;//起点下标赋给temp1
                            ui->startPointLabel->setText("起点：" + Pname[i]);
                            ui->endPointLabel->setText("终点：");
                            break;
                        }
                    }
            break;

               case 9://选择终点
                    for (int i = 1; i <= pointNum; i++)
                    {
                        if (P[i] != P1 && isin(p, P[i]))//判断不与起点重合
                        {
                            P2 = P[i];
                            temp1.b = i;//终点下标赋给temp1
                            ui->endPointLabel->setText("终点：" + Pname[i]);
                            ui->messagelist->clear();
                            ways = 0;
                            min = 0;
                            DFS(temp1.a, temp1.b);
                            if (ways)//若有路
                            {
                                if (ways > 200000) ui->messagelist->addItem("路径数过多，超过200000条，无法完全显示！最短路径为：" + temp2.getstr() + "，长度为：" + QString::number(min));
                                else ui->messagelist->addItem("共" + QString::number(ways) + "条路径，其中最短路径为：" + temp2.getstr() + "，长度为：" + QString::number(min));
                                temp2.showline();//最短路径展示
                                update();
                                //最短路径移动
                                QSequentialAnimationGroup* sequence = new QSequentialAnimationGroup;
                                for(int i=1,j=2;i<temp2.num;i++,j++)
                                {
                                    QPoint a = QPoint((P[temp2.a[i]].x()-451)*ratio+nowx,(P[temp2.a[i]].y()-331)*ratio+nowy);
                                    QPoint b = QPoint((P[temp2.a[j]].x()-451)*ratio+nowx,(P[temp2.a[j]].y()-331)*ratio+nowy);
                                    QPropertyAnimation* m_animation = new QPropertyAnimation();
                                    m_animation->setTargetObject(ui->human);    //设置使用动画的控件
                                    m_animation->setEasingCurve(QEasingCurve::Linear); //设置动画效果
                                    m_animation->setPropertyName("pos");//指定动画属性名
                                    m_animation->setDuration(1000);    //设置动画时间（单位：毫秒）
                                    m_animation->setStartValue(a);  //设置动画起始位置在label控件当前的pos
                                    m_animation->setEndValue(b);
                                    sequence->addAnimation(m_animation);
                                }
                                // 启动动画组
                                sequence->start();
                            }
                            else ui->messagelist->addItem("找不到路径");
                            tp = 0;
                            ui->underLabel->setText("");
                            break;
                        }
                    }
            break;

               case 10://编辑路径长度第一个点
                    if (lineNum == 0) QMessageBox::warning(this, "警告", "无任何路径");
                    else
                    {
                        for (int i = 1; i <= pointNum; i++)
                        {
                            if (isin(p, P[i]))//选中
                            {
                                P1 = P[i]; tp = 11; ui->underLabel->setText("请选择要编辑路径长度的第二个点");
                                temp1.a = i;
                                break;
                            }
                        }
                    }
            break;

               case 11://编辑路径长度第二个点
                    for (int i = 1; i <= pointNum; i++)
                    {
                        if (P[i] != P1 && isin(p, P[i]))//不与第一个点重合
                        {
                            P2 = P[i];
                            temp1.b = i;
                            tp = 10;
                            if (temp1.a > temp1.b)//保证线的第二个点下标大于第一个
                            {
                                int t1 = temp1.a; temp1.a = temp1.b; temp1.b = t1;
                            }
                            bool f0 = false;
                            for (int j = 1; j <= lineNum; j++)
                            {
                                if (temp1.a == line[j].a && temp1.b == line[j].b)
                                {
                                    double number = QInputDialog::getDouble(this, "编辑长度", "输入浮点数(0.0001~999999)", matrix[temp1.a][temp1.b], 0.0001, 999999, 4);
                                    if (number) matrix[temp1.a][temp1.b] = matrix[temp1.b][temp1.a] = number;//若输入了数，则点之间长度更改
                                    f0 = true;
                                    break;
                                }
                            }
                            ui->underLabel->setText("请选择要编辑路径长度的第一个点");
                            if (!f0) QMessageBox::warning(this, "警告", "找不到这条路径");
                            update();
                            break;
                        }
                    }
            break;
        }
    }
}

map::map(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::map)
{
    ui->setupUi(this);
    ui->myself->setScaledContents(true);

    ratio= 1.0;             //初始化图片缩放比例
    action = map::None;
    pixH = 950;
    pixW = 1520;
    this->initValue();
}

map::~map()
{
    delete ui;
}


void map::wheelEvent(QWheelEvent* event)     //鼠标滑轮事件
{
    if (event->delta()>0 && inrect(event->pos().x(),event->pos().y())) {      //上滑,放大
        action=map::Amplification;
        this->update();

    } else if(event->delta()<=0 && inrect(event->pos().x(),event->pos().y())){                    //下滑,缩小
        action=map::Shrink;
        this->update();
    }

    event->accept();
}


void map::on_addPointBt_clicked()
{
    clear();
    if (tp != 3) tp = 3, ui->underLabel->setText("请选择一个位置添加点");//转到case 3
    else tp = 0, ui->underLabel->setText("");//跳出
}

void map::on_delPointBt_clicked()
{
    clear();
    if (tp != 4) tp = 4, ui->underLabel->setText("请选择一个位置删除点");//转到case 4
    else tp = 0, ui->underLabel->setText("");//跳出
}

void map::on_addPathBt_clicked()
{
    clear();
    if (tp != 1) tp = 1, ui->underLabel->setText("请选择第一个点");//转到case 1
    else tp = 0, ui->underLabel->setText("");//跳出
}

void map::on_delPathBt_clicked()
{
    clear();
    if (tp != 5) tp = 5, ui->underLabel->setText("请选择要删除路径的第一个点");//转到case 5
    else tp = 0, ui->underLabel->setText("");//跳出
}

void map::on_changePnameBt_clicked()
{
    clear();
    if (tp != 7) tp = 7, ui->underLabel->setText("请选择要编辑标签的点");//转到case 7
    else tp = 0, ui->underLabel->setText("");//跳出
}

void map::on_changePathlenBt_clicked()
{
    clear();
    if (tp != 10) tp = 10, ui->underLabel->setText("请选择要编辑路径长度的第一个点");//转到case 10
    else tp = 0, ui->underLabel->setText("");//跳出
}

void map::on_showPathlenBt_clicked()
{
    if (!showlen) showlen = true, ui->showPathlenBt->setText("隐藏路径长度");
    else showlen = false, ui->showPathlenBt->setText("显示路径长度");//每点一下切换状态
    update();
}

void map::on_allandshortestBt_clicked()
{
    clear();
    if (tp != 8) {
        if (lineNum == 0) { QMessageBox::warning(this, "警告", "无任何路径"); return; }//判断整个地图是否存在路径

        tp = 8; ui->underLabel->setText("请选择起点");//转到case 8
        ui->startPointLabel->setText("起点："); ui->messagelist->clear();
        ui->endPointLabel->setText("");
        ui->messagelist->addItem("搜索结果将会显示在这里");

    }
    else tp = 0, ui->underLabel->setText("");//跳出
}

void map::on_changeBackBt_clicked()
{
    QString temp = QFileDialog::getOpenFileName(this, "选择文件", "D:/", "图像文件(*.jpg *.jpeg *.bmg *.png *.gif)");
    if (temp != "") pic = temp;//传入图片存储路径
    update();
}



void map::on_installMapBt_clicked()
{
    using namespace std;//使用ifstream和string
    QString filename = QFileDialog::getOpenFileName(this, "选择文件", "", "地图文件(*.map2)");
    if (filename != "")
    {
        ifstream in(filename.toStdString());
        if (in.is_open())
        {
            string c;
            in >> c;
            ui->Title->setText(QString::fromStdString(c));//转变为Qstring
            in >> lineNum >> pointNum;
            for (int i = 1; i <= pointNum; i++)
            {
                int a, b;
                in >> a >> b;
                P[i].setX(a);
                P[i].setY(b);
                in >> c;
                Pname[i] = QString::fromStdString(c);
            }
            for (int i = 1; i <= pointNum; i++)
                for (int j = 1; j <= pointNum; j++)
                    in >> matrix[i][j];
            for (int i = 1; i <= lineNum; i++)
                in >> line[i].a >> line[i].b;
            in >> c;
            //pic = QString::fromStdString(c);
            //string str=pic.toStdString();
            clear();
            QMessageBox::information(this, "提示", "读取成功");
            in.close();//关闭文件
        }
        else QMessageBox::information(this, "提示", "读取失败");
    }
}



void map::on_saveMapBt_clicked()
{
    using namespace std;
    QString filename = QFileDialog::getSaveFileName(this, "保存文件", "", "地图文件(*.map2)");//文件扩展名为.map2
    if (filename != "")//判断地址是否输入成功
    {
        ofstream out(filename.toStdString());//转换为标准字符串进行保存
        if (out.is_open())
        {
            out << ui->Title->text().toStdString() << endl;
            out << lineNum << ' ' << pointNum << endl;
            for (int i = 1; i <= pointNum; i++)
                out << P[i].x() << ' ' << P[i].y() << ' ' << Pname[i].toStdString() << endl;//空格隔开
            for (int i = 1; i <= pointNum; i++)
            {
                for (int j = 1; j <= pointNum; j++)
                    out << matrix[i][j] << ' ';
                out << endl;
            }
            for (int i = 1; i <= lineNum; i++)
                out << line[i].a << ' ' << line[i].b << endl;
            //out << pic.toStdString() << endl;
            QMessageBox::information(this, "提示", "保存成功");
            out.close();//关闭文档
        }
        else QMessageBox::information(this, "提示", "保存失败");
    }
}



void map::on_changeTitleBt_clicked()
{
    QString s0 = QInputDialog::getText(this, "编辑标题", "输入文本(最多28个字)", QLineEdit::Normal, ui->Title->text());
    if (s0 != "") ui->Title->setText(s0.left(28));//跳出
}

void map::on_clearTitleBt_clicked()
{
    QMessageBox::StandardButton btn;
    btn = QMessageBox::question(this, "提示", "确认要清除地图吗？", QMessageBox::Yes | QMessageBox::No);
    if (btn == QMessageBox::Yes)//清除地图
    {
        for (int i = 1; i <= pointNum; i++)
        {
            P[i] = P[0];
            Pname[i] = Pname[0];
            for (int j = 1; j <= pointNum; j++)
                matrix[i][j] = 0;
        }
        for (int i = 1; i <= lineNum; i++)
            line[i] = line[0];
        tp = pointNum = lineNum = count = 0;
        clear();
        ui->underLabel->setText("");
    }
}




bool map::event(QEvent * event)
{
    static bool press=false;
    static QPoint PreDot;
    if(event->type() == QEvent::MouseButtonPress )//按鼠标按键
    {
        QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);//dynamic_cast为强制转换为鼠标动作
        //判断鼠标是否是左键按下,且鼠标位置是否在绘画区域
        if(mouse->button()==Qt::LeftButton && mouse->pos().x() >= 451 && mouse->pos().x() <= 1971 && mouse->pos().y()>210 && mouse->pos().y() <= 1160)
        {
            press=true;
            QApplication::setOverrideCursor(Qt::OpenHandCursor); //设置鼠标样式
            PreDot = mouse->pos();
        }
    }
    else if(event->type() == QEvent::MouseButtonRelease)//释放鼠标按键
    {
        QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);
        //判断鼠标是否是左键释放,且之前是在绘画区域
        if(mouse->button()==Qt::LeftButton && press)
        {
            QApplication::setOverrideCursor(Qt::ArrowCursor); //改回鼠标样式
            press=false;
        }
    }

    if(event->type() == QEvent::MouseMove)//鼠标移动
    {
        if(press)//按下过且在绘画区域
        {
            QMouseEvent *mouse = dynamic_cast<QMouseEvent* >(event);
            offset.setX(mouse->x() - PreDot.x());//相对之前，鼠标偏移了x
            offset.setY(mouse->y() - PreDot.y());//相对之前，鼠标偏移了y
            PreDot = mouse->pos();//加载完成，更新现在指数
            action = map::Move;//更改动作
            this->update();//重新绘图
        }
    }
    return QWidget::event(event);
}

bool map::inrect(int x, int y)
{
    if(x >= 451 && x <= 1971 && y>=210 && y <= 1160)
    {
        return true;
    }
    else{
        return false;
    }
}

// 初始化
void map::initValue()
{
    ui->widgetTitle->setParentWidget(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}
