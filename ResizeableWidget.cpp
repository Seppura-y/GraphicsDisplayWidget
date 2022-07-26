#include "ResizeableWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QDebug>

#include <QApplication>

#define MARGIN (m_THandle.width())

ResizeableWidget::ResizeableWidget(QColor c, QWidget* parent)
    : QWidget(parent)
    , m_color(std::move(c))
    , m_activeSide(NO_SIDE)
    , m_ignoreMouse(false)
{
    // prevents this widget from consuming CaptureToolButton mouse events
    //setAttribute(Qt::WA_TransparentForMouseEvents);
    setStyleSheet("background-color:transparent");
    //parent->installEventFilter(this);

    QWidget::setGeometry(200, 200, 640, 360);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setMouseTracking(true);

    //int sideVal = GlobalValues::buttonBaseSize() * 0.6;
    //int sideVal = QApplication::fontMetrics().lineSpacing() * 2.2 * 0.6;
    int sideVal = 18;
    int handleSide = sideVal / 2;
    const QRect areaRect(0, 0, sideVal, sideVal);

    const QRect handleRect(0, 0, handleSide, handleSide);
    m_TLHandle = m_TRHandle = m_BLHandle = m_BRHandle = m_LHandle = m_THandle =
        m_RHandle = m_BHandle = handleRect;
    m_TLArea = m_TRArea = m_BLArea = m_BRArea = areaRect;

    m_areaOffset = QPoint(-sideVal / 2, -sideVal / 2);
    m_handleOffset = QPoint(-handleSide / 2, -handleSide / 2);

    updateAreas();

    //image_ = new QImage(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/9c5bbafec011cdbfc70d768223432bf5.png");
    //img(QString(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/9c5bbafec011cdbfc70d768223432bf5.png"));


    //QHBoxLayout layout(this);
    //QLabel* label = new QLabel(this);
    //QPainter painter;
    //QPixmap pix_map;

    //layout.addWidget(label);
    //pix_map.load(QString(":/GraphicsDisplayWidget/C:/Users/y7513/Desktop/9c5bbafec011cdbfc70d768223432bf5.png"));
    //label->setPixmap(pix_map);
    //this->setLayout(&layout);
}

ResizeableWidget::SideType ResizeableWidget::getMouseSide(const QPoint& mousePos) const
{
    if (!isVisible()) {
        return NO_SIDE;
    }
    //QPoint localPos = mapFromParent(mousePos);
    QPoint localPos = mousePos;
    if (m_TLArea.contains(localPos)) {
        return TOPLEFT_SIDE;
    }
    else if (m_TRArea.contains(localPos)) {
        return TOPRIGHT_SIDE;
    }
    else if (m_BLArea.contains(localPos)) {
        return BOTTOMLEFT_SIDE;
    }
    else if (m_BRArea.contains(localPos)) {
        return BOTTOMRIGHT_SIDE;
    }
    else if (m_LArea.contains(localPos)) {
        return LEFT_SIDE;
    }
    else if (m_TArea.contains(localPos)) {
        return TOP_SIDE;
    }
    else if (m_RArea.contains(localPos)) {
        return RIGHT_SIDE;
    }
    else if (m_BArea.contains(localPos)) {
        return BOTTOM_SIDE;
    }
    else if (rect().contains(localPos)) {
        return CENTER;
    }
    else {
        return NO_SIDE;
    }
}

QVector<QRect> ResizeableWidget::handlerAreas()
{
    QVector<QRect> areas;
    areas << m_TLHandle << m_TRHandle << m_BLHandle << m_BRHandle << m_LHandle
        << m_THandle << m_RHandle << m_BHandle;
    return areas;
}

void ResizeableWidget::updateAreas()
{
    QRect r = rect();
    QPoint p = r.topRight() + m_areaOffset;
    m_TLArea.moveTo(r.topLeft() + m_areaOffset);
    m_TRArea.moveTo(r.topRight() + m_areaOffset);
    m_BLArea.moveTo(r.bottomLeft() + m_areaOffset);
    m_BRArea.moveTo(r.bottomRight() + m_areaOffset);

    m_LArea = QRect(m_TLArea.bottomLeft(), m_BLArea.topRight());
    m_TArea = QRect(m_TLArea.topRight(), m_TRArea.bottomLeft());
    m_RArea = QRect(m_TRArea.bottomLeft(), m_BRArea.topRight());
    m_BArea = QRect(m_BLArea.topRight(), m_BRArea.bottomLeft());

    m_TLHandle.moveTo(m_TLArea.center() + m_handleOffset);
    m_BLHandle.moveTo(m_BLArea.center() + m_handleOffset);
    m_TRHandle.moveTo(m_TRArea.center() + m_handleOffset);
    m_BRHandle.moveTo(m_BRArea.center() + m_handleOffset);
    m_LHandle.moveTo(m_LArea.center() + m_handleOffset);
    m_THandle.moveTo(m_TArea.center() + m_handleOffset);
    m_RHandle.moveTo(m_RArea.center() + m_handleOffset);
    m_BHandle.moveTo(m_BArea.center() + m_handleOffset);
}

