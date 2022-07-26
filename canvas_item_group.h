#pragma once
#include <QObject>
#include <QGraphicsItemGroup>

class CanvasItem;
class DotHandler;

class CanvasItemGroup : public QObject , public QGraphicsItemGroup
{
	Q_OBJECT
        
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)
public:
	enum ItemsType {
		eBorderDot = QGraphicsItem::UserType + 1,
	};

    struct new_size_t {
        qreal new_x = .0;
        qreal new_y = .0;
        qreal new_w = .0;
        qreal new_h = .0;
    };

    enum ActionStates {
        ResizeState = 0x01,
        RotationState = 0x02
    };

    //enum CornerFlags {
    //    Top = 0x01,
    //    Bottom = 0x02,
    //    Left = 0x04,
    //    Right = 0x08,
    //    TopLeft = Top | Left,
    //    TopRight = Top | Right,
    //    BottomLeft = Bottom | Left,
    //    BottomRight = Bottom | Right
    //};

    enum CornerGrabbers {
        GrabberTopLeft = 0,
        GrabberTopRight,
        GrabberBottomLeft,
        GrabberBottomRight
    };

	CanvasItemGroup(int64_t z, QGraphicsItemGroup* parent = nullptr);
	//~CanvasItemGroup();

public:
    QRectF boundingRect() const override;
public:
    void clearItemGroup();
    void addItemToGroup(QGraphicsItem* item);
    void removeItemFromGroup(QGraphicsItem* item);
    //void printChilds();
    QPointF previousPosition() const;
    void setPreviousPosition(const QPointF previousPosition);

    void notifyCursorUpdater(QGraphicsSceneMouseEvent* event, qreal sf);
    //void setScaleControlFactor(qreal sf);

    bool isContain(const QGraphicsItem* item) const;
    bool isThisDots(const QGraphicsItem* item) const;
    bool isEmpty() const;
    void increaseZValue();

    //void dumpBits(QString text);
    //QImage mergedImages();

    //QVector<QGraphicsItem*> cloneItems();
    QRectF realRect() const { return rectItemGroup_; }

signals:
    void rectChanged(CanvasItemGroup* rect);
    void previousPositionChanged();
    //void clicked(CanvasItemGroup* rect);
    void signalMove(QGraphicsItemGroup* item, qreal dx, qreal dy);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    //void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    //void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    ////    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    //QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:


    void resizeTopLeft(const QPointF& pt);
    void resizeTopRight(const QPointF& pt);
    void resizeBottomLeft(const QPointF& pt);
    void resizeBottomRight(const QPointF& pt);

    //void rotateItem(const QPointF& pt);
    void setPositionGrabbers();
    void setVisibilityGrabbers();
    void hideGrabbers();

    //void updateScaleControl();
    QRectF currentSceneBoundingRect() const;
    //void printDotsCoords(const std::string& text) const;

    new_size_t calculateNewSize(const QRectF& tmpRect, CanvasItem* widget);
    QRectF calcNewBr();
private:
    QVector<QGraphicsItem*> v_items_;
    //DotHandler* cornerGrabber[4] = { nullptr, nullptr, nullptr, nullptr };
    DotHandler* corner_handle[4] = { nullptr, nullptr, nullptr, nullptr };


    uint64_t z_value_;

    QPointF shiftMouseCoords_;
    QRectF sceneRectItemGroup_;
    QRectF rectItemGroup_;

    //unsigned int m_cornerFlags;
    //unsigned int m_actionFlags;
    //QPointF m_previousPosition;
    //bool m_leftMouseButtonPressed;

    int sem_ = 0;
    qreal controlScaleFactor_ = 1;

    unsigned int handle_flags_;
    unsigned int actionFlags_;
    QPointF previousPosition_;
    bool is_leftButtonPressed_;



    signals:
        void clicked(CanvasItemGroup* rect);
};

