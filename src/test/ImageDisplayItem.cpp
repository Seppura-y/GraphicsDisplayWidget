#include "ImageDisplayItem.h"

ImageDisplayItem::ImageDisplayItem(QWidget* parent_wid, QGraphicsItem* parent_item) : QGraphicsItem(parent_item)
{

}

QRectF ImageDisplayItem::boundingRect() const
{
	return QRectF(0, 0, 0, 0);
}

void ImageDisplayItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{

}