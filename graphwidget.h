#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>

// 图的可视化控件类，用于显示图的相关内容
class GraphWidget : public QWidget {
    Q_OBJECT

public:
    explicit GraphWidget(QWidget* parent = nullptr);
};

#endif
