
#include "BinaryTreeWidget.h"
#include "NodeItem.h"

#include <QGraphicsScene>        // 必须：定义 QGraphicsScene
#include <QGraphicsView>         // 必须：定义 QGraphicsView
#include <QGraphicsLineItem>
#include <QVBoxLayout>
#include<QPushButton>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QTimer>
#include <QPen>
#include <QPainter>              // 必须：用于 setRenderHint
#include <cmath>

BinaryTreeWidget::BinaryTreeWidget(QWidget* parent)
    : QWidget(parent), nextNodeId(1)
{
    auto *mainLayout = new QVBoxLayout(this);

    scene = new QGraphicsScene(this);
    view  = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    mainLayout->addWidget(view);

    auto *hlay = new QHBoxLayout;
    addButton    = new QPushButton("添加节点", this);
    removeButton = new QPushButton("删除末尾节点", this);
    clearButton  = new QPushButton("清空", this);
    hlay->addWidget(addButton);
    hlay->addWidget(removeButton);
    hlay->addWidget(clearButton);
    mainLayout->addLayout(hlay);

    connect(addButton, &QPushButton::clicked, this, &BinaryTreeWidget::onAddNode);
    connect(removeButton, &QPushButton::clicked, this, &BinaryTreeWidget::onRemoveNode);
    connect(clearButton, &QPushButton::clicked, this, &BinaryTreeWidget::onClear);

    scene->setSceneRect(0,0,800,500);
}

void BinaryTreeWidget::onAddNode() {
    NodeItem* node = new NodeItem(nextNodeId++, nullptr);
    node->setOpacity(0.0);
    treeNodes.push_back(node);
    scene->addItem(node);
    animateNodeInsertion(node);
    updateScene();
}

void BinaryTreeWidget::onRemoveNode() {
    if (treeNodes.empty()) {
        QMessageBox::information(this, "提示", "二叉树为空！");
        return;
    }
    NodeItem* node = treeNodes.back();
    animateNodeDeletion(node, [this]() { updateScene(); });
    treeNodes.pop_back();
}

void BinaryTreeWidget::onClear() {
    for (auto node : treeNodes) {
        scene->removeItem(node);
        delete node;
    }
    treeNodes.clear();
    nextNodeId = 1;
    updateScene();
}

void BinaryTreeWidget::updateScene() {
    // 清除旧边
    QList<QGraphicsItem*> items = scene->items();
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (!dynamic_cast<NodeItem*>(*it)) {
            scene->removeItem(*it);
            delete *it;
        }
    }

    int n = treeNodes.size();
    const qreal W = 800;
    const qreal levelGap = 100;
    const qreal R = 20; // 半径

    // 布局所有节点
    for (int i = 0; i < n; i++) {
        int level = static_cast<int>(std::floor(std::log2(i + 1)));
        int idx = i - ((1 << level) - 1);
        int count = 1 << level;
        qreal xGap = W / (count + 1.0);
        qreal x = xGap * (idx + 1) - R;
        qreal y = level * levelGap;
        treeNodes[i]->setPos(x, y);
    }

    // 绘制父子连线（不穿透节点）
    for (int i = 0; i < n; i++) {
        int left  = 2*i + 1;
        int right = 2*i + 2;
        QPointF pc = treeNodes[i]->pos() + QPointF(R, R);
        auto drawEdge = [&](int childIdx) {
            if (childIdx < n) {
                QPointF cc = treeNodes[childIdx]->pos() + QPointF(R, R);
                qreal ang = std::atan2(cc.y() - pc.y(), cc.x() - pc.x());
                QPointF pEdge = pc  + QPointF(std::cos(ang)*R, std::sin(ang)*R);
                QPointF cEdge = cc  - QPointF(std::cos(ang)*R, std::sin(ang)*R);
                QGraphicsLineItem* edge = new QGraphicsLineItem(QLineF(pEdge, cEdge));
                edge->setPen(QPen(Qt::black, 2));
                scene->addItem(edge);
            }
        };
        drawEdge(left);
        drawEdge(right);
    }
}
void BinaryTreeWidget::animateNodeInsertion(NodeItem* node) {
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
void BinaryTreeWidget::animateNodeDeletion(NodeItem* node, std::function<void()> callback) {
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    connect(anim, &QPropertyAnimation::finished, this, [=]() {
        scene->removeItem(node);
        delete node;
        if (callback) callback();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
