#include "XThread.h"
#include "XTools.h"
#include <iostream>
#include <sstream>

extern"C"
{
#include <libavcodec/avcodec.h>
}

using namespace std;

long long NowMs()
{
	return (clock() / (CLOCKS_PER_SEC / 1000));
}

void MSleep(unsigned int ms)
{
	auto beg = clock();
	for (int i = 0; i < ms; i++)
	{
		this_thread::sleep_for(1ms);
		if ((clock() - beg) / (CLOCKS_PER_SEC / 1000) >= ms)
			break;
	}
}

void XThread::Start()
{
	unique_lock<mutex> lock(mtx_);
	static int i = 0;
	i++;
	thread_index_ = i;
	isExit_ = false;

	th_ = thread(&XThread::Task, this);
	LOGINFO("thread start");
}

void XThread::Stop()
{
	stringstream str;
	str << "thread stop begin : " << thread_index_;
	LOGINFO(str.str());
	isExit_ = true;
	if (th_.joinable())
	{
		th_.join();
	}
	str << " ";
	str << "thread stop end : " << thread_index_;
}


