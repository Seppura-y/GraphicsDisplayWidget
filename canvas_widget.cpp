#include "canvas_widget.h"

#include "canvas_view.h"

CanvasWidget::CanvasWidget(QWidget* parent) : QMainWindow(parent)
{
	setWindowTitle("CanvasWidget");
	//setSize(1280, 720);
	setMinimumSize(1280, 720);

	view_ = new CanvasView(this,this);
	view_->setGeometry(0, 0, this->width(), this->height());
	view_->show();

	//view_->addItem(QPointF(150, 100));
}

void CanvasWidget::resizeEvent(QResizeEvent * ev)
{
	view_->setGeometry(0, 0, this->width(), this->height());
	QWidget::resizeEvent(ev);
}