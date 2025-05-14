#ifndef TREETRAVERSALWIDGET_H
#define TREETRAVERSALWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QTextEdit>
#include <vector>
#include <queue>

// TreeNode 结构体，表示二叉树的一个节点
struct TreeNode {
    int id;
    QGraphicsEllipseItem* circle;  // 节点的图形表示
    QGraphicsTextItem* label;  // 节点的文本标签
    QGraphicsLineItem* parentEdge;  // 父节点和当前节点之间的连接线
    TreeNode* parent;  // 父节点指针
    TreeNode* left;  // 左子节点指针
    TreeNode* right;  // 右子节点指针
};

class TreeTraversalWidget : public QWidget {
    Q_OBJECT

public:
    explicit TreeTraversalWidget(QWidget* parent = nullptr);

protected:
    // 重写 showEvent 函数，在窗口显示时进行初始化操作
    void showEvent(QShowEvent* ev) override;

private slots:
    void onPreorder();  // 执行前序遍历
    void onInorder();   // 执行中序遍历
    void onPostorder(); // 插槽：执行后序遍历
    void onLevelorder();    // 插槽：执行层次遍历

private:
    QGraphicsScene* scene;
    QGraphicsView* mainView;
    QGraphicsView* thumbView;
    QPushButton* btnPre;
    QPushButton* btnIn;
    QPushButton* btnPost;
    QPushButton* btnLevel;
    QTextEdit* pathLog;
    TreeNode* nodes[16]  = {}; // 存储树的节点，最多支持 16 个节点
    TreeNode* root;
    std::vector<TreeNode*> visitOrder;    // 记录节点遍历的顺序
    int traversalDelayMs = 1000;    // 遍历的延迟时间，单位：毫秒

    void buildBinaryTree(); // 构建二叉树
    void layoutBinaryTree();    // 布局二叉树节点的位置
    void resetVisuals();    // 重置所有视觉元素
    void highlightTraversal();  // 高亮显示当前遍历路径
    void dfsPre(TreeNode* n);   // 前序遍历
    void dfsIn(TreeNode* n);    // 中序遍历
    void dfsPost(TreeNode* n);  // 后序遍历
};

#endif
