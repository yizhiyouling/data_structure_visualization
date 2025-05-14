#include "listnodeitem.h"
#include <QBrush>
#include <QPen>

// 构造函数：初始化链表节点的外观和显示文本
ListNodeItem::ListNodeItem(const QString &value, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // 设置节点的大小（80x40）、边框颜色为黑色、背景颜色为浅灰色
    setRect(0, 0, 80, 40);
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::lightGray));

    // 创建并设置文本项，显示节点的值
    textItem = new QGraphicsTextItem(value, this);
    textItem->setDefaultTextColor(Qt::black);
    textItem->setPos(10, 10);   // 设置文本项的位置，使其位于矩形的中心
}

// 设置节点的值，并更新显示文本
void ListNodeItem::setValue(const QString &value)
{
    textItem->setPlainText(value);
}

// 获取节点的当前值
QString ListNodeItem::value() const
{
    return textItem->toPlainText();
}
