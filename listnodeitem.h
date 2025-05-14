#ifndef LISTNODEITEM_H
#define LISTNODEITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

// 自定义的图形节点项类，表示链表中的一个节点
class ListNodeItem : public QGraphicsRectItem
{
public:
    ListNodeItem(const QString &value, QGraphicsItem *parent = nullptr);    // 构造函数，初始化节点，设置节点值
    void setValue(const QString &value);    // 设置节点的值
    QString value() const;  // 获取节点的值

private:
    QGraphicsTextItem *textItem;
};

#endif
