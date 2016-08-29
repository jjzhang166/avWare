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

	C_AvMediaHead MediaHead;
	av_u32 length = 0;
	if (av_true == AvMediaHeadParse(m_Buffer, &MediaHead, &length)){
		m_TimeStamp = MediaHead.pts;
		m_Channel = MediaHead.Channel;
		m_Slave = MediaHead.Slave;
		m_MediaPropertyMask = MediaHead.MediaPropertyMask;

		if (MediaHead.StreamType == avStreamT_V){
			m_StreamType = avStreamT_V;
			m_Comp = MediaHead.VHead.comp;
			m_ImageWidth = MediaHead.VHead.ImageWidth;
			m_ImageHeigh = MediaHead.VHead.ImageHeigh;
			m_FrameRate = MediaHead.VHead.FrameRate;
			m_FrameType = MediaHead.VHead.frametype;
	}
		else if (MediaHead.StreamType == avStreamT_A){
			m_StreamType = avStreamT_A;
			m_Comp = MediaHead.AHead.comp;
			m_SampleBits = MediaHead.AHead.sampleRate;
			m_SampleRate = MediaHead.AHead.sampleRate;
	}
		else {
			assert(0);
	}
		m_RawBuffer = m_Buffer + length;
		m_RawBufferLength = MediaHead.PlayLoadLength;
		
		if (length + MediaHead.PlayLoadLength != m_BufferLength){
			assert(0);
	}
	}
	else{
		m_RawBuffer = m_Buffer;
		m_RawBufferLength = m_BufferLength;
	}
	return av_true;
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

av_bool			CAvPacket::GetNaluSplitH264()
{
	CGuard m(m_Mutex);
	if (m_StreamType != avStreamT_V) return av_false;
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
			if (m_RawBuffer[DataPlace + 2] == 0x01){
				DataPlace += 3;
			}
			else{
				DataPlace += 4;
			}
			switch ((m_RawBuffer[DataPlace] & 0x1f)){
			case nal_unit_type_h264_nr:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_nr;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_h264_p:
				m_NaluInfo[m_NaluInfoCount].data	= &m_RawBuffer[DataPlace];
				m_NaluInfo[m_NaluInfoCount].len		= m_RawBufferLength - DataPlace;//数组下标与实际少1
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_p;
				m_NaluInfoCount++;
				m_IsSplitNalu = av_true;
				return av_true;

				break;
			case nal_unit_type_h264_dataA:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_dataA;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_h264_dataB:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_dataB;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_h264_dataC:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_dataC;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_h264_idr:
				m_NaluInfo[m_NaluInfoCount].data		= &m_RawBuffer[DataPlace];
				m_NaluInfo[m_NaluInfoCount].len			= m_RawBufferLength - DataPlace;//数组下标与实际少1
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_idr;
				m_NaluInfoCount++;
				m_IsSplitNalu = av_true;
				return av_true;
				break;
			case nal_unit_type_h264_sei:
				m_NaluInfo[m_NaluInfoCount].data		= &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len			= SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_sei;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_h264_sps:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_sps;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_h264_pps:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_pps;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_h264_delimiter:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_h264_delimiter;
				m_NaluInfoCount++;
				break;
			default:
				av_error("H264 NAL Unit TYPE ERROR %d\n", (m_RawBuffer[DataPlace] & 0x1f));
				break;
			}
		}
	}

	m_IsSplitNalu = av_true;
	return av_true;
}
av_bool			CAvPacket::GetnaluSplitHevc()
{
	CGuard m(m_Mutex);
	if (m_StreamType != avStreamT_V) return av_false;
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
			if (m_RawBuffer[DataPlace + 2] == 0x01){
				DataPlace += 3;
			}
			else{
				DataPlace += 4;
			}

			switch (((m_RawBuffer[DataPlace] >> 1) & 0x3f)){
			case nal_unit_type_hevc_trail_r:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				m_NaluInfo[m_NaluInfoCount].len = m_RawBufferLength - DataPlace;//数组下标与实际少1
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_hevc_trail_r;
				m_NaluInfoCount++;
				m_IsSplitNalu = av_true;
				return av_true;
				break;

			case nal_unit_type_hevc_idr:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				m_NaluInfo[m_NaluInfoCount].len = m_RawBufferLength - DataPlace;//数组下标与实际少1
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_hevc_idr;
				m_NaluInfoCount++;
				m_IsSplitNalu = av_true;
				return av_true;
				break;
			case nal_unit_type_hevc_sei:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_hevc_sei;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_hevc_sps:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_hevc_sps;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_hevc_pps:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_hevc_pps;
				m_NaluInfoCount++;
				break;
			case nal_unit_type_hevc_vps:
				m_NaluInfo[m_NaluInfoCount].data = &m_RawBuffer[DataPlace];
				for (SliceLen = 0; !FindStartPlace(&m_RawBuffer[DataPlace]); DataPlace++, SliceLen++);
				DataPlace--;
				m_NaluInfo[m_NaluInfoCount].len = SliceLen;
				m_NaluInfo[m_NaluInfoCount].type = (av_int)nal_unit_type_hevc_vps;
				m_NaluInfoCount++;
				break;
			default:
				av_error("H265 NAL Unit TYPE ERROR %d\n", ((m_RawBuffer[DataPlace] >> 1) & 0x3f));
				return av_false;
				break;

			}
		}
	}
	m_IsSplitNalu = av_true;
	return av_true;
}

av_bool			CAvPacket::GetNaluSplit()
{
	av_bool ret = av_false;
	if (m_Comp == AvComp_H264){
		ret = GetNaluSplitH264();
	}
	else if (m_Comp == AvComp_H265){
		ret = GetnaluSplitHevc();
	}
	else{
		return av_false;
	}

	return ret;
}
av_u32			CAvPacket::GetNaluCount()
{
	CGuard m(m_Mutex);
	if (m_IsSplitNalu == av_false) return 0;
	return m_NaluInfoCount;
}
av_bool			CAvPacket::GetNaluFrame(av_int naltype, av_uchar *&data, av_32 &datalen)
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
av_bool			CAvPacket::GetNaluFrame(av_32 index, av_int &naltype, av_uchar *&data, av_32 &datalen)
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

	m_SampleRate = 0;
	m_SampleBits = 0;
	m_MediaPropertyMask = 0;
	m_StreamType = avStreamT_Nr;
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

av_stream_type	CAvPacket::StreamType()
{
	return m_StreamType;
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

av_u32			CAvPacket::SampleRate()
{
	return m_SampleRate;
}
av_u32			CAvPacket::SampleBits()
{
	return m_SampleBits;
}

av_u32			CAvPacket::MediaPropertyMask()
{
	return m_MediaPropertyMask;
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
	if (NewLen <= AVPACKET_MAX_FRAME){
		if (NewLen >= (int)((float)AVPACKET_MAX_FRAME*0.8)){
			av_warning("GetAvPacket Length = %d kb\n", NewLen / 1024);
		}
	}
	else{
		av_error("GetAvPacket Length = %d kb\n", NewLen/1024);
		assert(0);
	}
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
	av_msg("Clear up CAvPacketManager time[%d]\n", (av_u32)time(NULL));
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