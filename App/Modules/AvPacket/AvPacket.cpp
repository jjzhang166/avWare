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
#include "AvPacket/AvPacket.h"


CPacket::CPacket()
{
	m_BDataNew = av_false;
	Reset();
}
CPacket::~CPacket()
{
	Reset();
}
CPacket::CPacket(const CPacket &packet)
{
	Reset();	
	FillInData(packet.m_Data, packet.m_DataLen);
}
CPacket& CPacket::operator=(const CPacket &packet)
{
	if (this == &packet){
		return *this;
	}

	Reset();
	FillInData(packet.m_Data, packet.m_DataLen);
	return *this;
}
av_bool CPacket::LoadData(char *data, int len)
{
	Reset();
	m_BDataNew = av_false;
	m_Data = data;
	m_DataLen = len;
	SplitHead();
	return av_true;
}

av_bool CPacket::FillInData(char *data, int len)
{
	if (m_BDataNew == av_true){
		if (m_DataSize < (av_u32)len){
			m_Data = (char *)realloc(m_Data, sizeof(char)*len);
			m_DataLen = sizeof(char)*len;
		}
	}
	else{
		m_BDataNew = av_true;
		m_Data = (char *)malloc(sizeof(char)*len);
		m_DataSize = sizeof(char)*len;
	}
	memcpy(m_Data, data, len);
	m_DataLen = len;
	SplitHead();
	return av_true;
}

inline av_bool CPacket::UnLoadData()
{
	Reset();
	return av_true;
}


inline av_void CPacket::Reset()
{
	m_TimeStamp = 0;
	m_ImageWidth = 0;
	m_ImageHeigh = 0;
	m_Channel = 0;
	m_Slave = 0;
	m_CompFormat = AvComp_NR;
	m_IsVideo = av_false;
	m_Frameformat = avFrameT_B;
	m_FrameRate = 0;

	if (av_true == m_BDataNew){
		free(m_Data);
	}

	m_BDataNew = av_false;
	m_DataSize = 0;

	m_Data = NULL;
	m_DataLen = 0;

	m_RawData = NULL;
	m_RawLen = 0;

	m_HeadData = NULL;
	m_HeadLen = 0;
}
#if 0
typedef struct {
	av_u16			SizeHight;
	av_u16			SizeWidth;
	av_u16			Sec;
	av_u16			Msec;
	av_uchar		Channel;
	av_uchar		Slave : 4;
	av_char			FrameRate : 4;
	av_uchar		Comp : 4;
	av_uchar		FrameType : 4;
}C_FormatStreamHead_CPacket;
#endif


inline av_bool CPacket::SplitHead()
{
	C_FormatStreamHead *pPacketHead = (C_FormatStreamHead *)m_Data;
	if (pPacketHead->sync != D_StreamSync_Code){
		return av_false;
	}
	m_ImageHeigh = pPacketHead->SizeHight;
	m_ImageWidth = pPacketHead->SizeWidth;
	m_Channel = pPacketHead->Channel;
	m_Slave = pPacketHead->Slave;
	m_CompFormat = (av_comp_t)pPacketHead->Comp;
	m_FrameRate = pPacketHead->FrameRate;
	m_Frameformat = (av_frame_type)pPacketHead->FrameType;
	m_TimeStamp = pPacketHead->pts;
	avFrameT_Audio == m_Frameformat ? m_IsVideo = av_false : m_IsVideo = av_true;
	m_HeadData = m_Data;
	m_HeadLen = sizeof(C_FormatStreamHead);

	m_RawData = m_Data + m_HeadLen;
	m_RawLen = pPacketHead->Length;

#if 1
	assert(m_RawLen == (m_DataLen - m_HeadLen));
#else
	if (m_RawLen != (m_DataLen - m_HeadLen)){
		av_error("Datalen= %d, headlen = %d, rawlen = %d\n", m_DataLen, m_HeadLen, m_RawLen);
		av_error("ch = %d, slave = %d, isvideo = %d\n", m_Channel, m_Slave, IsVideo());
	}
#endif
	av_bool ret = av_false;
	if (m_ImageWidth % 2 != 0){
		goto return_point;
	}
	if (m_ImageHeigh % 2 != 0){
		goto return_point;
	}
	if (m_CompFormat >= AvComp_NR){
		goto return_point;
	}
	if (m_FrameRate > 120){
		goto return_point;
	}
	if (m_Frameformat >= avFrameT_NR){
		goto return_point;
	}
	ret = av_true;
return_point:
	return ret;
}