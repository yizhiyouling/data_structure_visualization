#include "SinglyLinkedListWidget.h"
#include "NodeItem.h"
#include "ArrowItem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsLineItem>
#include <QPropertyAnimation>
#include <QMessageBox>
#include <QTimer>
#include <QPen>
#include <cmath>

SinglyLinkedListWidget::SinglyLinkedListWidget(QWidget* parent)
    : QWidget(parent), nextNodeId(1)
{
    // 创建垂直布局管理器，并将其设置为当前小部件的布局
    auto *vlay = new QVBoxLayout(this);

    scene = new QGraphicsScene(this);  // 创建一个图形场景
    view = new QGraphicsView(scene, this);  // 创建一个图形视图来显示场景
    view->setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    view->setDragMode(QGraphicsView::ScrollHandDrag);  // 设置为拖动模式
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);  // 设置视图缩放时的锚点
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);  // 设置视图变换时的锚点
    vlay->addWidget(view);  // 将视图添加到布局中

    // 控制面板
    auto *hlay = new QHBoxLayout;
    targetLineEdit = new QLineEdit(this);  // 输入框，用于指定目标节点编号
    targetLineEdit->setPlaceholderText("目标节点编号");

    addEndButton = new QPushButton("添加末尾节点", this);
    removeEndButton = new QPushButton("删除末尾节点", this);
    addAfterButton = new QPushButton("在指定节点后添加", this);
    removeSpecifiedButton = new QPushButton("删除指定节点", this);
    clearButton = new QPushButton("清空", this);

    hlay->addWidget(targetLineEdit);
    hlay->addWidget(addEndButton);
    hlay->addWidget(removeEndButton);
    hlay->addWidget(addAfterButton);
    hlay->addWidget(removeSpecifiedButton);
    hlay->addWidget(clearButton);
    vlay->addLayout(hlay);  // 将横向布局添加到垂直布局中

    // 信号与槽连接
    connect(addEndButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onAddEnd);
    connect(removeEndButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onRemoveEnd);
    connect(addAfterButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onAddAfter);
    connect(removeSpecifiedButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onRemoveSpecified);
    connect(clearButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onClear);

    // 初始化场景矩形区域
    scene->setSceneRect(0,0,800,200);
}

void SinglyLinkedListWidget::onAddEnd() {
    // 创建新节点并添加到链表末尾
    NodeItem* node = new NodeItem(nextNodeId++, nullptr);
    node->setOpacity(0.0);  // 设置节点的初始透明度为0
    nodes.push_back(node);  // 将节点添加到链表
    scene->addItem(node);  // 将节点添加到场景中
    animateNodeInsertion(node);  // 动画效果：节点插入
    updateScene();  // 更新场景
}

void SinglyLinkedListWidget::onRemoveEnd() {
    // 如果链表为空，则弹出提示信息
    if (nodes.empty()) {
        QMessageBox::information(this, "提示", "链表为空！");
        return;
    }
    NodeItem* node = nodes.back();  // 获取链表末尾节点
    animateNodeDeletion(node, [this]() { updateScene(); });  // 动画效果：节点删除
    nodes.pop_back();  // 删除链表末尾节点
}

void SinglyLinkedListWidget::onAddAfter() {
    // 获取目标节点编号并验证合法性
    bool ok;
    int target = targetLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入合法编号！");
        return;
    }
    // 查找目标节点
    int pos = -1;
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i]->getValue() == target) { pos = i; break; }
    }
    if (pos < 0) {
        QMessageBox::warning(this, "错误", "未找到目标节点！");
        return;
    }
    // 在目标节点后插入新节点
    NodeItem* newNode = new NodeItem(nextNodeId++, nullptr);
    newNode->setOpacity(0.0);
    nodes.insert(nodes.begin() + pos + 1, newNode);
    scene->addItem(newNode);
    animateNodeInsertion(newNode);  // 动画效果：节点插入
    QTimer::singleShot(600, this, &SinglyLinkedListWidget::updateScene);  // 延时更新场景
    targetLineEdit->clear();  // 清空输入框
}

