#include "canvas_background_item.h"
#include "canvas_header.h"

#include <QPainterPath>
#include <QPainter>

CanvasBackgroundItem::CanvasBackgroundItem(qreal x,qreal y,qreal w,qreal h,QGraphicsRectItem* parent) : QGraphicsRectItem::QGraphicsRectItem(x, y, w, h, parent)
{
    //qimage_ = new QImage(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/IMG_20170903_085200.jpg");
    //pixmap_ = QPixmap::fromImage(*qimage_);
    //rect_ = qimage_->rect();
    //rect_ = QRectF(300, 300, 200, 200);
    //setAcceptHoverEvents(true);

    QGraphicsRectItem* rect = new QGraphicsRectItem(this);
    rect->setRect(100, 100, 200, 200);
    rect->setFlag(QGraphicsItem::ItemIsMovable, true);
    rect->setFlag(QGraphicsItem::ItemIsSelectable, true);
    rect->setBrush(QBrush(Qt::black));

    QPainterPath path;
    path.addRect(QRectF(100, 100, 800, 600));

    //rect->setBoundaryPath(path);
    //scene.addItem(p_item);

}

CanvasBackgroundItem::CanvasBackgroundItem(const QRectF& rect, QGraphicsItem* parent) : QGraphicsRectItem(rect, parent)
{

}

void CanvasBackgroundItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(widget)

    QPen background_pen(QColor("green"));
    painter->setPen(background_pen);

    QGraphicsRectItem::paint(painter, option, widget);
    //painter->drawImage(boundingRect(), *qimage_);
    //qreal wsize = 2;
    //QPen outline_pen{ QColor(22, 142, 153), wsize };
    //outline_pen.setCosmetic(true);
    //painter->setPen(outline_pen);

    //if (inGroup_) {
    //    QPainterPath path;
    //    path.addPath(shape());

    //    painter->drawPath(path);
    //}
}

QRectF CanvasBackgroundItem::boundingRect() const
{
    //return QRectF(0, 0, rect_.width(), rect_.height());
    return QGraphicsRectItem::boundingRect();
}

int CanvasBackgroundItem::type() const
{

    return eBackgroundItem;
}