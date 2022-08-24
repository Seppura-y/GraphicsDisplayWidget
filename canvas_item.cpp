#include "canvas_item.h"

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QAction>

#include <iostream>

using namespace std;

CanvasItem::CanvasItem(uint64_t zc, QGraphicsRectItem* parent) : QGraphicsRectItem(parent), z_value_(zc)
{
    setZValue(z_value_);

    //qimage_ = new QImage(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/IMG_20170903_085200.jpg");
    qimage_ = new QImage(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/screen.jpg");

    pixmap_ = QPixmap::fromImage(*qimage_);
    _size = pixmap_.size();
    rect_ = qimage_->rect();

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    aspect_ratio_.width = rect_.width();
    aspect_ratio_.height = rect_.height();

    QAction* act = menu_.addAction(QString::fromLocal8Bit("add img"));
    act->setEnabled(true);
    QObject::connect(act, &QAction::triggered, this, &CanvasItem::onSigDelete);
}

CanvasItem::CanvasItem(QString path, uint64_t z, QGraphicsRectItem* parent) : QGraphicsRectItem(parent), z_value_(z)
{
    setZValue(z_value_);

    qimage_ = new QImage(path);
    qInfo() << qimage_->width() << ' ' << qimage_->height() << " SIZE=" << qimage_->sizeInBytes();
    pixmap_ = QPixmap::fromImage(*qimage_);
    _size = pixmap_.size();
    rect_ = qimage_->rect();

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

CanvasItem::CanvasItem(int width, int height, uint64_t z, QGraphicsRectItem* parent) :QGraphicsRectItem(parent),z_value_(z)
{
    setZValue(z_value_);

    aspect_ratio_.width = width;
    aspect_ratio_.height = height;

    qimage_ = new QImage(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/IMG_20170903_085200.jpg");
    //qInfo() << qimage_->width() << ' ' << qimage_->height() << " SIZE=" << qimage_->sizeInBytes();
    pixmap_ = QPixmap::fromImage(*qimage_);
    _size = pixmap_.size();
    rect_ = qimage_->rect();

    //rect_ = QRectF(300, 300, 200, 200);

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
}

CanvasItem::~CanvasItem()
{
    cout << "canvas item destruct" << endl;
}

QRectF CanvasItem::boundingRect() const
{
	return QRectF(0, 0, rect_.width(), rect_.height());
}

void CanvasItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (!boundary_path_.isEmpty()) 
    {
        QPainterPath path = mapFromScene(boundary_path_);
        if (!path.isEmpty())
        {
            painter->setClipPath(path);
            //cout << "clip rect " << painter->clipBoundingRect().x() << " : " << painter->clipBoundingRect().y() << " : " << painter->clipBoundingRect().width() << " : " << painter->clipBoundingRect().height() << endl;
            //cout << "bounding rect " << mapRectToScene(painter->clipBoundingRect()).x() << " : " << mapRectToScene(painter->clipBoundingRect()).y() << " : " << mapRectToScene(painter->clipBoundingRect()).width() << " : " << mapRectToScene(painter->clipBoundingRect()).height() << endl;
            //cout << "clip rect " << path.boundingRect().x() << " : " << path.boundingRect().y() << " : " << path.boundingRect().width() << " : " << path.boundingRect().height() << endl;
            //cout << "scene rect " << this->sceneBoundingRect().x() << " : " << this->sceneBoundingRect().y() << " : " << this->sceneBoundingRect().width() << " : " << this->sceneBoundingRect().height() << endl;

        }
    }

    painter->drawImage(boundingRect(), *qimage_);
    qreal wsize = 2;
    QPen outline_pen{ QColor(22, 142, 153), wsize };
    outline_pen.setCosmetic(true);
    painter->setPen(outline_pen);


    if (inGroup_) {
        QPainterPath path;
        path.addPath(shape());

        painter->drawPath(path);
    }

    //QGraphicsRectItem::paint(painter, option, widget);
}


void CanvasItem::setBoundaryPath(const QPainterPath& boundaryPath)
{
    boundary_path_ = boundaryPath;
    //update();
}

void CanvasItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    menu_.exec(QCursor::pos());
    event->accept();
    return QGraphicsItem::contextMenuEvent(event);
}

void CanvasItem::onSigDelete()
{
    cout << "delete" << endl;
}

QPainterPath CanvasItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void CanvasItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    setRect(QRectF(x, y, w, h));
}

void CanvasItem::setRect(const QRectF& rect)
{
    rect_ = rect;
    QGraphicsRectItem::setRect(rect);
}

void CanvasItem::setOriginRect(QRectF& rect)
{
    origin_rect_ = rect;
}

void CanvasItem::setPreviousRect(QRectF& rect)
{
    previous_rect_ = rect;
}

void CanvasItem::setClippedRect(QRectF& rect)
{
    clipped_rect_ = rect;
}

void CanvasItem::prepareChange()
{
    prepareGeometryChange();
}