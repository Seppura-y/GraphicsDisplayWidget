//#pragma once
//#include <QGraphicsObject>
//
//
//class MovableCircle : public QGraphicsObject
//{
//    Q_OBJECT
//public:
//    enum ECirclePos {
//        eTopLeft = 0,
//        eTopRight,
//        eBottomRight,
//        eBottomLeft,
//    };
//
//    explicit MovableCircle(ECirclePos cp, double ar, QGraphicsItem* parent = 0);
//
//private:
//    QRectF boundingRect() const;
//    QPainterPath shape() const;
//    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
//    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
//    void mousePressEvent(QGraphicsSceneMouseEvent* event);
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
//    QPointF _shiftMouseCoords;
//
//private:
//    double aspectRatio_;
//    ECirclePos circlePos_;
//signals:
//    void circleMoved();
//};
