/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#include "AvNetService/AvRtsp.h"
#include "AvCapture/AvManCapture.h"
CAvRtspServerMedia::CAvRtspServerMedia()
{

}
CAvRtspServerMedia::~CAvRtspServerMedia()
{
}

bool CAvRtspServerMedia::StartMedia()
{
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(0);
	pCapture->Start(CHL_MAIN_T, this, (Capture::SIG_PROC_ONDATA)&CAvRtspServerMedia::OnStream);
	return true;
}
bool CAvRtspServerMedia::StopMedia()
{
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(0);
	pCapture->Stop(CHL_MAIN_T, this, (Capture::SIG_PROC_ONDATA)&CAvRtspServerMedia::OnStream);
	return true;
}


av_void CAvRtspServerMedia::OnStream(av_int Channel, av_int Slave, CAvPacket *AvPacket)
{
	switch (AvPacket->Comp())
	{
	case AvComp_AAC:
		break;
	case AvComp_G711A:
		break;
	case AvComp_MJPEG:
		break;
	case AvComp_H265:
		PushVideoStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_H265, (const char *)AvPacket->GetRawBuffer(), AvPacket->GetRawLength());
		break;
	case AvComp_H264:
		PushVideoStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_H264, (const char *)AvPacket->GetRawBuffer(), AvPacket->GetRawLength());
		break;
	default:
		break;
	}

}



SINGLETON_IMPLEMENT(CAvRtspServer)

CAvRtspServer::CAvRtspServer()
{
}

CAvRtspServer::~CAvRtspServer()
{
}



static CRtspMedia *createMedia(std::string url)
{
	CAvRtspServerMedia *SerMedia = NULL;
	SerMedia = new CAvRtspServerMedia;
	return SerMedia;
}

static bool destoryMedia(CRtspMedia *media)
{
	delete media;
	return true;
}
av_bool CAvRtspServer::Start(int ServicePort)
{
	SetCreateMediaProc(createMedia);
	SetDestoryMediaProc(destoryMedia);
	RtspStart(ServicePort);
	return av_true;
}
av_bool CAvRtspServer::Stop()
{
	return av_true;
}

