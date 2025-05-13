#ifndef SINGLYLINKEDLISTWIDGET_H
#define SINGLYLINKEDLISTWIDGET_H

#include <QWidget>
#include <vector>
#include <functional>
#include <QGraphicsItem>
#include "ArrowItem.h"

class QGraphicsScene;
class QGraphicsView;
class QLineEdit;
class QPushButton;
class NodeItem;

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
    void onClear(); // 清空当前窗口
private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QLineEdit *targetLineEdit;
    QPushButton *addEndButton;
    QPushButton *removeEndButton;
    QPushButton *addAfterButton;
    QPushButton *removeSpecifiedButton;
    QPushButton *clearButton; // 清空按钮
    std::vector<NodeItem *> nodes;
    std::vector<QGraphicsItem *> arrowItems;
    int nextNodeId;
    ArrowItem *pointerItem; // 用于模拟指针（图形箭头）

    void updateScene();
    // 指针遍历动画：从头依次移动到目标节点（索引 targetIndex）
    void animatePointerTraversal(int targetIndex, std::function<void()> callback);
    void animateNodeInsertion(NodeItem *node);
    void animateNodeDeletion(NodeItem *node, std::function<void()> callback);
};

#endif // SINGLYLINKEDLISTWIDGET_H
