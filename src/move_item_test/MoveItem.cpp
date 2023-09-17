//#include "MoveItem.h"
//#include "MovableCircle.h"
//
//#include <QPainter>
//#include <QGraphicsSceneMouseEvent>
//
//MoveItem::MoveItem(uint64_t& zc, QGraphicsItem* parent) :
//    QGraphicsItem(parent), zCounter_(zc)
//{
//    setZValue(zCounter_);
//    qimage_ = QImage("kot.png");
//    pixmap_ = QPixmap::fromImage(qimage_);
//    _size = pixmap_.size();
//    rect_ = qimage_.rect();
//
//    setAcceptHoverEvents(true);
//    setFlag(QGraphicsItem::ItemIsMovable, true);
//
//    double ar = _size.width() / _size.height();
//    // Top Left
//    _topLeftCircle = new MovableCircle(MovableCircle::eTopLeft, ar, this);
//    _topLeftCircle->setPos(0, 0);
//    // Top Right
//    _topRightCircle = new MovableCircle(MovableCircle::eTopRight, ar, this);
//    _topRightCircle->setPos(_size.width(), 0);
//    // Bottom Right
//    _bottomRightCircle = new MovableCircle(MovableCircle::eBottomRight, ar, this);
//    _bottomRightCircle->setPos(_size.width(), _size.height());
//    // Bottom Left
//    _bottomLeftCircle = new MovableCircle(MovableCircle::eBottomLeft, ar, this);
//    _bottomLeftCircle->setPos(0, _size.height());
//
//    // Signals
//    // If a delimiter point has been moved, so force the item to redraw
//
//    connect(_topLeftCircle, &MovableCircle::circleMoved, this, [this]() {
//        _bottomLeftCircle->setPos(_topLeftCircle->pos().x(), _bottomLeftCircle->pos().y());
//        _topRightCircle->setPos(_topRightCircle->pos().x(), _topLeftCircle->pos().y());
//        update(); // force to Repaint
//        });
//
//    connect(_topRightCircle, &MovableCircle::circleMoved, this, [this]() {
//        _topLeftCircle->setPos(_topLeftCircle->pos().x(), _topRightCircle->pos().y());
//        _bottomRightCircle->setPos(_topRightCircle->pos().x(), _bottomRightCircle->pos().y());
//        update(); // force to Repaint
//        });
//
//    connect(_bottomLeftCircle, &MovableCircle::circleMoved, this, [this]() {
//        _topLeftCircle->setPos(_bottomLeftCircle->pos().x(), _topLeftCircle->pos().y());
//        _bottomRightCircle->setPos(_bottomRightCircle->pos().x(), _bottomLeftCircle->pos().y());
//        update(); // force to Repaint
//        });
//
//    connect(_bottomRightCircle, &MovableCircle::circleMoved, this, [this]() {
//        _bottomLeftCircle->setPos(_bottomLeftCircle->pos().x(), _bottomRightCircle->pos().y());
//        _topRightCircle->setPos(_bottomRightCircle->pos().x(), _topRightCircle->pos().y());
//        update(); // force to Repaint
//        });
//}
//
//MoveItem::~MoveItem()
//{
//
//}
//
//
//QRectF MoveItem::boundingRect() const
//{
//    qreal distX = sqrt(pow(_topLeftCircle->x() - _topRightCircle->x(), 2) +
//        pow(_topLeftCircle->y() - _topRightCircle->y(), 2)); // eucledian distance
//
//    qreal distY = sqrt(pow(_topLeftCircle->x() - _bottomLeftCircle->x(), 2) +
//        pow(_topLeftCircle->y() - _bottomLeftCircle->y(), 2)); // eucledian distance
//
//
//    return QRectF(qMin(_topLeftCircle->pos().x(), _topRightCircle->pos().x()),
//        qMin(_topLeftCircle->pos().y(), _bottomLeftCircle->pos().y()),
//        distX, distY);
//}
//
//void MoveItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
//{
//    painter->drawImage(boundingRect(), qimage_);
//    painter->setPen(QPen(QColor(0, 160, 230), 2));
//    painter->drawRect(boundingRect());
//    Q_UNUSED(widget);
//}
//
//void MoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
//{
//    this->setPos(mapToScene(event->pos() + shiftMouseCoords_));
//}
//
//void MoveItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
//{
//    setZValue(++zCounter_);
//
//    shiftMouseCoords_ = (this->pos() - mapToScene(event->pos())) / scale();
//
//    if (event->button() == Qt::LeftButton) {
//        if (event->modifiers() == Qt::ShiftModifier) {
//        }
//        else {
//            QGraphicsItem::mousePressEvent(event);
//        }
//    }
//}
//
//void MoveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
//{
//    if (event->button() == Qt::LeftButton) {
//        if (event->modifiers() == Qt::ShiftModifier) {
//        }
//        else {
//            QGraphicsItem::mousePressEvent(event);
//        }
//    }
//}
//
//
//
