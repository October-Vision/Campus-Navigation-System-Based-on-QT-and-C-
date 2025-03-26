/*****************************************
 * 功能：轮播图，使用双向循环链表思想，并使用Qt
 * 的动画组，然后对五张图片进行移动,如下所示
 * 1，2，3，4，5分别对应五个Label中的五个图片
 * 1的Label的坐标和大小，被2和5指向，即若向右移动
 * 则1的大小和位置变为2的大小和位置，2的位置变为3的
 * 大小和位置，依次类推
 * 若向左移动，则1的大小和位置则变为5的大小和位置，
 * 5的大小和位置则变为4的大小和位置，依次类推
 * 1 -> 2 -> 3 -> 4 -> 5 -> 1
 * 1 <- 2 <- 3 <- 4 <- 5 <- 1
 * 若不理解，课查看数据结构双向循环链表
 * ***************************************/
#include "bannacontrol.h"
#include "ui_bannacontrol.h"

// 构造函数
BannaControl::BannaControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BannaControl)
{
    ui->setupUi(this);
    this->initValue();
}

// 析构函数
BannaControl::~BannaControl()
{
    delete ui;
}

// 初始化数据
void BannaControl::initValue()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    changeImageTimer = new QTimer(this);
    connect(changeImageTimer, SIGNAL(timeout()), this, SLOT(slot_changeImageTimer()));//到达时间间隔则切换照片
    changeImageTimer->setInterval(BannaSpace::INTERVAL_TIMER);//设置切换时间间隔
    changeImageTimer->start();
    //五个按钮
    buttonGroup.addButton(ui->pushButtonFirst);
    buttonGroup.addButton(ui->pushButtonSecond);
    buttonGroup.addButton(ui->pushButtonThird);
    buttonGroup.addButton(ui->pushButtonFourth);
    buttonGroup.addButton(ui->pushButtonFifth);
    //初始照片为第三个
    ui->labelThird->raise();
    ui->labelFifth->lower();
    //添加五个图片框
    {
        BannaData bannaData;
        bannaData.text = "2007年，经著名雕塑家唐大禧修缮复制的创造太阳雕塑落成于青岛校区，是学校最具代表性的主题雕塑。它坐落于学校中轴线光华大道上，背靠图书馆大楼，面朝学校南门。雕塑以花岗岩和不锈钢制成，主体是一个创造太阳的“女大学生”，代表开发能源、从事高等教育的人。她矗立于钻井平台之上，下方飘动的长裙化作海洋波浪和地下岩层，寓意学校的能源开发来自海洋和陆地；上方托起的石油管线连接着太阳，象征着以知识能源开发物质能源。雕塑将海洋、大地、太阳和人体联系起来，呈现了能源开发创造的过程，充分展示着石大人为祖国奉献能源的使命与担当。";
        bannaData.title = "创造太阳";
        bannaData.pushButton = ui->pushButtonFirst;
        bannaData.currentImageLabel = ui->labelFirst;
        bannaData.nextImageLabel = ui->labelSecond;
        bannaData.preImageLabel = ui->labelFifth;
        bannaData.proAnimation = new QPropertyAnimation(ui->labelFirst, BannaSpace::ANIMATION_GEOMETRY);
        bannaDataList.append(bannaData);
    }


    {
        BannaData bannaData;
        bannaData.title = "荟萃苑餐厅";
        bannaData.text = "荟萃苑餐厅于2005年11月20日开工建设，2006年7月31日竣工，共4层，钢筋混凝土框架结构，由中国建筑西南设计研究院设计，青岛市胶州建设集团有限公司施工完成。占地面积5452平方米，建筑面积14946平方米，工程造价4168.03万元。2018年暑假，餐厅进行全面的提档升级，力求打造“安全、营养、绿色、时尚”等多维餐饮服务保障体系。就餐区域有三层，一层是基本大伙，满足师生对日常餐点的消费需求，其中东侧精选多个省市的热门风味小吃；二层安排了各式南北风味，增加现加工的菜肴品种，保证菜品的新鲜度；三层增设了休闲食品及卡座交流区域，设置了自助糕点及零食饮品，环境优雅，休闲时尚，让师生足不出户就能惬意地享受美食。";
        bannaData.pushButton = ui->pushButtonSecond;
        bannaData.currentImageLabel = ui->labelSecond;
        bannaData.nextImageLabel = ui->labelThird;
        bannaData.preImageLabel = ui->labelFirst;
        bannaData.proAnimation = new QPropertyAnimation(ui->labelSecond, BannaSpace::ANIMATION_GEOMETRY);
        bannaDataList.append(bannaData);
    }


    {
        BannaData bannaData;
        bannaData.title = "图书馆";
        bannaData.text = "建筑柔美曲线结合山体硬朗棱角，绿色环保融合现代科技，设计感与实用性并重，石大图书馆东馆层次感十足的建筑体内有浩如烟海的文献资料，也带给同学多样化的服务体验，成为石大闪亮的新地标！";
        bannaData.pushButton = ui->pushButtonThird;
        bannaData.currentImageLabel = ui->labelThird;
        bannaData.nextImageLabel = ui->labelFourth;
        bannaData.preImageLabel = ui->labelSecond;
        bannaData.proAnimation = new QPropertyAnimation(ui->labelThird, BannaSpace::ANIMATION_GEOMETRY);
        bannaDataList.append(bannaData);
    }


    {
        BannaData bannaData;
        bannaData.title = "南教楼";
        bannaData.text="南教楼位于光华大道东侧，唐岛湾餐厅西侧，南邻文理楼，北临华东路。2011年通过征集，因北京石油学院时期的南教楼而得名。该楼于2011年1月1日开工，2011年8月30日竣工，由山东中石大工程设计有限公司设计，中建八局第四建设有限公司施工。占地面积6885.93平方米，建筑面积27544平方米，工程造价8208.51万元，钢筋混凝土框架结构。楼高5层，共88个自然间，其中教室80间，共10700个座位。楼内宽敞明亮，现代化教学设施先进齐全。一层东侧有阶梯教室“100号”，该教室的命名来源于东营校区的“100号报告厅”。二层建有院士墙，布展介绍了在我校学习、工作过和外聘的中国科学院院士与中国工程院院士。";
        bannaData.pushButton = ui->pushButtonFourth;
        bannaData.currentImageLabel = ui->labelFourth;
        bannaData.nextImageLabel = ui->labelFifth;
        bannaData.preImageLabel = ui->labelThird;
        bannaData.proAnimation = new QPropertyAnimation(ui->labelFourth, BannaSpace::ANIMATION_GEOMETRY);
        bannaDataList.append(bannaData);
    }


    {
        BannaData bannaData;
        bannaData.title="体育馆";
        bannaData.text="体育馆位于校园西北部，是我校与开发区政府共建的项目，也是2009年第十一届全运会排球比赛场馆。于2006年12月25日举行奠基仪式，2007年5月14日开工建设，2009年5月22日竣工。政府负责拆迁，无偿提供80亩土地，减免部分事业性收费，学校出资建设。场馆占地面积53350.3平方米，建筑面积18834平方米，造价13371.34万元。由北京建筑设计研究院设计，中国建筑第八工程局负责施工。体育馆的建筑结构类型为网架结构和新型的空间结构体系，分主馆（比赛馆）和副馆（训练馆）两部分。从外形看，主馆和副馆是两个独立的单体建筑，但内部的通道使两座场馆实现了连通。";
        bannaData.pushButton = ui->pushButtonFifth;
        bannaData.currentImageLabel = ui->labelFifth;
        bannaData.nextImageLabel = ui->labelFirst;
        bannaData.preImageLabel = ui->labelFourth;
        bannaData.proAnimation = new QPropertyAnimation(ui->labelFifth, BannaSpace::ANIMATION_GEOMETRY);
        bannaDataList.append(bannaData);
    }

    for(int i = 0; i < bannaDataList.size(); i++)
    {
        bannaDataList.at(i).currentImageLabel->installEventFilter(this);//安装事件过滤器
        bannaDataList.at(i).proAnimation->setDuration(BannaSpace::ANIMATION_DURTION);//设置动画持续时间
        bannaDataList.at(i).proAnimation->setEasingCurve(QEasingCurve::BezierSpline);//设置缓动曲线
        animationGroup.addAnimation(bannaDataList.at(i).proAnimation);//向动画组加入动画
    }

    QList<QString> listImagePath;//加入五张照片
    listImagePath.append(BannaSpace::FIRST_IMAGE_PATH);
    listImagePath.append(BannaSpace::SECOND_IMAGE_PATH);
    listImagePath.append(BannaSpace::THIRD_IMAGE_PATH);
    listImagePath.append(BannaSpace::FOURTH_IMAGE_PATH);
    listImagePath.append(BannaSpace::FIFTH_IMAGE_PATH);
    this->addAnimationPixmap(listImagePath);
    ui->pushButtonLeft->raise();
    ui->pushButtonRight->raise();//两个切换按钮都显示上层

}

