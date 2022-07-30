#include "canvas_item_group.h"
#include "canvas_item.h"
#include "dot_handler.h"
#include "canvas_header.h"
#include "vec2d.h"

#include <QGuiApplication>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

#include <iostream>

using namespace std;

CanvasItemGroup::CanvasItemGroup(int64_t z, QGraphicsItemGroup* parent) : QGraphicsItemGroup(parent), z_value_(z), handle_flags_(0), actionFlags_(ResizeState)
{
    setAcceptHoverEvents(true);
    setFiltersChildEvents(true);
    setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
}

QRectF CanvasItemGroup::boundingRect()const
{
	return rectItemGroup_;
}

QPointF CanvasItemGroup::previousPosition() const
{
    return previousPosition_;
}

void CanvasItemGroup::setPreviousPosition(const QPointF previousPosition)
{
    if (previousPosition_ == previousPosition)
        return;

    previousPosition_ = previousPosition;
    emit previousPositionChanged();
}

void CanvasItemGroup::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    //cout << "item group mouse move" << endl;

    QPointF pt = event->pos();              // The current position of the mouse
    if (actionFlags_ == ResizeState) {
        switch (handle_flags_) {
        case TopLeft_Handle:
            resizeTopLeft(pt);
            break;
        case TopRight_Handle:
            resizeTopRight(pt);
            break;
        case BottomLeft_Handle:
            resizeBottomLeft(pt);
            break;
        case BottomRight_Handle:
            resizeBottomRight(pt);
            break;
        default:
            if (is_leftButtonPressed_) {
                //QGuiApplication::setOverrideCursor(Qt::ClosedHandCursor);
                setCursor(Qt::ClosedHandCursor);
                auto dx = event->scenePos().x() - previousPosition_.x();
                auto dy = event->scenePos().y() - previousPosition_.y();
                moveBy(dx, dy);
                setPreviousPosition(event->scenePos());
                //                emit signalMove(this, dx, dy);
            }
            break;
        }
    }
    else {
        switch (handle_flags_) {
        case TopLeft_Handle:
        case TopRight_Handle:
        case BottomLeft_Handle:
        case BottomRight_Handle: {
            //rotateItem(pt);
            break;
        }
        default:
            if (is_leftButtonPressed_) {
                //setCursor(Qt::ClosedHandCursor);
                QGuiApplication::setOverrideCursor(Qt::ClosedHandCursor);
                auto dx = event->scenePos().x() - previousPosition_.x();
                auto dy = event->scenePos().y() - previousPosition_.y();
                moveBy(dx, dy);
                setPreviousPosition(event->scenePos());
                //                emit signalMove(this, dx, dy);
            }
            break;
        }
    }

    QGraphicsItemGroup::mouseMoveEvent(event);
}

void CanvasItemGroup::mousePressEvent(QGraphicsSceneMouseEvent* ev)
{
    cout << "item group mouse pressed : " << ev->pos().x() << " " << ev->pos().y() << endl;
    cout << "item group pos : " << this->pos().x() << " " << this->pos().y() << endl;
    //cout << "item group scene pos : " << mapToScene(this->pos()).x() << " : " << mapToScene(this->pos()).y() << endl << endl;
    cout << "item group scene pos : " << this->scenePos().x() << " : " << this->scenePos().y() << endl;
    cout << "item group scene bounding rect : " << sceneBoundingRect().x() << " : " << sceneBoundingRect().y() << endl;
    cout << "item group current scene bounding rect : " << currentSceneBoundingRect().x() << " : " << currentSceneBoundingRect().y() << endl;
    cout << endl;

    setZValue(++z_value_);
    shiftMouseCoords_ = (this->pos() - mapToScene(ev->pos())) / scale();

    if (ev->button() & Qt::LeftButton) {
        is_leftButtonPressed_ = true;
        setPreviousPosition(ev->scenePos());
        emit clicked(this);
    }

    QGraphicsItemGroup::mousePressEvent(ev);
}

void CanvasItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent* ev)
{
    if (ev->button() & Qt::LeftButton)
    {
        is_leftButtonPressed_ = false;
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItemGroup::mouseReleaseEvent(ev);
}

void CanvasItemGroup::clearItemGroup()
{
    hideGrabbers();
    auto childs = childItems();
    for (auto& it : childs) {
        removeItemFromGroup(it);
    }

    for (int i = 0; i < 4; i++) {
        if (corner_handle[i] == nullptr) continue;
        //        qDebug()<<"DEALLOCATE: "<< (void*)cornerGrabber[i];
        delete corner_handle[i];
        corner_handle[i] = nullptr;
    }
    sceneRectItemGroup_ = QRectF();
}

void CanvasItemGroup::addItemToGroup(QGraphicsItem* item)
{
    if (item == this || item->type() == eBorderDot) return;
    //    qDebug()<<"Item pointer : "<< (void *)item;
    addToGroup(item);

    if (item->type() != eBorderDot) 
    {
        v_items_.push_back(item);
    }

    auto childs = childItems();
    auto scene_tmp = childs.first()->sceneBoundingRect();

    for (auto& it : childs) 
    {
        if (it->type() == eBorderDot) continue;
        scene_tmp = scene_tmp.united(it->sceneBoundingRect());
        auto widget = qgraphicsitem_cast<CanvasItem*>(it);
        widget->setInGroup(true);
    }

    sceneRectItemGroup_ = scene_tmp;
    rectItemGroup_ = calcNewBr();

    if (!corner_handle[0]) {
        for (int i = 0; i < 4; i++) {
            corner_handle[i] = new DotHandler(this);
            //            qDebug()<<"ALLOCATE: "<< (void*)cornerGrabber[i];
        }
        hideGrabbers();
    }

    setPositionGrabbers();
    //updateScaleControl();
    setVisibilityGrabbers();
}

void CanvasItemGroup::removeItemFromGroup(QGraphicsItem* item)
{
    if (item->type() != eBorderDot && item->type() != eBackgroundItem) 
    {
        auto widget = qgraphicsitem_cast<CanvasItem*>(item);
        if (!widget)return;
        widget->setInGroup(false);
        //        LOG_DEBUG(logger, "REMOVE ", item, ", type = ", item->type());
    }
    v_items_.erase(std::remove_if(v_items_.begin(), 
                                    v_items_.end(), 
                                    [&](QGraphicsItem* i) 
                                    { 
                                        return i == item; 
                                    }),
                        v_items_.end());

    removeFromGroup(item);
    rectItemGroup_ = calcNewBr();
    setPositionGrabbers();
    //need remove from rects
    //check that empty and delete dots
}

bool CanvasItemGroup::isContain(const QGraphicsItem* item) const
{
	if (this == item->parentItem()) return true;

	return false;
}

void CanvasItemGroup::increaseZValue()
{
	setZValue(++z_value_);
}

void CanvasItemGroup::hideGrabbers()
{
    if (!corner_handle[0]) return;
    for (int i = 0; i < 4; i++) {
        corner_handle[i]->setVisible(false);
        corner_handle[i]->setEnabled(false);
    }
}

QRectF CanvasItemGroup::calcNewBr()
{
    if (v_items_.empty()) return QRectF();

    qreal min_x = v_items_.first()->pos().x();
    qreal min_y = v_items_.first()->pos().y();
    qreal max_x = v_items_.first()->pos().x() + v_items_.first()->boundingRect().width();
    qreal max_y = v_items_.first()->pos().y() + v_items_.first()->boundingRect().height();

    for (auto& item : v_items_) {
        qreal cur_min_x = item->pos().x();
        qreal cur_min_y = item->pos().y();
        qreal cur_max_x = cur_min_x + item->boundingRect().width();
        qreal cur_max_y = cur_min_y + item->boundingRect().height();

        if (cur_min_x < min_x) min_x = cur_min_x;
        if (cur_min_y < min_y) min_y = cur_min_y;
        if (max_x < cur_max_x) max_x = cur_max_x;
        if (max_y < cur_max_y) max_y = cur_max_y;
    }

    return QRectF(min_x, min_y, max_x - min_x, max_y - min_y);
}

void CanvasItemGroup::setPositionGrabbers()
{
    if (!corner_handle[0]) return;

    QRectF tmpRect = boundingRect();

    corner_handle[GrabberTopLeft]->setPos(tmpRect.topLeft().x(), tmpRect.topLeft().y());
    corner_handle[GrabberTopRight]->setPos(tmpRect.topRight().x(), tmpRect.topRight().y());
    corner_handle[GrabberBottomLeft]->setPos(tmpRect.bottomLeft().x(), tmpRect.bottomLeft().y());
    corner_handle[GrabberBottomRight]->setPos(tmpRect.bottomRight().x(), tmpRect.bottomRight().y());
}

bool CanvasItemGroup::isEmpty() const
{
    return v_items_.empty();
}

void CanvasItemGroup::notifyCursorUpdater(QGraphicsSceneMouseEvent* event, qreal sf)
{
    if ((event->buttons() & Qt::LeftButton) == 0) {
        if (!corner_handle[0]) return;
        QPointF pt = event->scenePos();

        auto tlPoint = pt - corner_handle[0]->scenePos();
        auto trPoint = pt - corner_handle[1]->scenePos();
        auto blPoint = pt - corner_handle[2]->scenePos();
        auto brPoint = pt - corner_handle[3]->scenePos();

        auto tlLen = std::sqrt(std::pow(tlPoint.x(), 2) + std::pow(tlPoint.y(), 2));
        auto trLen = std::sqrt(std::pow(trPoint.x(), 2) + std::pow(trPoint.y(), 2));
        auto blLen = std::sqrt(std::pow(blPoint.x(), 2) + std::pow(blPoint.y(), 2));
        auto brLen = std::sqrt(std::pow(brPoint.x(), 2) + std::pow(brPoint.y(), 2));

        handle_flags_ = 0;
        int x = 4;
        if ((tlLen * sf - x) < 0) handle_flags_ = (Top_Handle | Left_Handle);
        if (trLen * sf < x) handle_flags_ = (Top_Handle | Right_Handle);
        if (blLen * sf < x) handle_flags_ = (Bottom_Handle | Left_Handle);
        if (brLen * sf < x) handle_flags_ = (Bottom_Handle | Right_Handle);
    }


    switch (handle_flags_) 
    {
        case TopLeft_Handle:
        case BottomRight_Handle:
        {
            if (sem_ == 0) 
            {
                QGuiApplication::setOverrideCursor(QCursor(Qt::SizeFDiagCursor));
                sem_ = 1;
            }
            break;
        } 
        case TopRight_Handle:
        case BottomLeft_Handle:
        {
            if (sem_ == 0) 
            {
                QGuiApplication::setOverrideCursor(QCursor(Qt::SizeBDiagCursor));
                sem_ = 2;
            }
            break;
        }
        default: 
        {
            sem_ = 0;
            QGuiApplication::restoreOverrideCursor();
            break;
        } 
    }
}

bool CanvasItemGroup::isThisDots(const QGraphicsItem* item) const
{
    if (item->type() == eBorderDot)
    {
        return true;
    }
    return false;
}


void CanvasItemGroup::setVisibilityGrabbers()
{
    if (!corner_handle[0]) return;

    corner_handle[GrabberTopLeft]->setVisible(true);
    corner_handle[GrabberTopRight]->setVisible(true);
    corner_handle[GrabberBottomLeft]->setVisible(true);
    corner_handle[GrabberBottomRight]->setVisible(true);
    for (int i = 0; i < 4; i++) {
        corner_handle[i]->setEnabled(true);
    }
}

QRectF CanvasItemGroup::currentSceneBoundingRect() const
{
    return boundingRect();
}

void CanvasItemGroup::resizeTopLeft(const QPointF& pt)
{
    auto pos = pt;
    auto rect = currentSceneBoundingRect();

    QPointF a(rect.x(), rect.y());
    QPointF b(rect.x() + rect.width(), rect.y());
    QPointF c(rect.x() + rect.width(), rect.y() + rect.height());
    QPointF d(rect.x(), rect.y() + rect.height());
    qDebug() << "point a : " << a.x() << " " << a.y();
    qDebug() << "point b : " << b.x() << " " << b.y();
    qDebug() << "point c : " << c.x() << " " << c.y();
    qDebug() << "point d : " << d.x() << " " << d.y();

    Vec2d<qreal> cb(b.x() - c.x(), b.y() - c.y());
    Vec2d<qreal> ba(a.x() - b.x(), a.y() - b.y());
    Vec2d<qreal> ca(a.x() - c.x(), a.y() - c.y());
    Vec2d<qreal> ck(pos.x() - c.x(), pos.y() - c.y());

    auto cb_len = cb.length();
    auto ba_len = ba.length();
    auto ca_len = ca.length();
    auto ck_len = ck.length();

    Vec2d<qreal> cd(d.x() - c.x(), d.y() - c.y());
    auto cd_len = cd.length();

    auto ckca_dot = Vec2d<qreal>::dot(ca, ck);
    auto cos_kca = ckca_dot / (ca_len * ck_len);
    auto cd2_len = ck_len * cos_kca;
    qDebug() << "cd2_len : " << cd2_len << "ck_ len : " << ck_len << "ca_len : " << ca_len;
    qDebug() << "cb_len : " << cb_len << "ba_len : " << ba_len << "cd_len : " << cd_len;

    auto y = (cd2_len * cb_len) / (std::sqrt(ba_len * ba_len + cb_len * cb_len));
    auto x = std::sqrt(cd2_len * cd2_len - y * y);

    qDebug() << "rect.x() : " << rect.x() << "rect.y() : " << rect.y() << "x : " << x << "y : " << y;

    qDebug() << "";

    if (x < 10 || y < 10) {
        return;
    }

    pos.setX(c.x() - x);
    pos.setY(c.y() - y);


    QRectF tmpRect = currentSceneBoundingRect();

    qreal heightOffset = (pos.y() - rect.bottom());
    if (heightOffset < 0)
        tmpRect.setHeight(-heightOffset);
    else
        tmpRect.setHeight(heightOffset);

    qreal widthOffset = (pos.x() - rect.right());
    if (widthOffset < 0)
        tmpRect.setWidth(-widthOffset);
    else
        tmpRect.setWidth(widthOffset);

    tmpRect.translate(currentSceneBoundingRect().width() - tmpRect.width(), currentSceneBoundingRect().height() - tmpRect.height());
    prepareGeometryChange();


    for (auto& it : v_items_) {
        auto widget = qgraphicsitem_cast<CanvasItem*>(it);

        auto new_size = calculateNewSize(tmpRect, widget);

        widget->setPos(new_size.new_x, new_size.new_y);
        widget->setRect(new_size.new_x, new_size.new_y, new_size.new_w, new_size.new_h);
    }

    rectItemGroup_ = tmpRect;
    update();
    setPositionGrabbers();
}


void CanvasItemGroup::resizeTopRight(const QPointF& pt)
{
    auto pos = pt;
    auto rect = currentSceneBoundingRect();

    QPointF a(rect.x(), rect.y());
    QPointF b(rect.x() + rect.width(), rect.y());
    QPointF c(rect.x() + rect.width(), rect.y() + rect.height());
    QPointF d(rect.x(), rect.y() + rect.height());

    Vec2d<qreal> dc(c.x() - d.x(), c.y() - d.y());
    Vec2d<qreal> cb(b.x() - c.x(), b.y() - c.y());
    Vec2d<qreal> db(b.x() - d.x(), b.y() - d.y());
    Vec2d<qreal> dk(pos.x() - d.x(), pos.y() - d.y());

    auto dc_len = dc.length();
    auto cb_len = cb.length();
    auto db_len = db.length();
    auto dk_len = dk.length();

    auto dkdb_dot = Vec2d<qreal>::dot(db, dk);
    auto cos_kdb = dkdb_dot / (db_len * dk_len);
    auto dd2_len = dk_len * cos_kdb;

    auto x = (dd2_len * dc_len) / (std::sqrt(cb_len * cb_len + dc_len * dc_len));
    auto y = std::sqrt(dd2_len * dd2_len - x * x);

    if (x < 10 || y < 10) return;
    pos.setX(d.x() + x);
    pos.setY(d.y() - y);

    QRectF tmpRect = currentSceneBoundingRect();

    qreal heightOffset = (pos.y() - rect.bottom());
    if (heightOffset < 0)
        tmpRect.setHeight(-heightOffset);
    else
        tmpRect.setHeight(heightOffset);

    qreal widthOffset = (pos.x() - tmpRect.left());
    if (widthOffset < 10) /// limit
        return;
    if (widthOffset < 0)
        tmpRect.setWidth(-widthOffset);
    else
        tmpRect.setWidth(widthOffset);

    tmpRect.translate(0, currentSceneBoundingRect().height() - tmpRect.height());

    prepareGeometryChange();


    for (auto& it : v_items_) {
        auto widget = qgraphicsitem_cast<CanvasItem*>(it);

        auto new_size = calculateNewSize(tmpRect, widget);

        widget->setX(new_size.new_x);
        widget->setY(new_size.new_y);
        widget->setRect(new_size.new_x, new_size.new_y, new_size.new_w, new_size.new_h);
    }

    rectItemGroup_ = tmpRect;
    update();
    setPositionGrabbers();
}


void CanvasItemGroup::resizeBottomLeft(const QPointF& pt)
{
    auto pos = pt;
    auto rect = currentSceneBoundingRect();

    QPointF a(rect.x(), rect.y());
    QPointF b(rect.x() + rect.width(), rect.y());
    QPointF c(rect.x() + rect.width(), rect.y() + rect.height());
    QPointF d(rect.x(), rect.y() + rect.height());

    Vec2d<qreal> ba(a.x() - b.x(), a.y() - b.y());
    Vec2d<qreal> ad(d.x() - a.x(), d.y() - a.y());
    Vec2d<qreal> bd(d.x() - b.x(), d.y() - b.y());
    Vec2d<qreal> bk(pos.x() - b.x(), pos.y() - b.y());

    auto ba_len = ba.length();
    auto ad_len = ad.length();
    auto bd_len = bd.length();
    auto bk_len = bk.length();

    auto bkbd_dot = Vec2d<qreal>::dot(bd, bk);
    auto cos_kdb = bkbd_dot / (bd_len * bk_len);
    auto bd2_len = bk_len * cos_kdb;

    auto x = (bd2_len * ba_len) / (std::sqrt(ad_len * ad_len + ba_len * ba_len));
    auto y = std::sqrt(bd2_len * bd2_len - x * x);

    if (x < 10 || y < 10) return;
    pos.setX(b.x() - x);
    pos.setY(b.y() + y);


    QRectF tmpRect = currentSceneBoundingRect();

    qreal heightOffset = (pos.y() - tmpRect.top());
    if (heightOffset < 11) /// limit
        return;
    if (heightOffset < 0)
        tmpRect.setHeight(-heightOffset);
    else
        tmpRect.setHeight(heightOffset);

    qreal widthOffset = (pos.x() - rect.right());
    if (widthOffset < 0)
        tmpRect.setWidth(-widthOffset);
    else
        tmpRect.setWidth(widthOffset);


    tmpRect.translate(currentSceneBoundingRect().width() - tmpRect.width(), 0);
    prepareGeometryChange();


    for (auto& it : v_items_) {
        auto widget = qgraphicsitem_cast<CanvasItem*>(it);

        auto new_size = calculateNewSize(tmpRect, widget);

        widget->setX(new_size.new_x);
        widget->setY(new_size.new_y);
        widget->setRect(new_size.new_x, new_size.new_y, new_size.new_w, new_size.new_h);
    }

    rectItemGroup_ = tmpRect;
    update();
    setPositionGrabbers();
}


void CanvasItemGroup::resizeBottomRight(const QPointF& pt)
{
    auto pos = pt;
    auto rect = currentSceneBoundingRect();

    QPointF a(rect.x(), rect.y());
    QPointF b(rect.x() + rect.width(), rect.y());
    QPointF c(rect.x() + rect.width(), rect.y() + rect.height());
    QPointF d(rect.x(), rect.y() + rect.height());

    Vec2d<qreal> ad(d.x() - a.x(), d.y() - a.y());
    Vec2d<qreal> dc(c.x() - d.x(), c.y() - d.y());
    Vec2d<qreal> ac(c.x() - a.x(), c.y() - a.y());
    Vec2d<qreal> ak(pos.x() - a.x(), pos.y() - a.y());

    auto ad_len = ad.length();
    auto dc_len = dc.length();
    auto ac_len = ac.length();
    auto ak_len = ak.length();

    auto akac_dot = Vec2d<qreal>::dot(ac, ak);
    auto cos_kac = akac_dot / (ac_len * ak_len);
    auto ad2_len = ak_len * cos_kac;

    auto x = (ad2_len * dc_len) / (std::sqrt(ad_len * ad_len + dc_len * dc_len));
    auto y = std::sqrt(ad2_len * ad2_len - x * x);

    if (x < 10 || y < 10) return;
    pos.setX(a.x() + x);
    pos.setY(a.y() + y);


    QRectF tmpRect = currentSceneBoundingRect();
    if (pos.y() < tmpRect.top())
        return;
    qreal heightOffset = (pos.y() - tmpRect.top());
    if (heightOffset < 11) /// limit
        return;
    if (heightOffset < 0)
        tmpRect.setHeight(-heightOffset);
    else
        tmpRect.setHeight(heightOffset);
    if (pos.x() < tmpRect.left())
        return;
    qreal widthOffset = (pos.x() - tmpRect.left());
    if (widthOffset < 10) /// limit
        return;
    if (widthOffset < 0)
        tmpRect.setWidth(-widthOffset);
    else
        tmpRect.setWidth(widthOffset);

    prepareGeometryChange();


    for (auto& it : v_items_) {
        auto widget = qgraphicsitem_cast<CanvasItem*>(it);

        auto new_size = calculateNewSize(tmpRect, widget);

        widget->setX(new_size.new_x);
        widget->setY(new_size.new_y);
        widget->setRect(new_size.new_x, new_size.new_y, new_size.new_w, new_size.new_h);
    }

    rectItemGroup_ = tmpRect;
    update();
    setPositionGrabbers();
}

CanvasItemGroup::new_size_t CanvasItemGroup::calculateNewSize(const QRectF& tmpRect, CanvasItem* widget)
{
    qreal old_ig_w = boundingRect().width();
    qreal old_ig_h = boundingRect().height();
    qreal new_ig_w = tmpRect.width();
    qreal new_ig_h = tmpRect.height();

    qreal old_h = abs(widget->boundingRect().bottom() - widget->boundingRect().top());
    qreal new_h = (old_h * new_ig_h) / old_ig_h;
    qreal old_w = abs(widget->boundingRect().left() - widget->boundingRect().right());
    qreal new_w = (old_w * new_h) / old_h;

    qreal new_x = 0;
    qreal new_y = 0;

    if (widget->pos().x() == boundingRect().left()) {
        qreal aa = abs(widget->pos().y() - boundingRect().bottom());
        qreal yy = (aa * new_ig_w) / old_ig_w;
        qreal y_delta = abs(new_ig_h - yy);

        new_x = tmpRect.topLeft().x();
        new_y = tmpRect.topLeft().y() + y_delta;
    }
    else if (widget->pos().y() == boundingRect().top()) {
        qreal aa = abs(widget->pos().x() - boundingRect().right());
        qreal xx = (aa * new_ig_h) / old_ig_h;
        qreal x_delta = abs(new_ig_w - xx);

        new_x = tmpRect.topLeft().x() + x_delta;
        new_y = tmpRect.topLeft().y();
    }
    else {
        qreal qq = abs(boundingRect().left() - widget->pos().x());
        qreal ww = abs(boundingRect().top() - widget->pos().y());
        qreal new_x_delta = (new_ig_w * qq) / old_ig_w;
        qreal new_y_delta = (new_ig_h * ww) / old_ig_h;

        new_x = tmpRect.left() + new_x_delta;
        new_y = tmpRect.top() + new_y_delta;
    }

    return { new_x, new_y, new_w, new_h };
}

void CanvasItemGroup::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    if (isEmpty())return;
}