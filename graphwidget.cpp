#include "GraphWidget.h"
#include <QLabel>
#include <QVBoxLayout>

// 构造函数，初始化图形控件
GraphWidget::GraphWidget(QWidget* parent) : QWidget(parent) {
    // 创建垂直布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("图模块待开发", this);
    layout->addWidget(label);
}
