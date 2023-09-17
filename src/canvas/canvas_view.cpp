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
    //item->setFlag(QGraphicsItem::ItemIgnoresTransformations, false);
    //scene_->AddCustomItem(item);

    //CanvasItem* img_item = new CanvasItem(z_value_ + 10);
    //img_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //img_item->setFlag(QGraphicsItem::ItemIsMovable, true);
    //scene_->addItem(img_item);

    //bg_item_ = new CanvasBackgroundItem(400, 400, 200, 200);
    //bg_item_->setFlag(QGraphicsItem::ItemIsSelectable, false);
    //bg_item_->setFlag(QGraphicsItem::ItemIsMovable, false);
    //bg_item_->setBrush(Qt::white);
    //scene_->addItem(bg_item_);

    //item_test_ = new CanvasItem(z_value_ + 10);
    //item_test_->setFlag(QGraphicsItem::ItemIsSelectable, false);
    //item_test_->setFlag(QGraphicsItem::ItemIsMovable, true);

    //scene_->AddCustomItem(item_test_);
    //item_test_->previous_rect_ = item_test_->origin_rect_ = item_test_->sceneBoundingRect();

    QAction* act = menu_.addAction(QString::fromLocal8Bit("add img"));
    act->setEnabled(true);
    QObject::connect(act, &QAction::triggered, this, &CanvasView::onSigAddImage);
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

//void CanvasView::resizeEvent(QResizeEvent* ev)
//{
//    //// First call, the scene is created
//    //if (ev->oldSize().width() == -1 || ev->oldSize().height() == -1) return;
//
//    //// Get the previous rectangle of the scene in the viewport
//    //QPointF P1 = mapToScene(QPoint(0, 0));
//    //QPointF P2 = mapToScene(QPoint(ev->oldSize().width(), ev->oldSize().height()));
//
//    //// Stretch the rectangle around the scene
//    //if (P1.x() < 0) P1.setX(0);
//    //if (P1.y() < 0) P1.setY(0);
//    //if (P2.x() > scene_->width()) P2.setX(scene_->width());
//    //if (P2.y() > scene_->height()) P2.setY(scene_->height());
//    //qDebug() << "canvas view resize";
//    QRectF current_rect;
//    QRectF previous_rect = scene_->sceneRect();
//    int current_x, current_y, current_width, current_height = 0;
//    int previous_x = previous_rect.x();
//    int previous_y = previous_rect.y();
//    int previous_width = this->width();
//    int previous_height = this->height();
//    float aspect_ratio = (float)16 / (float)9;
//
//    //int scene_height = height - 2;
//    //int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1) - 2;
//    //if (scene_width > width)
//    //{
//    //    scene_width = width - 2;
//    //    scene_height = (std::lrint(scene_width / aspect_ratio) & ~1) - 2;
//    //}
//    //x = (width - scene_width) / 2 + 1;
//    //y = (height - scene_height) / 2 + 1;
//
//    int scene_height = previous_height;
//    int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1);//用于舍入给定值并将其转换为长整数,它接受一个值( float ， double或long double )，并在根据fegetround()函数指定的舍入方向将其舍入后返回一个长整数值。 
//    if (scene_width > previous_width)
//    {
//        scene_width = previous_width;
//        scene_height = (std::lrint(scene_width / aspect_ratio) & ~1);
//    }
//    previous_x = (previous_width - scene_width) / 2;
//    previous_x = (previous_width - scene_height) / 2;
//
//    current_rect.setX(previous_x);
//    current_rect.setY(previous_y);
//    current_rect.setWidth(scene_width);
//    current_rect.setHeight(scene_height);
//    scene_->setSceneRect(current_rect);
//    scene_->setSceneBoundaryRect();
//    //bg_item_->setRect(rect);
//
//    //QPainterPath path;
//    //path.addRect(rect);
//
//    //item_test_->setBoundaryPath(path);
//
//    //QTransform matrix(1,0,0,0,1,0,0,0,1);
//    //matrix.scale(previous_rect.width() / current_rect.width(), previous_rect.height() / current_rect.height());
//    ////matrix.translate(current_rect.x() - previous_rect.x(), current_rect.y() - previous_rect.y());
//    //matrix.translate(previous_rect.x() - current_rect.x(), previous_rect.y() - current_rect.y());
//
//    //scene_->ItemsTransformation(matrix);
//
//    QGraphicsView::resizeEvent(ev);
//
//    //QRectF current_rect;
//    //QRectF previous_rect = scene_->sceneRect();
//
//    //int previous_x = previous_rect.x();
//    //int previous_y = previous_rect.y();
//    //int previous_width = this->width();
//    //int previous_height = this->height();
//    //float aspect_ratio = (float)16 / (float)9;
//
//    ////int scene_height = height - 2;
//    ////int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1) - 2;
//    ////if (scene_width > width)
//    ////{
//    ////    scene_width = width - 2;
//    ////    scene_height = (std::lrint(scene_width / aspect_ratio) & ~1) - 2;
//    ////}
//    ////x = (width - scene_width) / 2 + 1;
//    ////y = (height - scene_height) / 2 + 1;
//
//    //int scene_height = previous_height;
//    //int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1);//用于舍入给定值并将其转换为长整数,它接受一个值( float ， double或long double )，并在根据fegetround()函数指定的舍入方向将其舍入后返回一个长整数值。 
//    //if (scene_width > previous_width)
//    //{
//    //    scene_width = previous_width;
//    //    scene_height = (std::lrint(scene_width / aspect_ratio) & ~1);
//    //}
//    //previous_x = (previous_width - scene_width) / 2;
//    //previous_x = (previous_width - scene_height) / 2;
//
//    //current_rect.setX(previous_x);
//    //current_rect.setY(previous_y);
//    //current_rect.setWidth(scene_width);
//    //current_rect.setHeight(scene_height);
//    //scene_->setSceneRect(current_rect);
//    //scene_->setSceneBoundaryRect();
//    ////bg_item_->setRect(rect);
//
//    ////QPainterPath path;
//    ////path.addRect(rect);
//
//    ////item_test_->setBoundaryPath(path);
//
//    //QTransform matrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
//    //matrix.scale(previous_rect.width() / current_rect.width(), previous_rect.height() / current_rect.height());
//    ////matrix.translate(current_rect.x() - previous_rect.x(), current_rect.y() - previous_rect.y());
//    //matrix.translate(previous_rect.x() - current_rect.x(), previous_rect.y() - current_rect.y());
//
//    //scene_->ItemsTransformation(matrix);
//}