void ResizeableWidget::updateCursor(QPoint point)
{
    SideType mouseSide = m_activeSide;
    mouseSide = getMouseSide(point);
    //if (!m_activeSide) {
    //    //mouseSide = getMouseSide(parentWidget()->mapFromGlobal(QCursor::pos()));
    //    //mouseSide = getMouseSide(QCursor::pos());
    //    mouseSide = getMouseSide(point);
    //}
    //else
    //{
    //    //mouseSide = getMouseSide(point);
    //}

    switch (mouseSide) {
    case TOPLEFT_SIDE:
        //qDebug() << "TOPLEFT_SIDE";
        //setCursor(Qt::SizeFDiagCursor);
        QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
        break;
    case BOTTOMRIGHT_SIDE:
        //qDebug() << "BOTTOMRIGHT_SIDE";
        QApplication::setOverrideCursor(Qt::SizeFDiagCursor);
        break;
    case TOPRIGHT_SIDE:
        //qDebug() << "TOPRIGHT_SIDE";
        QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
        break;
    case BOTTOMLEFT_SIDE:
        //qDebug() << "BOTTOMLEFT_SIDE";
        QApplication::setOverrideCursor(Qt::SizeBDiagCursor);
        break;
    case LEFT_SIDE:
        //qDebug() << "LEFT_SIDE";
        QApplication::setOverrideCursor(Qt::SizeHorCursor);
        break;
    case RIGHT_SIDE:
        //qDebug() << "RIGHT_SIDE";
        QApplication::setOverrideCursor(Qt::SizeHorCursor);
        break;
    case TOP_SIDE:
        //qDebug() << "TOP_SIDE";
        QApplication::setOverrideCursor(Qt::SizeVerCursor);
        break;
    case BOTTOM_SIDE:
        //qDebug() << "BOTTOM_SIDE";
        QApplication::setOverrideCursor(Qt::SizeVerCursor);
        break;
    default:
        if (m_activeSide) {
            QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        }
        else {
            //setCursor(m_idleCentralCursor);
            //qDebug() << "ArrowCursor";
            QApplication::setOverrideCursor(Qt::ArrowCursor);
            return;
        }

        break;
    }
}



void ResizeableWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(rect(), image_);
    QPen pen = p.pen();
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(2);
    pen.setColor(m_color);
    p.setPen(pen);
    //p.drawRect(rect() + QMargins(0, 0, -1, -1));
    p.drawRect(rect());
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(m_color);

    for (auto rect : handlerAreas()) {

        pen = p.pen();
        pen.setStyle(Qt::SolidLine);
        pen.setColor("white");
        p.setPen(pen);

        p.setRenderHint(QPainter::Antialiasing);
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor("white");
        p.setBrush(brush);

        p.drawEllipse(rect.center(), 6, 6);

        pen.setColor("orange");
        brush.setColor("orange");

        p.setPen(pen);
        p.setBrush(brush);

        p.drawEllipse(rect.center(), 4, 4);
    }

    //p.drawImage(rect().x(), rect().y(), *image_);
}

void ResizeableWidget::resizeEvent(QResizeEvent*)
{
    updateAreas();
    //if (isVisible()) {
    //    emit geometryChanged();
    //}
}

void ResizeableWidget::moveEvent(QMoveEvent*)
{
    updateAreas();
    //if (isVisible()) {
    //    emit geometryChanged();
    //}
}

void ResizeableWidget::mouseDoubleClickEvent(QMouseEvent* e)
{
    move(QPoint(this->QWidget::geometry().x() + 10,this->QWidget::geometry().y()+10));
}

void ResizeableWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() != Qt::LeftButton) {
        return;
    }

    qDebug() << "clicked : x->" <<e->pos().x() << " y->" << e->pos().y();
    m_dragStartPos = e->pos();
    m_activeSide = getMouseSide(e->pos());

    left_bt_clicked_ = true;
    //m_dragStartPos = parentWidget()->mapFromGlobal(QCursor::pos());
    //m_activeSide = getMouseSide(parentWidget()->mapFromGlobal(QCursor::pos()));
}

void ResizeableWidget::mouseReleaseEvent(QMouseEvent* e)
{
    // released outside of the selection area
    //if (!getMouseSide(e->pos())) {
    //    //hide();
    //}

    m_activeSide = NO_SIDE;
    //updateCursor(e->pos());
    //emit geometrySettled();
}

