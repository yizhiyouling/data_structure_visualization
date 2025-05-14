#ifndef NODEITEM_H
#define NODEITEM_H

#include <QGraphicsObject>
#include <QFont>
#include <QRectF>
#include <QPointF>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

// NodeItem 类，用于在图形视图框架中表示节点对象
class NodeItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity)  // 支持透明度属性
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)            // 支持位置属性
public:
    explicit NodeItem(int value, QGraphicsItem* parent = nullptr);

    // 获取节点的边界矩形（用于绘制时确定区域）
    QRectF boundingRect() const override;

    // 绘制节点（绘制节点的内容，如文本、矩形等）
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    // 获取节点的值
    int getValue() const { return m_value; }

    // 设置节点的值，并调用 update() 刷新节点的显示
    void setValue(int v) {
        m_value = v;
        update();
    }

private:
    int   m_value;  // 节点值
    QFont m_font;   // 字体，控制节点值文本的显示样式
};

#endif
