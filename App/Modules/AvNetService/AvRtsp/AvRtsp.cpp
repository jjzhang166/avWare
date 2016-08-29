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
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(m_Channel);
	pCapture->Start(m_Slave, this, (Capture::SIG_PROC_ONDATA)&CAvRtspServerMedia::OnStream);
	return true;
}
bool CAvRtspServerMedia::StopMedia()
{
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(m_Channel);
	pCapture->Stop(m_Slave, this, (Capture::SIG_PROC_ONDATA)&CAvRtspServerMedia::OnStream);
	return true;
}


av_void CAvRtspServerMedia::OnStream(av_int Channel, av_int Slave, CAvPacket *AvPacket)
{
	
#if 1
	PushVideoStreamPacket(AvPacket);
#else
	switch (AvPacket->Comp())
	{
	case AvComp_AAC:
	{	
		static av_u64 LastAudioPts = 0;
		int InterlTime = 0;
		if (LastAudioPts != 0){
			InterlTime = (int)(AvPacket->TimeStamp() - LastAudioPts) / 1000;
			InterlTime *= (AvPacket->SampleRate()/1000);
			InterlTime += 5;
			InterlTime = 0;
		}
		else{

		}
		PushStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_AAC, (const char *)AvPacket->GetRawBuffer(), AvPacket->GetRawLength(), InterlTime);
		LastAudioPts = AvPacket->TimeStamp();
	}

		break;
	case AvComp_G711A:
		PushStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_G711A, (const char *)AvPacket->GetRawBuffer() + 4, AvPacket->GetRawLength() - 4, 0);
		break;
	case AvComp_MJPEG:
		break;
	case AvComp_H265:
		PushStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_H265, (const char *)AvPacket->GetRawBuffer(), AvPacket->GetRawLength(), 0);
		break;
	case AvComp_H264:
		PushStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC_H264, (const char *)AvPacket->GetRawBuffer(), AvPacket->GetRawLength(), 0);
		break;
	default:
		break;
	}
#endif
	

}

av_bool CAvRtspServerMedia::SetMediaInfo(av_u32 Channel, av_u32 Slave)
{
	m_Channel = Channel;
	m_Slave = Slave;
	return av_true;
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
	//rtsp://192.168.1.91:554/c=0&s=1
#define is_num(c) (c>='0' && c<='9')
	int		Channel = 0;
	int		Slave = 0;
	char	str[128] = {0};
	char	temp[12];
	sprintf(str, "%s", url.c_str());
	int len = strlen(str);

	for (int i = 0, j = 0; i < len;){
		if (0 == strncasecmp(&str[i], "c=", 2)){
			i += 2;
			j = 0;
			while (is_num(str[i])){
				temp[j++] = str[i++];
			}
			temp[j] = '\0';
			if (j == 0) {
				av_error("Url Error\n");
				return NULL;
			}

			Channel = atoi(temp);
		} else if (0 == strncasecmp(&str[i], "s=", 2)){
			i += 2;
			j = 0;
			while (is_num(str[i])){
				temp[j++] = str[i++];
			}
			temp[j] = '\0';
			if (j == 0) {
				av_error("Url Error\n");
				return NULL;
			}
			Slave = atoi(temp);
		}
		else{
			i++;
		}
	}

	if (Channel >= g_AvManCapture.GetAvCaptureTotal()){
		return NULL;
	}
	CAvRtspServerMedia *SerMedia = NULL;
	SerMedia = new CAvRtspServerMedia;
	assert(SerMedia != NULL);
	SerMedia->SetMediaInfo(Channel, Slave);
	
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

