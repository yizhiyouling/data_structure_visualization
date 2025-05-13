#ifndef DOUBLYLINKEDLISTWIDGET_H
#define DOUBLYLINKEDLISTWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsLineItem>    // 添加：QGraphicsLineItem 定义
#include "NodeItem.h"
#include "ArrowItem.h"          // 添加：ArrowItem 定义

#include <vector>
#include <functional>

class DoublyLinkedListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DoublyLinkedListWidget(QWidget* parent = nullptr);

private slots:
    void onAddEnd();
    void onRemoveEnd();
    void onAddAfter();
    void onRemoveSpecified();
    void onClear();

private:
    QGraphicsScene* scene;
    QGraphicsView*  view;
    QLineEdit*      targetLineEdit;
    QPushButton*    addEndButton;
    QPushButton*    removeEndButton;
    QPushButton*    addAfterButton;
    QPushButton*    removeSpecifiedButton;
    QPushButton*    clearButton;

    std::vector<NodeItem*>           nodes;
    std::vector<QGraphicsLineItem*>  linesFwd, linesBwd;   // 现在能够识别 QGraphicsLineItem
    std::vector<ArrowItem*>          arrowsFwd, arrowsBwd; // 现在能够识别 ArrowItem
    int                               nextNodeId;

    void updateScene();
    void drawConnection(NodeItem* a, NodeItem* b, bool forward);
    void animatePointerTraversal(int targetIndex, std::function<void()> callback);
    void animateNodeInsertion(NodeItem* node);
    void animateNodeDeletion(NodeItem* node, std::function<void()> callback);
};

#endif // DOUBLYLINKEDLISTWIDGET_H
