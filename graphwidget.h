#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>

class QLabel;

class GraphWidget : public QWidget {
    Q_OBJECT
public:
    explicit GraphWidget(QWidget* parent = nullptr);
};

#endif // GRAPHWIDGET_H
