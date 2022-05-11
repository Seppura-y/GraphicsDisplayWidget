#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <list>
#include <QImage>

#include "ui_GraphicsDisplayWidget.h"
#include "capture_thread.h"
#include "ResizeableWidget.h"

class GraphicsDisplayWidget : public QMainWindow
{
    Q_OBJECT

public:
    GraphicsDisplayWidget(QWidget *parent = Q_NULLPTR);

protected:
    void resizeEvent(QResizeEvent* ev) override;
    void mouseDoubleClickEvent(QMouseEvent* ev) override;
    void timerEvent(QTimerEvent* ev) override;
private:
    Ui::GraphicsDisplayWidgetClass ui;

    std::list<QGraphicsItem*> items_;
    QGraphicsView* gfx_view_ = nullptr;
    QGraphicsScene* gfx_scene_ = nullptr;
    QGraphicsProxyWidget* proxy_widget_ = nullptr;
    QGraphicsRectItem* rect_handle_ = nullptr;

    QWidget* content_wid_ = nullptr;
    ResizeableWidget* resize_wid_ = nullptr;

    CaptureThread* cap_th_ = nullptr;
    int output_width_ = 0;
    int output_height_ = 0;

    QImage* img_ = nullptr;
    bool first_save_ = true;
};
