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
	void mousePressEvent(QMouseEvent* ev) override;
	void mouseReleaseEvent(QMouseEvent* ev) override;
private:
	CanvasView* view_ = nullptr;
};

