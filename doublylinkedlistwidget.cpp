#include "DoublyLinkedListWidget.h"
#include "NodeItem.h"
#include "ArrowItem.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
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

DoublyLinkedListWidget::DoublyLinkedListWidget(QWidget* parent)
    : QWidget(parent), nextNodeId(1)
{
    // 初始化布局与场景
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    view->setScene(scene);
    scene->setSceneRect(0, 0, 800, 300);
    mainLayout->addWidget(view);

    // 控制面板
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

    // 信号连接
    connect(addEndButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onAddEnd);
    connect(removeEndButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onRemoveEnd);
    connect(addAfterButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onAddAfter);
    connect(removeSpecifiedButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onRemoveSpecified);
    connect(clearButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onClear);

    // 创建指针项：使用 ArrowItem 类构造图形箭头（三角形），指针将显示在节点下方（例如偏移(40,50)
    QPolygonF arrowPolygon;
    arrowPolygon << QPointF(0,0) << QPointF(10,0) << QPointF(5, -15);
    pointerItem = new ArrowItem(arrowPolygon);
    pointerItem->setBrush(Qt::red);
    pointerItem->setPen(QPen(Qt::red));
    scene->addItem(pointerItem);
    pointerItem->hide();
}

    // 末尾添加：先计算最终位置，再将新节点初始放在该位置上方，并执行渐显和向下移动动画
void DoublyLinkedListWidget::onAddEnd() {
    int index = nodes.size();
    QPointF finalPos(20 + index * 100, 150);
    NodeItem* node = new NodeItem(nextNodeId++, nullptr);
    node->setOpacity(0.0);
    node->setPos(finalPos - QPointF(0, 30)); // 初始放在最终位置上方
    nodes.push_back(node);
    scene->addItem(node);
    animateNodeInsertion(node);
    QTimer::singleShot(600, [this](){ updateScene(); });
}

// 末尾删除：让指针先移动到末尾节点，然后执行删除动画
void DoublyLinkedListWidget::onRemoveEnd() {
    if (nodes.empty()) {
        QMessageBox::information(this, "提示", "链表为空！");
        return;
       }
    int index = nodes.size() - 1;
    NodeItem* node = nodes.back();
    animatePointerTraversal(index, [this, node]() {
        animateNodeDeletion(node, [this]() {
            updateScene();
        });
        nodes.pop_back();
    });
}

// 在指定节点后添加：先让指针移动到目标节点下方，再计算新节点最终位置为目标节点与其后继中点，新节点从上方落下
void DoublyLinkedListWidget::onAddAfter() {
    QString targetVal = targetLineEdit->text();
    if (targetVal.isEmpty()) {
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
        QPointF targetPos = nodes[pos]->pos();
        QPointF nextPos;
        if (pos + 1 < nodes.size()) {
            nextPos = nodes[pos+1]->pos();
        }
        else {
            nextPos = targetPos + QPointF(100, 0);
        }
        // 新节点最终位置为原下一节点位置
        QPointF finalPos = nextPos;
       QPointF initPos = finalPos - QPointF(0, 30);
        NodeItem* newNode = new NodeItem(nextNodeId++, nullptr);
        newNode->setOpacity(0.0);
        newNode->setPos(initPos);
       nodes.insert(nodes.begin() + pos + 1, newNode);
        scene->addItem(newNode);
        animateNodeInsertion(newNode);
        updateScene();
    });
    targetLineEdit->clear();
}

