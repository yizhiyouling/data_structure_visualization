#include "TreeTraversalWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QMessageBox>
#include <QTimer>
#include <QShowEvent>
#include <cmath>

// 构造函数，初始化UI组件，布局，按钮连接信号槽
TreeTraversalWidget::TreeTraversalWidget(QWidget* parent)
    : QWidget(parent)
{
    auto *vlay = new QVBoxLayout(this);
    auto *hlay = new QHBoxLayout;

    // 初始化视图和场景
    scene    = new QGraphicsScene(this);
    mainView = new QGraphicsView(scene,this);
    mainView->setRenderHint(QPainter::Antialiasing);
    mainView->setDragMode(QGraphicsView::ScrollHandDrag);
    mainView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    mainView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    // 缩略图视图
    thumbView = new QGraphicsView(scene,this);
    thumbView->setFixedSize(200,150);
    thumbView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    thumbView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    hlay->addWidget(mainView);
    hlay->addWidget(thumbView);
    vlay->addLayout(hlay);

    // 初始化按钮
    btnPre   = new QPushButton("前序遍历", this);
    btnIn    = new QPushButton("中序遍历", this);
    btnPost  = new QPushButton("后序遍历", this);
    btnLevel = new QPushButton("层序遍历", this);
    auto *btnLay = new QHBoxLayout;
    btnLay->addWidget(btnPre);
    btnLay->addWidget(btnIn);
    btnLay->addWidget(btnPost);
    btnLay->addWidget(btnLevel);
    vlay->addLayout(btnLay);

    // 初始化文本框用于显示路径
    pathLog = new QTextEdit(this);
    pathLog->setReadOnly(true);
    pathLog->setPlaceholderText("遍历路径将在此显示");
    vlay->addWidget(pathLog);

    // 信号槽连接
    connect(btnPre,   &QPushButton::clicked, this, &TreeTraversalWidget::onPreorder);
    connect(btnIn,    &QPushButton::clicked, this, &TreeTraversalWidget::onInorder);
    connect(btnPost,  &QPushButton::clicked, this, &TreeTraversalWidget::onPostorder);
    connect(btnLevel, &QPushButton::clicked, this, &TreeTraversalWidget::onLevelorder);

    // 构建和布局二叉树
    buildBinaryTree();
    layoutBinaryTree();
}

// 重写showEvent方法，显示时调整视图和缩略图的显示区域
void TreeTraversalWidget::showEvent(QShowEvent* ev) {
    QWidget::showEvent(ev);
    QRectF br = scene->itemsBoundingRect();
    scene->setSceneRect(br.adjusted(-20,-20,20,20));  // 调整场景矩形边界
    thumbView->fitInView(br, Qt::KeepAspectRatio);     // 调整缩略图适配
}

// 构建二叉树，创建节点并建立父子关系
void TreeTraversalWidget::buildBinaryTree() {
    for (int i = 1; i <= 15; ++i) {
        nodes[i] = new TreeNode{i};
        nodes[i]->circle = new QGraphicsEllipseItem(0,0,40,40);
        nodes[i]->circle->setBrush(Qt::blue);   // 设置节点颜色为蓝色
        nodes[i]->circle->setPen(QPen(Qt::black,2));  // 设置边框颜色为黑色
        scene->addItem(nodes[i]->circle);
        nodes[i]->label = new QGraphicsTextItem(QString::number(i), nodes[i]->circle);
        QFont f; f.setPointSize(14); f.setBold(true);  // 设置字体样式
        nodes[i]->label->setFont(f);
        nodes[i]->label->setDefaultTextColor(Qt::white);  // 字体颜色为白色
        nodes[i]->label->setPos((40-nodes[i]->label->boundingRect().width())/2,
                                (40-nodes[i]->label->boundingRect().height())/2);
        nodes[i]->label->setZValue(1);
    }
    root = nodes[1];
    // 设定节点的父子关系
    for (int i = 1; i <= 7; ++i) {
        nodes[i]->left  = nodes[2*i]; nodes[2*i]->parent = nodes[i];
        nodes[i]->right = nodes[2*i+1]; nodes[2*i+1]->parent = nodes[i];
    }
}

