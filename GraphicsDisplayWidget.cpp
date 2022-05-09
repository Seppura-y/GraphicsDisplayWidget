#include "GraphicsDisplayWidget.h"
#include "ImageDisplayItem.h"
#include "SelectionWidget.h"
#include "ResizeableWidget.h"
#include "ResizeableTopWidget.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDial>
#include <QSizeGrip>
#include <QLabel>



GraphicsDisplayWidget::GraphicsDisplayWidget(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setGeometry(200, 200, 1280, 720);

    QVBoxLayout layout;

    QScreen* screen = QGuiApplication::primaryScreen();
    QRect  screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();

    QBrush* brush = new QBrush(QColor(230, 200, 167));
    //QBrush* brush = new QBrush(Qt::black);

    gfx_scene_ = new QGraphicsScene(0, 0, width, height, ui.centralWidget);

    gfx_view_ = new QGraphicsView(gfx_scene_, ui.centralWidget);
    gfx_view_->setGeometry(0, 0, 1280, 720);
    gfx_view_->setRenderHint(QPainter::Antialiasing);
    gfx_view_->setBackgroundBrush(*brush);

    rect_handle_ = gfx_scene_->addRect(QRectF(QPointF(width / 2 - 200, height / 2 - 200), QPointF(width / 2 + 200, height / 2 + 200)));
    auto flags = rect_handle_->flags();
    rect_handle_->setFlags(flags | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

    content_wid_ = new QWidget();
    proxy_widget_ = new QGraphicsProxyWidget(rect_handle_);                 // Adding the widget through the proxy

    content_wid_->setGeometry(QRect(QPoint(width / 2 - 200, height / 2 - 200), QPoint(width / 2 + 200, height / 2 + 200)));
    content_wid_->setStyleSheet("background-color:black;");
    content_wid_->show();

    proxy_widget_->setWidget(content_wid_);
    proxy_widget_->setAttribute(Qt::WA_TransparentForMouseEvents);
    content_wid_->setAttribute(Qt::WA_TransparentForMouseEvents);
    //QWidget* wid = new QWidget;
    //wid->setAutoFillBackground(true);
    //wid->setStyleSheet("background-color : black;");
    //wid->setFixedSize(200, 200);

    //QGraphicsProxyWidget* proxy_wid = gfx_scene_->addWidget(wid);
    //flags = proxy_wid->flags();
    //proxy_wid->setFlags(flags | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);

    
    //gfx_view_->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    //gfx_view_->setBackgroundBrush(QColor(230, 200, 167));
    //gfx_view_->setWindowTitle("Drag and Drop Robot");
    //gfx_view_->show();

    //layout.addWidget(gfx_view_);
    //ui.centralWidget->setLayout(&layout);
    //gfx_view_ = new QGraphicsView(this);
    //gfx_view_->setScene(gfx_scene_);

    //gfx_view_->setInteractive(false);
    //SelectionWidget* select_wid = new SelectionWidget(QColor(0, 255, 0),this);


    //QPixmap pix_map;
    //pix_map.load(QString(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/9c5bbafec011cdbfc70d768223432bf5.png"));
    //QGraphicsPixmapItem* pix_item = new QGraphicsPixmapItem(pix_map);

    //gfx_scene_->addItem(pix_item);
    //flags = pix_item->flags();
    //pix_item->setFlags(flags | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
 




    //auto* dial = new QDial();                                        // The widget
    //auto* handle = new QGraphicsRectItem(QRect(0, 0, 120, 120));    // Created to move and select on scene
    //auto* proxy = new QGraphicsProxyWidget(handle);                 // Adding the widget through the proxy

    //dial->setGeometry(0, 0, 100, 100);
    //dial->move(10, 10);

    //proxy->setWidget(dial);

    //QSizeGrip* sizeGrip = new QSizeGrip(dial);
    //QHBoxLayout* layout1 = new QHBoxLayout(dial);
    //layout1->setContentsMargins(0, 0, 0, 0);
    //layout1->addWidget(sizeGrip, 0, Qt::AlignRight | Qt::AlignBottom);

    //handle->setPen(QPen(Qt::transparent));
    //handle->setBrush(Qt::gray);
    //handle->setFlags(QGraphicsItem::ItemIsMovable |
    //    QGraphicsItem::ItemIsSelectable);

    //gfx_scene_->addItem(handle); // adding to scene 

    //auto* resize_top_wid = new ResizeableTopWidget(QColor("darkorange"));
    auto* resize_wid = new ResizeableWidget(QColor("darkorange"));
    //resize_top_wid->show();

    //auto* resize_wid = new ResizeableWidget(QColor("orange"));
    resize_wid->QWidget::setGeometry(QRect(300, 50, 800, 600));
    resize_wid->show();
    auto* handle = new QGraphicsRectItem(resize_wid->geometry());    // Created to move and select on scene
    auto* proxy = new QGraphicsProxyWidget(handle);                 // Adding the widget through the proxy

    proxy->setWidget(resize_wid);
    resize_wid->setAttribute(Qt::WA_TransparentForMouseEvents);
    proxy->setAttribute(Qt::WA_TransparentForMouseEvents);
    handle->setPen(QPen(Qt::transparent));
    handle->setBrush(Qt::gray);
    handle->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    handle->setSelected(true);


    gfx_scene_->addItem(handle); // adding to scene 

    gfx_scene_->addWidget(resize_wid);


}


void GraphicsDisplayWidget::resizeEvent(QResizeEvent* ev)
{
    int width = ui.centralWidget->width();
    int height = ui.centralWidget->height();

    gfx_view_->move(QPoint((width - gfx_view_->width())/2,(height - gfx_view_->height())/2));
}

void GraphicsDisplayWidget::mouseDoubleClickEvent(QMouseEvent* ev)
{
    if (rect_handle_ && content_wid_)
    {
        content_wid_->setGeometry(content_wid_->geometry().adjusted(0, 0, 5, 5));
        rect_handle_->setRect(content_wid_->geometry().adjusted(0,0,10,10));
    }
}