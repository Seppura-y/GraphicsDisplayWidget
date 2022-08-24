#include "canvas_display_item.h"

#include <QPainter>
#include <QPainterPath>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QAction>

#include <iostream>

using namespace std;

CanvasDisplayItem::CanvasDisplayItem(uint64_t z, QGraphicsRectItem* parent) : QGraphicsRectItem(parent), z_value_(z)
{
    setZValue(z_value_);

    //qimage_ = new QImage(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/IMG_20170903_085200.jpg");
    //qimage_ = new QImage(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/screen.jpg");

    //pixmap_ = QPixmap::fromImage(*qimage_);
    //_size = pixmap_.size();
    //rect_ = qimage_->rect();

    setAcceptHoverEvents(true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    //aspect_ratio_.width = rect_.width();
    //aspect_ratio_.height = rect_.height();



    QAction* act = menu_.addAction(QString::fromLocal8Bit("add img"));
    act->setEnabled(true);
    QObject::connect(act, &QAction::triggered, this, &CanvasDisplayItem::onSigDelete);

    bool ret = true;
    cap_th_ = new CaptureThread();
    ret = cap_th_->Init();
    if (!ret)
    {
        qDebug() << "capture_th_->Init() failed";
        return;
    }

    output_width_ = cap_th_->getInputWidth();
    output_height_ = cap_th_->getInputHeight();
    output_width_ = 1280;
    output_height_ = 720;
    rect_ = QRect(0, 0, output_width_, output_height_);

    ret = cap_th_->InitScale(output_width_, output_height_);
    if (!ret)
    {
        qDebug() << "capture_th_->InitScale(inWidth, inHeight) failed";
        return;
    }
    cap_th_->Start();

    startTimer(40);
    total_rect_ = this->rect();
}

CanvasDisplayItem::~CanvasDisplayItem()
{
    if (cap_th_)
    {
        cap_th_->Stop();
    }
    delete cap_th_;
}

void CanvasDisplayItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    //painter->drawImage(rect(), display_img_);
    if (!boundary_path_.isEmpty())
    {
        QPainterPath path = mapFromScene(boundary_path_);
        if (!path.isEmpty())
        {
            painter->setClipPath(path);
        }
    }
    
    QRectF scene_bounding_rect = this->sceneBoundingRect();
    QRectF bounding_rect = boundingRect();
    QRectF shape_rect = shape().boundingRect();
    cout << "parent scene rect : " << scene_rect_.x() << " : " << scene_rect_.y() << " : " << scene_rect_.width() << " : " << scene_rect_.height() << endl;
    cout << "bounding_rect : " << bounding_rect.x() << " : " << bounding_rect.y() << " : " << bounding_rect.width() << " : " << bounding_rect.height() << endl;
    cout << "scene_bounding_rect : " << scene_bounding_rect.x() << " : " << scene_bounding_rect.y() << " : " << scene_bounding_rect.width() << " : " << scene_bounding_rect.height() << endl;
    cout << "shape_rect : " << shape_rect.x() << " : " << shape_rect.y() << " : " << shape_rect.width() << " : " << shape_rect.height() << endl;

    QRectF intersect_rect = scene_rect_.intersected(scene_bounding_rect);
    cout << "intersect_rect : " << intersect_rect.x() << " : " << intersect_rect.y() << " : " << intersect_rect.width() << " : " << intersect_rect.height() << endl << endl;;

    painter->drawImage(boundingRect(), display_img_);
    qreal wsize = 2;
    QPen outline_pen{ QColor(22, 142, 153), wsize };
    outline_pen.setCosmetic(true);
    painter->setPen(outline_pen);

    //if (inGroup_) {
    //    QPainterPath path;
    //    path.addPath(shape());
    //    painter->drawPath(path);

    //    QPainterPath p = mapFromScene(boundary_path_);
    //    auto intersected_p = path.intersected(p);
    //    int count = intersected_p.elementCount();
    //    cout << "count : " << count << endl;
    //}
    bounding_rect_ = painter->clipBoundingRect();
    
    view_port_ = painter->viewport();

    QGraphicsRectItem::paint(painter, option, widget);
}

void CanvasDisplayItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ev)
{
    cout << "total : " << total_rect_.width() << " : " << total_rect_.height() << endl;
    cout << "view_port : " << view_port_.width() << " : " << view_port_.height() << endl;
    cout << "bounding rect : " << bounding_rect_.width() << " : " << bounding_rect_.height() << endl << endl;
}

bool CanvasDisplayItem::sceneEventFilter(QGraphicsItem* watched, QEvent* event)
{
    //if (event->type() == QGraphicsSceneEvent::GraphicsSceneMouseDoubleClick)
    if (event->type() == QEvent::Paint)
    {
        //this->mouseDoubleClickEvent((QGraphicsSceneMouseEvent*)event);
        cout << "paint" << endl << endl;
    }
    return QGraphicsItem::sceneEventFilter(watched, event);
}

void CanvasDisplayItem::setImageData(const unsigned char* raw_data, int width, int height)
{
    //auto b = image_.loadFromData(raw_data, size);
    //image_ = QImage::fromData(raw_data, size);
    display_img_ = QImage(raw_data, width, height, QImage::Format::Format_RGBX8888);
    if (first_save_)
    {
        image_.save("out.jpg");
        first_save_ = false;
    }
}

void CanvasDisplayItem::setBoundaryPath(const QPainterPath& boundaryPath)
{
    boundary_path_ = boundaryPath;
    //update();
}

void CanvasDisplayItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    menu_.exec(QCursor::pos());
    event->accept();
    return QGraphicsItem::contextMenuEvent(event);
}

void CanvasDisplayItem::onSigDelete()
{
    cout << "delete" << endl;
}

QPainterPath CanvasDisplayItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void CanvasDisplayItem::setRect(qreal x, qreal y, qreal w, qreal h)
{
    setRect(QRectF(x, y, w, h));
}

void CanvasDisplayItem::setRect(const QRectF& rect)
{
    rect_ = rect;
    QGraphicsRectItem::setRect(rect);
}

void CanvasDisplayItem::setParentSceneRect(const QRectF& rect)
{
    scene_rect_ = rect;
}

void CanvasDisplayItem::setOriginRect(QRectF& rect)
{
    origin_rect_ = rect;
}

void CanvasDisplayItem::setPreviousRect(QRectF& rect)
{
    previous_rect_ = rect;
}

void CanvasDisplayItem::setClippedRect(QRectF& rect)
{
    clipped_rect_ = rect;
}

void CanvasDisplayItem::prepareChange()
{
    prepareGeometryChange();
}

QRectF CanvasDisplayItem::boundingRect() const
{
    return QRectF(0, 0, rect_.width(), rect_.height());
}

void CanvasDisplayItem::timerEvent(QTimerEvent* ev)
{
    if (cap_th_)
    {
        auto a = cap_th_->getImageData();
        this->setImageData(cap_th_->getImageData(), output_width_, output_height_);
        this->update();
    }

    cout << "total : " << total_rect_.width() << " : " << total_rect_.height() << endl;
    cout << "view_port : " << view_port_.width() << " : " << view_port_.height() << endl;
    cout << "bounding rect : " << bounding_rect_.width() << " : " << bounding_rect_.height() << endl << endl;
}