// 布局二叉树，确定每个节点的显示位置
void TreeTraversalWidget::layoutBinaryTree() {
    constexpr int W=800, gapY=100;
    QRectF rect(0,0,W,gapY*4);
    for (int i = 1; i <= 15; ++i) {
        int lvl = int(std::floor(std::log2(i)));
        int idx = i - ((1<<lvl)-1), cnt=1<<lvl;
        qreal x = W*(idx/(qreal)(cnt+1)) - 20;  // 计算节点的x位置
        qreal y = lvl*gapY;                      // 计算节点的y位置
        nodes[i]->circle->setPos(x,y);
    }
    constexpr qreal R=20;
    for (int i = 1; i <= 7; ++i) {
        auto *p=nodes[i];
        for (auto *c: {p->left,p->right}) {
            QPointF pc = p->circle->pos()+QPointF(R,R);
            QPointF cc = c->circle->pos()+QPointF(R,R);
            qreal ang = std::atan2(cc.y()-pc.y(), cc.x()-pc.x());  // 计算角度
            QPointF pEdge=pc+QPointF(std::cos(ang)*R,std::sin(ang)*R);
            QPointF cEdge=cc-QPointF(std::cos(ang)*R,std::sin(ang)*R);
            auto *line=new QGraphicsLineItem(QLineF(pEdge,cEdge));
            line->setPen(QPen(Qt::black,2));  // 设置连线颜色为黑色
            scene->addItem(line);
            c->parentEdge=line;
        }
    }
    QRectF br = scene->itemsBoundingRect();
    scene->setSceneRect(br.adjusted(-20,-20,20,20));
}

// 重置节点和路径的视觉效果
void TreeTraversalWidget::resetVisuals() {
    for (int i = 1; i <= 15; ++i) {
        nodes[i]->circle->setBrush(Qt::blue);  // 恢复节点颜色为蓝色
        if (nodes[i]->parentEdge) nodes[i]->parentEdge->setPen(QPen(Qt::black,2));  // 恢复连线颜色为黑色
    }
    pathLog->clear();  // 清空路径日志
}

// 高亮显示遍历过程中的节点和边
void TreeTraversalWidget::highlightTraversal() {
    resetVisuals();
    int step = 0;
    for (TreeNode* tn : visitOrder) {
        QTimer::singleShot(traversalDelayMs * step, [this, tn]() {
            // 节点变色为黄色
            tn->circle->setBrush(Qt::yellow);
            // 边变色为红色
            if (tn->parentEdge)
                tn->parentEdge->setPen(QPen(Qt::red,2));
            // 序号文字改为黑色，确保可见
            tn->label->setDefaultTextColor(Qt::black);
            // 更新路径输出
            QStringList path;
            for (TreeNode* p = tn; p; p = p->parent)
                path.prepend(QString::number(p->id));
            pathLog->append(path.join(" -> "));
        });
        ++step;
    }
    QTimer::singleShot(traversalDelayMs * visitOrder.size(), [this]() {
        QMessageBox::information(this, "提示", "遍历结束");
    });
}

// 前序遍历的槽函数
void TreeTraversalWidget::onPreorder() {
    visitOrder.clear();
    dfsPre(root);  // 执行前序遍历
    highlightTraversal();
}

// 中序遍历的槽函数
void TreeTraversalWidget::onInorder() {
    visitOrder.clear();
    dfsIn(root);   // 执行中序遍历
    highlightTraversal();
}

// 后序遍历的槽函数
void TreeTraversalWidget::onPostorder() {
    visitOrder.clear();
    dfsPost(root); // 执行后序遍历
    highlightTraversal();
}

// 层序遍历的槽函数
void TreeTraversalWidget::onLevelorder() {
    visitOrder.clear();
    std::queue<TreeNode*> q;
    if (root) q.push(root);
    while (!q.empty()) {
        TreeNode* n = q.front(); q.pop();
        visitOrder.push_back(n);
        if (n->left)  q.push(n->left);
        if (n->right) q.push(n->right);
    }
    highlightTraversal();
}

// 前序遍历的递归实现
void TreeTraversalWidget::dfsPre(TreeNode* n) {
    if (!n) return;
    visitOrder.push_back(n);
    dfsPre(n->left);
    dfsPre(n->right);
}

// 中序遍历的递归实现
void TreeTraversalWidget::dfsIn(TreeNode* n) {
    if (!n) return;
    dfsIn(n->left);
    visitOrder.push_back(n);
    dfsIn(n->right);
}

// 后序遍历的递归实现
void TreeTraversalWidget::dfsPost(TreeNode* n) {
    if (!n) return;
    dfsPost(n->left);
    dfsPost(n->right);
    visitOrder.push_back(n);
}
