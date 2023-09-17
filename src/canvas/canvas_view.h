#pragma once
#include <QGraphicsView>
#include <QMenu>

class CanvasItem;
class CanvasWidget;
class CanvasScene;
class CanvasBackgroundItem;

class CanvasView : public QGraphicsView
{
	Q_OBJECT

public:
    struct AspectRatio
    {
        uint32_t width;
        uint32_t height;
    };

public:
	CanvasView(CanvasWidget* canvas, QWidget* parent = nullptr);

    void addItem(QPointF point);
    void AddImageItem(QPointF point, QString url);

    //void addImage(const QString& path, QPointF point);
    //void cleanupWorkplace();
    //void zoomFactor(double factor) noexcept { zoomFactor_ = factor; }
    //double zoomFactor() const noexcept { return zoomFactor_; }


    //void addImage(QImage* img, QPointF point);
    //void addImage(QByteArray ba, int w, int h, QRect br, qsizetype bpl, QImage::Format f);
    //QByteArray fml_payload();
    //QString path();
    //void setPath(const QString& path);
    //QString projectName();
    //void setProjectName(const QString& pn);
    //bool isModified();
    //void setModified(bool mod);
    //bool isUntitled();
private slots:

    //void onSelectionChanged();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void drawBackground(QPainter* painter, const QRectF& rect) override;

    //void contextMenuEvent(QContextMenuEvent* event) override;
    //void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    //void mouseReleaseEvent(QMouseEvent* event) override;





    //void mouseDoubleClickEvent(QMouseEvent* event) override;
//void wheelEvent(QWheelEvent* event) override;

private:
    //void drawForeground(QPainter* painter, const QRectF& rect) override;
    //void calculateCanvasSceneSize();
private:
    CanvasBackgroundItem* bg_item_;
    CanvasWidget* mainwindow_;
    CanvasScene* scene_;
    double zoomFactor_ = 1.15;
    Qt::MouseButton pan_;
    bool rightMoveflag_{ false };
    QPoint pressPos_;
    QPoint wndPos_;
    bool isMoving_{ false };
    qreal panStartX_;
    qreal panStartY_;
    uint64_t z_value_ = 0;
    bool fitViewF_{ false };

    AspectRatio aspectRatio_{0};

private slots:
    void onSigAddImage();
private:
    QMenu menu_;
    CanvasItem* item_test_ = nullptr;
};

