#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"

#include <QGraphicsItem>
#include <QMessageBox>
#include <QColorDialog>
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>

//主窗口初始化类
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myView = new Graphics_view_zoom(ui->QGraphicsView);
    myView->set_modifiers(Qt::NoModifier);
    ui->QGraphicsView->setRenderHint(QPainter::Antialiasing);
    scene=new QGraphicsScene;
    scene->setSceneRect(-LINE_INFO_WIDTH,0,SCENE_WIDTH,SCENE_HEIGHT);
    ui->QGraphicsView->setScene(scene);
    ui->QGraphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    //将action和对应的槽函数连接起来
    myConnect();
}

//按下“北京仅地铁”
void MainWindow::on_ActionOnlySubwayBJ_triggered(){
    //查询结果栏显示清空
    ui->DisplayTextBrowser->clear();
    //初始化vehicleMap对象
    vehicleMap=new VehicleMap;
    //加载北京地铁数据
    LoadVehicleInfo(vehicleMap,"Beijing","subway");
    //更新查询信息
    updateTranserQueryInfo();
    //地图显示栏清空
    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    //获取读取进来的地铁站点和线路
    vehicleMap->getGraph(stationsList,edgesList);
    //画路线
    drawEdges(edgesList);
    //画站点
    drawStations(stationsList);
}


//按下“上海仅地铁”
void MainWindow::on_ActionOnlySubwaySH_triggered(){
    //查询结果栏显示清空
    ui->DisplayTextBrowser->clear();
    //初始化vehicleMap对象
    vehicleMap=new VehicleMap;
    //加载上海地铁数据
    LoadVehicleInfo(vehicleMap,"Shanghai","subway");
    //更新查询信息
    updateTranserQueryInfo();
    //地图显示栏清空
    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    //获取读取进来的地铁站点和线路
    vehicleMap->getGraph(stationsList,edgesList);
    //画路线
    drawEdges(edgesList);
    //画站点
    drawStations(stationsList);
}

//按下“北京仅公交”
void MainWindow::on_ActionOnlyBusBJ_triggered(){
    //查询结果栏显示清空
    ui->DisplayTextBrowser->clear();
    //初始化vehicleMap对象
    vehicleMap=new VehicleMap;
    LoadVehicleInfo(vehicleMap,"Beijing","bus");
    //更新查询信息
    updateTranserQueryInfo();
    //地图显示栏清空
    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    //获取读取进来的地铁站点和线路
    vehicleMap->getGraph(stationsList,edgesList);
    //画路线
    drawEdges(edgesList);
    //画站点
    drawStations(stationsList);
}

//按下“上海仅公交”
void MainWindow::on_ActionOnlyBusSH_triggered(){
    //查询结果栏显示清空
    ui->DisplayTextBrowser->clear();
    //初始化vehicleMap对象
    vehicleMap=new VehicleMap;
    LoadVehicleInfo(vehicleMap,"Shanghai","bus");
    //更新查询信息
    updateTranserQueryInfo();
    //地图显示栏清空
    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    //获取读取进来的地铁站点和线路
    vehicleMap->getGraph(stationsList,edgesList);
    //画路线
    drawEdges(edgesList);
    //画站点
    drawStations(stationsList);
}

//按下“北京”
void MainWindow::on_BJ_triggered(){
    //查询结果栏显示清空
    ui->DisplayTextBrowser->clear();
    //初始化vehicleMap对象
    vehicleMap=new VehicleMap;
    LoadVehicleInfo(vehicleMap,"Beijing","all");
    //更新查询信息
    updateTranserQueryInfo();
    //地图显示栏清空
    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    //获取读取进来的地铁站点和线路
    vehicleMap->getGraph(stationsList,edgesList);
    //画路线
    drawEdges(edgesList);
    //画站点
    drawStations(stationsList);
}

//按下“上海”
void MainWindow::on_SH_triggered(){
    //查询结果栏显示清空
    ui->DisplayTextBrowser->clear();
    //初始化vehicleMap对象
    vehicleMap=new VehicleMap;
    LoadVehicleInfo(vehicleMap,"Shanghai","all");
    //更新查询信息
    updateTranserQueryInfo();
    //地图显示栏清空
    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    //获取读取进来的地铁站点和线路
    vehicleMap->getGraph(stationsList,edgesList);
    //画路线
    drawEdges(edgesList);
    //画站点
    drawStations(stationsList);
}


