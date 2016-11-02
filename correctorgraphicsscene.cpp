#include "correctorgraphicsscene.h"

#include <math.h>
#include <vector>
#include <algorithm>
#include <QDebug>
#include <QPen>
#include <QColor>
#include <QBrush>

CorrectorGraphicsScene::CorrectorGraphicsScene()
{
    _handle = eNone;
    _rect = new QGraphicsRectItem();
    _rect->setPen(QPen(QColor(255, 0, 0)));
    _rect->setZValue(2);
    addItem(_rect);
}

CorrectorGraphicsScene::~CorrectorGraphicsScene()
{
    delete _rect;
}

void CorrectorGraphicsScene::setRect(const QRectF &rect)
{
    _rect->setRect(rect);
}

QRectF CorrectorGraphicsScene::getRect()
{
    return _rect->rect();
}

void CorrectorGraphicsScene::showRect()
{
    _rect->show();
}

void CorrectorGraphicsScene::hideRect()
{
    _rect->hide();
}

void CorrectorGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "Custom item clicked.";
    //qDebug() << event->pos();
    //qDebug() << std::min_element(d.begin(), d.end()) - d.begin();
    //qDebug() << _handle;
    if (_rect->rect().isNull()) {
        QRectF r;
        r.setTopLeft(event->scenePos());
        _handle = eHandleBottomRight;
    } else {
        setHandlePoint(event->scenePos());
    }
}

void CorrectorGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "mouse moved";
    //qDebug() << event->pos();
    // if mResizeHandle is eNone then nothing is resized
    QRectF r = _rect->rect();
    switch( _handle)
    {
    case eHandleTopLeft:
        // code to resize the object
        r.setTopLeft(event->scenePos());
        break;
    case eHandleTopRight:
        // code to resize the object
        r.setTopRight(event->scenePos());
        break;
    case eHandleBottomLeft:
        // code to resize the object
        r.setBottomLeft(event->scenePos());
        break;
    case eHandleBottomRight:
        // code to resize the object
        r.setBottomRight(event->scenePos());
        break;
    default:
        break;
    }
    _rect->setRect(r);
}

void CorrectorGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _handle = eNone;
    emit bboxChanged(_rect->rect().toRect());
}

qreal CorrectorGraphicsScene::distance(const QPointF &p1, const QPointF &p2) {
    QPointF p = p2-p1;
    qreal res = p.x()*p.x() + p.y()*p.y();
    return sqrt(res);
}

void CorrectorGraphicsScene::setHandlePoint(const QPointF &p)
{
    std::vector<qreal> d;
    d.push_back(distance(p, _rect->rect().topLeft()));
    d.push_back(distance(p, _rect->rect().topRight()));
    d.push_back(distance(p, _rect->rect().bottomLeft()));
    d.push_back(distance(p, _rect->rect().bottomRight()));
    //handle = d.begin() - std::min_element(d.begin(), d.end());
    switch (std::min_element(d.begin(), d.end()) - d.begin()) {
    case 0:
        _handle = eHandleTopLeft;
        break;
    case 1:
        _handle = eHandleTopRight;
        break;
    case 2:
        _handle = eHandleBottomLeft;
        break;
    case 3:
        _handle = eHandleBottomRight;
        break;
    default:
        _handle = eNone;
        break;
    }
}
