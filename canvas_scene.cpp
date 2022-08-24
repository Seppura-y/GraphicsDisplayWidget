#include "canvas_scene.h"
#include "canvas_item_group.h"
#include "canvas_header.h"
#include "canvas_item.h"
#include "canvas_display_item.h"


#include <cmath>
#include <iostream>

#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneEvent>
#include <QGraphicsSceneMouseEvent>
#include <QAction>
#include <QObject>
#include <QFileDialog>

using namespace std;

CanvasScene::CanvasScene(CanvasWidget* mw, uint64_t z, QObject* parent) : QGraphicsScene(parent), mainwindow_(mw), z_value_(z)
{
    item_group_ = new CanvasItemGroup(z);
    item_group_->setPos({ 0,0 });
    addItem(item_group_);
    //connect(item_group_, &CanvasItemGroup::signalMove, this, &CanvasScene::slotMove);

    //QAction* act = menu_.addAction(QString::fromLocal8Bit("delete"));
    //act->setCheckable(false);
    //act->setEnabled(true);
    //QObject::connect(act, &QAction::triggered, this, &CanvasScene::onSigDelete);
    QObject::connect(this, SIGNAL(sigDelete(QPointF)), this, SLOT(onSigDelete(QPointF)));
    //QObject::connect(this, SIGNAL(foo()), this, SLOT(onSigDelete()));
    //QObject::connect(this, SIGNAL(foo()), this, SLOT(fooSlot()));

    QAction* act = menu_.addAction(QString::fromLocal8Bit("add image"));
    act->setCheckable(false);
    QObject::connect(act, &QAction::triggered, this, &CanvasScene::onAddImageItem);

    act = item_menu_.addAction(QString::fromLocal8Bit("delete"));
    act->setCheckable(false);
    QObject::connect(act, &QAction::triggered, this, &CanvasScene::onDeleteItem);

}

CanvasScene::~CanvasScene()
{

}

QGraphicsItem* CanvasScene::getFirstItemUnderCursor(const QPointF& p)
{
    auto childs = this->items(Qt::DescendingOrder);

    for (auto& it : childs) {
        if (it == item_group_) continue;
        if (it->type() == eBorderDot) { // borderdot control
            // NOTE: Maybe I must reimplement QGraphicsItem::contains,
            //       but I don't know how yet.
            auto point = p - it->scenePos();
            auto len = std::sqrt(std::pow(point.x(), 2) + std::pow(point.y(), 2));
            int x = 4;
            if ((len * parentViewScaleFactor_ - x) < 0) return it;
        }
        else {
            if (it->sceneBoundingRect().contains(p)) {
                return it;
            }
        }
    }

    return nullptr;
}

void CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    cout << "canvas scene mouse pressed : " << event->scenePos().x() << " " << event->scenePos().y() << endl;
    //cout << "canvas scene mouse global : " << mapToGlobal(event->scenePos().x()) << " " << event->scenePos().y() << endl;
    cout << "canvas scene bounding rect : " << sceneRect().x() << " : " << sceneRect().y() << " : " << sceneRect().width() << " : " << sceneRect().height() << endl;

    auto item = getFirstItemUnderCursor(event->scenePos());
    if (item && item->type() != CanvasItemGroup::eBorderDot && item->type() != eBackgroundItem) 
    {
        item->setZValue(++z_value_);
    }
    if (item && item->type() == CanvasItemGroup::eBorderDot) 
    {
        state_ = eGroupItemResizing;

        QGraphicsScene::mousePressEvent(event);
        return;
    }

    if (item && item->type() == eBackgroundItem)
    {
        item = nullptr;
    }

    if (event->button() == Qt::LeftButton) 
    {
        if (event->modifiers() == Qt::ShiftModifier) 
        {

        }
        else if (event->modifiers() != Qt::ShiftModifier) 
        {
            if (item) 
            {
                if (!item_group_->isContain(item)) 
                {
                    item_group_->clearItemGroup();
                    item_group_->addItemToGroup(item);
                    item_group_->increaseZValue();
                    //mainSelArea_.setReady(true);
                    //LOG_DEBUG(logger, "Group size: ", item_group_->childItems().size(), ". Empty: ", itemGroup_->isEmpty());
                    //                    LOG_DEBUG(logger, "DEBUG MESSAGE1 state: ", stateText(state_));
                }
                //                state_ = eMouseSelection;
            }
            else 
            {
                state_ = eMouseSelection;
                origin_ = event->scenePos();
                rubberBand_.setRect(origin_.x(), origin_.y(), 0, 0);
                item_group_->clearItemGroup();
                //mainSelArea_.setReady(false);
                //                state_ = eMouseSelection;
            }
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void CanvasScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton)) 
    {
        if (state_ == eMouseSelection) 
        {

            //QRectF tmp(this->sceneRect().topLeft(), this->sceneRect().bottomRight());
            QRectF tmp(origin_, event->scenePos());
            rubberBand_ = tmp;
            rubberBand_ = rubberBand_.normalized();

            item_group_->clearItemGroup();
            auto selItems = this->items(rubberBand_);
            if (selItems.size() > 0)
            {
                cout << "selItem size > 0" << endl;
            }
            for (auto& item : selItems) 
            {
                if (item->type() == 3)
                {
                    item_group_->addItemToGroup(item);
                }
                //            it->setSelected(false);
            }
            item_group_->increaseZValue();

            if (item_group_->isEmpty()) 
            {
                //mainSelArea_.setReady(false);
            }
            else 
            {
                //mainSelArea_.setReady(true);
            }
        }
        else if (state_ == eGroupItemResizing || state_ == eGroupItemMoving) 
        {
            //projectSettings_->modified(true);
        }
        else if (item_group_->isUnderMouse() && state_ != eGroupItemResizing) 
        {
            state_ = eGroupItemMoving;
            //            LOG_DEBUG(logger, "DEBUG: CHANGE TO mouse state: ", stateText(state_), "\n");
        }
    }

    item_group_->notifyCursorUpdater(event, parentViewScaleFactor_);
    QGraphicsScene::mouseMoveEvent(event);
}

void CanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    //lastClickedPoint_ = event->scenePos();
    //LOG_DEBUG(logger, "DEBUG: mouse state: ", stateText(state_), "\n");

    if (state_ == eGroupItemResizing || state_ == eMouseSelection) {
        state_ = eMouseMoving;
        //LOG_DEBUG(logger, "DEBUG: CHANGE TO mouse state: ", stateText(state_), "\n");
        item_group_->notifyCursorUpdater(event, parentViewScaleFactor_);
        QGraphicsScene::mouseReleaseEvent(event);
        return;
    }

    auto item = getFirstItemUnderCursor(event->scenePos());
    if (event->button() == Qt::LeftButton) {
        if (event->modifiers() == Qt::ShiftModifier) {
            if (item) { // add to group
                if (!item_group_->isContain(item)) {
                    item->setZValue(++z_value_);
                    item_group_->addItemToGroup(item);
                    item_group_->increaseZValue();
                    //mainSelArea_.setReady(true);
                }
                else {
                    item_group_->removeItemFromGroup(item);
                    if (item_group_->isEmpty()) {
                        item_group_->clearItemGroup();
                        //mainSelArea_.setReady(false);
                    }
                }
            }
        }
        else if (event->modifiers() != Qt::ShiftModifier) {
            if (item) {
                if (item_group_->isContain(item) && !item_group_->isThisDots(item)) {
                    if (state_ != eGroupItemMoving) {
                        item_group_->clearItemGroup();
                        item->setZValue(++z_value_);
                        item_group_->addItemToGroup(item);
                        item_group_->increaseZValue();
                        //mainSelArea_.setReady(true);
                    }

                    state_ = eMouseMoving;
                    //LOG_DEBUG(logger, "DEBUG: CHANGE TO mouse state: ", stateText(state_), "\n");
                }
            }
            else {
                item_group_->clearItemGroup();
                //mainSelArea_.setReady(false);
                state_ = eMouseMoving;
                //LOG_DEBUG(logger, "DEBUG: CHANGE TO mouse state: ", stateText(state_), "\n");
            }
        }
    }

    item_group_->notifyCursorUpdater(event, parentViewScaleFactor_);
    QGraphicsScene::mouseReleaseEvent(event);
}

void CanvasScene::drawForeground(QPainter* painter, const QRectF& rect)
{
    //if (!mainSelArea_.isReady()) return;
    painter->save();
    qreal wsize = 2;
    //QPen outline_pen{QColor(22, 142, 153), wsize};
    QPen outline_pen{ QColor("orange"), wsize };
    outline_pen.setCosmetic(true);
    painter->setPen(outline_pen);
    auto r = item_group_->sceneBoundingRect();
    painter->drawRect(r);
    painter->restore();

    QGraphicsScene::drawForeground(painter, rect);
}

