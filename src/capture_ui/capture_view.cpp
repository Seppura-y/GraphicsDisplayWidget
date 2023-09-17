#include "capture_view.h"
#include <QGraphicsScene>

CaptureView::CaptureView(QGraphicsScene* scene, QWidget* widget) : QGraphicsView(scene,widget)
{
	//setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void CaptureView::wheelEvent(QWheelEvent* ev)
{
	Q_UNUSED(ev);
}