// 为轮播图添加图片
void BannaControl::addAnimationPixmap(QList<QString> &listPixmap)
{
    if(listPixmap.size() != bannaDataList.size()) return;//图片和图片框数量不一致则返回

    for(int i = 0; i < bannaDataList.size(); i++)
    {
        //设置样式表
        QString styleSheet = BannaSpace::STYLE_SHEET.arg(listPixmap.at(i));
        bannaDataList.at(i).currentImageLabel->setStyleSheet(styleSheet);
    }
}

// 每3S进行一次动画操作
void BannaControl::slot_changeImageTimer()
{
    if(animationGroup.state() != QParallelAnimationGroup::Running)//如果动画未运行中
    {
        this->setNextAnimation();//向右移动动画
    }
}

// 向左移动
void BannaControl::on_pushButtonLeft_clicked()
{
    if(animationGroup.state() != QParallelAnimationGroup::Running)
    {
        this->setPreAnimation();
    }
}

// 向右移动
void BannaControl::on_pushButtonRight_clicked()
{
    if(animationGroup.state() != QParallelAnimationGroup::Running)
    {
        this->setNextAnimation();
    }
}

// 获取最右边的Label的Index
int BannaControl::getMaxGeometryIndex(const QList<BannaData> &listData)
{
    int maxPosX = 0;
    int maxIndex = 0;
    for(int i = 0; i < listData.size(); i++)
    {
        int geometryX = listData.at(i).currentImageLabel->geometry().x();//获取每个图片的x值
        if(maxPosX < geometryX)//最大的x值就是最右边的图片
        {
            maxPosX = geometryX;
            maxIndex = i;
        }
    }
    return maxIndex;
}

