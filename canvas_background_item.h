#pragma once
#include <QGraphicsRectItem>


class CanvasBackgroundItem : public QObject ,public QGraphicsRectItem
{
	
	Q_OBJECT

public:
	CanvasBackgroundItem(qreal x,qreal y,qreal w,qreal h,QGraphicsRectItem* parent = nullptr);
	CanvasBackgroundItem(const QRectF& rect, QGraphicsItem* parent = nullptr);

protected:
	int type() const override;
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
protected:

private:
	QRectF rect_;
	QImage* qimage_;
	QPixmap pixmap_;
};

