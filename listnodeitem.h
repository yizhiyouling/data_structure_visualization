#ifndef LISTNODEITEM_H
#define LISTNODEITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>

class ListNodeItem : public QGraphicsRectItem
{
public:
    ListNodeItem(const QString &value, QGraphicsItem *parent = nullptr);
    void setValue(const QString &value);
    QString value() const;

private:
    QGraphicsTextItem *textItem;
};

#endif // LISTNODEITEM_H
