#pragma once
#include <QWidget>
#include <list>

class CaptureView;
class ResizeableWidget;
class QGraphicsScene;

class CaptureWidget : public QWidget
{
public:
	CaptureWidget(QWidget* parent = nullptr);

	//void setViewResolution(int width, int height);
	//void addCaptureItem(ResizeableWidget* item);
signals:

protected slots:
	inline void onSetCursor(Qt::CursorShape type) { this->setCursor(type); }
protected:

private:

private:
	QGraphicsScene* m_scene_ = nullptr;
	CaptureView* m_cap_view_ = nullptr;
	std::list<ResizeableWidget*> m_item_list_;
};