//读取公交和地铁数据
void MainWindow::LoadVehicleInfo(VehicleMap* vehicleMap,QString city,QString vehicle){
    //是否读取成功
    bool flag=false;
    //根据传入的城市和交通工具加载对应的数据
    if(city=="Beijing")
        if(vehicle=="subway")
            flag = vehicleMap->readFileData("C:/Users/cxl/Desktop/BeijingAndShanghaiVehicleTransferSystem/map_resource/Beijing_subway.txt");
        else if(vehicle=="bus")
            flag = vehicleMap->readFileData("C:/Users/cxl/Desktop/BeijingAndShanghaiVehicleTransferSystem/map_resource/Beijing_bus.txt");
        else
            flag = vehicleMap->readFileData("C:/Users/cxl/Desktop/BeijingAndShanghaiVehicleTransferSystem/map_resource/Beijing.txt");
    else
        if(city=="Shanghai")
            if(vehicle=="subway")
                flag = vehicleMap->readFileData("C:/Users/cxl/Desktop/BeijingAndShanghaiVehicleTransferSystem/map_resource/Shanghai_subway.txt");
            else if(vehicle=="bus")
                flag = vehicleMap->readFileData("C:/Users/cxl/Desktop/BeijingAndShanghaiVehicleTransferSystem/map_resource/Shanghai_bus.txt");
            else
                flag = vehicleMap->readFileData("C:/Users/cxl/Desktop/BeijingAndShanghaiVehicleTransferSystem/map_resource/Shanghai.txt");
    //读取失败的警示
    if (!flag)
    {
        QMessageBox box;
        box.setWindowTitle(tr("error information"));
        box.setIcon(QMessageBox::Warning);
        box.setText("读取数据错误!\n将无法展示内置线路！");
        box.addButton(tr("确定"), QMessageBox::AcceptRole);
        if (box.exec() == QMessageBox::Accepted)
        {
            box.close();
        }
    }
}

//连接信号和槽函数
void MainWindow::myConnect(){
    //路线查询组件的槽函数
    connect(ui->StartLineQComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(TransferStartLineChanged(QString)));
    connect(ui->EndLineQComboBox, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(TransferEndLineChanged(QString)));
    connect(ui->LookUpPushButton, SIGNAL(clicked()), this, SLOT(transferQuery()));

}

//更新换乘选择信息
void MainWindow::updateTranserQueryInfo()
{

    QComboBox* StartBox=ui->StartLineQComboBox;
    QComboBox* EndBox=ui->EndLineQComboBox;

    StartBox->clear();
    EndBox->clear();
    QList<QString> LinesList=vehicleMap->getLinesNameList();
    for(auto &a:LinesList)
    {
        StartBox->addItem(a);
        EndBox->addItem(a);
    }
    TransferStartLineChanged(StartBox->itemText(0));
    TransferEndLineChanged(EndBox->itemText(0));
}

//换乘出发线路改变槽函数
void MainWindow::TransferStartLineChanged(QString LineName)
{
    QComboBox* comboS1=ui->StartStationQComboBox;
    comboS1->clear();

    int LineHash=vehicleMap->getLineHash(LineName);
    if(LineHash==-1)
    {
        return ;
    }

    QList<QString> StationsList=vehicleMap->getLineStationsList(LineHash);
    for(auto &a:StationsList)
    {
        comboS1->addItem(a);
    }
}

//换乘目的线路改变槽函数
void MainWindow::TransferEndLineChanged(QString LineName)
{
    QComboBox* comboS2=ui->EndStationQComboBox;
    comboS2->clear();

    int LineHash=vehicleMap->getLineHash(LineName);
    if(LineHash==-1)
    {
        return ;
    }

    QList<QString> stationsList=vehicleMap->getLineStationsList(LineHash);
    for(auto &a:stationsList)
    {
        comboS2->addItem(a);
    }
}

