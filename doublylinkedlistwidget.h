#ifndef DOUBLYLINKEDLISTWIDGET_H
#define DOUBLYLINKEDLISTWIDGET_H

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

// DoublyLinkedListWidget 类用于展示双向链表的可视化控件，提供节点的插入、删除、清空等操作
class DoublyLinkedListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DoublyLinkedListWidget(QWidget* parent = nullptr);

private slots:
    void onAddEnd();    // 在链表尾部插入节点槽函数
    void onRemoveEnd(); // 删除链表尾部节点槽函数
    void onAddAfter();  // 在指定节点后插入新节点槽函数
    void onRemoveSpecified();   // 删除指定节点槽函数
    void onClear(); // 清空双向链表槽函数

private:
    QGraphicsScene* scene;
    QGraphicsView*  view;
    QLineEdit*      targetLineEdit;
    QPushButton*    addEndButton;
    QPushButton*    removeEndButton;
    QPushButton*    addAfterButton;
    QPushButton*    removeSpecifiedButton;
    QPushButton*    clearButton;

    std::vector<NodeItem*> nodes;  // 存储链表节点的容器
    std::vector<QGraphicsLineItem*> linesFwd, linesBwd; // 存储前向和后向连线的容器
    std::vector<ArrowItem*> arrowsFwd, arrowsBwd;    // 存储前向和后向箭头的容器
    int nextNodeId;


    void updateScene(); // 重新绘制/更新整个场景
    void drawConnection(NodeItem* a, NodeItem* b, bool forward);    // 绘制连接节点的前向/后向连线
    void animatePointerTraversal(int targetIndex, std::function<void()> callback);  // 动画展示指针遍历过程
    void animateNodeInsertion(NodeItem* node);  // 节点插入动画
    void animateNodeDeletion(NodeItem* node, std::function<void()> callback);   // 节点删除动画，删除后执行回调函数
};

#endif
