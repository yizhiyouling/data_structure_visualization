#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPen>
#include <QBrush>

class ArrowItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    // 按值传递构造函数
    explicit ArrowItem(QPolygonF polygon, QGraphicsItem *parent = nullptr)
        : QObject(), QGraphicsPolygonItem(polygon, parent)
    {
    }
};

#endif // ARROWITEM_H
