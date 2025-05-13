#include "BinaryTreeWidget.h"
#include "NodeItem.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>
#include <QMessageBox>
#include <cmath>

BinaryTreeWidget::BinaryTreeWidget(QWidget* parent)
    : QWidget(parent), nextNodeId(1)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    scene->setSceneRect(0, 0, 800, 400);
    mainLayout->addWidget(view);

    QHBoxLayout* controlLayout = new QHBoxLayout;
    addButton = new QPushButton("添加节点");
    removeButton = new QPushButton("删除末尾节点");
    clearButton = new QPushButton("清空");
    controlLayout->addWidget(addButton);
    controlLayout->addWidget(removeButton);
    controlLayout->addWidget(clearButton);
    mainLayout->addLayout(controlLayout);

    connect(addButton, &QPushButton::clicked, this, &BinaryTreeWidget::onAddNode);
    connect(removeButton, &QPushButton::clicked, this, &BinaryTreeWidget::onRemoveNode);
    connect(clearButton, &QPushButton::clicked, this, &BinaryTreeWidget::onClear);
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
    animateNodeDeletion(node, [this]() {
        updateScene();
    });
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
    // 清除场景中除 NodeItem 外的其他图形（即箭头）
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* item : items) {
        if (dynamic_cast<NodeItem*>(item) == nullptr)
        {
            scene->removeItem(item);
            delete item;
        }
    }

    int n = treeNodes.size();
    for (int i = 0; i < n; i++) {
        int level = static_cast<int>(std::floor(std::log2(i+1)));
        int indexInLevel = i - ((1 << level) - 1);
        int nodesInLevel = 1 << level;
        int sceneWidth = 800;
        int xSpacing = sceneWidth / (nodesInLevel + 1);
        int x = xSpacing * (indexInLevel + 1) - 40;
        int y = level * 70 + 20;
        treeNodes[i]->setPos(x, y);
    }

    // 绘制父子连接箭头
    for (int i = 0; i < n; i++) {
        int leftChild = 2 * i + 1;
        int rightChild = 2 * i + 2;
        QPointF parentCenter = treeNodes[i]->pos() + QPointF(40, 20);
        if (leftChild < n) {
            QPointF childCenter = treeNodes[leftChild]->pos() + QPointF(40, 20);
            QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(parentCenter, childCenter));
            line->setPen(QPen(Qt::black, 2));
            scene->addItem(line);
            double angle = std::atan2(childCenter.y() - parentCenter.y(), childCenter.x() - parentCenter.x());
            const double arrowSize = 10;
            QPointF arrowP1 = childCenter - QPointF(arrowSize * std::cos(angle - M_PI / 6),
                                                    arrowSize * std::sin(angle - M_PI / 6));
            QPointF arrowP2 = childCenter - QPointF(arrowSize * std::cos(angle + M_PI / 6),
                                                    arrowSize * std::sin(angle + M_PI / 6));
            QPolygonF arrowHead;
            arrowHead << childCenter << arrowP1 << arrowP2;
            QGraphicsPolygonItem* arrowHeadItem = new QGraphicsPolygonItem(arrowHead);
            arrowHeadItem->setBrush(Qt::black);
            arrowHeadItem->setPen(QPen(Qt::black));
            scene->addItem(arrowHeadItem);
        }
        if (rightChild < n) {
            QPointF childCenter = treeNodes[rightChild]->pos() + QPointF(40, 20);
            QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(parentCenter, childCenter));
            line->setPen(QPen(Qt::black, 2));
            scene->addItem(line);
            double angle = std::atan2(childCenter.y() - parentCenter.y(), childCenter.x() - parentCenter.x());
            const double arrowSize = 10;
            QPointF arrowP1 = childCenter - QPointF(arrowSize * std::cos(angle - M_PI / 6),
                                                    arrowSize * std::sin(angle - M_PI / 6));
            QPointF arrowP2 = childCenter - QPointF(arrowSize * std::cos(angle + M_PI / 6),
                                                    arrowSize * std::sin(angle + M_PI / 6));
            QPolygonF arrowHead;
            arrowHead << childCenter << arrowP1 << arrowP2;
            QGraphicsPolygonItem* arrowHeadItem = new QGraphicsPolygonItem(arrowHead);
            arrowHeadItem->setBrush(Qt::black);
            arrowHeadItem->setPen(QPen(Qt::black));
            scene->addItem(arrowHeadItem);
        }
    }
}

void BinaryTreeWidget::animateNodeInsertion(NodeItem* node) {
    QPropertyAnimation* anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void BinaryTreeWidget::animateNodeDeletion(NodeItem* node, std::function<void()> callback) {
    QPropertyAnimation* anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    connect(anim, &QPropertyAnimation::finished, this, [this, node, callback]() {
        scene->removeItem(node);
        if (callback) callback();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
