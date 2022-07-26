#include "GraphicsDisplayWidget.h"
#include "canvas_widget.h"
//
//#include <QtWidgets/QApplication>
//
//int main(int argc, char *argv[])
//{
//    QApplication a(argc, argv);
//    GraphicsDisplayWidget w;
//    w.show();
//
//    //CanvasWidget cw;
//    //cw.show();
//    return a.exec();
//}

#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QPainter>

class GraphicsPixmapItem : public QGraphicsPixmapItem {

public:
    GraphicsPixmapItem(const QPixmap& pixmap, QGraphicsItem* parent = 0)
        : QGraphicsPixmapItem(pixmap, parent) {
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
    }
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
        QWidget* widget) override{
        if (!m_boundaryPath.isEmpty()) {
            QPainterPath path = mapFromScene(m_boundaryPath);
            if (!path.isEmpty())
                painter->setClipPath(path);
        }
        QGraphicsPixmapItem::paint(painter, option, widget);
    }

    QPainterPath boundaryPath() const { return m_boundaryPath; }

    void setBoundaryPath(const QPainterPath& boundaryPath) {
        m_boundaryPath = boundaryPath;
        update();
    }

private:
    QPainterPath m_boundaryPath;
};

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    //QGraphicsView view;

    //view.setBackgroundBrush(QBrush(Qt::gray));
    //view.setGeometry(0, 0, 1920, 1080);

    //QGraphicsScene scene(0, 0, 1280, 720);
    //view.setScene(&scene);
    //scene.setBackgroundBrush(QBrush(QColor(32, 255, 32)));

    //QPainter painter;
    //painter.fillRect(scene.sceneRect(), scene.backgroundBrush());
    //painter.setPen(QPen(QColor(0, 35, 0), 2));
    //painter.drawRect(scene.sceneRect());

    //GraphicsPixmapItem* p_item = new GraphicsPixmapItem(QPixmap(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/IMG_20170903_085157.jpg"));
    //p_item->setPos(100, 100);

    //// Define the area that will be visible
    //QPainterPath path;
    //path.addRect(QRectF(100, 100, 800, 600));

    //p_item->setBoundaryPath(path);
    //scene.addItem(p_item);

    //// the item is added to visualize the intersection
    //QGraphicsPathItem* path_item =
    //    scene.addPath(path, QPen(Qt::black), QBrush(Qt::white));
    //path_item->setZValue(-1);
    //view.show();

    
    CanvasWidget cw;
    cw.show();
    return a.exec();
}