void SinglyLinkedListWidget::onRemoveSpecified() {
    // 获取目标节点编号并验证合法性
    bool ok;
    int target = targetLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入合法编号！");
        return;
    }
    // 查找目标节点
    int pos = -1;
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i]->getValue() == target) { pos = i; break; }
    }
    if (pos < 0) {
        QMessageBox::warning(this, "错误", "未找到目标节点！");
        return;
    }
    // 删除指定节点
    NodeItem* node = nodes[pos];
    animateNodeDeletion(node, [this]() { updateScene(); });  // 动画效果：节点删除
    nodes.erase(nodes.begin() + pos);  // 删除节点
    targetLineEdit->clear();  // 清空输入框
}

void SinglyLinkedListWidget::onClear() {
    // 清空所有节点、连线和箭头
    for (auto *n : nodes) { scene->removeItem(n); delete n; }
    for (auto *l : lines) { scene->removeItem(l); delete l; }
    for (auto *a : arrows){ scene->removeItem(a); delete a; }
    nodes.clear(); lines.clear(); arrows.clear();  // 清空容器
    nextNodeId = 1;  // 重置节点ID
    targetLineEdit->clear();  // 清空输入框
    updateScene();  // 更新场景
}

void SinglyLinkedListWidget::updateScene() {
    // 清除旧的连线和箭头
    for (auto *l : lines) { scene->removeItem(l); delete l; }
    for (auto *a : arrows){ scene->removeItem(a); delete a; }
    lines.clear(); arrows.clear();  // 清空容器

    // 布局节点
    int n = nodes.size();
    const qreal startX = 50, gap = 100, y = 80;
    for (int i = 0; i < n; ++i) {
        nodes[i]->setPos(startX + i*gap, y);  // 设置每个节点的位置
    }

    // 绘制节点之间的连接
    for (int i = 0; i + 1 < n; ++i) {
        drawConnection(nodes[i], nodes[i+1]);
    }

    // 自动扩展场景
    QRectF br = scene->itemsBoundingRect();
    scene->setSceneRect(br.adjusted(-20,-20,20,20));  // 调整场景矩形区域
}

void SinglyLinkedListWidget::drawConnection(NodeItem* from, NodeItem* to) {
    // 计算连接线的起点和终点
    QPointF p = from->pos() + QPointF(20, 20);
    QPointF c = to->pos() + QPointF(20, 20);
    qreal ang = std::atan2(c.y() - p.y(), c.x() - p.x());  // 计算角度
    constexpr qreal R = 20.0;
    QPointF pEdge = p + QPointF(std::cos(ang) * R, std::sin(ang) * R);
    QPointF cEdge = c - QPointF(std::cos(ang) * R, std::sin(ang) * R);

    // 绘制连线
    auto *line = new QGraphicsLineItem(QLineF(pEdge, cEdge));
    line->setPen(QPen(Qt::black, 2));
    scene->addItem(line);
    lines.push_back(line);

    // 绘制箭头
    QPolygonF tri;
    tri << QPointF(0, 0) << QPointF(-8, -5) << QPointF(-8, 5);
    auto *arrow = new ArrowItem(tri);
    arrow->setBrush(Qt::black);
    arrow->setPos(cEdge);
    arrow->setRotation(ang * 180.0 / M_PI);
    scene->addItem(arrow);
    arrows.push_back(arrow);
}

void SinglyLinkedListWidget::animateNodeInsertion(NodeItem* node) {
    // 动画效果：节点插入时逐渐显示
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void SinglyLinkedListWidget::animateNodeDeletion(NodeItem* node, std::function<void()> callback) {
    // 动画效果：节点删除时逐渐消失
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    connect(anim, &QPropertyAnimation::finished, this, [=]() {
        scene->removeItem(node);  // 移除节点
        delete node;  // 删除节点
        if (callback) callback();  // 执行回调函数
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}
