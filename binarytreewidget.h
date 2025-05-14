#ifndef BINARYTREEWIDGET_H
#define BINARYTREEWIDGET_H

#include <QWidget>
#include <vector>
#include "NodeItem.h"
#include <QGraphicsLineItem>

class QGraphicsScene;
class QGraphicsView;
class QPushButton;

// BinaryTreeWidget 类用于展示二叉树的可视化控件，提供节点添加、删除、清空等功能
class BinaryTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BinaryTreeWidget(QWidget* parent = nullptr);

private slots:
    void onAddNode();   // 插入节点槽函数
    void onRemoveNode();    // 删除节点槽函数
    void onClear(); // 清空二叉树槽函数

private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* clearButton;
    std::vector<NodeItem*> treeNodes;    // 存储节点的容器
    int nextNodeId;


    void updateScene(); // 重新绘制/更新整个场景
    void animateNodeInsertion(NodeItem* node);  // 节点插入动画
    void animateNodeDeletion(NodeItem* node, std::function<void()> callback);   // 节点删除动画，删除后执行回调函数
};

#endif
