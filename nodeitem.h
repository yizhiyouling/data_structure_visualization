#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsObject>
#include <QFont>
#include <QRectF>
#include <QPointF>

// 前向声明
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class NodeItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    explicit NodeItem(int value, QGraphicsItem* parent = nullptr);

    // 必须实现的两个虚函数
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    int getValue() const { return m_value; }
    void setValue(int v) {
        m_value = v;
        update();
    }

private:
    int   m_value;
    QFont m_font;
};

#endif // NODEITEM_H
