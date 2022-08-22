#include "VehicleMap.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <queue>
#include "qalgorithms.h"
//构造函数
VehicleMap::VehicleMap()
{

}

//插入一条边
bool VehicleMap::insertEdge(int n1, int n2)
{
    if (edges.contains(Edge(n1, n2)) || edges.contains(Edge(n2, n1)))
    {
        return false;
    }
    edges.insert(Edge(n1, n2));
    return true;
}

//从文件读取数据
bool VehicleMap::readFileData(QString fileName)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream in(&file);
    //逐行读取，针对txt中存储站点和线路的格式，解析对应的数据，存储在相应的变量中
    while(!in.atEnd())
    {
        Line line;
        QString id, name, colour, fromTo, totalStations;
        QString color, froms, tos;
        bool ok;
        int total;
        Station station;
        int lvIndex, svIndex1, svIndex2;

        in>>id>>line.id;
        in>>name>>line.name;
        in>>colour>>color;
        line.color.setRgba(color.remove(0,1).toUInt(&ok, 16));
        in>>fromTo>>froms>>tos;
        in>>totalStations>>total;

        line.fromTo.push_back(froms);
        line.fromTo.push_back(tos);
        if (linesHash.count(line.name))
        {
            lvIndex = linesHash[line.name];
            lines[lvIndex].fromTo.push_back(froms);
            lines[lvIndex].fromTo.push_back(tos);
        }
        else
        {
            lvIndex = linesHash[line.name] = lines.size();
            lines.push_back(line);
        }

        QString longlat;
        QStringList strList;
        for (int i=0; !in.atEnd()&&i<total; ++i)
        {
            in>>station.id>>station.name>>longlat;
            strList=longlat.split(QChar(','));
            station.longitude=strList.first().toDouble();
            station.latitude=strList.last().toDouble();

            if (stationsHash.count(station.name))
            {
                svIndex2 = stationsHash[station.name];
            }
            else
            {
                svIndex2 = stationsHash[station.name] = stations.size();
                stations.push_back(station);
            }

            stations[svIndex2].linesInfo.insert(lvIndex);
            lines[lvIndex].stationsSet.insert(svIndex2);

            if (i)
            {
                lines[lvIndex].edges.insert(Edge(svIndex1, svIndex2));
                lines[lvIndex].edges.insert(Edge(svIndex2, svIndex1));
                insertEdge(svIndex1, svIndex2);
            }
            svIndex1 = svIndex2;
        }

        bool flag = id=="id:" && name=="name:" && colour=="colour:" && fromTo=="fromTo:"
                && totalStations=="totalStations:" && ok && !in.atEnd();


        if(flag==false)
        {
            file.close();
            clearData();
            return false ;
        }
        in.readLine();
    }
    file.close();

    updateMinMaxLongiLati();

    return true;
}

//清空数据
void VehicleMap::clearData()
{
    stations.clear();
    lines.clear();
    stationsHash.clear();
    linesHash.clear();
    edges.clear();
    graph.clear();
}


//生成图结构
void VehicleMap::makeGraph()
{
    graph.clear();
    //graph类似于两维数组，每一行存储一个站点直接相连的站点距离信息
    graph=QVector<QVector<Node>>(stations.size(), QVector<Node>());
    for (auto &a : edges)
    {   //计算某站点和一个与它直接相连的站点的距离信息
        double dist=stations[a.first].distance(stations[a.second]);
        //将这个距离加入这两个站点对应的行
        graph[a.first].push_back(Node(a.second, dist));
        graph[a.second].push_back(Node(a.first, dist));
    }
}

//获取线路颜色
QColor VehicleMap::getLineColor(int l)
{
    return lines[l].color;
}

//获取线路名
QString VehicleMap::getLineName(int l)
{
    return lines[l].name;
}

//根据hash值得到站点名并保存
void VehicleMap::makeHash2Station(){
    hash2Station.clear();
    for(int i=0;i<stations.size();i++){
        hash2Station.insert(stationsHash[stations[i].name],stations[i].name);
    }
}

//根据hash值得到站点名
QString VehicleMap::getHash2Station(int hash){
    return hash2Station[hash];
}

//获取线路hash值
int VehicleMap::getLineHash(QString lineName)
{
    if(linesHash.contains(lineName))
    {
        return linesHash[lineName];
    }
    return -1;
}

//获取线路集合hash值
QList<int> VehicleMap::getLinesHash(QList<QString> linesList)
{
    QList<int> hashList;
    for (auto &a:linesList)
    {
        hashList.push_back(getLineHash(a));
    }
    return hashList;
}

//获取线路名集合
QList<QString> VehicleMap::getLinesNameList()
{
    QList<QString> linesNameList;
    for (auto a:lines)
    {
        linesNameList.push_back(a.name);
    }
    return linesNameList;
}

//获取线路的所有包含站点
QList<QString> VehicleMap::getLineStationsList(int l)
{
    QList<QString> stationsList;
    for (auto &a:lines[l].stationsSet)
    {
        stationsList.push_back(stations[a].name);
    }
    return stationsList;
}



