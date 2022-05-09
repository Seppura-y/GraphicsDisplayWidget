#pragma once
#include <QGraphicsItem>
#include <QWidget>

class ImageDisplayItem : public QGraphicsItem
{
public:
	ImageDisplayItem(QWidget* parent_wid = nullptr, QGraphicsItem* parent_item = nullptr);

	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0) override;
private:
	//void resizeEvent(QResizeEvent* ev) override;
};

