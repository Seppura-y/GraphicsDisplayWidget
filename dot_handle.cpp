#include "dot_handler.h"

#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>

DotHandler::DotHandler(QGraphicsItem* parentItem, QObject* parent) : QObject(parent)
{
    setZValue(999999999);
    //    setFlags(ItemIsMovable);
    setParentItem(parentItem);
    setAcceptHoverEvents(true);
    setBrush(QBrush(QColor(22, 142, 153)));
    QPen outline_pen{ QColor(22, 142, 153), 0 };
    setPen(outline_pen);
    int x = 4;
    setRect(-x, -x, 2 * x, 2 * x);
    setDotFlags(0);
    //    setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
}

DotHandler::DotHandler(QPointF pos, QGraphicsItem* parentItem, QObject* parent) : QObject(parent)
{
    setZValue(999999999);
    //    setFlags(ItemIsMovable);
    setParentItem(parentItem);
    setAcceptHoverEvents(true);
    setBrush(QBrush(QColor(22, 142, 153)));
    QPen outline_pen{ QColor(22, 142, 153), 0 };
    setPen(outline_pen);
    int x = 4;
    setRect(-x, -x, 2 * x, 2 * x);
    setPos(pos);
    setPreviousPosition(pos);
    setDotFlags(0);
    //    setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
}

void DotHandler::mousePressEvent(QGraphicsSceneMouseEvent* ev)
{
    if (flags_ & Movable) 
    {
        setPreviousPosition(ev->scenePos());
    }
    else 
    {
        QGraphicsItem::mousePressEvent(ev);
    }
}

void DotHandler::mouseMoveEvent(QGraphicsSceneMouseEvent* ev)
{
    if (flags_ & Movable) 
    {
        auto dx = ev->scenePos().x() - m_previousPosition.x();
        auto dy = ev->scenePos().y() - m_previousPosition.y();
        moveBy(dx, dy);
        setPreviousPosition(ev->scenePos());
    }
    else 
    {
        QGraphicsItem::mouseMoveEvent(ev);
    }
}

void DotHandler::mouseReleaseEvent(QGraphicsSceneMouseEvent* ev)
{
    emit signalMouseRelease();
    QGraphicsItem::mouseReleaseEvent(ev);
}

void DotHandler::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{

}

void DotHandler::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{

}

void DotHandler::setDotFlags(unsigned int flags)
{
    flags_ = flags;
}

void DotHandler::SetScale(qreal qrScale)
{
    resetTransform();
    setScale(qrScale);
}

QPointF DotHandler::previousPosition() const noexcept
{
    return m_previousPosition;
}

void DotHandler::setPreviousPosition(const QPointF previousPosition) noexcept
{
    if (m_previousPosition == previousPosition)return;

    m_previousPosition = previousPosition;
    emit previousPositionChanged();
}