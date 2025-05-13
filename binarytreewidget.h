#ifndef BINARYTREEWIDGET_H
#define BINARYTREEWIDGET_H

#include <QWidget>
#include <vector>
#include <functional>

class QGraphicsScene;
class QGraphicsView;
class QPushButton;
class NodeItem;

class BinaryTreeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BinaryTreeWidget(QWidget* parent = nullptr);
private slots:
    void onAddNode();
    void onRemoveNode();
    void onClear();
private:
    QGraphicsScene* scene;
    QGraphicsView* view;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* clearButton;
    std::vector<NodeItem*> treeNodes; // 用数组表示完整二叉树
    int nextNodeId;

    void updateScene();
    void animateNodeInsertion(NodeItem* node);
    void animateNodeDeletion(NodeItem* node, std::function<void()> callback);
};

#endif // BINARYTREEWIDGET_H
