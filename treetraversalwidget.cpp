#include "TreeTraversalWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QMessageBox>
#include <QTimer>
#include <cmath>
#include <QShowEvent>

TreeTraversalWidget::TreeTraversalWidget(QWidget* parent)
    : QWidget(parent)
{
    auto *vlay = new QVBoxLayout(this);
    auto *hlay = new QHBoxLayout;
    scene    = new QGraphicsScene(this);
    mainView = new QGraphicsView(scene, this);
    mainView->setMinimumSize(800, 500);

    thumbView = new QGraphicsView(scene, this);
    thumbView->setFixedSize(200, 150);
    thumbView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    thumbView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    hlay->addWidget(mainView);
    hlay->addWidget(thumbView);
    vlay->addLayout(hlay);

    auto *btnLay = new QHBoxLayout;
    btnPre   = new QPushButton("前序遍历", this);
    btnIn    = new QPushButton("中序遍历", this);
    btnPost  = new QPushButton("后序遍历", this);
    btnLevel = new QPushButton("层序遍历", this);
    btnLay->addWidget(btnPre);
    btnLay->addWidget(btnIn);
    btnLay->addWidget(btnPost);
    btnLay->addWidget(btnLevel);
    vlay->addLayout(btnLay);

    pathLog = new QTextEdit(this);
    pathLog->setReadOnly(true);
    pathLog->setPlaceholderText("遍历路径将在此显示");
    vlay->addWidget(pathLog);

    connect(btnPre,   &QPushButton::clicked, this, &TreeTraversalWidget::onPreorder);
    connect(btnIn,    &QPushButton::clicked, this, &TreeTraversalWidget::onInorder);
    connect(btnPost,  &QPushButton::clicked, this, &TreeTraversalWidget::onPostorder);
    connect(btnLevel, &QPushButton::clicked, this, &TreeTraversalWidget::onLevelorder);

    buildBinaryTree();
    layoutBinaryTree();

    setWindowTitle("二叉树遍历演示");
}

void TreeTraversalWidget::showEvent(QShowEvent* ev) {
    QWidget::showEvent(ev);
    // 确保视图显示后再 fitInView，避免 scene rect 尚未计算好导致 crash
    auto rect = scene->itemsBoundingRect();
    scene->setSceneRect(rect);
    thumbView->fitInView(rect, Qt::KeepAspectRatio);
}

void TreeTraversalWidget::buildBinaryTree() {
    for (int i = 1; i <= 15; ++i) {
        nodes[i] = new TreeNode{ i };
        // 圆形节点
        nodes[i]->circle = new QGraphicsEllipseItem(0,0,40,40);
        nodes[i]->circle->setBrush(Qt::blue);
        nodes[i]->circle->setPen(QPen(Qt::black,2));
        scene->addItem(nodes[i]->circle);

        // 文本标签：放大字体，并置于最上层
        QFont font;
        font.setPointSize(14);  // 这里把字号调大到14
        font.setBold(true);
        nodes[i]->label = new QGraphicsTextItem(QString::number(i), nodes[i]->circle);
        nodes[i]->label->setFont(font);
        nodes[i]->label->setDefaultTextColor(Qt::white);
        nodes[i]->label->setPos( (40 - nodes[i]->label->boundingRect().width())/2,
                                (40 - nodes[i]->label->boundingRect().height())/2 );
        nodes[i]->label->setZValue(1);
    }
    root = nodes[1];
    for (int i = 1; i <= 7; ++i) {
        nodes[i]->left  = nodes[2*i];
        nodes[2*i]->parent = nodes[i];
        nodes[i]->right = nodes[2*i+1];
        nodes[2*i+1]->parent = nodes[i];
    }
}

void TreeTraversalWidget::layoutBinaryTree() {
    constexpr int W = 800, levelGap=100;
    scene->setSceneRect(0,0,W,levelGap*4+50);
    for (int i = 1; i <= 15; ++i) {
        int lvl = int(std::floor(std::log2(i)));
        int idx = i - ((1<<lvl)-1);
        int cnt = 1<<lvl;
        qreal dx = W/(cnt+1.0);
        qreal x = dx*idx - 20;
        qreal y = lvl*levelGap;
        nodes[i]->circle->setPos(x,y);
    }
    constexpr qreal R=20.0;
    for (int i = 1; i <= 7; ++i) {
        auto *p = nodes[i];
        for (auto *c : {p->left,p->right}) {
            QPointF pc = p->circle->pos() + QPointF(R,R);
            QPointF cc = c->circle->pos() + QPointF(R,R);
            qreal ang = std::atan2(cc.y()-pc.y(), cc.x()-pc.x());
            QPointF pEdge = pc + QPointF(std::cos(ang)*R, std::sin(ang)*R);
            QPointF cEdge = cc - QPointF(std::cos(ang)*R, std::sin(ang)*R);
            auto *e = new QGraphicsLineItem(QLineF(pEdge,cEdge));
            e->setPen(QPen(Qt::black,2));
            scene->addItem(e);
            c->parentEdge = e;
        }
    }
}

void TreeTraversalWidget::resetVisuals() {
    for (int i = 1; i <= 15; ++i) {
        nodes[i]->circle->setBrush(Qt::blue);
        if (nodes[i]->parentEdge) nodes[i]->parentEdge->setPen(QPen(Qt::black,2));
    }
    pathLog->clear();
}

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

void TreeTraversalWidget::onPreorder() {
    visitOrder.clear();
    dfsPre(root);
    highlightTraversal();
}

void TreeTraversalWidget::onInorder() {
    visitOrder.clear();
    dfsIn(root);
    highlightTraversal();
}

void TreeTraversalWidget::onPostorder() {
    visitOrder.clear();
    dfsPost(root);
    highlightTraversal();
}

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

void TreeTraversalWidget::dfsPre(TreeNode* n) {
    if (!n) return;
    visitOrder.push_back(n);
    dfsPre(n->left);
    dfsPre(n->right);
}

void TreeTraversalWidget::dfsIn(TreeNode* n) {
    if (!n) return;
    dfsIn(n->left);
    visitOrder.push_back(n);
    dfsIn(n->right);
}

void TreeTraversalWidget::dfsPost(TreeNode* n) {
    if (!n) return;
    dfsPost(n->left);
    dfsPost(n->right);
    visitOrder.push_back(n);
}
