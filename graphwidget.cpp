#include "GraphWidget.h"
#include <QLabel>
#include <QVBoxLayout>

GraphWidget::GraphWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* label = new QLabel("图模块待开发", this);
    layout->addWidget(label);
}
