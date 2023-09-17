#pragma once
#include <thread>
#include <mutex>
#include <list>

struct AVPacket;
struct AVCodecParameters;
struct AVRational;
struct AVFrame;

class XThread
{
public:
	virtual void Start();
	virtual void Stop();
	virtual void Task() = 0;

	//responsibility chain
	virtual void Do(AVPacket* packet) {};
	virtual void Do(AVFrame* frame) {};

	virtual void set_next(XThread* thread)
	{
		std::unique_lock<std::mutex> lock(mtx_);
		next_ = thread;
	}

	virtual void do_next(AVPacket* packet)
	{
		std::unique_lock<std::mutex> lock(mtx_);
		if (next_)
		{
			next_->Do(packet);
		}
	}
	virtual void do_next(AVFrame* frame)
	{
		std::unique_lock<std::mutex> lock(mtx_);
		if (next_)
		{
			next_->Do(frame);
		}
	}

protected:
	bool isExit_ = true;
	int thread_index_ = 0;
private:
	std::mutex mtx_;
	std::thread th_;
	XThread* next_ = nullptr;
};