//更新边界经纬度
void VehicleMap::updateMinMaxLongiLati()
{
    double minLongitude=200, minLatitude=200;
    double maxLongitude=0, maxLatitude=0;

    for (auto &s : stations)
    {
        minLongitude = qMin(minLongitude, s.longitude);
        minLatitude = qMin(minLatitude, s.latitude);
        maxLongitude = qMax(maxLongitude, s.longitude);
        maxLatitude = qMax(maxLatitude, s.latitude);
    }
    Station::minLongitude = minLongitude;
    Station::minLatitude = minLatitude;
    Station::maxLongitude = maxLongitude;
    Station::maxLatitude = maxLatitude;
}

 //获取站点最小坐标
QPointF VehicleMap::getMinCoord()
{
    return QPointF(Station::minLongitude, Station::minLatitude);
}

//获取站点最大坐标
QPointF VehicleMap::getMaxCoord()
{
    return QPointF(Station::maxLongitude, Station::maxLatitude);
}

//获取两个站点的公共所属线路
QList<int> VehicleMap::getCommonLines(int s1, int s2)
{
    QList<int> linesList;
    for (auto &s : stations[s1].linesInfo)
    {
        if(stations[s2].linesInfo.contains(s))
            linesList.push_back(s);
    }
    return linesList;
}

//获取站点名
QString VehicleMap::getStationName(int s)
{
    return stations[s].name;
}

//获取站点地理坐标
QPointF VehicleMap::getStationCoord(int s)
{
    return QPointF(stations[s].longitude, stations[s].latitude);
}

//获取站点所属线路信息
QList<int> VehicleMap::getStationLinesInfo(int s)
{
    return stations[s].linesInfo.toList();
}

//获取站点hash值
int VehicleMap::getStationHash(QString stationName)
{
    if(stationsHash.contains(stationName))
    {
        return stationsHash[stationName];
    }
    return -1;
}

//获取站点集合hash值
QList<QString> VehicleMap::getStationsNameList()
{
    QList<QString> list;
    for (auto &a: stations)
    {
        list.push_back(a.name);
    }
    return list;
}



//获取网络结构，用于前端显示
void VehicleMap::getGraph(QList<int>&stationsList, QList<Edge>&edgesList)
{
    stationsList.clear();
    for (int i=0; i<stations.size(); ++i)
    {
        stationsList.push_back(i);
    }
    edgesList=edges.toList();
    return ;
}



//获取最少时间的线路
bool VehicleMap::QueryTransfer(int s1, int s2, QList<int>&stationsList, QList<Edge>&edgesList)
{
    #define INF 999999999
    //清空存储站点类的列表
    stationsList.clear();
    //清空存储两个站点之间的边的类的列表
    edgesList.clear();
    //若起始站点和最终站点相同
    if(s1==s2)
    {
        stationsList.push_back(s2);
        stationsList.push_back(s1);
        return true;
    }
    //建图，主要是根据edge信息，记录下每个站点和它直接相连的站点的经纬度信息
    makeGraph();

    std::vector<int> path(stations.size(), -1);
    std::vector<double> dist(stations.size(), INF);
    dist[s1]=0;
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> priQ;
    priQ.push(Node(s1, 0));
    while(!priQ.empty())
    {
        Node top=priQ.top();
        priQ.pop();
        if(top.stationID==s2)
        {
            break ;
        }

        for (int i=0; i<graph[top.stationID].size(); ++i)
        {
            Node &adjNode=graph[top.stationID][i];
            if(top.distance+adjNode.distance<dist[adjNode.stationID])
            {
                path[adjNode.stationID]=top.stationID;
                dist[adjNode.stationID]=top.distance+adjNode.distance;
                priQ.push(Node(adjNode.stationID, dist[adjNode.stationID]));
            }
        }
    }

    if(path[s2]==-1)
    {
        return false;
    }
    int p=s2;
    while(path[p]!=-1)
    {
        stationsList.push_front(p);
        edgesList.push_front(Edge(path[p],p));
        p=path[p];
    }
    stationsList.push_front(s1);

    return true;
}


//存储站点和它直接相连的站点
void VehicleMap::makeStation2Stations(){
    //生成存储位置到站点名
    makeHash2Station();
    //先清空
    station2Stations.clear();
    //先对每个站点对应的，与它直接相连的站点集合初始化
    for(int i=0;i<stations.size();i++){
        QSet<QString> qset;
        station2Stations[stations[i].name]=qset;
    }
    //根据边信息知道哪些站点直接相连，并通过哈希到站点得到站点到哈希
    for(QSet<Edge>::iterator it=edges.begin();it!=edges.end();it++){
        station2Stations[getHash2Station(it->first)].insert(getHash2Station(it->second));
        station2Stations[getHash2Station(it->second)].insert(getHash2Station(it->first));

    }
}

 //计算所有站点间两两间的距离
void VehicleMap::makeFullMapDistanceInfo()
{   //先清空
    fullMapDIstanceInfo.clear();
    //计算过程
    for(int i=0;i<stations.size();i++){
       for(int j=i+1;j<stations.size();j++){
           double dist=stations[i].distance(stations[j]);
           fullMapDIstanceInfo.insert(QPair<QString,QString>(stations[i].name,stations[i].name),dist);
           fullMapDIstanceInfo.insert(QPair<QString,QString>(stations[j].name,stations[i].name),dist);
       }
    }
}

