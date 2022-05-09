#include "capture_thread.h"
extern"C"
{
	#include "libavcodec/avcodec.h"
}

#pragma comment(lib,"avcodec.lib")

using namespace std;

static long long NowMs()
{
	return (clock() / (CLOCKS_PER_SEC / 1000));
}

void CaptureThread::Task()
{
	//unique_lock<mutex> lock(mtx_);
	while (!isExit_)
	{
		{
			unique_lock<mutex> lock(mtx_);
			if (frame_)
			{
				//av_frame_free(&frame_);
				av_frame_unref(frame_);
				frame_ = nullptr;
			}
		}
		frame_ = capture_.GetCapturedFrame();
		if (!frame_)
		{
			this_thread::sleep_for(1ms);
			continue;
		}
		//long long now_sec = NowMs() * 1000;
		//frame_->pts = now_sec;

		is_need_play_ = true;
		AVFrame* frame = av_frame_alloc();
		av_frame_ref(frame, frame_);

		if (!next_)
		{
			this_thread::sleep_for(1ms);
			av_frame_unref(frame);
			continue;
		}
		do_next(frame);
		this_thread::sleep_for(1ms);
	}
	
}
void CaptureThread::do_next(AVFrame* frame)
{
	unique_lock<mutex> lock(mtx_);
	if (next_)
	{
		next_->Do(frame);
	}
}

bool CaptureThread::Init()
{
	if (!capture_.CaptureInit())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CaptureThread::InitScale(int oWidth, int oHeight)
{
	capture_.setOutputSize(oWidth, oHeight);
	if (!capture_.ScaleInit())
	{
		return false;
	}
	else
	{
		return true;
	}
}

AVFrame* CaptureThread::GetFrame()
{
	unique_lock<mutex> lock(mtx_);
	if (!frame_ || !is_need_play_)return nullptr;
	AVFrame* frame = av_frame_alloc();
	int ret = av_frame_ref(frame, frame_);
	if (ret != 0)
	{
		av_frame_free(&frame);
		return nullptr;
	}

	return frame;
}

int CaptureThread::getInputWidth()
{
	return capture_.getInputWidth();
}

int CaptureThread::getInputHeight()
{
	return capture_.getInputHeight();
}

void CaptureThread::setOutputSize(int width, int height)
{
	capture_.setOutputSize(width, height);
}

void CaptureThread::set_next(XThread* thread)
{
	unique_lock<mutex> lock(mtx_);
	next_ = thread;
}