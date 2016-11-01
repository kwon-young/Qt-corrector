#ifndef CORRECTORGRAPHICSSCENE_H
#define CORRECTORGRAPHICSSCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

typedef enum EResizeHandle
{
  eNone,
  eHandleTopLeft,
  eHandleTopRight,
  eHandleBottomRight,
  eHandleBottomLeft,
} EResizeHandle;

class CorrectorGraphicsScene : public QGraphicsScene
{
public:
    CorrectorGraphicsScene();
    ~CorrectorGraphicsScene();

    void setRect(const QRectF &rect);
    QRectF getRect();
    void showRect();
    void hideRect();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    qreal distance(const QPointF &p1, const QPointF &p2);

    EResizeHandle _handle;
    QGraphicsRectItem * _rect;
};

#endif // CORRECTORGRAPHICSSCENE_H
