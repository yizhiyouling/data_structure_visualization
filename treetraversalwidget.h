#ifndef TREETRAVERSALWIDGET_H
#define TREETRAVERSALWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QTextEdit>
#include <vector>
#include <queue>

// 二叉树节点结构
struct TreeNode {
    int                     id;
    QGraphicsEllipseItem*   circle     = nullptr;
    QGraphicsTextItem*      label      = nullptr;
    QGraphicsLineItem*      parentEdge = nullptr;
    TreeNode*               parent     = nullptr;
    TreeNode*               left       = nullptr;
    TreeNode*               right      = nullptr;
};

class TreeTraversalWidget : public QWidget {
    Q_OBJECT
public:
    explicit TreeTraversalWidget(QWidget* parent = nullptr);

    // 调整遍历动画间隔，单位毫秒
    void setTraversalDelay(int ms) { traversalDelayMs = ms; }

protected:
    // 重载 showEvent，保证缩略图能在可视后正确 fitInView
    void showEvent(QShowEvent* ev) override;

private slots:
    void onPreorder();
    void onInorder();
    void onPostorder();
    void onLevelorder();

private:
    QGraphicsScene*        scene      = nullptr;
    QGraphicsView*         mainView   = nullptr;
    QGraphicsView*         thumbView  = nullptr;
    QPushButton*           btnPre     = nullptr;
    QPushButton*           btnIn      = nullptr;
    QPushButton*           btnPost    = nullptr;
    QPushButton*           btnLevel   = nullptr;
    QTextEdit*             pathLog    = nullptr;

    TreeNode*              nodes[16]  = {};  // 1..15
    TreeNode*              root       = nullptr;
    std::vector<TreeNode*> visitOrder;

    int                    traversalDelayMs = 1000;  // 每一步的动画间隔时间

    void buildBinaryTree();
    void layoutBinaryTree();
    void resetVisuals();
    void highlightTraversal();

    void dfsPre(TreeNode* n);
    void dfsIn(TreeNode* n);
    void dfsPost(TreeNode* n);
};

#endif // TREETRAVERSALWIDGET_H
