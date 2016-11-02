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
    Q_OBJECT

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
signals:
    void bboxChanged(const QRect &bbox);
private:
    qreal distance(const QPointF &p1, const QPointF &p2);
    void setHandlePoint(const QPointF &p);
    void updateHandlePoint(const QPointF &p);

    EResizeHandle _handle;
    QGraphicsRectItem * _rect;
};

#endif // CORRECTORGRAPHICSSCENE_H
