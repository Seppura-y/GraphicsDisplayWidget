//#include "qtcamera.h"
#include "ResizeableTopWidget.h"
#include "ResizeableWidget.h"
#include "GraphicsDisplayWidget.h"
#include "canvas_widget.h"

//#include <QtGui/QApplication>
#include <QtWidgets/QApplication>



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//QtCamera w;
	//w.show();

	//ResizeableTopWidget resize_top_wid();
	//resize_top_wid.show();

	CanvasWidget canvas_wid;
	canvas_wid.show();

	//GraphicsDisplayWidget display_wid;
	//display_wid.show();
	return a.exec();
}
