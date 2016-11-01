#ifndef GRAPHICSBBOXITEM_H
#define GRAPHICSBBOXITEM_H

#include <QDebug>
#include <QGraphicsRectItem>

class GraphicsBBoxItem : public QGraphicsRectItem
{
public:
    GraphicsBBoxItem();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        qDebug() << "Custom item clicked.";
    }
};

#endif // GRAPHICSBBOXITEM_H
