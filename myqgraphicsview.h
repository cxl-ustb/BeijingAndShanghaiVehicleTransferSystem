#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>
//MyQGraphicsView父类为QGraphicsView，该类主要实现放大、缩小、重置、刷新地图的功能
class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MyQGraphicsView(QWidget *parent = nullptr);

    void zoomIn();
    void zoomOut();
    void resetZoom();
    void refresh();

private:
    void zoom(double);

signals:

};

#endif // MYQGRAPHICSVIEW_H
