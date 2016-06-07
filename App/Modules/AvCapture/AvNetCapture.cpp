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
#include "AvCapture/AvNetCapture.h"

CAvNetCapture::CAvNetCapture()
{

}
CAvNetCapture::~CAvNetCapture()
{

}
av_bool CAvNetCapture::StreamData(av_u16 Channel, av_uchar Slave, CPacket &Packet)
{
	CGuard m(m_MediaMutex);
	CPacket *kPacket = NULL;
	if (m_MediaDeque[Slave].size() > MAX_DEQUE_SIZE){
		kPacket = m_MediaDeque[Slave].front();
		m_MediaDeque[Slave].pop_front();
		delete kPacket;
		kPacket = NULL;
	}

	kPacket = new CPacket(Packet);

	m_MediaDeque[Slave].push_back(kPacket);
	return av_true;
}
av_bool CAvNetCapture::AlarmMsg(CAvAlarm::AlmMsg &Msg)
{
	av_msg("Alarm Recv Msg\n");
	//send alm to sys alm system;
	return av_true;
}

av_bool CAvNetCapture::StreamStart(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc)
{
	int ret = 0;
	ret = m_StreamSignal[Slave].Attach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}
av_bool CAvNetCapture::StreamStop(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc)
{
	int ret = 0;
	ret = m_StreamSignal[Slave].Detach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}