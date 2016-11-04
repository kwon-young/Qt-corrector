#include "correctorgraphicsscene.h"

#include <math.h>
#include <vector>
#include <algorithm>
#include <QDebug>
#include <QPen>
#include <QColor>
#include <QBrush>
#include <QFont>
#include <QTimeLine>

CorrectorGraphicsScene::CorrectorGraphicsScene()
{
    _handle = eNone;
    _rect = new QGraphicsRectItem();
    _rect->setPen(QPen(QColor(255, 0, 0)));
    _rect->setZValue(2);
    addItem(_rect);
    _classname = new QGraphicsTextItem();
    _classname->setZValue(3);
    _classname->setPos(QPointF(0, 0));
    _classname->setDefaultTextColor(QColor(255, 0, 0));
    QFont font;
    font.setBold(true);
    _classname->setFont(font);
    addItem(_classname);
    _numScheduledScalings = 0;
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

void CorrectorGraphicsScene::setClassname(const QString &classname)
{
    _classname->setPlainText(classname);
}

void CorrectorGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "Custom item clicked.";
    //qDebug() << event->pos();
    //qDebug() << std::min_element(d.begin(), d.end()) - d.begin();
    //qDebug() << _handle;
    if (event->button() == Qt::LeftButton) {
        if (_rect->rect().isNull()) {
            QRectF r;
            r.setTopLeft(event->scenePos());
            _rect->setRect(r);
            _rect->show();
            _handle = eHandleBottomRight;
        } else {
            setHandlePoint(event->scenePos());
        }
        updateHandlePoint(event->scenePos());
    }
}

void CorrectorGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //qDebug() << "mouse moved";
    //qDebug() << event->pos();
    // if mResizeHandle is eNone then nothing is resized
    updateHandlePoint(event->scenePos());
}

void CorrectorGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton) {
        _rect->setRect(QRect());
        emit bboxChanged(_rect->rect().toRect());
    } else if (event->button() == Qt::LeftButton) {
        updateHandlePoint(event->scenePos());
        emit bboxChanged(_rect->rect().toRect());
    } else if (event->button() == Qt::MiddleButton) {
        emit resetedInfo();
    }
    _handle = eNone;
}

void CorrectorGraphicsScene::wheelEvent (QGraphicsSceneWheelEvent *event )
{
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15; // see QWheelEvent documentation
    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledScalings = numSteps;

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

void CorrectorGraphicsScene::scalingTime(qreal x)
{
    qreal factor = 1.0+ qreal(_numScheduledScalings) / 300.0;
    //scale(factor, factor);
    emit zoomChanged(factor);
}

void CorrectorGraphicsScene::animFinished()
{
    if (_numScheduledScalings > 0)
        _numScheduledScalings--;
    else
        _numScheduledScalings++;
    sender()->~QObject();
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
    qreal middlex = (_rect->rect().left()+_rect->rect().right())/2;
    qreal middley = (_rect->rect().top()+_rect->rect().bottom())/2;
    d.push_back(distance(p, QPoint(middlex, _rect->rect().top())));
    d.push_back(distance(p, QPoint(_rect->rect().top(), middley)));
    d.push_back(distance(p, QPoint(middlex, _rect->rect().bottom())));
    d.push_back(distance(p, QPoint(_rect->rect().right(), middley)));
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
    case 4:
        _handle = eHandleTop;
        break;
    case 5:
        _handle = eHandleLeft;
        break;
    case 6:
        _handle = eHandleBottom;
        break;
    case 7:
        _handle = eHandleRight;
        break;
    default:
        _handle = eNone;
        break;
    }
}

void CorrectorGraphicsScene::updateHandlePoint(const QPointF &p)
{
    QRectF r = _rect->rect();
    switch( _handle)
    {
    case eHandleTopLeft:
        r.setTopLeft(p);
        break;
    case eHandleTopRight:
        r.setTopRight(p);
        break;
    case eHandleBottomLeft:
        r.setBottomLeft(p);
        break;
    case eHandleBottomRight:
        r.setBottomRight(p);
        break;
    case eHandleTop:
        r.setTop(p.y());
        break;
    case eHandleLeft:
        r.setLeft(p.x());
        break;
    case eHandleBottom:
        r.setBottom(p.y());
        break;
    case eHandleRight:
        r.setRight(p.x());
        break;
    default:
        break;
    }
    _rect->setRect(r);
}
