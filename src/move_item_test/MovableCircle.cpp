//#include "MovableCircle.h"
//
//#include <QCursor>
//#include <QPen>
//#include <QBrush>
//#include <QPainter>
//#include <QGraphicsSceneMouseEvent>
//#include <QDebug>
//
//#include <cmath>
//
//#include <qvector2d.h>
//
//
//MovableCircle::MovableCircle(ECirclePos cp, double ar, QGraphicsItem* parent) :
//    QGraphicsObject(parent), aspectRatio_(ar), circlePos_(cp)
//{
//    setFlag(ItemClipsToShape, true);
//    setCursor(QCursor(Qt::PointingHandCursor));
//}
//
//QRectF MovableCircle::boundingRect() const
//{
//    qreal adjust = 0.5;
//    return QRectF(-5 - adjust, -5 - adjust,
//        10 + adjust, 10 + adjust);
//}
//
//QPainterPath MovableCircle::shape() const
//{
//    QPainterPath path;
//    qreal adjust = 0.5;
//    path.addEllipse(-5 - adjust, -5 - adjust,
//        10 + adjust, 10 + adjust);
//    return path;
//}
//
//void MovableCircle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
//{
//    Q_UNUSED(option);
//    Q_UNUSED(widget);
//
//    painter->setBrush(QBrush(QColor(0, 160, 230)));
//    painter->setPen(QPen(QColor(0, 160, 230)));
//    painter->drawEllipse(-5, -5, 10, 10);
//}
//
//void MovableCircle::mousePressEvent(QGraphicsSceneMouseEvent* event)
//{
//    _shiftMouseCoords = this->pos() - mapToScene(event->pos());
//    this->setCursor(QCursor(Qt::ClosedHandCursor));
//}
//
//void MovableCircle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
//{
//    auto pos = mapToScene(event->pos() + _shiftMouseCoords);
//
//    qreal xl = (pos.x() == 0) ? .1 : pos.x();
//    qreal yl = (pos.y() == 0) ? .1 : pos.y();
//
//    qreal arl = qAbs(xl / yl);
//
//    if (circlePos_ == eBottomRight) {
//        if (arl > aspectRatio_) {
//            pos.setX(yl * aspectRatio_);
//        }
//        else {
//            pos.setY(xl / aspectRatio_);
//
//        }
//    }
//
//    if (circlePos_ == eTopLeft) {
//        if (arl > aspectRatio_) {
//            pos.setY(xl / aspectRatio_);
//        }
//        else {
//            pos.setX(yl * aspectRatio_);
//        }
//    }
//
//    setPos(pos);
//    emit circleMoved();
//}
//
//void MovableCircle::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
//{
//    Q_UNUSED(event);
//    this->setCursor(QCursor(Qt::PointingHandCursor));
//}
//
////void MovableCircle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
////{
////    auto pos = mapToScene(event->pos() + _shiftMouseCoords);
////
////    qreal xl = (pos.x() == 0) ? .1 : pos.x();
////    qreal yl = (pos.y() == 0) ? .1 : pos.y();
////
////    qreal arl = qAbs(xl / yl);
////
////    if (circlePos_ == eBottomRight) {
////        if (arl > aspectRatio_) {
////            pos.setX(yl * aspectRatio_);
////        }
////        else {
////            pos.setY(xl / aspectRatio_);
////
////        }
////    }
////
////    if (circlePos_ == eTopLeft) {
////        LOG_WARNING(logger, "Circle Pos: ", circlePos_, ", ", pos.x(), " ", pos.y());
////        LOG_WARNING(logger, "Init Aspect Ratio: ", aspectRatio_, ", Current AspectRatio:", arl);
////        if (arl > aspectRatio_) {
////            LOG_DEBUG(logger, "> Before: ", pos.x(), ", ", pos.y());
////            pos.setY(xl / aspectRatio_);
////            LOG_DEBUG(logger, "> After: ", pos.x(), ", ", pos.y());
////        }
////        else {
////            LOG_DEBUG(logger, "< Before: ", pos.x(), ", ", pos.y());
////            pos.setX(yl * aspectRatio_);
////            LOG_DEBUG(logger, "< After: ", pos.x(), ", ", pos.y());
////        }
////    }
////
////    setPos(pos);
////    emit circleMoved();
////}
//
//
////void MovableCircle::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
////{
////    auto pos = mapToScene(event->pos() + _shiftMouseCoords);
////
////    qreal xl = pos.x();
////    qreal yl = pos.y();
////    auto rect = parentItem()->boundingRect();
////
////    QPointF a(rect.x(), rect.y());
////    QPointF b(rect.x() + rect.width(), rect.y());
////    QPointF c(rect.x() + rect.width(), rect.y() + rect.height());
////    QPointF d(rect.x(), rect.y() + rect.height());
////
////
////    if (circlePos_ == eTopRight) {
////        Vec2d dc(c.x() - d.x(), c.y() - d.y());
////        Vec2d cb(b.x() - c.x(), b.y() - c.y());
////        Vec2d db(b.x() - d.x(), b.y() - d.y());
////        Vec2d dk(pos.x() - d.x(), pos.y() - d.y());
////
////        auto dc_len = dc.length();
////        auto cb_len = cb.length();
////        auto db_len = db.length();
////        auto dk_len = dk.length();
////
////        auto dkdb_dot = Vec2d<qreal>::dot(db, dk);
////        auto cos_kdb = dkdb_dot / (db_len * dk_len);
////        auto dd2_len = dk_len * cos_kdb;
////
////        auto x = (dd2_len * dc_len) / (std::sqrt(cb_len * cb_len + dc_len * dc_len));
////        auto y = std::sqrt(dd2_len * dd2_len - x * x);
////
////        if (x < 10 || y < 10) return;
////        pos.setX(d.x() + x);
////        pos.setY(d.y() - y);
////    }
////
////    if (circlePos_ == eBottomRight) {
////        Vec2d ad(d.x() - a.x(), d.y() - a.y());
////        Vec2d dc(c.x() - d.x(), c.y() - d.y());
////        Vec2d ac(c.x() - a.x(), c.y() - a.y());
////        Vec2d ak(pos.x() - a.x(), pos.y() - a.y());
////
////        auto ad_len = ad.length();
////        auto dc_len = dc.length();
////        auto ac_len = ac.length();
////        auto ak_len = ak.length();
////
////        auto akac_dot = Vec2d<qreal>::dot(ac, ak);
////        auto cos_kac = akac_dot / (ac_len * ak_len);
////        auto ad2_len = ak_len * cos_kac;
////
////        auto x = (ad2_len * dc_len) / (std::sqrt(ad_len * ad_len + dc_len * dc_len));
////        auto y = std::sqrt(ad2_len * ad2_len - x * x);
////
////        if (x < 10 || y < 10) return;
////        pos.setX(a.x() + x);
////        pos.setY(a.y() + y);
////    }
////
////    if (circlePos_ == eTopLeft) {
////        qDebug() << this->parentItem()->boundingRect();
////        Vec2d cb(b.x() - c.x(), b.y() - c.y());
////        Vec2d ba(a.x() - b.x(), a.y() - b.y());
////        Vec2d ca(a.x() - c.x(), a.y() - c.y());
////        Vec2d ck(pos.x() - c.x(), pos.y() - c.y());
////
////        auto cb_len = cb.length();
////        auto ba_len = ba.length();
////        auto ca_len = ca.length();
////        auto ck_len = ck.length();
////
////        auto ckca_dot = Vec2d<qreal>::dot(ca, ck);
////        auto cos_kca = ckca_dot / (ca_len * ck_len);
////        auto cd2_len = ck_len * cos_kca;
////
////        auto y = (cd2_len * cb_len) / (std::sqrt(ba_len * ba_len + cb_len * cb_len));
////        auto x = std::sqrt(cd2_len * cd2_len - y * y);
////
////        if (x < 10 || y < 10) return;
////        pos.setX(c.x() - x);
////        pos.setY(c.y() - y);
////    }
////
////    if (circlePos_ == eBottomLeft) {
////        qDebug() << this->parentItem()->boundingRect();
////        Vec2d ba(a.x() - b.x(), a.y() - b.y());
////        Vec2d ad(d.x() - a.x(), d.y() - a.y());
////        Vec2d bd(d.x() - b.x(), d.y() - b.y());
////        Vec2d bk(pos.x() - b.x(), pos.y() - b.y());
////
////        auto ba_len = ba.length();
////        auto ad_len = ad.length();
////        auto bd_len = bd.length();
////        auto bk_len = bk.length();
////
////        auto bkbd_dot = Vec2d<qreal>::dot(bd, bk);
////        auto cos_kdb = bkbd_dot / (bd_len * bk_len);
////        auto bd2_len = bk_len * cos_kdb;
////
////        auto x = (bd2_len * ba_len) / (std::sqrt(ad_len * ad_len + ba_len * ba_len));
////        auto y = std::sqrt(bd2_len * bd2_len - x * x);
////
////        if (x < 10 || y < 10) return;
////        pos.setX(b.x() - x);
////        pos.setY(b.y() + y);
////    }
////
////    setPos(pos);
////    emit circleMoved();
////}