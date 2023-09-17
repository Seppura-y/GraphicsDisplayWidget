#include "GraphicsDisplayWidget.h"
#include "ImageDisplayItem.h"
#include "SelectionWidget.h"
#include "ResizeableWidget.h"
#include "ResizeableTopWidget.h"
#include "capture_view.h"

#include "capture_thread.h"

#include <QVBoxLayout>
#include <QScreen>
#include <QGraphicsProxyWidget>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDial>
#include <QSizeGrip>
#include <QLabel>
#include <QTimer>
#include <QDebug>

extern"C"
{
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
}
//#pragma comment(lib,"avcodec.lib")
//#pragma comment(lib,"swscale.lib")
//#pragma comment(lib,"avutil.lib")

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
    //gfx_view_ = new CaptureView(gfx_scene_, ui.centralWidget);
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


    auto* resize_top_wid = new ResizeableTopWidget(QColor("darkorange"));
    resize_top_wid->show();

    resize_wid_ = new ResizeableWidget(QColor("darkorange"));
    auto px_wid = gfx_scene_->addWidget(resize_wid_);
    //auto* px_wid = new QGraphicsProxyWidget();
    //px_wid->setWidget(resize_wid_);
    //px_wid->setGeometry(QRect(300, 50, 100, 100));

    //auto* resize_wid = new ResizeableWidget(QColor("orange"));
    resize_wid_->QWidget::setGeometry(QRect(300, 50, 800, 600));
    resize_wid_->show();
    //auto* handle = new QGraphicsRectItem(resize_wid->geometry());    // Created to move and select on scene
    //auto* proxy = new QGraphicsProxyWidget(handle);                 // Adding the widget through the proxy

    //proxy->setWidget(resize_wid);
    //resize_wid->setAttribute(Qt::WA_TransparentForMouseEvents);
    //proxy->setAttribute(Qt::WA_TransparentForMouseEvents);
    //handle->setPen(QPen(Qt::transparent));
    //handle->setBrush(Qt::gray);
    //handle->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    //handle->setSelected(true);


    //gfx_scene_->addItem(handle); // adding to scene 

    //2022.7.20
    //auto* resize_wid = new ResizeableWidget(QColor("orange"));
    //resize_wid->QWidget::setGeometry(QRect(300, 50, 800, 600));
    //resize_wid->show();

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
    //output_width_ = 400;
    //output_height_ = 300;

    ret = cap_th_->InitScale(output_width_, output_height_);
    if (!ret)
    {
        qDebug() << "capture_th_->InitScale(inWidth, inHeight) failed";
        return;
    }
    cap_th_->Start();

    startTimer(40);
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
        //content_wid_->setGeometry(content_wid_->geometry().adjusted(0, 0, 5, 5));
        //rect_handle_->setRect(content_wid_->geometry().adjusted(0,0,10,10));
        content_wid_->setGeometry(content_wid_->geometry().adjusted(0, 0, 5, 5));
        rect_handle_->setRect(content_wid_->geometry().adjusted(0, 0, 10, 10));
    }
}

void GraphicsDisplayWidget::timerEvent(QTimerEvent* ev)
{
    //qDebug() << "time out";
    if (cap_th_)
    {
        auto a = cap_th_->getImageData();
        resize_wid_->setImageData(cap_th_->getImageData(), output_width_, output_height_);
        resize_wid_->update();
    }
}