void CanvasScene::setSceneBoundaryRect()
{
    QPainterPath path;
    path.addRect(this->sceneRect());

    for (auto iter : this->items())
    {
        if (iter)
        {
            //cout << "scene rect " << this->sceneRect().x() << " : " << this->sceneRect().y() << " : " << this->sceneRect().width() << " : " << this->sceneRect().height() << endl;
            //cout << "iter scene pos " << iter->scenePos().x() << " : " << iter->scenePos().y() << endl;
            //cout << "iter pos " << iter->pos().x() << " : " << iter->pos().y() << endl << endl;;
            //CanvasItem item = qgraphicsitem_cast<CanvasItem>(iter);
            //CanvasItem* item = dynamic_cast<CanvasItem*>(iter);
            CanvasDisplayItem* item = dynamic_cast<CanvasDisplayItem*>(iter);
            if (item)
            {
                item->setBoundaryPath(path);
                item->setParentSceneRect(this->sceneRect());
            }

        }
    }
}

void CanvasScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QPointF p = event->scenePos();
    auto item = getFirstItemUnderCursor(p);
    if (item)
    {
        this->clearFocus();
        this->setFocusItem(item);
        item_menu_.exec(QCursor::pos());
        event->accept();
    }
    else
    {
        menu_.exec(QCursor::pos());
        event->accept();
    }
    return QGraphicsScene::contextMenuEvent(event);
}

void CanvasScene::onSigDelete(QPointF p)
//void CanvasScene::onSigDelete()
{
    cout << "delete" << endl;
    auto item = getFirstItemUnderCursor(p);
    if (item)
    {
        this->removeItem(item);
        l_items_.removeOne(item);
        delete item;
    }
}

void CanvasScene::fooSlot()
{
    cout << "foo slot" << endl;
}

void CanvasScene::AddCustomItem(QGraphicsItem* item)
{
    l_items_.push_back(item);
    this->addItem(item);
}

void CanvasScene::AddImageItem(QPoint pos, QString url, uint64_t z)
{
    QPainterPath path;
    path.addRect(this->sceneRect());

    CanvasItem* item = new CanvasItem(url, z);
    item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    item->setBoundaryPath(path);
    this->addItem(item);
}

void CanvasScene::onAddImageItem()
{
    //QString img_url = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("Ñ¡ÔñÎÄ¼þ"), QString("/"), QString("Files (*.jpg)"));

    QPainterPath path;
    path.addRect(this->sceneRect());

    //CanvasItem* item = new CanvasItem(img_url, z_value_ + 1);
    CanvasDisplayItem* item = new CanvasDisplayItem(z_value_ + 1);
    //item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    //item->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsFocusable);
    item->setBoundaryPath(path);
    item->setParentSceneRect(this->sceneRect());
    this->addItem(item);
    l_items_.push_back(item);
    
}

void CanvasScene::onDeleteItem()
{
    auto item = focusItem();
    if (item)
    {
        this->setFocusItem(nullptr);
        item_group_->removeItemFromGroup(item);
        l_items_.removeOne(item);
        this->removeItem(item);
        delete item;
    }
}

bool CanvasScene::IsEmpty()
{
    return l_items_.empty();
}

void CanvasScene::ItemsTransformation(QTransform matrix)
{
    for (auto i : l_items_)
    {
        auto item = qgraphicsitem_cast<CanvasDisplayItem*>(i);
        if (item)
        {
            QRectF rect = item->boundingRect();

            QRectF rect_map = matrix.mapRect(rect);

            item->setTransform(matrix);

            item->setRect(rect_map);
        }

    }
}

void CanvasScene::ItemsTransformation(qreal scalar,int x,int y)
{
    for (auto i : l_items_)
    {
        auto item = qgraphicsitem_cast<CanvasItem*>(i);
        if (item)
        {
            //QRectF rect = item->boundingRect();
            //QRectF rect = item->getRect();
            //QRectF rect_map = matrix.mapRect(rect);

            //item->setPos(rect.x(), rect.y());
            //item->setRect(rect_map);
            //item->update();

            QRectF rect = item->boundingRect();

            QTransform tf(1, 0, 0, 0, 1, 0, 0, 0, 1);
            tf.scale(scalar, scalar);
            tf.translate(x, y);
            QRectF rect_map = tf.mapRect(rect);

            item->setTransform(tf);
            item->setRect(rect_map);
            item->prepareChange();
        }
    }
}
