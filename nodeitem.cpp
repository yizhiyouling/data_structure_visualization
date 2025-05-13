#include "NodeItem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

NodeItem::NodeItem(int value, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_value(value)
{
    // 初始化字体：字号 14、加粗
    m_font.setPointSize(14);
    m_font.setBold(true);
}

QRectF NodeItem::boundingRect() const
{
    // 节点大小 40×40
    return QRectF(0, 0, 40, 40);
}

void NodeItem::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem* /*option*/,
                     QWidget* /*widget*/)
{
    QRectF rect = boundingRect();

    // 绘制蓝色圆形背景
    painter->setBrush(Qt::blue);
    painter->setPen(QPen(Qt::black, 2));
    painter->drawEllipse(rect);

    // 绘制白色编号文字（居中）
    painter->setFont(m_font);
    painter->setPen(Qt::white);
    painter->drawText(rect, Qt::AlignCenter, QString::number(m_value));
}
