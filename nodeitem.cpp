#include "NodeItem.h"
#include <QPainter>
#include <QFont>

NodeItem::NodeItem(int value, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_value(value)
{
    setOpacity(1.0);
}

QRectF NodeItem::boundingRect() const {
    return QRectF(0, 0, 80, 40);
}

void NodeItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRectF rect = boundingRect();
    painter->setBrush(Qt::lightGray);
    painter->setPen(Qt::black);
    painter->drawRect(rect);
    painter->setFont(QFont("Arial", 12));
    painter->drawText(rect, Qt::AlignCenter, QString::number(m_value));
}
