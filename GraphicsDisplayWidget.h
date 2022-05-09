#pragma once

#include <QtWidgets/QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <list>

#include "ui_GraphicsDisplayWidget.h"

class GraphicsDisplayWidget : public QMainWindow
{
    Q_OBJECT

public:
    GraphicsDisplayWidget(QWidget *parent = Q_NULLPTR);

protected:
    void resizeEvent(QResizeEvent* ev) override;
    void mouseDoubleClickEvent(QMouseEvent* ev);
private:
    Ui::GraphicsDisplayWidgetClass ui;

    std::list<QGraphicsItem*> items_;
    QGraphicsView* gfx_view_ = nullptr;
    QGraphicsScene* gfx_scene_ = nullptr;
    QGraphicsProxyWidget* proxy_widget_ = nullptr;
    QGraphicsRectItem* rect_handle_ = nullptr;

    QWidget* content_wid_ = nullptr;
};
