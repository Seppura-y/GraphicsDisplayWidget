#include "ResizeableTopWidget.h"

#include <QPainter>
#include <QMouseEvent>
#include <QPen>
#include <QBrush>

#define MARGIN (m_THandle.width())

ResizeableTopWidget::ResizeableTopWidget(QColor c, QWidget* parent)
    : QWidget(parent)
    , m_color(std::move(c))
    , m_activeSide(NO_SIDE)
    , m_ignoreMouse(false)
{
    // prevents this widget from consuming CaptureToolButton mouse events
    //setAttribute(Qt::WA_TransparentForMouseEvents);
    //parent->installEventFilter(this);

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
}

ResizeableTopWidget::SideType ResizeableTopWidget::getMouseSide(const QPoint& mousePos) const
{
    if (!isVisible()) {
        return NO_SIDE;
    }
    QPoint localPos = mapFromParent(mousePos);
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

QVector<QRect> ResizeableTopWidget::handlerAreas()
{
    QVector<QRect> areas;
    areas << m_TLHandle << m_TRHandle << m_BLHandle << m_BRHandle << m_LHandle
        << m_THandle << m_RHandle << m_BHandle;
    return areas;
}

void ResizeableTopWidget::updateAreas()
{
    QRect r = rect();
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

void ResizeableTopWidget::updateCursor()
{
    SideType mouseSide = m_activeSide;
    if (!m_activeSide) {
        mouseSide = getMouseSide(parentWidget()->mapFromGlobal(QCursor::pos()));
    }

    switch (mouseSide) {
    case TOPLEFT_SIDE:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case BOTTOMRIGHT_SIDE:
        setCursor(Qt::SizeFDiagCursor);
        break;
    case TOPRIGHT_SIDE:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case BOTTOMLEFT_SIDE:
        setCursor(Qt::SizeBDiagCursor);
        break;
    case LEFT_SIDE:
        setCursor(Qt::SizeHorCursor);
        break;
    case RIGHT_SIDE:
        setCursor(Qt::SizeHorCursor);
        break;
    case TOP_SIDE:
        setCursor(Qt::SizeVerCursor);
        break;
    case BOTTOM_SIDE:
        setCursor(Qt::SizeVerCursor);
        break;
    default:
        if (m_activeSide) {
            setCursor(Qt::ClosedHandCursor);
        }
        else {
            setCursor(m_idleCentralCursor);
            return;
        }
        break;
    }
}

void ResizeableTopWidget::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    //p.setPen(m_color);
    QPen pen = p.pen();
    pen.setStyle(Qt::DashLine);
    pen.setColor(m_color);
    p.setPen(pen);
    p.drawRect(rect() + QMargins(0, 0, -1, -1));
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(m_color);
    for (auto rect : handlerAreas()) {
        //p.drawEllipse(rect.center(), 8, 8);
    }

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
        //p.drawEllipse(rect.center().x(), rect.center().y(), 15, 15);

        pen.setColor("orange");
        brush.setColor("orange");

        p.setPen(pen);
        p.setBrush(brush);

        p.drawEllipse(rect.center(), 4, 4);
    }
}

void ResizeableTopWidget::resizeEvent(QResizeEvent*)
{
    updateAreas();
    //if (isVisible()) {
    //    emit geometryChanged();
    //}
}

void ResizeableTopWidget::moveEvent(QMoveEvent*)
{
    updateAreas();
    //if (isVisible()) {
    //    emit geometryChanged();
    //}
}

void ResizeableTopWidget::mousePressEvent(QMouseEvent* e)
{
    if (e->button() != Qt::LeftButton) {
        return;
    }

    //m_dragStartPos = e->pos();
    //m_activeSide = getMouseSide(e->pos());

    m_dragStartPos = parentWidget()->mapFromGlobal(QCursor::pos());
    m_activeSide = getMouseSide(parentWidget()->mapFromGlobal(QCursor::pos()));
}

void ResizeableTopWidget::mouseReleaseEvent(QMouseEvent* e)
{
    // released outside of the selection area
    //if (!getMouseSide(e->pos())) {
    //    //hide();
    //}

    m_activeSide = NO_SIDE;
    updateCursor();
    //emit geometrySettled();
}

void ResizeableTopWidget::mouseMoveEvent(QMouseEvent* e)
{
    updateCursor();

    if (e->buttons() != Qt::LeftButton) {
        return;
    }


    SideType mouseSide = m_activeSide;
    if (!m_activeSide) {
        //mouseSide = getMouseSide(e->pos());
        mouseSide = getMouseSide(parentWidget()->mapFromGlobal(QCursor::pos()));
    }

    //if (!isVisible() || !mouseSide) {
    //    show();
    //    m_dragStartPos = e->pos();
    //    m_activeSide = TOPLEFT_SIDE;
    //    setGeometry({ e->pos(), e->pos() });
    //}

    //QPoint pos = e->pos();
    QPoint pos = parentWidget()->mapFromGlobal(QCursor::pos());
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
        //setGeometry(geom);
        m_activeSide = getProperSide(m_activeSide, geom);
    }
    m_dragStartPos = pos;
}

// helper function
ResizeableTopWidget::SideType ResizeableTopWidget::getProperSide(ResizeableTopWidget::SideType side, const QRect& r)
{
    using SideType = ResizeableTopWidget::SideType;
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

void ResizeableTopWidget::setGeometry(const QRect& r)
{
    QWidget::setGeometry(r + QMargins(MARGIN, MARGIN, MARGIN, MARGIN));
    updateCursor();
    //if (isVisible()) {
    //    //emit geometryChanged();
    //}
}

QRect ResizeableTopWidget::geometry() const
{
    return QWidget::geometry() - QMargins(MARGIN, MARGIN, MARGIN, MARGIN);
}

QRect ResizeableTopWidget::rect() const
{
    return QWidget::rect() - QMargins(MARGIN, MARGIN, MARGIN, MARGIN);
}