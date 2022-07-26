#pragma once

#include "canvas_header.h"

#include <QGraphicsScene>
#include <QMenu>
#include <QObject>

class CanvasWidget;
class CanvasItemGroup;
class CanvasBackgroundItem;

//enum EState {
//    eMouseMoving = 0x0000,
//    eMouseSelection = 0x0001,
//    eGroupItemMoving = 0x0002,
//    eGroupItemResizing = 0x0003,
//};

class CanvasScene : public QGraphicsScene
{

    Q_OBJECT

public:
    CanvasScene(CanvasWidget* mw, uint64_t z, QObject* parent = nullptr);
    ~CanvasScene();

public:
    //void onSelectionChanged();
    //void cleanupWorkplace();
    QGraphicsItem* getFirstItemUnderCursor(const QPointF& p);
signals:
    void foo();
public slots:
    void fooSlot();

    //void addImageToSceneToPosition(QImage&& image, QPointF position);

    /*
    //void pasteFromClipboard();
    //void pasteFromTemp();
    //void copyToClipboard();

    //QByteArray fml_payload();
    //void updateViewScaleFactor(qreal newval) {
    //    itemGroup_->setScaleControlFactor(newval);
    //    parentViewScaleFactor_ = newval;
    //}

    //void setProjectSettings(project_settings* ps);

    //QString path();
    //void setPath(const QString& path);
    //QString projectName();
    //void setProjectName(const QString& pn);
    //bool isModified();
    //void setModified(bool mod);
    //bool isUntitled();
    //ItemGroup* itemGroup() const noexcept { return itemGroup_; }
    */
protected:


    /*
    //void keyPressEvent(QKeyEvent* event) override;
    //void dragEnterEvent(QGraphicsSceneDragDropEvent* event) override;
    //void dragMoveEvent(QGraphicsSceneDragDropEvent* event) override;
    //void dropEvent(QGraphicsSceneDragDropEvent* event) override;
    */


//public slots:
//    void slotMove(QGraphicsItem* signalOwner, qreal dx, qreal dy);
//private slots:
//    void clipboardChanged();
private:
    //qint16 objectsCount() const;
    //bool isAnySelectedUnderCursor() const;
    //void deselectItems();

    //void handleImageFromClipboard(const QImage& image);
    //void handleHtmlFromClipboard(const QString& html);

private:
    CanvasWidget* mainwindow_;
    CanvasItemGroup* item_group_ = nullptr;
    CanvasBackgroundItem* bg_item_ = nullptr;

    qreal parentViewScaleFactor_ = 1;

    uint64_t z_value_;

    QPointF origin_;
    QRectF rubberBand_;
    QPointF lastClickedPoint_{ 0,0 };


    
    //MainSelectedArea mainSelArea_;

    OperationState state_ = eMouseMoving;

    //ImageDownloader* imgdownloader_ = nullptr;//image loader need c++threads!!!


    //project_settings* projectSettings_;//tab ±ÍÃ‚–«∫≈

    QMenu menu_;
    QMenu item_menu_;

    QVector<QGraphicsItem*> v_items_;
    
public:
    void setSceneBoundaryRect();
    void AddImageItem(QPoint pos, QString url, uint64_t z);
    bool IsEmpty();
signals:
    void sigDelete(QPointF p);
    //void sigDelete();
protected slots:
    void onSigDelete(QPointF p);
    //void onSigDelete();
    void onAddImageItem();
    void onDeleteItem();
protected:
    void drawForeground(QPainter* painter, const QRectF& rect) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
};

