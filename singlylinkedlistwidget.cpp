#include "SinglyLinkedListWidget.h"
#include "NodeItem.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QPauseAnimation>
#include <QParallelAnimationGroup>
#include <QMessageBox>
#include <QTimer>
#include <cmath>
#include <functional>

SinglyLinkedListWidget::SinglyLinkedListWidget(QWidget* parent)
    : QWidget(parent), nextNodeId(1)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    scene->setSceneRect(0, 0, 800, 300);
    mainLayout->addWidget(view);

    // 控制面板：目标节点输入框、操作按钮以及清空按钮
    QHBoxLayout* controlLayout = new QHBoxLayout;
    targetLineEdit = new QLineEdit;
    targetLineEdit->setPlaceholderText("目标节点编号 (用于插入/删除指定节点)");
    addEndButton = new QPushButton("添加末尾节点");
    removeEndButton = new QPushButton("删除末尾节点");
    addAfterButton = new QPushButton("在指定节点后添加");
    removeSpecifiedButton = new QPushButton("删除指定节点");
    clearButton = new QPushButton("清空");
    controlLayout->addWidget(targetLineEdit);
    controlLayout->addWidget(addEndButton);
    controlLayout->addWidget(removeEndButton);
    controlLayout->addWidget(addAfterButton);
    controlLayout->addWidget(removeSpecifiedButton);
    controlLayout->addWidget(clearButton);
    mainLayout->addLayout(controlLayout);

    connect(addEndButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onAddEnd);
    connect(removeEndButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onRemoveEnd);
    connect(addAfterButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onAddAfter);
    connect(removeSpecifiedButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onRemoveSpecified);
    connect(clearButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onClear);

    // 创建指针项，用图形箭头（三角形，指向上方）
    QPolygonF arrowPolygon;
    arrowPolygon << QPointF(0,0) << QPointF(10,0) << QPointF(5, -15);
    pointerItem = new ArrowItem(arrowPolygon);
    pointerItem->setBrush(Qt::red);
    pointerItem->setPen(QPen(Qt::red));
    scene->addItem(pointerItem);
    pointerItem->hide();
}

void SinglyLinkedListWidget::onAddEnd() {
    NodeItem* node = new NodeItem(nextNodeId++, nullptr);
    node->setOpacity(0.0);
    nodes.push_back(node);
    scene->addItem(node);
    animateNodeInsertion(node);
    updateScene();
}

void SinglyLinkedListWidget::onRemoveEnd() {
    if (nodes.empty()) {
        QMessageBox::information(this, "提示", "链表为空！");
        return;
    }
    NodeItem* node = nodes.back();
    animatePointerTraversal(nodes.size() - 1, [this, node]() {
        animateNodeDeletion(node, [this]() {
            updateScene();
        });
        nodes.pop_back();
    });
}

void SinglyLinkedListWidget::onAddAfter() {
    QString targetVal = targetLineEdit->text();
    if (targetVal.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入目标节点编号！");
        return;
    }
    int pos = -1;
    for (size_t i = 0; i < nodes.size(); i++) {
        if (QString::number(nodes[i]->getValue()) == targetVal) {
            pos = static_cast<int>(i);
            break;
        }
    }
    if (pos == -1) {
        QMessageBox::warning(this, "错误", "未找到目标节点！");
        return;
    }
    animatePointerTraversal(pos, [this, pos]() {
        NodeItem* newNode = new NodeItem(nextNodeId++, nullptr);
        newNode->setOpacity(0.0);
        nodes.insert(nodes.begin() + pos + 1, newNode);
        scene->addItem(newNode);
        animateNodeInsertion(newNode);
        updateScene();
    });
    targetLineEdit->clear();
}

void SinglyLinkedListWidget::onRemoveSpecified() {
    QString targetVal = targetLineEdit->text();
    if (targetVal.isEmpty()){
        QMessageBox::warning(this, "输入错误", "请输入目标节点编号！");
        return;
    }
    int pos = -1;
    for (size_t i = 0; i < nodes.size(); i++) {
        if (QString::number(nodes[i]->getValue()) == targetVal) {
            pos = static_cast<int>(i);
            break;
        }
    }
    if (pos == -1) {
        QMessageBox::warning(this, "错误", "未找到目标节点！");
        return;
    }
    animatePointerTraversal(pos, [this, pos]() {
        NodeItem* targetNode = nodes[pos];
        animateNodeDeletion(targetNode, [this]() {
            updateScene();
        });
        nodes.erase(nodes.begin() + pos);
    });
    targetLineEdit->clear();
}

