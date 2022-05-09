#include "GraphicsDisplayWidget.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GraphicsDisplayWidget w;
    w.show();
    return a.exec();
}