void ResizeableWidget::mouseMoveEvent(QMouseEvent* e)
{
    updateCursor(e->pos());
    //qDebug() << "move : x->" << e->pos().x() << " y->" << e->pos().y();
    //setCursor(Qt::ClosedHandCursor);
    if (e->buttons() != Qt::LeftButton) {
        return;
    }


    SideType mouseSide = m_activeSide;
    if (!m_activeSide) {
        mouseSide = getMouseSide(e->pos());
        //mouseSide = getMouseSide(parentWidget()->mapFromGlobal(QCursor::pos()));
    }

    //if (!isVisible() || !mouseSide) {
    //    show();
    //    m_dragStartPos = e->pos();
    //    m_activeSide = TOPLEFT_SIDE;
    //    setGeometry({ e->pos(), e->pos() });
    //}

    if (!left_bt_clicked_)return;
    //QPoint pos = QPoint(QWidget::geometry().x(), QWidget::geometry().y());
    QPoint pos = e->pos();
    pos = parentWidget()->mapToGlobal(QCursor::pos());
    //QPoint pos = parentWidget()->mapFromGlobal(QCursor::pos());
    auto geom = geometry();
    //bool symmetryMod = qApp->keyboardModifiers() & Qt::ShiftModifier;


    qDebug() << "e->pos() : x->" << pos.x() << " y->" << pos.y();
    QPoint newTopLeft = geom.topLeft(), newBottomRight = geom.bottomRight();
    int& newLeft = newTopLeft.rx(), & newRight = newBottomRight.rx(),
        & newTop = newTopLeft.ry(), & newBottom = newBottomRight.ry();
    switch (mouseSide) {
    case TOPLEFT_SIDE:
        if (m_activeSide) {
            newTopLeft = pos;
        }
        break;
    case BOTTOMRIGHT_SIDE:
        if (m_activeSide) {
            newBottomRight = pos;
        }
        break;
    case TOPRIGHT_SIDE:
        if (m_activeSide) {
            newTop = pos.y();
            newRight = pos.x();
        }
        break;
    case BOTTOMLEFT_SIDE:
        if (m_activeSide) {
            newBottom = pos.y();
            newLeft = pos.x();
        }
        break;
    case LEFT_SIDE:
        if (m_activeSide) {
            newLeft = pos.x();
        }
        break;
    case RIGHT_SIDE:
        if (m_activeSide) {
            newRight = pos.x();
        }
        break;
    case TOP_SIDE:
        if (m_activeSide) {
            newTop = pos.y();
        }
        break;
    case BOTTOM_SIDE:
        if (m_activeSide) {
            newBottom = pos.y();
        }
        break;
    default:
        if (m_activeSide) {
            //move(this->pos() + pos - m_dragStartPos);
            //m_dragStartPos = pos;
        }
        //return;
        break;
    }
    // finalize geometry change
    if (m_activeSide) {
        //if (symmetryMod) {
        //    QPoint deltaTopLeft = newTopLeft - geom.topLeft();
        //    QPoint deltaBottomRight = newBottomRight - geom.bottomRight();
        //    newTopLeft = geom.topLeft() + deltaTopLeft - deltaBottomRight;
        //    newBottomRight =
        //        geom.bottomRight() + deltaBottomRight - deltaTopLeft;
        //}
        geom = { newTopLeft, newBottomRight };
        setGeometry(geom.normalized());
        //setGeometry(geom);
        //m_activeSide = getProperSide(m_activeSide, geom);

        //qDebug() << "resize : x->" << geom.x() << " y->" << geom.y() << " width : " << geom.width() << " height : " << geom.height();
    }
    //m_dragStartPos = pos;
}

// helper function
ResizeableWidget::SideType ResizeableWidget::getProperSide(ResizeableWidget::SideType side, const QRect& r)
{
    using SideType = ResizeableWidget::SideType;
    int intSide = side;
    if (r.right() < r.left()) {
        intSide ^= SideType::LEFT_SIDE;
        intSide ^= SideType::RIGHT_SIDE;
    }
    if (r.bottom() < r.top()) {
        intSide ^= SideType::TOP_SIDE;
        intSide ^= SideType::BOTTOM_SIDE;
    }

    return (SideType)intSide;
}

void ResizeableWidget::setGeometry(const QRect& r)
{
    QWidget::setGeometry(r + QMargins(MARGIN, MARGIN, MARGIN, MARGIN));
    //updateCursor(e->pos());
    //if (isVisible()) {
    //    //emit geometryChanged();
    //}
}

QRect ResizeableWidget::geometry() const
{
    return QWidget::geometry() - QMargins(MARGIN, MARGIN, MARGIN, MARGIN);
}

