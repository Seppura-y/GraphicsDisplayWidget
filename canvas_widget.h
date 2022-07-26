#pragma once
#include <QWidget>
#include <QMainWindow>


class CanvasView;

class CanvasWidget : public QMainWindow
{
	Q_OBJECT

public:
	CanvasWidget(QWidget* parent = nullptr);

protected:
	void resizeEvent(QResizeEvent* ev) override;
private:
	CanvasView* view_ = nullptr;
};

