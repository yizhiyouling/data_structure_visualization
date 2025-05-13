#include "ArrowItem.h"
#include "NodeItem.h"
#include <QLineF>

ArrowItem::ArrowItem(NodeItem *startNode, NodeItem *endNode, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent), startNode(startNode), endNode(endNode)
{
    setPen(QPen(Qt::black, 2));
    updatePosition();
}

void ArrowItem::updatePosition()
{
    if (!startNode || !endNode)
        return;

    // 计算起点和终点
    QPointF startPos = startNode->scenePos() + QPointF(40, 20); // 调整箭头位置
    QPointF endPos = endNode->scenePos() + QPointF(0, 20);

    // 设置箭头线
    QLineF line(startPos, endPos);
    setLine(line);
}
