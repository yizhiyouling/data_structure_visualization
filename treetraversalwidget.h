#ifndef TREETRAVERSALWIDGET_H
#define TREETRAVERSALWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QTextEdit>
#include <vector>
#include <queue>

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
protected:
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
    TreeNode*              nodes[16]  = {};
    TreeNode*              root       = nullptr;
    std::vector<TreeNode*> visitOrder;
    int                    traversalDelayMs = 1000;
    void buildBinaryTree();
    void layoutBinaryTree();
    void resetVisuals();
    void highlightTraversal();
    void dfsPre(TreeNode* n);
    void dfsIn(TreeNode* n);
    void dfsPost(TreeNode* n);
};

#endif // TREETRAVERSALWIDGET_H
