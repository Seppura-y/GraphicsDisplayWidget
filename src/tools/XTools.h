#pragma once
#include <iostream>
#include <list>
#include <mutex>

struct AVFrame;
struct AVPacket;
struct AVCodecParameters;
struct AVRational;
enum LogLevel
{
	Log_Info,
	Log_Debug,
	Log_Error,
	Log_Fatal
};
#define MIN_LOG_LEVEL Log_Info
#define LOG(s,level) if(level >= Log_Info)\
std::cout<<"level_" << level<<":"<<"file : "<<__FILE__<<" "<<"line : "<<__LINE__<<": "\
<<s<<"\n"<<std::endl;

#define LOGINFO(s)     LOG(s,Log_Info);
#define LOGDEBUG(s)  LOG(s,Log_Debug);
#define LOGERROR(s)   LOG(s,Log_Error);
#define LOGFATAL(s)	LOG(s,Log_Fatal);

long long NowMs();
void MSleep(unsigned int ms);
void XFreeFrame(AVFrame** frame);


class XTools
{
};


class XParam
{
public:
	AVCodecParameters* para = nullptr;
	AVRational* time_base = nullptr;
	static XParam* Create();
	~XParam();
private:
	XParam();
};

class XAVPacketList
{
public:

	void Push(AVPacket* packet);
	AVPacket* Pop();
	void Clear();
private:
	std::list<AVPacket*> pkt_list_;
	int max_list_ = 100;
	std::mutex mtx_;
};

class XAVFrameList
{
public:
	void Push(AVFrame* frame);
	AVFrame* Pop();
	void Clear();

private:
	std::list<AVFrame*> frm_list_;
	int max_list_ = 100;
	std::mutex mtx_;
};
