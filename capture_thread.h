#pragma once
#include "XThread.h"
#include "capturer.h"

#include <mutex>
struct AVFrame;
class CaptureThread : public XThread
{
public:
	void Task()override;
	void do_next(AVFrame* frame)override;
	void set_next(XThread* thread)override;

	bool Init();
	bool InitScale(int oWidth, int oHeight);

	AVFrame* GetFrame();
	int getInputWidth();
	int getInputHeight();
	void setOutputSize(int width, int height);
private:
	bool is_need_play_ = false;
	Capture capture_;
	XThread* next_ = nullptr;
	std::mutex mtx_;

	AVFrame* frame_ = nullptr;
};

