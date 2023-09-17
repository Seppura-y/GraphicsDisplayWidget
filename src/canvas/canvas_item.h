#pragma once
#include <QObject>
#include <QGraphicsRectItem>
#include <QMenu>

#include "canvas_header.h"

#include "capture_thread.h"

class CanvasItem : public QObject , public QGraphicsRectItem
{
	Q_OBJECT
    //Q_INTERFACES(QGraphicsItem)
public:
	CanvasItem(uint64_t zc, QGraphicsRectItem* parent = nullptr);
    CanvasItem(QString path, uint64_t z, QGraphicsRectItem* parent = nullptr);
    CanvasItem(int width, int height, uint64_t z, QGraphicsRectItem* parent = nullptr);
	~CanvasItem();

    QRectF getRect() const { return boundingRect(); }
    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setRect(const QRectF& rect);
    void setInGroup(bool f) { inGroup_ = f; }

    void setOriginRect(QRectF& rect);
    void setPreviousRect(QRectF& rect);
    void setClippedRect(QRectF& rect);
    void setBoundaryPath(const QPainterPath& boundaryPath);

    void prepareChange();
signals:

public:
    QRectF boundingRect() const override;
protected:
    QPainterPath shape() const override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

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
    QPixmap pixmap_;
    QByteArray ba_;

    QSizeF _size;
    QRectF rect_;
    bool inGroup_ = false;
    QPainterPath boundary_path_;
    ItemsAspectRatio aspect_ratio_;
    QMenu menu_;

};

