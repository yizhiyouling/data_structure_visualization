#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsObject>

class NodeItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    explicit NodeItem(int value, QGraphicsItem* parent = nullptr);
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;
    int getValue() const { return m_value; }
    void setValue(int v) { m_value = v; update(); }
private:
    int m_value;
};

#endif // NODEITEM_H