QRect ResizeableWidget::rect() const
{
    QRect re = QWidget::rect();
    re = re - QMargins(MARGIN, MARGIN, MARGIN, MARGIN);
    return QWidget::rect() - QMargins(MARGIN, MARGIN, MARGIN, MARGIN);
}

void ResizeableWidget::setParentWidget(QWidget* wid)
{
    this->setParent(wid);
    wid->installEventFilter(this);
}

bool ResizeableWidget::eventFilter(QObject* obj, QEvent* event)
{
    if (m_ignoreMouse && dynamic_cast<QMouseEvent*>(event)) {
        m_activeSide = NO_SIDE;
        unsetCursor();
    }
    else if (event->type() == QEvent::MouseButtonRelease) {
        parentMouseReleaseEvent(static_cast<QMouseEvent*>(event));
    }
    else if (event->type() == QEvent::MouseButtonPress) {
        parentMousePressEvent(static_cast<QMouseEvent*>(event));
    }
    else if (event->type() == QEvent::MouseMove) {
        parentMouseMoveEvent(static_cast<QMouseEvent*>(event));
    }
    return false;
}


void ResizeableWidget::parentMousePressEvent(QMouseEvent* e)
{
    if (e->button() != Qt::LeftButton) {
        return;
    }

    m_dragStartPos = e->pos();
    m_activeSide = getMouseSide(e->pos());
}

void ResizeableWidget::parentMouseReleaseEvent(QMouseEvent* e)
{
    // released outside of the selection area
    if (!getMouseSide(e->pos())) {
        //hide();
    }

    m_activeSide = NO_SIDE;
    //updateCursor(e->pos());
    //emit geometrySettled();
}

void ResizeableWidget::parentMouseMoveEvent(QMouseEvent* e)
{
    //updateCursor(e->pos());

    if (e->buttons() != Qt::LeftButton) {
        return;
    }

    SideType mouseSide = m_activeSide;
    if (!m_activeSide) {
        mouseSide = getMouseSide(e->pos());
    }

    //if (!isVisible() || !mouseSide) {
    //    show();
    //    m_dragStartPos = e->pos();
    //    m_activeSide = TOPLEFT_SIDE;
    //    setGeometry({ e->pos(), e->pos() });
    //}

    QPoint pos = e->pos();
    auto geom = geometry();
    //bool symmetryMod = qApp->keyboardModifiers() & Qt::ShiftModifier;

    QPoint newTopLeft = geom.topLeft(), newBottomRight = geom.bottomRight();
    int& newLeft = newTopLeft.rx(), & newRight = newBottomRight.rx(),
        & newTop = newTopLeft.ry(), & newBottom = newBottomRight.ry();
    switch (mouseSide) {
    case TOPLEFT_SIDE:
        if (m_activeSide) {
            newTopLeft = pos;
        }
        break;
    case BOTTOMRIGHT_SIDE:
        if (m_activeSide) {
            newBottomRight = pos;
        }
        break;
    case TOPRIGHT_SIDE:
        if (m_activeSide) {
            newTop = pos.y();
            newRight = pos.x();
        }
        break;
    case BOTTOMLEFT_SIDE:
        if (m_activeSide) {
            newBottom = pos.y();
            newLeft = pos.x();
        }
        break;
    case LEFT_SIDE:
        if (m_activeSide) {
            newLeft = pos.x();
        }
        break;
    case RIGHT_SIDE:
        if (m_activeSide) {
            newRight = pos.x();
        }
        break;
    case TOP_SIDE:
        if (m_activeSide) {
            newTop = pos.y();
        }
        break;
    case BOTTOM_SIDE:
        if (m_activeSide) {
            newBottom = pos.y();
        }
        break;
    default:
        if (m_activeSide) {
            move(this->pos() + pos - m_dragStartPos);
            m_dragStartPos = pos;
        }
        return;
    }
    // finalize geometry change
    if (m_activeSide) {
        //if (symmetryMod) {
        //    QPoint deltaTopLeft = newTopLeft - geom.topLeft();
        //    QPoint deltaBottomRight = newBottomRight - geom.bottomRight();
        //    newTopLeft = geom.topLeft() + deltaTopLeft - deltaBottomRight;
        //    newBottomRight =
        //        geom.bottomRight() + deltaBottomRight - deltaTopLeft;
        //}
        geom = { newTopLeft, newBottomRight };
        setGeometry(geom.normalized());
        m_activeSide = getProperSide(m_activeSide, geom);
    }
    m_dragStartPos = pos;
}

void ResizeableWidget::setImageData(const unsigned char* raw_data,int width,int height)
{
    //auto b = image_.loadFromData(raw_data, size);
    //image_ = QImage::fromData(raw_data, size);
    image_ = QImage(raw_data, width, height, QImage::Format::Format_RGBX8888);
    if (first_save_)
    {
        image_.save("out.jpg");
        first_save_ = false;
    }
}