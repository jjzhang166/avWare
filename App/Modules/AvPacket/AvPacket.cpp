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
#include "AvDevice/AvDevice.h"
#if 0
CPacket::CPacket()
{
	m_Ref = 0;
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

av_u32 CPacket::AddRef()
{
	return m_Ref++;
}
av_u32 CPacket::ReleaseRef()
{
	m_Ref--;

	if (m_Ref <= 0){
		delete this;
	}
	else{
		return m_Ref;
	}
	return 0;
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
#endif



CAvPacket::CAvPacket(av_u32 MaxLength)
{
	ReSet();
	m_Buffer = (av_uchar *)malloc(sizeof(av_uchar)*MaxLength);
	m_Size = sizeof(av_uchar)*MaxLength;
	//av_msg("%s\n", __FUNCTION__);
}
CAvPacket::~CAvPacket()
{
	//av_msg("%s\n", __FUNCTION__);
	if (NULL != m_Buffer){
		free(m_Buffer);
		m_Buffer = NULL;
		m_Size = 0;
	}
}

void *CAvPacket::operator new (size_t size)
{
	//av_msg("%s\n", __FUNCTION__);
	return malloc(size);

}
void CAvPacket::operator delete(void *p)
{
	//av_msg("%s\n", __FUNCTION__);
	free(p);
}


av_bool			CAvPacket::PutBuffer(av_uchar *Buffer, const av_u32 Length)
{
	if (m_BufferLength + Length > m_Size){
		av_error("[%s] Memory is not enough\n", __FUNCTION__);
		return av_false;
	}

	memcpy(m_Buffer, Buffer, Length);
	m_BufferLength = Length;
	return av_true;
}
av_bool			CAvPacket::AppendBuffer(av_uchar *Buffer, const av_u32 Length)
{
	if (m_BufferLength + Length > m_Size){
		av_error("[%s] Memory is not enough\n", __FUNCTION__);
		return av_false;
	}

	memcpy(m_Buffer + m_BufferLength, Buffer, Length);
	m_BufferLength += Length;
	return av_true;
}
av_bool	CAvPacket::PutBufferOver()
{
	C_FormatStreamHead *pPacketHead = (C_FormatStreamHead *)m_Buffer;
	if (pPacketHead->sync != D_StreamSync_Code){
		return av_false;
	}
	m_ImageHeigh = pPacketHead->SizeHight;
	m_ImageWidth = pPacketHead->SizeWidth;
	m_Channel = pPacketHead->Channel;
	m_Slave = pPacketHead->Slave;
	m_Comp = (av_comp_t)pPacketHead->Comp;
	m_FrameRate = pPacketHead->FrameRate;
	m_FrameType = (av_frame_type)pPacketHead->FrameType;
	m_TimeStamp = pPacketHead->pts;
	//avFrameT_Audio == m_Frameformat ? m_IsVideo = av_false : m_IsVideo = av_true;
	m_RawBuffer = m_Buffer + sizeof(C_FormatStreamHead);
	m_RawBufferLength = pPacketHead->Length;

#if 1
	assert(m_RawBufferLength == (m_BufferLength - sizeof(C_FormatStreamHead)));
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
	if (m_Comp >= AvComp_NR){
		goto return_point;
	}
	if (m_FrameRate > 120){
		goto return_point;
	}
	if (m_FrameType >= avFrameT_NR){
		goto return_point;
	}
	ret = av_true;
return_point:
	return ret;

}
av_bool			CAvPacket::GetBuffer(av_uchar *Buffer, const av_u32 &Length)
{
	if (Length < m_BufferLength){
		av_error("[%s] Memory is not enough\n", __FUNCTION__);
		return av_false;
	}
	memcpy(Buffer, m_Buffer, m_BufferLength);
	return av_true;
}
const av_uchar *	CAvPacket::GetBuffer()
{
	return m_Buffer;
}
av_u32			CAvPacket::GetLength()
{
	return m_BufferLength;
}

av_bool			CAvPacket::GetRawBuffer(av_uchar *Buffer, const av_u32 &Length)
{
	if (Length < m_RawBufferLength){
		av_error("[%s] Memory is not enough\n", __FUNCTION__);
		return av_false;
	}
	memcpy(Buffer, m_RawBuffer, m_RawBufferLength);
	return av_true;
}
const av_uchar * CAvPacket::GetRawBuffer()
{
	return m_RawBuffer;
}
av_u32			CAvPacket::GetRawLength()
{
	return m_RawBufferLength;
}

av_u32			CAvPacket::GetBufferSize()
{
	return m_Size;
}
av_bool			CAvPacket::IsVideoFrame()
{
	if (m_Comp == AvComp_H264 ||
		m_Comp == AvComp_H265 ||
		m_Comp == AvComp_MJPEG ||
		m_Comp == AvComp_JPEG){
		return av_true;
	}
	return av_false;
}

//#define FindStartPlace(data) ((data)[0]==0x00 && (data)[1] == 0x00 && (data)[2] == 0x00 && (data)[3] == 0x01) 
#define FindStartPlace(data) ((data)[0]==0x00 && (data)[1] == 0x00 && (((data)[2] == 0x01) ||((data)[2] == 0x00 && (data)[3] == 0x01 ))) 

av_bool			CAvPacket::GetNaluSplit()
{
	CGuard m(m_Mutex);
	if (m_IsSplitNalu == av_true) return av_true;

	m_NaluInfoCount = 0;
	memset(m_NaluInfo, 0x00, sizeof(m_NaluInfo));
	{
		unsigned int DataPlace = 0;
		int SliceLen = 0;

		for (DataPlace = 0; DataPlace < m_RawBufferLength - 4; DataPlace++){
			if (!FindStartPlace(&m_RawBuffer[DataPlace])){
				continue;
			}

		//	DataPlace += 4;//让出 00 00 00 01
			if (m_RawBuffer[DataPlace + 2] == 0x01){
				DataPlace += 3;
			}
			else{
				DataPlace += 4;
			}
			switch (0x01 << (m_RawBuffer[DataPlace] & 0x1f)){
			case nal_unit_type_nr:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_nr;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_p:
				m_NaluInfo[m_NaluInfoCount].data	= &m_RawBuffer[DataPlace];
				m_NaluInfo[m_NaluInfoCount].len		= m_RawBufferLength - DataPlace;//数组下标与实际少1
				m_NaluInfo[m_NaluInfoCount].type	= nal_unit_type_p;
				m_NaluInfoCount++;
				m_IsSplitNalu = av_true;
				return av_true;

				break;
			case nal_unit_type_dataA:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_dataA;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_dataB:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_dataB;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_dataC:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_dataC;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_idr:
				m_NaluInfo[m_NaluInfoCount].data		= &m_RawBuffer[DataPlace];
				m_NaluInfo[m_NaluInfoCount].len			= m_RawBufferLength - DataPlace;//数组下标与实际少1
				m_NaluInfo[m_NaluInfoCount].type		= nal_unit_type_idr;
				m_NaluInfoCount++;
				m_IsSplitNalu = av_true;
				return av_true;
				break;
			case nal_unit_type_sei:
				m_NaluInfo[m_NaluInfoCount].data		= &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len			= SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_sei;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_sps:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_sps;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_pps:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_pps;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_delimiter:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = nal_unit_type_delimiter;
				m_NaluInfoCount++;
				break;
			default:
				//printf ("why default have no nal_unit_type???\n");
				break;
			}
		}
	}

	m_IsSplitNalu = av_true;
	return av_true;
}
av_u32			CAvPacket::GetNaluCount()
{
	CGuard m(m_Mutex);
	if (m_IsSplitNalu == av_false) return 0;
	return m_NaluInfoCount;
}
av_bool			CAvPacket::GetNaluFrame(av_nal_unit_type naltype, av_uchar *&data, av_32 &datalen)
{
	CGuard m(m_Mutex);
	if (m_IsSplitNalu == av_false) return av_false;

	for (av_32 i = 0; i < (av_32)m_NaluInfoCount; i++){
		if (naltype == m_NaluInfo[i].type){
			data = m_NaluInfo[i].data;
			datalen = m_NaluInfo[i].len;
			return av_true;
		}
	}
	return av_false;
}
av_bool			CAvPacket::GetNaluFrame(av_32 index, av_nal_unit_type &naltype, av_uchar *&data, av_32 &datalen)
{
	CGuard m(m_Mutex);
	if (m_IsSplitNalu == av_false) return av_false;

	if (index >= (av_32)m_NaluInfoCount) {
		return av_false;
	}
	data = m_NaluInfo[index].data;
	datalen = m_NaluInfo[index].len;
	naltype = m_NaluInfo[index].type;
	return av_true;
}

av_bool			CAvPacket::ReSet()
{
	m_TimeStamp = 0;
	m_ImageWidth = 0;
	m_ImageHeigh = 0;
	m_Channel = -1;
	m_Slave = -1;
	m_Comp = AvComp_NR;
	m_FrameType = avFrameT_NR;
	m_FrameRate = 0;

	m_RawBuffer = NULL;
	m_RawBufferLength = 0;

	m_BufferLength = 0;

	m_Refer = 1;

	m_NaluInfoCount = 0;
	m_IsSplitNalu = av_false;

	memset(m_NaluInfo, 0x00, sizeof(m_NaluInfo));

	return av_true;
}
av_u32			CAvPacket::AddRefer()
{
	CGuard m(m_Mutex);
	m_Refer++;
	return m_Refer;
}
av_u32			CAvPacket::Release()
{
	CGuard m(m_Mutex);

	m_Refer--;
	if (m_Refer <= 0){
		g_AvPacketManager.PutAvPacket(this);
	}
	else{

	}
	return m_Refer;
}
av_u32			CAvPacket::Refer()
{
	return m_Refer;
}

av_u64			CAvPacket::TimeStamp()
{
	return m_TimeStamp;
}
av_u32			CAvPacket::ImageWidth()
{
	return m_ImageWidth;
}
av_u32			CAvPacket::ImageHeigh()
{
	return m_ImageHeigh;
}
av_u32			CAvPacket::Channel()
{
	return m_Channel;
}
av_u32			CAvPacket::Slave()
{
	return m_Slave;
}
av_comp_t		CAvPacket::Comp()
{
	return m_Comp;
}
av_frame_type   CAvPacket::FrameType()
{
	return m_FrameType;
}
av_ushort		CAvPacket::FrameRate()
{
	return m_FrameRate;
}



SINGLETON_IMPLEMENT(CAvPacketManager)


CAvPacketManager::CAvPacketManager()
{

}
CAvPacketManager::~CAvPacketManager()
{

}

av_bool CAvPacketManager::Initialize()
{
	for (int i = 0; i < AVPACKET_MAX_FRAME / AVPACKET_UNIT; i++){
		m_AvPacketNodeInfo[i].total = 0;
	}
	SetTimerName(__FUNCTION__);
	StartTimer(30 * 1000, 0, av_true, av_false);
	return av_true;
}

CAvPacket *CAvPacketManager::GetAvPacket(av_u32 NewLen)
{
	CAvPacket *packet = NULL;
	assert(NewLen <= AVPACKET_MAX_FRAME);
	int nodeLen = (NewLen + AVPACKET_UNIT - 1) / AVPACKET_UNIT;
	nodeLen > 0 ? nodeLen--: NULL;
	
	CGuard m(m_AvPacketNodeInfo[nodeLen].mutex);
	if (m_AvPacketNodeInfo[nodeLen].LeftPacket.size() == 0){
		m_AvPacketNodeInfo[nodeLen].total++;
		packet = new CAvPacket((nodeLen + 1)*AVPACKET_UNIT);
	}
	else{
		packet = m_AvPacketNodeInfo[nodeLen].LeftPacket.back();
		m_AvPacketNodeInfo[nodeLen].LeftPacket.pop_back();
	}

	return packet;

}
av_bool CAvPacketManager::PutAvPacket(CAvPacket *Packet)
{
	assert(Packet != NULL);
	int nodeLen = (Packet->m_Size) / AVPACKET_UNIT;
	nodeLen -= 1;
	CGuard m(m_AvPacketNodeInfo[nodeLen].mutex);
	Packet->ReSet();
	m_AvPacketNodeInfo[nodeLen].LeftPacket.push_back(Packet);
	return av_true;
}

av_void CAvPacketManager::OnTime()
{
	av_msg("Clear up CAvPacketManager\n");
	Dump();
	av_u32 NodePacketUse = 0;
	for (int i = 0; i < AVPACKET_MAX_FRAME / AVPACKET_UNIT; i++){
		m_AvPacketNodeInfo[i].mutex.Enter();
		NodePacketUse = m_AvPacketNodeInfo[i].total - m_AvPacketNodeInfo[i].LeftPacket.size();
		m_AvPacketNodeInfo[i].mutex.Leave();
	}
}

av_bool CAvPacketManager::Dump()
{
	av_u32 total = 0;
	av_u32 free = 0;
	av_u32 use = 0;
	puts("\n");
	
	for (int i = 0; i < AVPACKET_MAX_FRAME / AVPACKET_UNIT; i++){
		m_AvPacketNodeInfo[i].mutex.Enter();
		total = m_AvPacketNodeInfo[i].total;
		free = m_AvPacketNodeInfo[i].LeftPacket.size();
		use = total - free;
		m_AvPacketNodeInfo[i].mutex.Leave();
		if (total != 0){
			printf("size [%3d]kb total [%3d] free [%3d] use [%3d] userate [%f]\n", (1 + i)*AVPACKET_UNIT / 1024, total, free, use, (float)((float)use / (float)total));
		}
		
	}
	puts("\n");

	return av_true;
}