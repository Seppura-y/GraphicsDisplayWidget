#pragma once
#include <QWidget>
#include <QImage>
class ResizeableWidget : public QWidget
{
	Q_OBJECT

public:
    enum SideType
    {
        NO_SIDE = 0,
        TOP_SIDE = 0b0001,
        BOTTOM_SIDE = 0b0010,
        RIGHT_SIDE = 0b0100,
        LEFT_SIDE = 0b1000,
        TOPLEFT_SIDE = TOP_SIDE | LEFT_SIDE,
        BOTTOMLEFT_SIDE = BOTTOM_SIDE | LEFT_SIDE,
        TOPRIGHT_SIDE = TOP_SIDE | RIGHT_SIDE,
        BOTTOMRIGHT_SIDE = BOTTOM_SIDE | RIGHT_SIDE,
        CENTER = 0b10000,
    };

public:
    explicit ResizeableWidget(QColor color, QWidget* parent = nullptr);
    SideType getMouseSide(const QPoint& mousePos) const;
    QVector<QRect> handlerAreas();
    
    void setParentWidget(QWidget* wid);

    void setGeometry(const QRect& r);
    QRect geometry() const;

    void setImageData(const unsigned char* data,int width,int height);
signals:
    void sig_SizeChanged();
protected:

private:
    void updateAreas();
    void updateCursor(QPoint p);
    QRect rect() const;
    SideType getProperSide(SideType side, const QRect& r);



    void paintEvent(QPaintEvent*) override;
    void resizeEvent(QResizeEvent*) override;
    void moveEvent(QMoveEvent*) override;

    //void hoverEvent(QHoverEvent* e) override;
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseDoubleClickEvent(QMouseEvent* e) override;

    void parentMousePressEvent(QMouseEvent* e);
    void parentMouseReleaseEvent(QMouseEvent* e);
    void parentMouseMoveEvent(QMouseEvent* e);

    bool eventFilter(QObject*, QEvent*) override;
private:
    QColor m_color;
    QPoint m_areaOffset;
    QPoint m_handleOffset;

    QPoint m_dragStartPos;
    SideType m_activeSide;
    QCursor m_idleCentralCursor;
    bool m_ignoreMouse;
    bool m_mouseStartMove;

    // naming convention for handles
    // T top, B bottom, R Right, L left
    // 2 letters: a corner
    // 1 letter: the handle on the middle of the corresponding side
    QRect m_TLHandle, m_TRHandle, m_BLHandle, m_BRHandle;
    QRect m_LHandle, m_THandle, m_RHandle, m_BHandle;

    QRect m_TLArea, m_TRArea, m_BLArea, m_BRArea;
    QRect m_LArea, m_TArea, m_RArea, m_BArea;

    //QImage* image_ = nullptr;
    QImage image_;
    bool first_save_ = true;

    bool left_bt_clicked_ = false;
};

