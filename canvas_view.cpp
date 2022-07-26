#include "canvas_view.h"
#include "canvas_scene.h"
#include "canvas_item.h"
#include "canvas_background_item.h"

#include <QPainterPath>
#include <QGraphicsRectItem>
#include <QResizeEvent>
#include <QDebug>
#include <QFileDialog>

#include <cmath>
#include <iostream>

using namespace std;

CanvasView::CanvasView(CanvasWidget* canvas, QWidget* parent) : QGraphicsView(parent)
{
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene_ = new CanvasScene(canvas, z_value_);
    connect(scene_, SIGNAL(selectionChanged()), this, SLOT(onSelectionChanged()));

    setMouseTracking(true);
    setScene(scene_);

    // Update all the view port when needed, otherwise, the drawInViewPort may experience trouble
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    // When zooming, the view stay centered over the mouse
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::RubberBandDrag);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);

    //QGraphicsRectItem* item = new QGraphicsRectItem(200, 200, 200, 200);
    //item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //item->setFlag(QGraphicsItem::ItemIsMovable, true);
    //scene_->addItem(item);

    //CanvasItem* img_item = new CanvasItem(z_value_ + 10);
    //img_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //img_item->setFlag(QGraphicsItem::ItemIsMovable, true);
    //scene_->addItem(img_item);

    //bg_item_ = new CanvasBackgroundItem(400, 400, 200, 200);
    //bg_item_->setFlag(QGraphicsItem::ItemIsSelectable, false);
    //bg_item_->setFlag(QGraphicsItem::ItemIsMovable, false);
    //bg_item_->setBrush(Qt::white);
    //scene_->addItem(bg_item_);

    item_test_ = new CanvasItem(z_value_ + 10);
    item_test_->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item_test_->setFlag(QGraphicsItem::ItemIsMovable, true);

    scene_->addItem(item_test_);
    item_test_->previous_rect_ = item_test_->origin_rect_ = item_test_->sceneBoundingRect();

    //QAction* act = menu_.addAction(QString::fromLocal8Bit("add img"));
    //act->setEnabled(true);
    //QObject::connect(act, &QAction::triggered, this, &CanvasView::onSigAddImage);
}


void CanvasView::drawBackground(QPainter* painter, const QRectF& rect)
{
    setCacheMode(CacheNone);
    painter->save();
    setBackgroundBrush(QBrush(QColor(92, 92, 92)));
    painter->fillRect(rect, backgroundBrush());
    scene_->setBackgroundBrush(QBrush(QColor(32, 32, 32)));
    painter->fillRect(scene_->sceneRect(), scene_->backgroundBrush());
    //painter->setPen( QPen(QColor(247, 0, 255),2) );
    painter->setPen(QPen(QColor(0, 255, 0), 2));
    painter->drawRect(scene_->sceneRect());
    painter->restore();
}

void CanvasView::addItem(QPointF point)
{
    ++z_value_;
    CanvasItem* item = new CanvasItem(z_value_);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setFlag(QGraphicsItem::ItemIsMovable, true);
    item->setFlag(QGraphicsItem::ItemIsFocusable, true);

    item->setPos(point);
    scene_->addItem(item);
}

void CanvasView::resizeEvent(QResizeEvent* ev)
{
    //// First call, the scene is created
    //if (ev->oldSize().width() == -1 || ev->oldSize().height() == -1) return;

    //// Get the previous rectangle of the scene in the viewport
    //QPointF P1 = mapToScene(QPoint(0, 0));
    //QPointF P2 = mapToScene(QPoint(ev->oldSize().width(), ev->oldSize().height()));

    //// Stretch the rectangle around the scene
    //if (P1.x() < 0) P1.setX(0);
    //if (P1.y() < 0) P1.setY(0);
    //if (P2.x() > scene_->width()) P2.setX(scene_->width());
    //if (P2.y() > scene_->height()) P2.setY(scene_->height());
    //qDebug() << "canvas view resize";

    QRectF rect = scene_->sceneRect();
    int x = rect.x();
    int y = rect.y();
    int width = this->width();
    int height = this->height();
    float aspect_ratio = (float)16 / (float)9;

    //int scene_height = height - 2;
    //int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1) - 2;
    //if (scene_width > width)
    //{
    //    scene_width = width - 2;
    //    scene_height = (std::lrint(scene_width / aspect_ratio) & ~1) - 2;
    //}
    //x = (width - scene_width) / 2 + 1;
    //y = (height - scene_height) / 2 + 1;

    int scene_height = height;
    int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1);
    if (scene_width > width)
    {
        scene_width = width;
        scene_height = (std::lrint(scene_width / aspect_ratio) & ~1);
    }
    x = (width - scene_width) / 2;
    y = (height - scene_height) / 2;

    rect.setX(x);
    rect.setY(y);
    rect.setWidth(scene_width);
    rect.setHeight(scene_height);
    scene_->setSceneRect(rect);
    scene_->setSceneBoundaryRect();
    //bg_item_->setRect(rect);

    //QPainterPath path;
    //path.addRect(rect);

    //item_test_->setBoundaryPath(path);

    QGraphicsView::resizeEvent(ev);
}

void CanvasView::mousePressEvent(QMouseEvent* ev)
{
    cout << "canvas view mouse pressed : " << ev->pos().x() << " " << ev->pos().y() << endl;
    QGraphicsView::mousePressEvent(ev);
}

//void CanvasView::contextMenuEvent(QContextMenuEvent* ev)
//{
//    menu_.exec(QCursor::pos());
//    ev->accept();
//    return QGraphicsView::contextMenuEvent(ev);
//}

void CanvasView::onSigAddImage()
{
    QString openPicUrl = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("Ñ¡ÔñÎÄ¼þ"), QString("/"), QString("Files (*.jpg)"));
    AddImageItem(QPointF(sceneRect().x(), sceneRect().y()), openPicUrl);
}

void CanvasView::AddImageItem(QPointF point, QString url)
{
    CanvasItem* item = new CanvasItem(url,z_value_ + 1);
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    item->setFlag(QGraphicsItem::ItemIsMovable, true);
    item->setFlag(QGraphicsItem::ItemIsFocusable, true);
    scene_->addItem(item);
}