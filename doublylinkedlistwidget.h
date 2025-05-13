#ifndef DOUBLYLINKEDLISTWIDGET_H
#define DOUBLYLINKEDLISTWIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLineEdit>
#include <QPushButton>
#include <QGraphicsLineItem>
#include "NodeItem.h"
#include "ArrowItem.h"
#include <vector>
#include <functional>

class DoublyLinkedListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DoublyLinkedListWidget(QWidget* parent = nullptr);

private slots:
    void onAddEnd();
    void onRemoveEnd();
    void onAddAfter();
    void onRemoveSpecified();
    void onClear();

private:
    QGraphicsScene* scene;
    QGraphicsView*  view;
    QLineEdit*      targetLineEdit;
    QPushButton*    addEndButton;
    QPushButton*    removeEndButton;
    QPushButton*    addAfterButton;
    QPushButton*    removeSpecifiedButton;
    QPushButton*    clearButton;

    std::vector<NodeItem*>           nodes;
    std::vector<QGraphicsLineItem*>  linesFwd, linesBwd;
    std::vector<ArrowItem*>          arrowsFwd, arrowsBwd;
    int                               nextNodeId;

    void updateScene();
    void drawConnection(NodeItem* a, NodeItem* b, bool forward);
    void animatePointerTraversal(int targetIndex, std::function<void()> callback);
    void animateNodeInsertion(NodeItem* node);
    void animateNodeDeletion(NodeItem* node, std::function<void()> callback);
};

#endif // DOUBLYLINKEDLISTWIDGET_H
