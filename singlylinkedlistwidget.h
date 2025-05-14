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
    void onAddEnd();    // 添加节点到链表末尾
    void onRemoveEnd(); // 删除链表末尾节点
    void onAddAfter();  // 在指定节点后插入新节点
    void onRemoveSpecified();   // 删除指定节点
    void onClear(); // 清空链表

private:
    QGraphicsScene *scene;
    QGraphicsView  *view;
    QLineEdit      *targetLineEdit;
    QPushButton    *addEndButton;
    QPushButton    *removeEndButton;
    QPushButton    *addAfterButton;
    QPushButton    *removeSpecifiedButton;
    QPushButton    *clearButton;

    std::vector<NodeItem*> nodes; // 存储所有节点的列表
    std::vector<QGraphicsLineItem*> lines; // 存储节点之间连接的线条
    std::vector<ArrowItem*> arrows; // 存储箭头，表示节点指向关系
    int nextNodeId;

    void updateScene(); // 更新图形场景
    void drawConnection(NodeItem* from, NodeItem* to);  // 绘制从一个节点到另一个节点的连接线
    void animateNodeInsertion(NodeItem *node);  // 插入节点动画效果
    void animateNodeDeletion(NodeItem *node, std::function<void()> callback);   // 删除节点动画效果，并在删除完成后执行回调
};

#endif
