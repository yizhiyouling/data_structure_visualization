#include "listnodeitem.h"
#include <QBrush>
#include <QPen>

ListNodeItem::ListNodeItem(const QString &value, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    // 设置节点大小、边框和背景颜色
    setRect(0, 0, 80, 40);
    setPen(QPen(Qt::black));
    setBrush(QBrush(Qt::lightGray));

    // 添加显示文本
    textItem = new QGraphicsTextItem(value, this);
    textItem->setDefaultTextColor(Qt::black);
    // 将文本放置在矩形内部适当位置
    textItem->setPos(10, 10);
}

void ListNodeItem::setValue(const QString &value)
{
    textItem->setPlainText(value);
}

QString ListNodeItem::value() const
{
    return textItem->toPlainText();
}
