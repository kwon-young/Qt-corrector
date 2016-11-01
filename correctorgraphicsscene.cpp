#include "correctorgraphicsscene.h"

#include <math.h>
#include <vector>
#include <algorithm>
#include <QDebug>

CorrectorGraphicsScene::CorrectorGraphicsScene()
{
    _handle = eNone;
    _rect = new QGraphicsRectItem();
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
    qDebug() << "Custom item clicked.";
    qDebug() << event->pos();
    std::vector<qreal> d;
    d.push_back(distance(event->pos(), _rect->rect().topLeft()));
    d.push_back(distance(event->pos(), _rect->rect().topRight()));
    d.push_back(distance(event->pos(), _rect->rect().bottomLeft()));
    d.push_back(distance(event->pos(), _rect->rect().bottomRight()));
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
    qDebug() << std::min_element(d.begin(), d.end()) - d.begin();
    qDebug() << _handle;
}

void CorrectorGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "mouse moved";
    qDebug() << event->pos();
    // if mResizeHandle is eNone then nothing is resized
    QRectF r = _rect->rect();
    switch( _handle)
    {
    case eHandleTopLeft:
        // code to resize the object
        r.setTopLeft(event->pos());
        break;
    case eHandleTopRight:
        // code to resize the object
        r.setTopRight(event->pos());
        break;
    case eHandleBottomLeft:
        // code to resize the object
        r.setBottomLeft(event->pos());
        break;
    case eHandleBottomRight:
        // code to resize the object
        r.setBottomRight(event->pos());
        break;
    default:
        break;
    }
    _rect->setRect(r);
}

void CorrectorGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    _handle = eNone;
}

qreal CorrectorGraphicsScene::distance(const QPointF &p1, const QPointF &p2) {
    QPointF p = p2-p1;
    qreal res = p.x()*p.x() + p.y()*p.y();
    return sqrt(res);
}
