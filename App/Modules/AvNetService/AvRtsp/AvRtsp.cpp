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
#include "AvCapture/AvCapture.h"
CAvRtspServerMedia::CAvRtspServerMedia()
{

}
CAvRtspServerMedia::~CAvRtspServerMedia()
{
}

bool CAvRtspServerMedia::StartMedia()
{
	CAvCapture *pAvCapture = g_MCapture.GetAvCaptureInstance(0);
	pAvCapture->StreamStart(CHL_MAIN_T, this, (CAvCapture::OnStreamSigNalFunc)&CAvRtspServerMedia::OnStream);
	return true;
}
bool CAvRtspServerMedia::StopMedia()
{
	CAvCapture *pAvCapture = g_MCapture.GetAvCaptureInstance(0);
	pAvCapture->StreamStop(CHL_MAIN_T, this, (CAvCapture::OnStreamSigNalFunc)&CAvRtspServerMedia::OnStream);

	return true;
}


av_void CAvRtspServerMedia::OnStream(av_uchar Channel, av_uchar Slave, CPacket &packet)
{
	switch (packet.GetCompFormat())
	{
	case AvComp_AAC:
		break;
	case AvComp_G711A:
		break;
	case AvComp_MJPEG:
		break;
	case AvComp_H265:
		PushVideoStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_H265, packet.GetRawData(), packet.GetRawDataLen());
		break;
	case AvComp_H264:
		PushVideoStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_H264, packet.GetRawData(), packet.GetRawDataLen());
		break;
	default:
		break;
	}

}



PATTERN_SINGLETON_IMPLEMENT(CAvRtspServer)

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

