#include "DoublyLinkedListWidget.h"
#include "NodeItem.h"
#include "ArrowItem.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsLineItem>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QMessageBox>
#include <QTimer>
#include <QPen>
#include <cmath>

// 构造函数，初始化控件并连接信号槽
DoublyLinkedListWidget::DoublyLinkedListWidget(QWidget* parent)
    : QWidget(parent), nextNodeId(1)
{
    auto *vlay = new QVBoxLayout(this);

    // 初始化场景和视图
    scene = new QGraphicsScene(this);
    view  = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing);
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    view->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    vlay->addWidget(view);

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

    // 连接信号与槽
    connect(addEndButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onAddEnd);
    connect(removeEndButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onRemoveEnd);
    connect(addAfterButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onAddAfter);
    connect(removeSpecifiedButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onRemoveSpecified);
    connect(clearButton, &QPushButton::clicked, this, &DoublyLinkedListWidget::onClear);
}

// 在链表末尾添加节点
void DoublyLinkedListWidget::onAddEnd() {
    NodeItem* node = new NodeItem(nextNodeId++, nullptr);
    node->setOpacity(0.0);
    nodes.push_back(node);
    scene->addItem(node);
    animateNodeInsertion(node);
    QTimer::singleShot(600, this, &DoublyLinkedListWidget::updateScene);
}

// 删除链表末尾节点
void DoublyLinkedListWidget::onRemoveEnd() {
    if (nodes.empty()) {
        QMessageBox::information(this, "提示", "链表为空！");
        return;
    }
    int idx = nodes.size() - 1;
    NodeItem* node = nodes.back();
    animatePointerTraversal(idx, [=]() {
        animateNodeDeletion(node, [=]() { updateScene(); });
        nodes.pop_back();
    });
}

// 在指定节点后插入新节点
void DoublyLinkedListWidget::onAddAfter() {
    bool ok; int target = targetLineEdit->text().toInt(&ok);
    if (!ok) { QMessageBox::warning(this,"输入错误","请输入合法编号"); return; }
    int pos=-1; for (int i=0;i<(int)nodes.size();++i)
        if(nodes[i]->getValue()==target){pos=i;break;}
    if(pos<0){QMessageBox::warning(this,"错误","未找到目标节点");return;}
    animatePointerTraversal(pos, [=]() {
        NodeItem* node = new NodeItem(nextNodeId++, nullptr);
        node->setOpacity(0.0);
        nodes.insert(nodes.begin()+pos+1, node);
        scene->addItem(node);
        animateNodeInsertion(node);
        updateScene();
    });
    targetLineEdit->clear();
}

// 删除指定节点
void DoublyLinkedListWidget::onRemoveSpecified() {
    bool ok; int target = targetLineEdit->text().toInt(&ok);
    if (!ok){ QMessageBox::warning(this,"输入错误","请输入合法编号"); return; }
    int pos=-1; for(int i=0;i<(int)nodes.size();++i)
        if(nodes[i]->getValue()==target){pos=i;break;}
    if(pos<0){ QMessageBox::warning(this,"错误","未找到目标节点"); return;}
    animatePointerTraversal(pos, [=]() {
        NodeItem* node = nodes[pos];
        animateNodeDeletion(node, [=]() {
            nodes.erase(nodes.begin()+pos);
            updateScene();
        });
    });
    targetLineEdit->clear();
}

// 清空链表，移除所有节点和连接
void DoublyLinkedListWidget::onClear() {
    for(auto* n: nodes){ scene->removeItem(n); delete n; }
    for(auto* l: linesFwd){ scene->removeItem(l); delete l; }
    for(auto* l: linesBwd){ scene->removeItem(l); delete l; }
    for(auto* a: arrowsFwd){ scene->removeItem(a); delete a; }
    for(auto* a: arrowsBwd){ scene->removeItem(a); delete a; }
    nodes.clear(); linesFwd.clear(); linesBwd.clear();
    arrowsFwd.clear(); arrowsBwd.clear();
    nextNodeId=1;
}

// 更新场景，重新排列节点并更新连线和箭头
void DoublyLinkedListWidget::updateScene() {
    // 清除旧连线与箭头
    for(auto* l: linesFwd){ scene->removeItem(l); delete l; }
    for(auto* a: arrowsFwd){ scene->removeItem(a); delete a; }
    for(auto* l: linesBwd){ scene->removeItem(l); delete l; }
    for(auto* a: arrowsBwd){ scene->removeItem(a); delete a; }
    linesFwd.clear(); arrowsFwd.clear();
    linesBwd.clear(); arrowsBwd.clear();

    int n = nodes.size();
    const qreal startX=50, gap=100, y=80;
    for(int i=0;i<n;++i){
        nodes[i]->setPos(startX + i*gap, y);
    }

    for(int i=0;i+1<n;++i){
        drawConnection(nodes[i],   nodes[i+1], true);
        drawConnection(nodes[i+1], nodes[i],   false);
    }
}

// 绘制连接两个节点的前向或后向连线
void DoublyLinkedListWidget::drawConnection(NodeItem* a, NodeItem* b, bool forward) {
    QPointF pa = a->pos() + QPointF(20,20);
    QPointF pb = b->pos() + QPointF(20,20);
    qreal ang = std::atan2(pb.y()-pa.y(), pb.x()-pa.x());
    constexpr qreal R=20;
    QPointF aEdge = pa + QPointF(std::cos(ang)*R, std::sin(ang)*R);
    QPointF bEdge = pb - QPointF(std::cos(ang)*R, std::sin(ang)*R);

    auto *line = new QGraphicsLineItem(QLineF(aEdge,bEdge));
    line->setPen(QPen(Qt::black,2));
    scene->addItem(line);
    if(forward) linesFwd.push_back(line);
    else         linesBwd.push_back(line);

    QPolygonF tri;
    tri << QPointF(0,0) << QPointF(-8,-5) << QPointF(-8,5);
    auto *arrow = new ArrowItem(tri);
    arrow->setBrush(Qt::black);
    arrow->setPos(bEdge);
    arrow->setRotation(ang * 180.0/M_PI);
    scene->addItem(arrow);
    if(forward) arrowsFwd.push_back(arrow);
    else         arrowsBwd.push_back(arrow);
}

// 动画展示指针遍历到目标节点
void DoublyLinkedListWidget::animatePointerTraversal(int targetIndex, std::function<void()> callback) {
    if (nodes.empty() || targetIndex < 0 || targetIndex >= (int)nodes.size()) {
        if (callback) callback();
        return;
    }
    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    connect(group, &QSequentialAnimationGroup::finished, this, [=]() {
        if (callback) callback();
    });
    group->start(QAbstractAnimation::DeleteWhenStopped);
}

// 节点插入时的动画效果
void DoublyLinkedListWidget::animateNodeInsertion(NodeItem* node) {
    auto *anim = new QPropertyAnimation(node, "opacity");
    anim->setDuration(500);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

// 节点删除时的动画效果
void DoublyLinkedListWidget::animateNodeDeletion(NodeItem* node, std::function<void()> callback) {
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

