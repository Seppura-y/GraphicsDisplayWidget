#pragma once
#include <QGraphicsView>
class CaptureView : public QGraphicsView
{
public:
	CaptureView(QGraphicsScene* scene, QWidget* widget);

signals:

protected slots:


protected:
	//void mouseMoveEvent(QMouseEvent* ev) override;
	//void mousePressEvent(QMouseEvent* ev) override;
	//void mouseReleaseEvent(QMouseEvent* ev) override;
	void wheelEvent(QWheelEvent* ev) override;
private:

};