void CanvasView::mousePressEvent(QMouseEvent* ev)
{
    cout << "canvas view mouse pressed : " << ev->pos().x() << " " << ev->pos().y() << endl;
    cout << "canvas view mouse global : " << mapToGlobal(ev->pos()).x() << " " << mapToGlobal(ev->pos()).y() << endl;
    
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
    QString openPicUrl = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择文件"), QString("/"), QString("Files (*.jpg)"));
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

//void CanvasView::resizeEvent(QResizeEvent* ev)
//{
//    QRectF rect = scene_->sceneRect();
//    int x = rect.x();
//    int y = rect.y();
//    int width = this->width();
//    int height = this->height();
//    float aspect_ratio = (float)16 / (float)9;
//
//    int scene_height = height;
//    int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1);
//    if (scene_width > width)
//    {
//        scene_width = width;
//        scene_height = (std::lrint(scene_width / aspect_ratio) & ~1);
//    }
//    x = (width - scene_width) / 2;
//    y = (height - scene_height) / 2;
//
//    rect.setX(x);
//    rect.setY(y);
//    rect.setWidth(scene_width);
//    rect.setHeight(scene_height);
//    scene_->setSceneRect(rect);
//    scene_->setSceneBoundaryRect();
//    QGraphicsView::resizeEvent(ev);
//}

//static int scale_counter = 0;
//static QRectF scale_rect;
//void CanvasView::resizeEvent(QResizeEvent* ev)
//{
//    //QRectF scale_rect;
//    scale_counter++;
//    int scale_width, scale_height, scale_x, scale_y = 0;
//    QRectF new_rect;
//    int new_x, new_y, new_width, new_height = 0;
//
//    QRectF current_rect = scene_->sceneRect();
//    int current_x = current_rect.x();
//    int current_y = current_rect.y();
//    int current_width = this->width();
//    int current_height = this->height();
//    float aspect_ratio = (float)16 / (float)9;
//
//
//    new_height = current_height;
//    new_width = (std::lrint(new_height * aspect_ratio) & ~1);//用于舍入给定值并将其转换为长整数,它接受一个值( float ， double或long double )，并在根据fegetround()函数指定的舍入方向将其舍入后返回一个长整数值。
//    if (new_width > current_width)
//    {
//        new_width = current_width;
//        new_height = (std::lrint(new_width / aspect_ratio) & ~1);
//    }
//    new_x = abs((current_width - new_width) / 2);
//    new_y = abs((current_height - new_height) / 2);
//    //new_x = ((current_width - new_width) / 2);
//    //new_y = ((current_height - new_height) / 2);
//
//    new_rect.setX(new_x);
//    new_rect.setY(new_y);
//    new_rect.setWidth(new_width);
//    new_rect.setHeight(new_height);
//
//
//    auto p = scene_->sceneRect().topLeft();
//    QTransform matrix(1,0,0,0,1,0,0,0,1);
//    //qreal scale_factor = (new_rect.width() * new_rect.height()) / (current_rect.width() * current_rect.height());
//    //matrix.translate(-p.x(), -p.y());
//    qreal scale_factor = ((new_rect.width() / current_rect.width()) == 1) ? new_rect.height() / current_rect.height() : new_rect.width() / current_rect.width();
//    matrix.scale(scale_factor, scale_factor);
//
//    int tx =  (new_rect.width() / current_rect.width() == 1) ? (new_rect.x() - current_rect.x() + current_rect.x()) : current_rect.x() * (new_rect.width() / current_rect.width());
//    int ty =  (new_rect.height() / current_rect.height() == 1) ? (new_rect.y() - current_rect.y() + current_rect.y()) : current_rect.y() * (new_rect.height() / current_rect.height());
//    cout << "x : " << new_rect.x() << " : " << current_rect.x() << "tx : " << tx << endl;
//    cout << "y : " << new_rect.y() << " : " << current_rect.y() << "ty : " << ty << endl;
//    cout << endl;
//
//    matrix.translate(tx, ty);
//    //matrix.translate(new_rect.x() - current_rect.x(), new_rect.y() - current_rect.y());
//    //matrix.scale(current_rect.width() / new_rect.width(), current_rect.height() / new_rect.height());
//    //matrix.translate(current_rect.x() - new_rect.x(), current_rect.y() - new_rect.y());
//
//    //cout << "new rect " << new_rect.x() << " : " << new_rect.y() << " : " << new_rect.width() << ":" << new_rect.height() << endl;
//    //cout << "ooo rect " << current_rect.x() << " : " << current_rect.y() << " : " << current_rect.width() << ":" << current_rect.height() << endl;
//    
//    scene_->ItemsTransformation(matrix);
//    //matrix.translate(p.x(), p.y());
//    //scene_->ItemsTransformation(scale_factor,tx,ty);
//
//    scene_->setSceneRect(new_rect);
//    scene_->setSceneBoundaryRect();
//
//    //QGraphicsView::resizeEvent(ev);
//
//
//    //QRectF current_rect;
//    //QRectF previous_rect = scene_->sceneRect();
//    //int current_x, current_y, current_width, current_height = 0;
//    //int previous_x = previous_rect.x();
//    //int previous_y = previous_rect.y();
//    //int previous_width = this->width();
//    //int previous_height = this->height();
//    //float aspect_ratio = (float)16 / (float)9;
//
//    //int scene_height = previous_height;
//    //int scene_width = (std::lrint(scene_height * aspect_ratio) & ~1);//用于舍入给定值并将其转换为长整数,它接受一个值( float ， double或long double )，并在根据fegetround()函数指定的舍入方向将其舍入后返回一个长整数值。 
//    //if (scene_width > previous_width)
//    //{
//    //    scene_width = previous_width;
//    //    scene_height = (std::lrint(scene_width / aspect_ratio) & ~1);
//    //}
//    //previous_x = (previous_width - scene_width) / 2;
//    //previous_y = (previous_width - scene_height) / 2;
//
//    //current_rect.setX(previous_x);
//    //current_rect.setY(previous_y);
//    //current_rect.setWidth(scene_width);
//    //current_rect.setHeight(scene_height);
//    //scene_->setSceneRect(current_rect);
//    //scene_->setSceneBoundaryRect();
//
//    //QTransform matrix(1,0,0,0,1,0,0,0,1);
//    //matrix.scale(previous_rect.width() / current_rect.width(), previous_rect.height() / current_rect.height());
//    ////matrix.translate(current_rect.x() - previous_rect.x(), current_rect.y() - previous_rect.y());
//    //matrix.translate(previous_rect.x() - current_rect.x(), previous_rect.y() - current_rect.y());
//
//    //scene_->ItemsTransformation(matrix);
//
//
//    //QRectF new_rect;
//    //QRectF current_rect = scene_->sceneRect();
//    //int new_x, new_y, new_width, new_height = 0;
//    //int current_x = current_rect.x();
//    //int current_y = current_rect.y();
//    //int current_width = this->width();
//    //int current_height = this->height();
//    //float aspect_ratio = (float)16 / (float)9;
//
//    //new_height = current_height;
//    //new_width = (std::lrint(new_height * aspect_ratio) & ~1);//用于舍入给定值并将其转换为长整数,它接受一个值( float ， double或long double )，并在根据fegetround()函数指定的舍入方向将其舍入后返回一个长整数值。 
//    //if (new_width > current_width)
//    //{
//    //    new_width = current_width;
//    //    new_height = (std::lrint(new_width / aspect_ratio) & ~1);
//    //}
//    //new_x = (current_width - new_width) / 2;
//    //new_y = (current_height - new_height) / 2;
//
//    //new_rect.setX(new_x);
//    //new_rect.setY(new_y);
//    //new_rect.setWidth(new_width);
//    //new_rect.setHeight(new_height);
//    //scene_->setSceneRect(new_rect);
//    //scene_->setSceneBoundaryRect();
//
//    //QTransform matrix(1, 0, 0, 0, 1, 0, 0, 0, 1);
//    //matrix.scale(current_rect.width() / new_rect.width(), current_rect.height() / new_rect.height());
//    ////matrix.translate(current_rect.x() - previous_rect.x(), current_rect.y() - previous_rect.y());
//    //matrix.translate(current_rect.x() - new_rect.x(), current_rect.y() - new_rect.y());
//
//    //scene_->ItemsTransformation(matrix);
//    //QGraphicsView::resizeEvent(ev);
//}




static int scale_counter = 0;
static QRectF scale_rect;
void CanvasView::resizeEvent(QResizeEvent* ev)
{
    scale_counter++;
    int scale_width, scale_height, scale_x, scale_y = 0;
    QRectF new_rect;
    int new_x, new_y, new_width, new_height = 0;

    QRectF current_rect = scene_->sceneRect();
    int current_x = current_rect.x();
    int current_y = current_rect.y();
    int current_width = this->width();
    int current_height = this->height();
    float aspect_ratio = (float)16 / (float)9;


    new_height = current_height;
    new_width = (std::lrint(new_height * aspect_ratio) & ~1);//用于舍入给定值并将其转换为长整数,它接受一个值( float ， double或long double )，并在根据fegetround()函数指定的舍入方向将其舍入后返回一个长整数值。
    if (new_width > current_width)
    {
        new_width = current_width;
        new_height = (std::lrint(new_width / aspect_ratio) & ~1);
    }
    new_x = abs((current_width - new_width) / 2);
    new_y = abs((current_height - new_height) / 2);

    new_rect.setX(new_x);
    new_rect.setY(new_y);
    new_rect.setWidth(new_width);
    new_rect.setHeight(new_height);


    auto p = scene_->sceneRect().topLeft();
    QTransform matrix(1, 0, 0, 0, 1, 0, 0, 0, 1);

    qreal scale_factor = ((new_rect.width() / current_rect.width()) == 1) ? new_rect.height() / current_rect.height() : new_rect.width() / current_rect.width();
    matrix.scale(scale_factor, scale_factor);

    int tx = (new_rect.width() / current_rect.width() == 1) ? (new_rect.x() - current_rect.x() + current_rect.x()) : current_rect.x() * (new_rect.width() / current_rect.width());
    int ty = (new_rect.height() / current_rect.height() == 1) ? (new_rect.y() - current_rect.y() + current_rect.y()) : current_rect.y() * (new_rect.height() / current_rect.height());
    cout << "x : " << new_rect.x() << " : " << current_rect.x() << "tx : " << tx << endl;
    cout << "y : " << new_rect.y() << " : " << current_rect.y() << "ty : " << ty << endl;
    cout << endl;

    matrix.translate(tx, ty);

    scene_->ItemsTransformation(matrix);


    scene_->setSceneRect(new_rect);
    scene_->setSceneBoundaryRect();

    QGraphicsView::resizeEvent(ev);

}