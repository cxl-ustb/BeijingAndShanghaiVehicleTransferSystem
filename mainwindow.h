#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "VehicleMap.h"
#include "graphics_view_zoom.h"
#include <QMainWindow>
#include <QLabel>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    //换乘出发线路改变槽函数
    void TransferStartLineChanged(QString LineName);
    //换乘终点线路改变槽函数
    void TransferEndLineChanged(QString lineName);
    //换乘查询槽函数
    void transferQuery();
    //动作查看北京所有线路图槽函数
    void on_ActionDisplayAllMapBJ_triggered();
    //动作查看上海所有线路图槽函数
    void on_ActionDisplayAllMapSH_triggered();
    //视图缩小槽函数
    void on_ActionZoomInMap_triggered();
    //视图放大槽函数
    void on_ActionEnlargeMap_triggered();

    //动作开启仅北京地铁模式
    void on_ActionOnlySubwayBJ_triggered();
    //动作开启仅上海地铁模式
    void on_ActionOnlySubwaySH_triggered();
    //动作开启仅北京公交模式
    void on_ActionOnlyBusBJ_triggered();
    //动作开启仅上海公交模式
    void on_ActionOnlyBusSH_triggered();
    //北京公交+地铁
    void on_BJ_triggered();
    //上海公交+地铁
    void on_SH_triggered();


private:
    //连接信号和槽函数
    void myConnect();
    //读取北京市公交和地铁数据
    void LoadVehicleInfo(VehicleMap* vehicleMap,QString city,QString vehicle);
    //更新换乘选择信息
    void updateTranserQueryInfo();


protected:
    Ui::MainWindow *ui;             //主窗口UI
    Graphics_view_zoom *myView;     //自定义视图，用于鼠标缩放
    QGraphicsScene *scene;          //场景
    VehicleMap* vehicleMap;       //后端管理类

    //由线路表计算混合颜色
    QColor getLinesColor(const QList<int>& linesList);
    //获得线路表的名字集
    QString getLinesName(const QList<int>& linesList);
    //将站点的经纬度地理坐标转为视图坐标
    QPointF transferCoord(QPointF coord);
    //绘制网络图的边
    void drawEdges (const QList<Edge>& edgesList);
    //绘制网络图的站点节点
    void drawStations (const QList<int>& stationsList);
};

#define LINE_INFO_WIDTH 0   //预留边框用于信息展示
#define MARGIN 30           //视图左边距
#define NET_WIDTH 2000      //网络图最大宽度
#define NET_HEIGHT 2000     //网络图最大高度
#define SCENE_WIDTH (LINE_INFO_WIDTH+MARGIN*2+NET_WIDTH)    //视图宽度
#define SCENE_HEIGHT (MARGIN*2+NET_HEIGHT)                  //视图高度

#define EDGE_PEN_WIDTH 2    //线路边宽
#define NODE_HALF_WIDTH 3   //节点大小

#endif // MAINWINDOW_H
