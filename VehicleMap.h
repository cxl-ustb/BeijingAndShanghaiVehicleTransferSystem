#ifndef VehicleMap_H
#define VehicleMap_H

#include "station.h"
#include "line.h"
#include <QString>
#include <QPoint>
#include <QVector>
#include <QHash>
#include <QMap>
//定义描述一对（两个）站点的数据类型
typedef QPair<QString,QString> stationPair;

//图的邻接点结构
class Node{
public:
    int stationID;      //邻接点ID
    double distance;    //两点距离

    //构造函数
    Node(){};
    Node(int s, double dist) :stationID(s), distance(dist)
    {};

    //">"运算重载，用于小顶堆
    bool operator > (const Node& n) const
    {
        return this->distance>n.distance;
    }
};

//后端管理类
class VehicleMap
{
protected:
    QVector<Station> stations;          //存储所有站点
    QVector<Line> lines;                //存储所有线路
    QHash<QString, int> stationsHash;   //站点名到存储位置的hash
    QMap<int,QString> hash2Station;    //存储位置到站点名
    QHash<QString, int> linesHash;      //线路名到存储位置的hash
    QSet<Edge> edges;                   //所有edge的集合,一条edge上有2个点，一条line上有多个点
    QVector<QVector<Node>> graph;       //交通线路网络图
    //下面这两个属性在A*算法中使用
    QMap<QString,QSet<QString>> station2Stations; //存储站点和它直接相连的站点
    QMap<stationPair,double> fullMapDIstanceInfo;   //存储站点对以及这两个站点间的距离

public:
    //构造函数
    VehicleMap();
    //获取线路名
    QString getLineName(int l);
    //获取线路颜色
    QColor getLineColor(int l);
    //获取线路hash值
    int getLineHash(QString lineName);
    //获取线路集合hash值
    QList<int> getLinesHash(QList<QString> linesList);
    //获取线路名集合
    QList<QString> getLinesNameList();
    //获取线路的所有包含站点
    QList<QString> getLineStationsList(int l);
    //获取站点名
    QString getStationName(int s);
    //获取站点地理坐标
    QPointF getStationCoord(int s);
    //获取站点最小坐标
    QPointF getMinCoord();
    //获取站点最大坐标
    QPointF getMaxCoord();
    //获取站点所属线路信息
    QList<int> getStationLinesInfo(int s);
    //获取两个站点的公共所属线路
    QList<int> getCommonLines(int s1, int s2);
    //获取站点hash值
    int getStationHash(QString stationName);
    //获取站点集合hash值
    QList<QString> getStationsNameList();
    //获取网络结构，用于前端显示
    void getGraph(QList<int>&stationsList, QList<Edge>&edgesList);
    //获取最少时间的线路（A*算法)
    bool QueryTransfer_Astar(int s1, int s2,
                              QList<int>&stationsList,
                              QList<Edge>&edgesList);
    //获取最少时间的线路（迪杰斯特拉算法)
    bool QueryTransfer(int s1, int s2,
                              QList<int>&stationsList,
                              QList<Edge>&edgesList);
    //从文件读取数据
    bool readFileData(QString fileName);
    //根据hash值获取站点名
    QString getHash2Station(int hash);
    //根据A* star算法的代价函数，获取排序后的可选择站点
    QList<QString> leastCostStation(QString cur_station,QString end_station);


private:
    //清空数据
    void clearData();
    //插入一条边
    bool insertEdge(int s1, int s2);
    //更新边界经纬度
    void updateMinMaxLongiLati();
    //生成图结构
    void makeGraph();
    //计算所有站点间两两间的距离
    void makeFullMapDistanceInfo();
    //存储站点和它直接相连的站点
    void makeStation2Stations();
    //存储位置到站点名
    void makeHash2Station();
};

#endif // VehicleMap_H