double CmpByValue(const QPair<QString,double> elem1,const QPair<QString,double> elem2)
{

    return elem1.second-elem2.second;
}


QList<QString> VehicleMap::leastCostStation(QString cur_station, QString end_station)
{   //获取到与当前站点相连的所有站点集合
    QSet<QString> nextStationSet=station2Stations[cur_station];
    //用数组来存储，而不是集合
    QList<QString> nextStationList;
    //将集合的值赋给数组，得到与当前站点相连的所有站点名称
    for(QSet<QString>::iterator it=nextStationSet.begin();
        it!=nextStationSet.end();
        it++){
        nextStationList.push_back(*it);
    }
    //集合清空
    nextStationSet.clear();
    //定义A*算法中的H函数，即待选择站点和终点站间的经纬度距离作为代价函数
    QMap<QString,double> estimate_table;
    //先清空H表
    estimate_table.clear();
    for(auto& station:nextStationList){
        //得到一个站点对，（终点站，待选择站点），以便下面获取终点站和待选择站点间的经纬度距离
        stationPair station_pair(end_station,station);
        //记录到H表中
        estimate_table[station]=fullMapDIstanceInfo[station_pair];
    }
    //从当前站点出发，下一个可选择站点的站名和代价函数f=g+h,g是从当前站点到可选择站点的实际距离，h是从可选择站点到站点站的距离
    QMap<QString,double> nextStationListWithDIstance;
    for(auto& station:nextStationList){
        //得到一个站点对，（当前站点，待选择站点），以便下面获取当前站点和待选择站点间的经纬度距离
        stationPair station_pair(cur_station,station);
        //更新代价函数f
        nextStationListWithDIstance[station]=fullMapDIstanceInfo[station_pair]+1*estimate_table[station];
    }
    //因为已经得到了nextStationListWithDIstance，所以nextStationList不需要了
    nextStationList.clear();
    //根据代价函数进行排序,因为QMap排序不方便，因此借助QVector来排序
    typedef QPair<QString,double> elem;
    QVector<elem> test;
    //将nextStationListWithDIstance（QMap)的信息复制到test(QVector)
    for(auto it=nextStationListWithDIstance.begin();
        it!=nextStationListWithDIstance.end();it++){
        elem ele(it.key(),it.value());
        test.append(ele);
    }
    //清空
    nextStationListWithDIstance.clear();
    //根据代价函数进行排序
    qSort(test.begin(),test.end(),[]
          (elem ele1,elem ele2){
        return ele1.second<ele2.second;
    });
    nextStationList.clear();
    //nextStation是根据代价函数排序后的
    for(auto& station:test){
        nextStationList.push_back(station.first);
    }
    return nextStationList;
}


//获取最少时间的线路（A*算法）,s1和s2是站点的哈希值，stationList是最终查询路线中站点的哈希值，edgesList是最终查询路线(形式为<int,int>)
bool VehicleMap::QueryTransfer_Astar(int s1, int s2, QList<int>&stationsList, QList<Edge>&edgesList)
{

    //清空存储站点类的列表
    stationsList.clear();
    //清空存储两个站点之间的边的类的列表
    edgesList.clear();
    //若起始站点和最终站点相同

    if(s1==s2)
    {
        stationsList.push_back(s2);
        stationsList.push_back(s1);
        return true;
    }
    stationsList.push_back(s1);
    //建图，主要是根据edge信息，记录下每个站点和它直接相连的站点的经纬度信息
    makeGraph();
    //计算并存储所有站点，两两之间的经纬度距离
    makeFullMapDistanceInfo();
    //用字典的形式，记录每个站点和它直接相连的站点
    makeStation2Stations();
    //当前站点为出发站点
    QString cur_station=hash2Station[s1];
    QString next_station;
    //站点站
    QString end_station=hash2Station[s2];
    //A*算法中的Open表
    QVector<QString> path;
    //A*算法中的Close表
    QVector<QString> close;
    //下一站可选择站点列表
    QList<QString> nextStationList;
    //将出发站点加入close表
    close.push_back(cur_station);
    //循环直至达到目标站点
    while(cur_station!=end_station){
        //获得与当前节点相连的，经排序的节点列表
        nextStationList=leastCostStation(cur_station,end_station);
        //取代价函数最小的节点
        next_station=nextStationList[0];
        //判断该站点是否已访问
        while(close.contains(next_station)){
            //已访问后清除
            nextStationList.removeOne(next_station);
            //再次取代价函数最小的节点
            next_station=nextStationList[0];
        }
        //更新Close表
        close.push_back(next_station);
        //更新路径
        path.push_back(next_station);
        //添加路线
        edgesList.append(QPair<int,int>(stationsHash[cur_station],stationsHash[next_station]));
        //继续遍历下一个站点
        cur_station=next_station;
    }

    for(auto& p:path){
        //将经过的站点都添加一下
        stationsList.append(stationsHash[p]);
    }
    return true;
}
