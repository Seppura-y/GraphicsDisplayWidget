#pragma once
#include <QObject> 
#include <QGraphicsEllipseItem>

class DotHandler :public QObject , public QGraphicsEllipseItem
{
	Q_OBJECT

public:
	enum Flags {
		Movable = 0x01
	};
	enum { 
		Type = QGraphicsItem::UserType + 1
	};

	explicit DotHandler(QGraphicsItem* parentItem, QObject* parent = nullptr);
	explicit DotHandler(QPointF pos, QGraphicsItem* parentItem, QObject* parent = nullptr);

	QPointF previousPosition() const noexcept;
	void setPreviousPosition(const QPointF previousPosition) noexcept;
	void SetScale(qreal qrScale);
signals:
	void previousPositionChanged();
	void signalMouseRelease();
protected:
	void setDotFlags(unsigned int flags);

protected:
	int type()const override
	{
		return Type;
	}
	void mousePressEvent(QGraphicsSceneMouseEvent* ev) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* ev) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* ev) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
private:
	unsigned int flags_;
	QPointF m_previousPosition;
};