//动作查看北京所有线路图槽函数
void MainWindow::on_ActionDisplayAllMapBJ_triggered()
{
    vehicleMap=new VehicleMap;
    LoadVehicleInfo(vehicleMap,"Beijing","all");
    updateTranserQueryInfo();

    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    vehicleMap->getGraph(stationsList,edgesList);
    drawEdges(edgesList);
    drawStations(stationsList);
}

//动作查看上海所有线路图槽函数
void MainWindow::on_ActionDisplayAllMapSH_triggered()
{
    vehicleMap=new VehicleMap;
    LoadVehicleInfo(vehicleMap,"Shanghai","all");
    updateTranserQueryInfo();

    scene->clear();
    QList<int> stationsList;
    QList<Edge> edgesList;
    vehicleMap->getGraph(stationsList,edgesList);
    drawEdges(edgesList);
    drawStations(stationsList);
}

//由线路表计算混合颜色
QColor MainWindow::getLinesColor(const QList<int>& linesList)
{
    QColor color1=QColor(255,255,255);
    QColor color2;
    for (int i=0; i<linesList.size(); ++i)
    {
        color2=vehicleMap->getLineColor(linesList[i]);
        color1.setRed(color1.red()*color2.red()/255);
        color1.setGreen(color1.green()*color2.green()/255);
        color1.setBlue(color1.blue()*color2.blue()/255);
    }
    return color1;
}

//获得线路表的名字集
QString MainWindow::getLinesName(const QList<int>& linesList)
{
    QString str;
    str+="\t";
    for (int i=0; i<linesList.size(); ++i)
    {
        str+=" ";
        str+=vehicleMap->getLineName(linesList[i]);
    }

    return str;
}

//将站点的经纬度地理坐标转为视图坐标
QPointF MainWindow::transferCoord(QPointF coord)
{
    QPointF minCoord=vehicleMap->getMinCoord();
    QPointF maxCoord=vehicleMap->getMaxCoord();
    double x = (coord.x()-minCoord.x())/(maxCoord.x()-minCoord.x())*NET_WIDTH+MARGIN;
    double y = (maxCoord.y()-coord.y())/(maxCoord.y()-minCoord.y())*NET_HEIGHT+MARGIN;
    return QPointF(x,y);
}
//绘制网络图的边
void MainWindow::drawEdges(const QList<Edge>& edgesList)
{
    for(int i=0; i<edgesList.size(); ++i)
    {
        int s1=edgesList[i].first;
        int s2=edgesList[i].second;

        QList<int> linesList=vehicleMap->getCommonLines(s1, s2);
        QColor color=getLinesColor(linesList);
        QString tip="途经： "+vehicleMap->getStationName(s1)+"--"+vehicleMap->getStationName(s2)+"\n线路：";
        tip+=getLinesName(linesList);
        QPointF s1Pos=transferCoord(vehicleMap->getStationCoord(s1));
        QPointF s2Pos=transferCoord(vehicleMap->getStationCoord(s2));

        QGraphicsLineItem* edgeItem=new QGraphicsLineItem;
        edgeItem->setPen(QPen(color, EDGE_PEN_WIDTH));
        edgeItem->setCursor(Qt::PointingHandCursor);
        edgeItem->setToolTip(tip);
        edgeItem->setPos(s1Pos);
        edgeItem->setLine(0, 0, s2Pos.x()-s1Pos.x(), s2Pos.y()-s1Pos.y());
        scene->addItem(edgeItem);
    }
}
//视图放大槽函数
void MainWindow::on_ActionEnlargeMap_triggered()
{
    ui->QGraphicsView->scale(1.5,1.5);
}

//动作视图缩小槽函数
void MainWindow::on_ActionZoomInMap_triggered()
{

    ui->QGraphicsView->scale(2.0/3,2.0/3);
}

