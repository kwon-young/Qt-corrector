#ifndef CORRECTORGRAPHICSSCENE_H
#define CORRECTORGRAPHICSSCENE_H

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QGraphicsSceneWheelEvent>

typedef enum EResizeHandle
{
  eNone,
  eHandleTopLeft,
  eHandleTopRight,
  eHandleBottomRight,
  eHandleBottomLeft,
  eHandleTop,
  eHandleLeft,
  eHandleBottom,
  eHandleRight,
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
    void setClassname(const QString &classname);

public slots:
    void scalingTime(qreal x);
    void animFinished();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
signals:
    void bboxChanged(const QRect &bbox);
    void resetedInfo();
    void zoomChanged(const qreal x);

private:
    qreal distance(const QPointF &p1, const QPointF &p2);
    void setHandlePoint(const QPointF &p);
    void updateHandlePoint(const QPointF &p);

    EResizeHandle _handle;
    QGraphicsRectItem * _rect;
    QGraphicsTextItem * _classname;
    int _numScheduledScalings;
};

#endif // CORRECTORGRAPHICSSCENE_H
