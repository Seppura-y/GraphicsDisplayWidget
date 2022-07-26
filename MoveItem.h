//#pragma once
//#include <QObject>
//#include <QGraphicsItem>
//
//class MovableCircle;
//
//class MoveItem : public QObject, public QGraphicsItem//public QGraphicsObject
//{
//    Q_OBJECT
//        Q_INTERFACES(QGraphicsItem)
//public:
//    explicit MoveItem(uint64_t& zc, QGraphicsItem* parent = 0);
//    ~MoveItem();
//
//signals:
//protected:
//    QRectF boundingRect() const override;
//    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
//    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
//    void wheelEvent(QGraphicsSceneWheelEvent* event) override;
//    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
//    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
//    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
//private:
//    QPointF shiftMouseCoords_;
//    QImage qimage_;
//    QPixmap pixmap_;
//    uint64_t& zCounter_;
//    MovableCircle* _topLeftCircle, * _topRightCircle, * _bottomLeftCircle, * _bottomRightCircle;
//    QSizeF _size;
//    QRectF rect_;
//public slots:
//};
//
//
