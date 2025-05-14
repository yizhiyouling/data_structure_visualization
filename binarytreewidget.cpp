#include "BinaryTreeWidget.h"
#include "NodeItem.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QTimer>
#include <QPen>
#include <QPainter>
#include <cmath>

// BinaryTreeWidget 构造函数
BinaryTreeWidget::BinaryTreeWidget(QWidget* parent)
    : QWidget(parent), nextNodeId(1)
{
    // 创建并设置主布局
    auto *mainLayout = new QVBoxLayout(this);

    // 初始化 QGraphicsScene 和 QGraphicsView
    scene = new QGraphicsScene(this);
    view  = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    view->setDragMode(QGraphicsView::ScrollHandDrag);  // 设置拖动模式
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);  // 设置缩放锚点
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);  // 设置转换锚点
    mainLayout->addWidget(view);  // 添加视图到布局

    // 创建并设置控制按钮布局
    auto *hlay = new QHBoxLayout;
    addButton    = new QPushButton("添加节点", this);
    removeButton = new QPushButton("删除末尾节点", this);
    clearButton  = new QPushButton("清空", this);
    hlay->addWidget(addButton);
    hlay->addWidget(removeButton);
    hlay->addWidget(clearButton);
    mainLayout->addLayout(hlay);  // 添加按钮布局到主布局

    // 连接按钮的点击信号到相应槽函数
    connect(addButton, &QPushButton::clicked, this, &BinaryTreeWidget::onAddNode);
    connect(removeButton, &QPushButton::clicked, this, &BinaryTreeWidget::onRemoveNode);
    connect(clearButton, &QPushButton::clicked, this, &BinaryTreeWidget::onClear);

    // 设置场景大小
    scene->setSceneRect(0, 0, 800, 500);
}

// 添加节点的槽函数
void BinaryTreeWidget::onAddNode() {
    // 创建一个新的节点
    NodeItem* node = new NodeItem(nextNodeId++, nullptr);
    node->setOpacity(0.0);  // 初始时设置节点的透明度为 0（不可见）
    treeNodes.push_back(node);  // 将新节点添加到节点列表
    scene->addItem(node);  // 将节点添加到场景中
    animateNodeInsertion(node);  // 执行节点插入动画
    updateScene();  // 更新场景布局
}

// 删除末尾节点的槽函数
void BinaryTreeWidget::onRemoveNode() {
    if (treeNodes.empty()) {
        // 如果树为空，显示提示信息
        QMessageBox::information(this, "提示", "二叉树为空！");
        return;
    }
    // 获取末尾节点并执行删除动画
    NodeItem* node = treeNodes.back();
    animateNodeDeletion(node, [this]() { updateScene(); });
    treeNodes.pop_back();  // 从节点列表中移除末尾节点
}

// 清空二叉树的槽函数
void BinaryTreeWidget::onClear() {
    // 删除所有节点
    for (auto node : treeNodes) {
        scene->removeItem(node);  // 从场景中移除节点
        delete node;  // 删除节点
    }
    treeNodes.clear();  // 清空节点列表
    nextNodeId = 1;  // 重置节点ID
    updateScene();  // 更新场景
}

// 更新场景的函数，重新布局所有节点和连线
void BinaryTreeWidget::updateScene() {
    // 清除旧的连线
    QList<QGraphicsItem*> items = scene->items();
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (!dynamic_cast<NodeItem*>(*it)) {
            scene->removeItem(*it);  // 删除非节点项
            delete *it;  // 释放内存
        }
    }

    // 获取当前树的节点数
    int n = treeNodes.size();
    const qreal W = 800;  // 场景宽度
    const qreal levelGap = 100;  // 节点之间的垂直间距
    const qreal R = 20;  // 节点半径

    // 布局所有节点
    for (int i = 0; i < n; i++) {
        int level = static_cast<int>(std::floor(std::log2(i + 1)));  // 计算节点所在层级
        int idx = i - ((1 << level) - 1);  // 当前层级的索引
        int count = 1 << level;  // 当前层级的节点数
        qreal xGap = W / (count + 1.0);  // 计算横向间距
        qreal x = xGap * (idx + 1) - R;  // 计算节点的横坐标
        qreal y = level * levelGap;  // 计算节点的纵坐标
        treeNodes[i]->setPos(x, y);  // 设置节点位置
    }

    // 绘制父子节点之间的连线
    for (int i = 0; i < n; i++) {
        int left  = 2*i + 1;  // 左子节点索引
        int right = 2*i + 2;  // 右子节点索引
        QPointF pc = treeNodes[i]->pos() + QPointF(R, R);  // 父节点中心点

        // lambda 表达式，绘制父节点与子节点之间的连线
        auto drawEdge = [&](int childIdx) {
            if (childIdx < n) {
                QPointF cc = treeNodes[childIdx]->pos() + QPointF(R, R);  // 子节点中心点
                qreal ang = std::atan2(cc.y() - pc.y(), cc.x() - pc.x());  // 计算父子节点的角度
                QPointF pEdge = pc  + QPointF(std::cos(ang)*R, std::sin(ang)*R);  // 父节点连线起点
                QPointF cEdge = cc  - QPointF(std::cos(ang)*R, std::sin(ang)*R);  // 子节点连线终点
                QGraphicsLineItem* edge = new QGraphicsLineItem(QLineF(pEdge, cEdge));  // 创建连线项
                edge->setPen(QPen(Qt::black, 2));  // 设置线条颜色和粗细
                scene->addItem(edge);  // 添加连线到场景中
            }
        };
        drawEdge(left);  // 绘制左子节点连线
        drawEdge(right);  // 绘制右子节点连线
    }
}

// 节点插入动画
void BinaryTreeWidget::animateNodeInsertion(NodeItem* node) {
    // 创建一个节点插入的动画
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);  // 动画持续时间 500ms
    anim->setStartValue(0.0);  // 动画开始时透明度为 0
    anim->setEndValue(1.0);  // 动画结束时透明度为 1
    anim->start(QAbstractAnimation::DeleteWhenStopped);  // 动画结束时删除动画对象
}

// 节点删除动画
void BinaryTreeWidget::animateNodeDeletion(NodeItem* node, std::function<void()> callback) {
    // 创建一个节点删除的动画
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);  // 动画持续时间 500ms
    anim->setStartValue(1.0);  // 动画开始时透明度为 1
    anim->setEndValue(0.0);  // 动画结束时透明度为 0
    connect(anim, &QPropertyAnimation::finished, this, [=]() {
        scene->removeItem(node);  // 动画结束后从场景中移除节点
        delete node;  // 删除节点
        if (callback) callback();  // 调用回调函数
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);  // 动画结束时删除动画对象
}
