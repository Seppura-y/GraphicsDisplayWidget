#pragma once
#include <QObject>
#include <QGraphicsRectItem>
#include <QMenu>

#include "canvas_header.h"

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


    int height() const noexcept { return qimage_->height(); }
    int width() const noexcept { return qimage_->width(); }
    QImage::Format format() const noexcept { return qimage_->format(); }
    const QImage& qimage() { return *qimage_; }
    const QImage* qimage_ptr_const() { return qimage_; }
    QImage* qimage_ptr() { return qimage_; }
    uint64_t& zcounter() noexcept { return z_value_; }
signals:

public:
    QRectF boundingRect() const override;
protected:
    QPainterPath shape() const override;
    //QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    //void wheelEvent(QGraphicsSceneWheelEvent* event) override;

    //    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
private slots:


private:
    QPointF shiftMouseCoords_;
    QImage* qimage_;
    QPixmap pixmap_;
    QByteArray ba_;
    uint64_t z_value_;
    QSizeF _size;
    QRectF rect_;
    bool inGroup_ = false;


protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;

public:
    void setBoundaryPath(const QPainterPath& boundaryPath);

public:
    QRectF origin_rect_;
    QRectF previous_rect_;
    QRectF clipped_rect_;

signals:

protected slots:
    void onSigDelete();
private:
    QPainterPath boundary_path_;

    QPointF previous_coordinate_;
    QPointF clipped_coordinate_;



    ItemsAspectRatio aspect_ratio_;

    QMenu menu_;
};

