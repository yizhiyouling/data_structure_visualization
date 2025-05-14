#include "NodeItem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>

NodeItem::NodeItem(int value, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_value(value)
{
    // 初始化字体：设置字号为 14，并加粗
    m_font.setPointSize(14);
    m_font.setBold(true);
}

// 定义节点的边界矩形（节点大小为 40×40）
QRectF NodeItem::boundingRect() const
{
    return QRectF(0, 0, 40, 40);
}

// 绘制节点的图形表示
void NodeItem::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem* ,
                     QWidget* )
{
    QRectF rect = boundingRect();

    // 设置蓝色背景并绘制圆形
    painter->setBrush(Qt::blue);  // 设置填充色为蓝色
    painter->setPen(QPen(Qt::black, 2));  // 设置边框为黑色，宽度为 2
    painter->drawEllipse(rect);  // 绘制圆形

    // 设置字体为之前初始化的加粗字体
    painter->setFont(m_font);
    painter->setPen(Qt::white);  // 设置文字颜色为白色
    // 绘制节点值，居中显示
    painter->drawText(rect, Qt::AlignCenter, QString::number(m_value));
}
