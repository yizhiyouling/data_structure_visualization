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
    auto *vlay = new QVBoxLayout(this);

    // GraphicsView + Scene
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    vlay->addWidget(view);

    // Control panel
    auto *hlay = new QHBoxLayout;
    targetLineEdit        = new QLineEdit(this);
    targetLineEdit->setPlaceholderText("目标节点编号");
    addEndButton          = new QPushButton("添加末尾节点", this);
    removeEndButton       = new QPushButton("删除末尾节点", this);
    addAfterButton        = new QPushButton("在指定节点后添加", this);
    removeSpecifiedButton = new QPushButton("删除指定节点", this);
    clearButton           = new QPushButton("清空", this);
    hlay->addWidget(targetLineEdit);
    hlay->addWidget(addEndButton);
    hlay->addWidget(removeEndButton);
    hlay->addWidget(addAfterButton);
    hlay->addWidget(removeSpecifiedButton);
    hlay->addWidget(clearButton);
    vlay->addLayout(hlay);

    connect(addEndButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onAddEnd);
    connect(removeEndButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onRemoveEnd);
    connect(addAfterButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onAddAfter);
    connect(removeSpecifiedButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onRemoveSpecified);
    connect(clearButton, &QPushButton::clicked, this, &SinglyLinkedListWidget::onClear);

    // initial sceneRect
    scene->setSceneRect(0,0,800,200);
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
    animateNodeDeletion(node, [this]() { updateScene(); });
    nodes.pop_back();
}

void SinglyLinkedListWidget::onAddAfter() {
    bool ok;
    int target = targetLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入合法编号！");
        return;
    }
    int pos = -1;
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i]->getValue() == target) { pos = i; break; }
    }
    if (pos < 0) {
        QMessageBox::warning(this, "错误", "未找到目标节点！");
        return;
    }
    NodeItem* newNode = new NodeItem(nextNodeId++, nullptr);
    newNode->setOpacity(0.0);
    nodes.insert(nodes.begin() + pos + 1, newNode);
    scene->addItem(newNode);
    animateNodeInsertion(newNode);
    QTimer::singleShot(600, this, &SinglyLinkedListWidget::updateScene);
    targetLineEdit->clear();
}

void SinglyLinkedListWidget::onRemoveSpecified() {
    bool ok;
    int target = targetLineEdit->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "输入错误", "请输入合法编号！");
        return;
    }
    int pos = -1;
    for (int i = 0; i < (int)nodes.size(); ++i) {
        if (nodes[i]->getValue() == target) { pos = i; break; }
    }
    if (pos < 0) {
        QMessageBox::warning(this, "错误", "未找到目标节点！");
        return;
    }
    NodeItem* node = nodes[pos];
    animateNodeDeletion(node, [this]() { updateScene(); });
    nodes.erase(nodes.begin() + pos);
    targetLineEdit->clear();
}

void SinglyLinkedListWidget::onClear() {
    for (auto *n : nodes) { scene->removeItem(n); delete n; }
    for (auto *l : lines) { scene->removeItem(l); delete l; }
    for (auto *a : arrows){ scene->removeItem(a); delete a; }
    nodes.clear(); lines.clear(); arrows.clear();
    nextNodeId = 1;
    targetLineEdit->clear();
    updateScene();
}

void SinglyLinkedListWidget::updateScene() {
    // clear old
    for (auto *l : lines) { scene->removeItem(l); delete l; }
    for (auto *a : arrows){ scene->removeItem(a); delete a; }
    lines.clear(); arrows.clear();

    // layout nodes
    int n = nodes.size();
    const qreal startX=50, gap=100, y=80;
    for (int i = 0; i < n; ++i) {
        nodes[i]->setPos(startX + i*gap, y);
    }
    // draw connections
    for (int i = 0; i + 1 < n; ++i) {
        drawConnection(nodes[i], nodes[i+1]);
    }

    // auto expand scene
    QRectF br = scene->itemsBoundingRect();
    scene->setSceneRect(br.adjusted(-20,-20,20,20));
}

void SinglyLinkedListWidget::drawConnection(NodeItem* from, NodeItem* to) {
    QPointF p = from->pos() + QPointF(20,20);
    QPointF c = to  ->pos() + QPointF(20,20);
    qreal ang = std::atan2(c.y()-p.y(), c.x()-p.x());
    constexpr qreal R = 20.0;
    QPointF pEdge = p + QPointF(std::cos(ang)*R, std::sin(ang)*R);
    QPointF cEdge = c - QPointF(std::cos(ang)*R, std::sin(ang)*R);

    // line
    auto *line = new QGraphicsLineItem(QLineF(pEdge, cEdge));
    line->setPen(QPen(Qt::black, 2));
    scene->addItem(line);
    lines.push_back(line);

    // arrow
    QPolygonF tri;
    tri << QPointF(0,0) << QPointF(-8,-5) << QPointF(-8,5);
    auto *arrow = new ArrowItem(tri);
    arrow->setBrush(Qt::black);
    arrow->setPos(cEdge);
    arrow->setRotation(ang * 180.0/M_PI);
    scene->addItem(arrow);
    arrows.push_back(arrow);
}

void SinglyLinkedListWidget::animateNodeInsertion(NodeItem *node) {
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void SinglyLinkedListWidget::animateNodeDeletion(NodeItem *node, std::function<void()> callback) {
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
