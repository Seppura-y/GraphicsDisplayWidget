#include "XTools.h"
extern"C"
{
#include <libavcodec/avcodec.h>
}
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avutil.lib")
using namespace std;
XParam* XParam::Create()
{
	XParam* para = new XParam();
	return para;
}
XParam::~XParam()
{
	if (para)
	{
		avcodec_parameters_free(&para);
	}
	if (time_base)
	{
		delete time_base;
		time_base = nullptr;
	}
}

XParam::XParam()
{
	para = avcodec_parameters_alloc();
	time_base = new AVRational();
}


void XAVPacketList::Push(AVPacket* packet)
{
	//AVPacket* pkt;
	//av_packet_ref(pkt, packet);

	//unique_lock<mutex> lock(mtx_);
	//if (!pkt_list_.empty())
	//{
	//	if (pkt_list_.size() > max_list_)
	//	{
	//		AVPacket* p = pkt_list_.front();
	//		if (p->flags == AV_PKT_FLAG_KEY)
	//		{
	//			av_packet_free(&p);
	//			pkt_list_.pop_front();
	//			pkt_list_.push_back(pkt);
	//			return;
	//		}
	//		while (1)
	//		{
	//			p = pkt_list_.front();
	//			if (p->flags == AV_PKT_FLAG_KEY)
	//			{
	//				return;
	//			}
	//			av_packet_free(&p);
	//			pkt_list_.pop_front();
	//		}
	//		pkt_list_.push_back(pkt);
	//	}
	//	pkt_list_.push_back(pkt);
	//}
	unique_lock<mutex> lock(mtx_);
	AVPacket* pkt = av_packet_alloc();
	av_packet_ref(pkt, packet);
	pkt_list_.push_back(pkt);
	if (pkt_list_.size() > max_list_)
	{
		if (pkt_list_.front()->flags & AV_PKT_FLAG_KEY)
		{
			av_packet_free(&pkt_list_.front());
			pkt_list_.pop_front();
			return;
		}
		while (!pkt_list_.empty())
		{
			if (pkt_list_.front()->flags & AV_PKT_FLAG_KEY)
			{
				return;
			}
			av_packet_free(&pkt_list_.front());
			pkt_list_.pop_front();
		}
	}
}

AVPacket* XAVPacketList::Pop()
{
	unique_lock<mutex> lock(mtx_);
	if (pkt_list_.empty())
	{
		return nullptr;
	}
	AVPacket* pkt = pkt_list_.front();
	pkt_list_.pop_front();
	return pkt;
}

void XAVPacketList::Clear()
{
	unique_lock<mutex> lock(mtx_);
	while (!pkt_list_.empty())
	{
		pkt_list_.pop_front();
	}
	return;
}

void XFreeFrame(AVFrame** frame)
{
	if (!frame)return;
	av_frame_free(frame);
}

void XAVFrameList::Push(AVFrame* frm)
{
	{
		unique_lock<mutex> lock(mtx_);
		if (frm_list_.size() > max_list_)
		{
			av_frame_free(&frm_list_.front());
			frm_list_.pop_front();
		}
		frm_list_.push_back(frm);
	}


	//AVFrame* frame = av_frame_alloc();
	//av_frame_ref(frame, frm);



}

AVFrame* XAVFrameList::Pop()
{
	unique_lock<mutex> lock(mtx_);
	if (frm_list_.empty())
	{
		return nullptr;
	}
	AVFrame* frame = frm_list_.front();
	frm_list_.pop_front();
	return frame;
}

void XAVFrameList::Clear()
{
	unique_lock<mutex> lock(mtx_);
	if (!frm_list_.empty())
	{
		frm_list_.pop_front();
	}
}