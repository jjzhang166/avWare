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
#ifndef _AVPACKET_H_
#define _AVPACKET_H_
#include "Apis/AvWareType.h"
#include "Apis/LibEncode.h"


class CPacket
{
public:
	CPacket();
	~CPacket();
	CPacket(const CPacket &packet);
public:
	CPacket& operator=(const CPacket &packet);

public:
	//指针赋值
	av_bool LoadData(char *data, int len);
	//数据搬移
	av_bool FillInData(char *data, int len);

	inline av_bool UnLoadData();
public:
	inline av_char* GetData()				{ return m_Data; }
	inline av_u32   GetDataLen()			{ return m_DataLen; }
	
	//去头
	inline av_char* GetRawData()			{return		m_RawData;	}

	inline av_u32   GetRawDataLen()			{return		m_RawLen;	}


public:
	//包解析
	inline av_u16			GetImageWidth()	{ return m_ImageWidth; }
	inline av_u16			GetImageHeigh()	{ return m_ImageHeigh; }

	inline av_u64			GetTimeStamp()	{ return m_TimeStamp; }
	inline av_uchar			GetChannel()	{ return m_Channel; }
	inline av_uchar			GetSlave()		{ return m_Slave; }
	inline av_comp_t		GetCompFormat()	{ return m_CompFormat;}
	inline av_bool			IsVideo()		{ return avFrameT_Audio == m_Frameformat ? av_false : av_true;}
	inline av_frame_type	GetFrameFormat(){ return m_Frameformat; }
	inline av_uchar			GetFrameRate()	{ return m_FrameRate; }

private:
	inline av_void			Reset();
	inline av_bool			SplitHead();
private:
	av_u64		m_TimeStamp;
	av_u16		m_ImageWidth;
	av_u16		m_ImageHeigh;
	av_uchar	m_Channel;
	av_uchar	m_Slave;
	av_comp_t   m_CompFormat;
	av_bool		m_IsVideo;
	av_frame_type m_Frameformat;
	av_uchar	m_FrameRate;

private:
	av_bool m_BDataNew;
	av_u32  m_DataSize;
	
	av_char *m_Data;
	av_u32  m_DataLen;

	av_char *m_RawData;
	av_u32  m_RawLen;
	
	av_char *m_HeadData;
	av_u32  m_HeadLen;

};











#endif
