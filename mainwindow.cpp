#include "MainWindow.h"
#include <QStackedWidget>
#include <QMenuBar>
#include <QMenu>
#include "SinglyLinkedListWidget.h"
#include "DoublyLinkedListWidget.h"
#include "BinaryTreeWidget.h"
#include "TreeTraversalWidget.h"
#include "GraphWidget.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // 创建 QStackedWidget，它允许在不同的控件间切换
    QStackedWidget* stack = new QStackedWidget(this);
    setCentralWidget(stack);  // 设置中心部件为 stack

    // 创建各模块控件
    SinglyLinkedListWidget* singlyList = new SinglyLinkedListWidget;  // 单链表模块
    DoublyLinkedListWidget* doublyList = new DoublyLinkedListWidget;  // 双向链表模块
    BinaryTreeWidget* binaryTree = new BinaryTreeWidget;              // 二叉树模块
    TreeTraversalWidget* treeTraversal = new TreeTraversalWidget;     // 树的遍历模块
    GraphWidget* graphWidget = new GraphWidget;                        // 图模块

    // 将各模块添加到 stack 中
    stack->addWidget(singlyList);
    stack->addWidget(doublyList);
    stack->addWidget(binaryTree);
    stack->addWidget(treeTraversal);  // 新增
    stack->addWidget(graphWidget);

    // 创建菜单栏
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

    // 连接菜单项与显示相应模块的逻辑
    connect(singlyAction, &QAction::triggered, this, [stack]() { stack->setCurrentIndex(0); });
    connect(doublyAction, &QAction::triggered, this, [stack]() { stack->setCurrentIndex(1); });
    connect(binaryTreeAction, &QAction::triggered, this, [stack]() { stack->setCurrentIndex(2); });
    connect(traversalAction,  &QAction::triggered, this, [stack]() { stack->setCurrentIndex(3); });  // 新增
    connect(graphAction,     &QAction::triggered, this, [stack]() { stack->setCurrentIndex(4); });

    // 默认显示单链表模块
    stack->setCurrentIndex(0);
    setWindowTitle("数据结构可视化实验平台");
}