// 删除指定节点：先让指针移动到目标节点下方，然后目标节点上移后渐隐消失
void DoublyLinkedListWidget::onRemoveSpecified() {
    QString targetVal = targetLineEdit->text();
    if (targetVal.isEmpty()) {
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
        QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
        // 步骤1：目标节点上移
        QPropertyAnimation* animUp = new QPropertyAnimation(targetNode, "pos");
        animUp->setDuration(500);
        animUp->setStartValue(targetNode->pos());
        animUp->setEndValue(targetNode->pos() - QPointF(0, 30));
        group->addAnimation(animUp);
        // 步骤2：渐隐
        QPropertyAnimation* animFade = new QPropertyAnimation(targetNode, "opacity");
        animFade->setDuration(500);
        animFade->setStartValue(1.0);
        animFade->setEndValue(0.0);
        group->addAnimation(animFade);
        connect(group, &QSequentialAnimationGroup::finished, this, [this, pos, targetNode]() {
            scene->removeItem(targetNode);
            delete targetNode;
            nodes.erase(nodes.begin() + pos);
           updateScene();
        });
        group->start(QAbstractAnimation::DeleteWhenStopped);
    });
    targetLineEdit->clear();
}
void DoublyLinkedListWidget::onClear() {
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

void DoublyLinkedListWidget::animatePointerTraversal(int targetIndex, std::function<void()> callback) {
    if (nodes.empty() || targetIndex < 0 || targetIndex >= static_cast<int>(nodes.size())) {
        if (callback) callback();
        return;
    }
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    // 指针初始位置：第一个节点下方一定距离（例如(40,50)）
    pointerItem->setPos(nodes[0]->pos() + QPointF(40, 50));
    pointerItem->show();
    for (int i = 0; i <= targetIndex; ++i) {
        QPointF targetPos = nodes[i]->pos() + QPointF(40, 50);
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

void DoublyLinkedListWidget::animateNodeInsertion(NodeItem* node) {    // 新节点插入动画：先渐显，再向下移动至平行链表位置
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    QPropertyAnimation* animOpacity = new QPropertyAnimation(node, "opacity");
    animOpacity->setDuration(500);
    animOpacity->setStartValue(0.0);
    animOpacity->setEndValue(1.0);
    group->addAnimation(animOpacity);

    // 假设目标最终位置是由 updateScene 调整后的位置，动画中先将节点向下移动 30 像素，再由 updateScene 平移到最终位置
    QPropertyAnimation* animMove = new QPropertyAnimation(node, "pos");
    animMove->setDuration(500);
    animMove->setStartValue(node->pos());
    animMove->setEndValue(node->pos() + QPointF(0, 30));
    group->addAnimation(animMove);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void DoublyLinkedListWidget::animateNodeDeletion(NodeItem* node, std::function<void()> callback) {
    // 删除节点动画：目标节点先缓慢上移，然后渐隐消失
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    QPropertyAnimation* animUp = new QPropertyAnimation(node, "pos");
    animUp->setDuration(500);
    animUp->setStartValue(node->pos());
    animUp->setEndValue(node->pos() - QPointF(0, 30));
    group->addAnimation(animUp);
    QPropertyAnimation* animFade = new QPropertyAnimation(node, "opacity");
    animFade->setDuration(500);
    animFade->setStartValue(1.0);
    animFade->setEndValue(0.0);
    group->addAnimation(animFade);
    connect(group, &QSequentialAnimationGroup::finished, this, [this, node, callback]() {
        scene->removeItem(node);
        if (callback) callback();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void DoublyLinkedListWidget::updateScene() {
    // 清除旧箭头
    for (auto arrow : arrowItems) {
        scene->removeItem(arrow);
        delete arrow;
    }
    arrowItems.clear();

    int startX = 20;
    int y = 150;
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
        const double arrowSize = 10;
        // 绘制正向（蓝色，位于上方）和反向（绿色，位于下方）箭头
        for (size_t i = 0; i + 1 < nodes.size(); i++) {
            NodeItem* node1 = nodes[i];
            NodeItem* node2 = nodes[i + 1];
            // 正向箭头：位于节点边界上方：从 node1 的右边界到 node2 的左边界
            QPointF p1 = node1->pos() + QPointF(80, 20); // 80为节点宽度，20为垂直中心
            QPointF p2 = node2->pos() + QPointF(0, 20);
            QGraphicsLineItem* line = new QGraphicsLineItem(QLineF(p1, p2));
            line->setPen(QPen(Qt::blue, 2));
            scene->addItem(line);
            arrowItems.push_back(line);
            double angle = std::atan2(p2.y() - p1.y(), p2.x() - p1.x());
            QPointF arrowP1 = p2 - QPointF(arrowSize * std::cos(angle - M_PI / 6),arrowSize * std::sin(angle - M_PI / 6));
            QPointF arrowP2 = p2 - QPointF(arrowSize * std::cos(angle + M_PI / 6),arrowSize * std::sin(angle + M_PI / 6));
            QPolygonF arrowHead;
            arrowHead << p2 << arrowP1 << arrowP2;
            QGraphicsPolygonItem* arrowHeadItem = new QGraphicsPolygonItem(arrowHead);
            arrowHeadItem->setBrush(Qt::blue);
            arrowHeadItem->setPen(QPen(Qt::blue));
            scene->addItem(arrowHeadItem);
            arrowItems.push_back(arrowHeadItem);
            // 反向箭头：位于节点边界下方：从 node2 的左边界到 node1 的右边界
            QPointF bp1 = node2->pos() + QPointF(0, 20);
            QPointF bp2 = node1->pos() + QPointF(80, 20);
            QGraphicsLineItem* bline = new QGraphicsLineItem(QLineF(bp1, bp2));
            bline->setPen(QPen(Qt::green, 2));
            scene->addItem(bline);
            arrowItems.push_back(bline);
            double bangle = std::atan2(bp2.y() - bp1.y(), bp2.x() - bp1.x());
            QPointF barrowP1 = bp2 - QPointF(arrowSize * std::cos(bangle - M_PI / 6),arrowSize * std::sin(bangle - M_PI / 6));
            QPointF barrowP2 = bp2 - QPointF(arrowSize * std::cos(bangle + M_PI / 6),arrowSize * std::sin(bangle + M_PI / 6));
            QPolygonF barrowHead;
            barrowHead << bp2 << barrowP1 << barrowP2;
            QGraphicsPolygonItem* bArrowHeadItem = new QGraphicsPolygonItem(barrowHead);
            bArrowHeadItem->setBrush(Qt::green);
            bArrowHeadItem->setPen(QPen(Qt::green));
            scene->addItem(bArrowHeadItem);
            arrowItems.push_back(bArrowHeadItem);
        }
    });
}
