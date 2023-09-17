#pragma once
#include "canvas_header.h"
#include "capture_thread.h"

#include <QMenu>
#include <QGraphicsRectItem>

class CanvasDisplayItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    //Q_INTERFACES(QGraphicsItem)
public:
    CanvasDisplayItem(uint64_t zc, QGraphicsRectItem* parent = nullptr);
    //CanvasDisplayItem(QString path, uint64_t z, QGraphicsRectItem* parent = nullptr);
    //CanvasDisplayItem(int width, int height, uint64_t z, QGraphicsRectItem* parent = nullptr);
    ~CanvasDisplayItem();

    QRectF getRect() const { return boundingRect(); }
    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setRect(const QRectF& rect);
    void setInGroup(bool f) { inGroup_ = f; }
    void setParentSceneRect(const QRectF& rect);

    void setOriginRect(QRectF& rect);
    void setPreviousRect(QRectF& rect);
    void setClippedRect(QRectF& rect);
    void setBoundaryPath(const QPainterPath& boundaryPath);

    void prepareChange();

    void setImageData(const unsigned char* data, int width, int height);
signals:

public:
    QRectF boundingRect() const override;
protected:
    QPainterPath shape() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    void timerEvent(QTimerEvent* ev) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* ev)override;
    //void moveEvent(QGraphicsSceneMoveEvent* ev) override;

    bool sceneEventFilter(QGraphicsItem* watched, QEvent* event) override;
protected slots:
    void onSigDelete();

public:
    QRectF origin_rect_;
    QRectF previous_rect_;
    QRectF clipped_rect_;
private:
    //QPointF shiftMouseCoords_;
    uint64_t z_value_;
    QImage* qimage_;

    QImage display_img_;
    QPixmap pixmap_;
    QByteArray ba_;

    QSizeF _size;
    QRectF rect_;
    bool inGroup_ = false;
    QPainterPath boundary_path_;
    ItemsAspectRatio aspect_ratio_;
    QMenu menu_;

    CaptureThread* cap_th_ = nullptr;
    QImage image_;
    int output_width_ = 0;
    int output_height_ = 0;

    bool first_save_ = true;

    QRectF view_port_;
    QRectF bounding_rect_;
    QRectF total_rect_;

    QRectF scene_rect_;
};

