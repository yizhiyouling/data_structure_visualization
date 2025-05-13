#ifndef DOUBLYLINKEDLISTWIDGET_H
#define DOUBLYLINKEDLISTWIDGET_H

#include <QWidget>
#include <vector>
#include <functional>
#include <QGraphicsItem>
#include "ArrowItem.h"  // 使用 ArrowItem 类
#include "NodeItem.h"

class QGraphicsScene;
class QGraphicsView;
class QLineEdit;
class QPushButton;

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
    void onClear();  // 清空当前窗口

private:
    QGraphicsView* view;
    QGraphicsScene* scene;
    QLineEdit* targetLineEdit;
    QPushButton* addEndButton;
    QPushButton* removeEndButton;
    QPushButton* addAfterButton;
    QPushButton* removeSpecifiedButton;
    QPushButton* clearButton;
    std::vector<NodeItem*> nodes;
    std::vector<QGraphicsItem*> arrowItems;
    int nextNodeId;
    ArrowItem* pointerItem; // 使用 ArrowItem 作为指针项

    void updateScene();
    // 指针遍历动画：将指针（显示在节点下方一定距离，并带箭身与箭头）依次移动到目标节点后执行回调函数
    void animatePointerTraversal(int targetIndex, std::function<void()> callback);
    // 新节点插入动画：新节点初始放在最终位置上方，渐显并向下移动到最终位置
    void animateNodeInsertion(NodeItem* node);
    // 删除节点动画：目标节点先缓慢上移，再渐隐消失
    void animateNodeDeletion(NodeItem* node, std::function<void()> callback);
};

#endif // DOUBLYLINKEDLISTWIDGET_H