// 图层排序
void BannaControl::sortGeometry(const bool &isNextFlage)
{
    /******************************************
     * 思路：
     * 将五张图片进行排序，根据其移动后的位置
     * 来确定哪个图层在上，哪个图层在下
     * lower函数 将图层沉下去
     * raise函数 将图层浮上来
     * *****************************************/
    QList<BannaData>listData = bannaDataList;
    QLabel *topImageLabel = nullptr;
    int countNumber = 0;//控制符
    while(!listData.isEmpty())//列表非空
    {
        int index = getMaxGeometryIndex(listData);//最右侧index
        if(index < listData.size())
        {
            switch (countNumber)
            {
            case BannaSpace::SORT_FIRST:
            {
                listData.at(index).currentImageLabel->lower();
                break;
            }
            case BannaSpace::SORT_SECOND:
            {
                if(isNextFlage)
                {
                    topImageLabel = listData.at(index).currentImageLabel;
                    listData.at(index).pushButton->setChecked(true);
                }
                listData.at(index).currentImageLabel->raise();
                break;
            }
            case BannaSpace::SORT_THIRD:
            {
                listData.at(index).currentImageLabel->raise();
                break;
            }
            case BannaSpace::SORT_FOURTH:
            {
                if(!isNextFlage)
                {
                    topImageLabel = listData.at(index).currentImageLabel;
                    listData.at(index).pushButton->setChecked(true);
                }
                listData.at(index).currentImageLabel->lower();
                break;
            }
            case BannaSpace::SORT_FIFTH:
            {
                if(!isNextFlage)
                {
                    listData.at(index).currentImageLabel->raise();
                }
                else
                {
                    listData.at(index).currentImageLabel->lower();
                }
                break;
            }
            }
           listData.removeAt(index);
        }
        countNumber++;
    }

    if(topImageLabel)
    {
        topImageLabel->raise();//提升到最高层级
    }

    ui->pushButtonLeft->raise();
    ui->pushButtonRight->raise();
}

// 设置向pre移动动画
void BannaControl::setNextAnimation()
{
    for(int i = 0; i < bannaDataList.size(); i++)
    {
        bannaDataList.at(i).proAnimation->setStartValue(bannaDataList.at(i).currentImageLabel->geometry());
        bannaDataList.at(i).proAnimation->setEndValue(bannaDataList.at(i).preImageLabel->geometry());
    }
    animationGroup.start();
    this->sortGeometry(true);
}

// 设置向next移动动画
void BannaControl::setPreAnimation()
{
    for(int i = 0; i < bannaDataList.size(); i++)
    {
        bannaDataList.at(i).proAnimation->setStartValue(bannaDataList.at(i).currentImageLabel->geometry());
        bannaDataList.at(i).proAnimation->setEndValue(bannaDataList.at(i).nextImageLabel->geometry());
    }
    animationGroup.start();
    this->sortGeometry(false);
}

