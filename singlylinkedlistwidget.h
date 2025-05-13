#ifndef SINGLYLINKEDLISTWIDGET_H
#define SINGLYLINKEDLISTWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsLineItem>
#include "NodeItem.h"
#include "ArrowItem.h"
#include <vector>
#include <functional>

class SinglyLinkedListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SinglyLinkedListWidget(QWidget *parent = nullptr);

private slots:
    void onAddEnd();
    void onRemoveEnd();
    void onAddAfter();
    void onRemoveSpecified();
    void onClear();

private:
    QGraphicsScene *scene;
    QGraphicsView  *view;
    QLineEdit      *targetLineEdit;
    QPushButton    *addEndButton;
    QPushButton    *removeEndButton;
    QPushButton    *addAfterButton;
    QPushButton    *removeSpecifiedButton;
    QPushButton    *clearButton;

    std::vector<NodeItem*>           nodes;
    std::vector<QGraphicsLineItem*>  lines;
    std::vector<ArrowItem*>          arrows;
    int                               nextNodeId;

    void updateScene();
    void drawConnection(NodeItem* from, NodeItem* to);
    void animateNodeInsertion(NodeItem *node);
    void animateNodeDeletion(NodeItem *node, std::function<void()> callback);
};

#endif // SINGLYLINKEDLISTWIDGET_H
