#ifndef ARROWITEM_H
#define ARROWITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include <QPen>
#include <QBrush>

// ArrowItem 类用于在图形场景中绘制一个可视化箭头。
class ArrowItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    // 构造函数，接收箭头多边形形状和可选的父图元
    explicit ArrowItem(QPolygonF polygon, QGraphicsItem *parent = nullptr)
        : QObject(), QGraphicsPolygonItem(polygon, parent)
    {
    }
};

#endif