void SinglyLinkedListWidget::onClear() {
    for (auto node : nodes) {
        scene->removeItem(node);
        delete node;
    }
    nodes.clear();
    for (auto arrow : arrowItems) {
        scene->removeItem(arrow);
        delete arrow;
    }
    arrowItems.clear();
    nextNodeId = 1;
    targetLineEdit->clear();
    pointerItem->hide();
    updateScene();
}

void SinglyLinkedListWidget::animatePointerTraversal(int targetIndex, std::function<void()> callback) {
    if (nodes.empty() || targetIndex < 0 || targetIndex >= static_cast<int>(nodes.size())) {
        if (callback) callback();
        return;
    }
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    // 指针从第一个节点上方开始：节点中心上方一定距离
    pointerItem->setPos(nodes[0]->pos() + QPointF(40, -20));
    pointerItem->show();
    for (int i = 0; i <= targetIndex; ++i) {
        QPointF targetPos = nodes[i]->pos() + QPointF(40, -20);
        QPropertyAnimation* anim = new QPropertyAnimation(pointerItem, "pos");
        anim->setDuration(300);
        anim->setEndValue(targetPos);
        group->addAnimation(anim);
        if (i < targetIndex) {
            QPauseAnimation* pause = new QPauseAnimation(200);
            group->addAnimation(pause);
        }
    }
    connect(group, &QSequentialAnimationGroup::finished, this, [this, callback]() {
        pointerItem->hide();
        if (callback) callback();
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void SinglyLinkedListWidget::animateNodeInsertion(NodeItem* node) {
    QPropertyAnimation* anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void SinglyLinkedListWidget::animateNodeDeletion(NodeItem* node, std::function<void()> callback) {
    QPropertyAnimation* anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    connect(anim, &QPropertyAnimation::finished, this, [this, node, callback]() {
        scene->removeItem(node);
        if (callback)
            callback();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void SinglyLinkedListWidget::updateScene() {
    // 清除旧箭头
    for (auto arrow : arrowItems) {
        scene->removeItem(arrow);
        delete arrow;
    }
    arrowItems.clear();

    int startX = 20;
    int y = 150;
    // 平移节点（动画）
    QParallelAnimationGroup* group = new QParallelAnimationGroup(this);
    for (size_t i = 0; i < nodes.size(); i++) {
        QPointF targetPos(startX + static_cast<int>(i * 100), y);
        QPropertyAnimation* anim = new QPropertyAnimation(nodes[i], "pos");
        anim->setDuration(500);
        anim->setEndValue(targetPos);
        group->addAnimation(anim);
    }
    group->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(600, [this, startX, y]() {
        // 绘制节点间的箭头（使用黑色）
        for (size_t i = 0; i + 1 < nodes.size(); i++) {
            NodeItem* node1 = nodes[i];
            NodeItem* node2 = nodes[i + 1];
            QPointF p1 = node1->pos() + QPointF(80, 20);
            QPointF p2 = node2->pos() + QPointF(0, 20);
            QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(p1, p2));
            line->setPen(QPen(Qt::black, 2));
            scene->addItem(line);
            arrowItems.push_back(line);

            double angle = std::atan2(p2.y() - p1.y(), p2.x() - p1.x());
            const double arrowSize = 10;
            QPointF arrowP1 = p2 - QPointF(arrowSize * std::cos(angle - M_PI / 6),
                                           arrowSize * std::sin(angle - M_PI / 6));
            QPointF arrowP2 = p2 - QPointF(arrowSize * std::cos(angle + M_PI / 6),
                                           arrowSize * std::sin(angle + M_PI / 6));
            QPolygonF arrowHead;
            arrowHead << p2 << arrowP1 << arrowP2;
            QGraphicsPolygonItem* arrowHeadItem = new QGraphicsPolygonItem(arrowHead);
            arrowHeadItem->setBrush(Qt::black);
            arrowHeadItem->setPen(QPen(Qt::black));
            scene->addItem(arrowHeadItem);
            arrowItems.push_back(arrowHeadItem);
        }
    });
}