//绘制网络图的站点节点
void MainWindow::drawStations (const QList<int>& stationsList)
{
    for (int i=0; i<stationsList.size(); ++i)
    {
        int s=stationsList[i];
        QString name=vehicleMap->getStationName(s);
        QList<int> linesList=vehicleMap->getStationLinesInfo(s);
        QColor color=getLinesColor(linesList);
        QPointF longiLati=vehicleMap->getStationCoord(s);
        QPointF coord=transferCoord(longiLati);
        QString tip="站名：  "+name+"\n"+
                "经度：  "+QString::number(longiLati.x(),'f',7)+"\n"+
                "纬度：  "+QString::number(longiLati.y(),'f',7)+"\n"+
                "线路："+getLinesName(linesList);

        QGraphicsEllipseItem* stationItem=new QGraphicsEllipseItem;
        stationItem->setRect(-NODE_HALF_WIDTH, -NODE_HALF_WIDTH, NODE_HALF_WIDTH<<1, NODE_HALF_WIDTH<<1);
        stationItem->setPos(coord);
        stationItem->setPen(color);
        stationItem->setCursor(Qt::PointingHandCursor);
        stationItem->setToolTip(tip);

        if(linesList.size()<=1)
        {
            stationItem->setBrush(QColor(QRgb(0xffffff)));
        }

        scene->addItem(stationItem);

        QGraphicsTextItem* textItem=new QGraphicsTextItem;
        textItem->setPlainText(name);
        textItem->setFont(QFont("consolas",4,1));
        textItem->setPos(coord.x(),coord.y()-NODE_HALF_WIDTH*2);
        scene->addItem(textItem);
    }
}


//换乘查询槽函数
void MainWindow::transferQuery()
{   //获取选择的起始站点
    int startsta=vehicleMap->getStationHash(ui->StartStationQComboBox->currentText());
    //获取选择的终点终点
    int endsta=vehicleMap->getStationHash(ui->EndStationQComboBox->currentText());
    //判断选择的查询模式是“时间少”还是“换乘少”
    int selection=ui->StationLessRadioButton->isChecked()?1:2;
    if(startsta==-1||endsta==-1)
    {
        QMessageBox box;
        box.setWindowTitle(tr("换乘查询"));
        box.setWindowIcon(QIcon(":/images/query.png"));
        box.setIcon(QMessageBox::Warning);
        box.setText(tr("请选择有站点的线路"));
        box.addButton(tr("确定"),QMessageBox::AcceptRole);
        if(box.exec()==QMessageBox::Accepted)
        {
            box.close();
        }
    }
    else
    {   //存储从起始站点到目的站点间，经过的站点和路线
        QList<int> stationsList;
        QList<Edge> edgesList;
        bool flag=true;
        if(selection==1)
        {   //进入QueryTransfer后，执行相应的算法，stationsList和edgesList都改变
            flag=vehicleMap->QueryTransfer(startsta, endsta, stationsList, edgesList);
        }
        else
        {   //进入QueryTransfer后，执行相应的算法，stationsList和edgesList都改变
            flag=vehicleMap->QueryTransfer(startsta, endsta, stationsList, edgesList);
        }
        //查询结果，用文本和地图两种方式显示
        if(flag)
        {
            //地图显示栏清空
            scene->clear();
            //在地图上单独显示查询结果
            //画路线
            drawEdges(edgesList);
            //画站点
            drawStations(stationsList);
            QString text=selection==1?("以下线路时间最短，共换乘"+QString::number(stationsList.size()-1)+"个站点\n\n"):
                                ("以下线路换乘最少，共换乘"+QString::number(stationsList.size()-1)+"条线路\n\n");
            for(int i=0; i<stationsList.size(); ++i)
            {
                if(i)
                {
                    text+="\n  ↓\n";
                }
                text+=vehicleMap->getStationName(stationsList[i]);
                QString linesStr=getLinesName(vehicleMap->getStationLinesInfo(stationsList[i]));
                text+=linesStr;
            }
            QTextBrowser* browser=ui->DisplayTextBrowser;
            browser->clear();
            //将查询结果文本显示在文本框里面
            browser->setText(text);
        }
        else
        {
            QMessageBox box;
            box.setWindowTitle(tr("换乘查询"));
            box.setWindowIcon(QIcon(":/icon/icon/query.png"));
            box.setIcon(QMessageBox::Warning);
            box.setText(tr("您选择的起始和终止站点暂时无法到达！"));
            box.addButton(tr("确定"),QMessageBox::AcceptRole);
            if(box.exec()==QMessageBox::Accepted)
            {
                box.close();
            }
        }
    }

}

//主窗口类析构函数
MainWindow::~MainWindow()
{
    delete ui;
}
