#include "MainWindow.h"
#include <QStackedWidget>
#include <QMenuBar>
#include <QMenu>
#include "SinglyLinkedListWidget.h"
#include "DoublyLinkedListWidget.h"
#include "BinaryTreeWidget.h"
#include "TreeTraversalWidget.h"   // 新增
#include "GraphWidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QStackedWidget* stack = new QStackedWidget(this);
    setCentralWidget(stack);

    // 创建各模块
    SinglyLinkedListWidget* singlyList = new SinglyLinkedListWidget;
    DoublyLinkedListWidget* doublyList = new DoublyLinkedListWidget;
    BinaryTreeWidget* binaryTree = new BinaryTreeWidget;
    TreeTraversalWidget* treeTraversal = new TreeTraversalWidget;  // 新增
    GraphWidget* graphWidget = new GraphWidget;

    // 添加到 stack 中
    // 0 - 单链表，1 - 双向链表，2 - 二叉树，3 - 树的遍历，4 - 图
    stack->addWidget(singlyList);
    stack->addWidget(doublyList);
    stack->addWidget(binaryTree);
    stack->addWidget(treeTraversal);
    stack->addWidget(graphWidget);

    // 菜单栏
    QMenuBar* menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // “链表”菜单
    QMenu* listMenu = menuBar->addMenu("链表");
    QAction* singlyAction = listMenu->addAction("单链表");
    QAction* doublyAction = listMenu->addAction("双向链表");

    // “树”菜单
    QMenu* treeMenu = menuBar->addMenu("树");
    QAction* binaryTreeAction = treeMenu->addAction("二叉树");
    QAction* traversalAction  = treeMenu->addAction("二叉树的遍历");  // 新增

    // “图”菜单
    QAction* graphAction = menuBar->addAction("图");

    // 连接切换
    connect(singlyAction, &QAction::triggered, this, [stack]() { stack->setCurrentIndex(0); });
    connect(doublyAction, &QAction::triggered, this, [stack]() { stack->setCurrentIndex(1); });
    connect(binaryTreeAction, &QAction::triggered, this, [stack]() { stack->setCurrentIndex(2); });
    connect(traversalAction,  &QAction::triggered, this, [stack]() { stack->setCurrentIndex(3); });  // 新增
    connect(graphAction,     &QAction::triggered, this, [stack]() { stack->setCurrentIndex(4); });

    // 默认显示单链表模块
    stack->setCurrentIndex(0);
    setWindowTitle("数据结构可视化实验平台");
}
