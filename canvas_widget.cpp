#include "canvas_widget.h"

#include "canvas_view.h"

#include <iostream>

#include <QMouseEvent>

using namespace std;

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

void CanvasWidget::mousePressEvent(QMouseEvent* ev)
{
	cout << "canvas widget " << ev->pos().x() << " : " << ev->pos().y() << endl;
	//QWidget::mousePressEvent(ev);
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent* ev)
{
	cout << "canvas widget mouse release" << endl;
	QWidget::mouseReleaseEvent(ev);
}