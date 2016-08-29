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
#include "AvNetService/AvRtmp.h"
#include "AvCapture/AvManCapture.h"
#include "Rtmp/rtmp.h"


#define RTMP_AUDIO_SAMPLEBITS_NUM(SampleBits, SampleBitsNum)\
	switch(SampleBits)\
				{\
	case 16:\
		SampleBitsNum = 1;\
		break;\
	case 8:\
		SampleBitsNum = 0;\
		break;\
	default:\
		assert(0);\
		break;\
				}


#define RTMP_AUDIO_SAMPLERATE_NUM(SampleRateValue, SampleRateNum)\
		switch (SampleRateValue){\
		case 96000:\
			SampleRateNum = 0;\
			break;\
		case 88200:\
			SampleRateNum = 1;\
			break;\
		case 64000:\
			SampleRateNum = 2;\
			break;\
		case 48000:\
			SampleRateNum = 3;\
			break;\
		case 44100:\
			SampleRateNum = 4;\
			break;\
		case 32000:\
			SampleRateNum = 5;\
			break;\
		case 24000:\
			SampleRateNum = 6;\
			break;\
		case 22050:\
			SampleRateNum = 7;\
			break;\
		case 1600:\
			SampleRateNum = 8;\
			break;\
		default:\
			assert(0);\
			break;\
}

static char * put_byte(char *output, uint8_t nVal)
{
	output[0] = nVal;
	return output + 1;
}
static char * put_be16(char *output, uint16_t nVal)
{
	output[1] = nVal & 0xff;
	output[0] = nVal >> 8;
	return output + 2;
}

static char * put_be24(char *output, uint32_t nVal)
{
	output[2] = nVal & 0xff;
	output[1] = nVal >> 8;
	output[0] = nVal >> 16;
	return output + 3;
}
static char * put_be32(char *output, uint32_t nVal)
{
	output[3] = nVal & 0xff;
	output[2] = nVal >> 8;
	output[1] = nVal >> 16;
	output[0] = nVal >> 24;
	return output + 4;
}
static char * put_be64(char *output, uint64_t nVal)
{
	output = put_be32(output, nVal >> 32);
	output = put_be32(output, (uint32_t)nVal);
	return output;
}
static char * put_amf_string(char *c, const char *str)
{
	uint16_t len = strlen(str);
	c = put_be16(c, len);
	memcpy(c, str, len);
	return c + len;
}
static char * put_amf_double(char *c, double d)
{
	*c++ = AMF_NUMBER; /* type: Number */
	{
		unsigned char *ci, *co;
		ci = (unsigned char *)&d;
		co = (unsigned char *)c;
		co[0] = ci[7];
		co[1] = ci[6];
		co[2] = ci[5];
		co[3] = ci[4];
		co[4] = ci[3];
		co[5] = ci[2];
		co[6] = ci[1];
		co[7] = ci[0];
	}

	return c + 8;
}

AvRtmp::AvRtmp(E_RTMP_MODE mode) :CThread(__FUNCTION__)
{
	m_RtmpHandle = NULL;
	m_RtmpMode = mode;
	m_Channel = 0;
	m_Slave = CHL_MAIN_T;
	m_CacheBuffer = (av_char *)malloc(sizeof(av_char)*AVPACKET_MAX_FRAME);
	m_CacheBufferLen = AVPACKET_MAX_FRAME;
	m_PicWidth	= 0;
	m_PicHeigh  = 0;
	m_nVTimestamp = 0;
	m_nATimestamp = 0;
	m_GetIFrame = av_true;

	m_nVBasePts		= 0;
	m_nABasePts		= 0;

	m_AudioCfgSampleRate = -1;
	m_AudioCfgSampleBits = -1;
	m_AudioComp = AvComp_NR;
	m_AudioChannels = -1;



}
AvRtmp::~AvRtmp()
{
	if (m_RtmpHandle != NULL){
		Stop();
	}

	if (NULL != m_CacheBuffer){
		free(m_CacheBuffer);
		m_CacheBuffer = NULL;
		m_CacheBufferLen = 0;
	}

	CAvPacket *pack = NULL;
	while (m_Avpacket.empty() == false){
		pack = m_Avpacket.front();
		pack->Release();
		m_Avpacket.pop();
	}
}
av_bool AvRtmp::Start(av_uchar Channel, av_uchar Slave, std::string url)
{
	
	m_Channel = Channel;
	m_Slave = Slave;
	
	
	sprintf(m_Url, "%s", url.c_str());
	m_RtmpHandle = RTMP_Alloc();

	RTMP_Init(m_RtmpHandle);

	m_RtmpHandle->Link.timeout = 30;
	
	RTMP_SetupURL(m_RtmpHandle, m_Url);


	
	if (m_RtmpMode == RTMP_PULL){

	}
	else if (m_RtmpMode == RTMP_PUSH){
		RTMP_EnableWrite(m_RtmpHandle);
	}
	else{
		assert(0);
	}
	
	

	CThread::run();

	return av_true;
}

av_bool AvRtmp::Stop()
{
	if (m_RtmpMode == RTMP_PULL){

	}
	else{
		Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(m_Channel);
		pCapture->Stop(m_Slave, this, (Capture::SIG_PROC_ONDATA)&AvRtmp::OnStream);
	}
	CThread::stop(av_true);
	RTMP_DeleteStream(m_RtmpHandle);
	RTMP_Close(m_RtmpHandle);
	RTMP_Free(m_RtmpHandle);
	m_RtmpHandle = NULL;

	return av_true;
}

av_void AvRtmp::OnStream(av_uchar Channel, av_uchar Slave, CAvPacket *AvPacket)
{
	if (m_AudioCfgSampleRate != AvPacket->SampleRate() && AvPacket->StreamType() == avStreamT_A){
		m_AudioCfgSampleRate = AvPacket->SampleRate();
		m_AudioCfgSampleBits = AvPacket->SampleBits();
		m_AudioComp = AvPacket->Comp();
	}
	if ((m_GetIFrame == av_true && AvPacket->FrameType() != avFrameT_I) || m_AudioCfgSampleRate == -1){
		return;
	}
	if (m_Avpacket.size() > 100){
		av_error("%s Cache too mach frame\n", __FUNCTION__);
		m_GetIFrame = av_true;
		return;
	}
	m_GetIFrame = av_false;
	//保证每帧数据是I帧，并且音频已经配置完。
	AvPacket->AddRefer();
	m_Mutex.Enter();
	m_Avpacket.push(AvPacket);
	m_Mutex.Leave();
	return;
}

av_bool AvRtmp::RtmpPullTask()
{
	while (m_Loop == av_true)
	{
		while (m_Loop == av_true)
		{
			//link server;
			//is error , send alarm;
			//is right , break;
			av_msleep(2 * 1000);
		}


		while (m_Loop == av_true)
		{
			//if error , relink, send alarm;

			av_msleep(200);
		}
	}

	return av_true;
}
#define RTMP_HEAD_SIZE   (sizeof(RTMPPacket)+RTMP_MAX_HEADER_SIZE)
av_int AvRtmp::SendMetaPacket(av_comp_t Vcomp, av_int PicWidth, av_int PicHight, av_int FrameRate,
	av_comp_t Acomp, av_int ASampleRate, av_int ABits)
{
	char body[1024] = { 0 };
	char * p = (char *)body;
	int len;
	p = put_byte(p, AMF_STRING);
	p = put_amf_string(p, "@setDataFrame");
	p = put_byte(p, AMF_STRING);
	p = put_amf_string(p, "onMetaData");
	p = put_byte(p, AMF_OBJECT);

	p = put_amf_string(p, "duration");
	p = put_amf_double(p, 0);

	p = put_amf_string(p, "fileSize");
	p = put_amf_double(p, 0);

	p = put_amf_string(p, "copyright");
	p = put_byte(p, AMF_STRING);
	p = put_amf_string(p, "firehood");

#define FLV_CODECID_H264 7
#if 0
	p = put_amf_string(p, "videocodecid");
	p = put_amf_double(p, FLV_CODECID_H264);
#else
	p = put_amf_string(p, "videocodecid");
	p = put_byte(p, AMF_STRING);
	p = put_amf_string(p, "avc1");
#endif
	p = put_amf_string(p, "width");
	p = put_amf_double(p, PicWidth);
	p = put_amf_string(p, "height");
	p = put_amf_double(p, PicHight);
	p = put_amf_string(p, "framerate");
	p = put_amf_double(p, FrameRate);

	p = put_amf_string(p, "encoder");
	p = put_byte(p, AMF_STRING);
	p = put_amf_string(p, "avWare");

	p = put_amf_string(p, "audiocodecid");
	p = put_byte(p, AMF_STRING);
	if (m_AudioComp == AvComp_AAC){
		p = put_amf_string(p, "mp4a");
	}
	else if (m_AudioComp == AvComp_G711A){
		p = put_amf_string(p, "g711a");
	}
	p = put_amf_string(p, "audiodatarate");
	p = put_amf_double(p, 128);

	p = put_amf_string(p, "audiosamplerate");
	p = put_amf_double(p, m_AudioCfgSampleRate);

	p = put_amf_string(p, "audiosamplesize");
	p = put_amf_double(p, m_AudioCfgSampleBits);
	p = put_amf_string(p, "audiochannels");
	p = put_amf_double(p, 1);

	p = put_amf_string(p, "");
	p = put_byte(p, AMF_OBJECT_END);
	len = p - body;
	return SendRtmpData(RTMP_PACKET_TYPE_INFO, (unsigned char*)body, len, NULL, 0);
}

av_int AvRtmp::SendRtmpData(unsigned int nPacketType, unsigned char *predata, unsigned int presize,
	unsigned char *loaddata, unsigned int loaddatasize, unsigned char headerType)
{
	RTMPPacket* packet;
	int nRet = 0;
	packet = (RTMPPacket*)m_CacheBuffer;
	memset(packet, 0, RTMP_HEAD_SIZE);
	RTMPPacket_Reset(packet);
	packet->m_body = (char *)packet + RTMP_HEAD_SIZE;
	packet->m_nBodySize = presize + loaddatasize;
	if (NULL != predata && presize > 0){
		memcpy(packet->m_body, predata, presize);
	}
	if (NULL != loaddata && loaddatasize > 0){
		memcpy(packet->m_body + presize, loaddata, loaddatasize);
	}
	packet->m_hasAbsTimestamp = 0;
	packet->m_packetType = nPacketType; /*此处为类型有两种一种是音频,一种是视频*/
	packet->m_nInfoField2 = m_RtmpHandle->m_stream_id;
	packet->m_nChannel = 0x04;
	packet->m_headerType = headerType;
	if (RTMP_PACKET_TYPE_AUDIO == nPacketType && packet->m_nBodySize != 4)
	{
		packet->m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet->m_nTimeStamp = m_nATimestamp;
	}
	else{
		packet->m_nTimeStamp = m_nVTimestamp;
	}
	

	if (RTMP_IsConnected(m_RtmpHandle))
	{
		nRet = RTMP_SendPacket(m_RtmpHandle, packet, FALSE); /*TRUE为放进发送队列,FALSE是不放进发送队列,直接发送*/
	}
	else {
		nRet = -1;
	}
	return nRet;
}
av_int AvRtmp::SendRtmpPacket(unsigned int nPacketType, av_nal_unit_type_h264 nalType, CAvPacket *AvPacket)
{
	int prelen = 0;
	unsigned char predata[256] = { 0 };

	char *pData = NULL;
	int   DataLen = 0;
	int ret = 0;
	AvPacket->GetNaluFrame(nalType, (av_uchar *&)pData, DataLen);
	switch (nalType)
	{
	case nal_unit_type_h264_idr:
	{

		predata[prelen++] = 0x17;// 1:Iframe  7:AVC   
		predata[prelen++] = 0x01;// AVC NALU   
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;

		predata[prelen++] = (DataLen >> 24) & 0xff;
		predata[prelen++] = (DataLen >> 16) & 0xff;
		predata[prelen++] = (DataLen >> 8)  & 0xff;
		predata[prelen++] = DataLen		  & 0xff;
		//av_msg("Send idr prelen = %d, DataLen = %d\n", prelen, DataLen);
		ret = SendRtmpData(nPacketType, predata, prelen, (unsigned char *)pData, DataLen);
	}
		break;
	case nal_unit_type_h264_p:
	{
		predata[prelen++] = 0x27;// 1:Iframe  7:AVC   
		predata[prelen++] = 0x01;// AVC NALU   
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;

		predata[prelen++] = (DataLen >> 24) & 0xff;
		predata[prelen++] = (DataLen >> 16) & 0xff;
		predata[prelen++] = (DataLen >> 8) & 0xff;
		predata[prelen++] = DataLen & 0xff;
		ret = SendRtmpData(nPacketType, predata, prelen, (unsigned char *)pData, DataLen);
	}
		break;
	case nal_unit_type_h264_pps:
	{
		predata[prelen++] = 0x17;// 1:Iframe  7:AVC   
		predata[prelen++] = 0x01;// AVC NALU   
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;

		predata[prelen++] = (DataLen >> 24) & 0xff;
		predata[prelen++] = (DataLen >> 16) & 0xff;
		predata[prelen++] = (DataLen >> 8) & 0xff;
		predata[prelen++] = DataLen & 0xff;
		ret = SendRtmpData(nPacketType, predata, prelen, (unsigned char *)pData, DataLen);
	}
		break;
	case nal_unit_type_h264_sps:
	{
		char *DataSps, *DataPps;
		int   LenSps, LenPps;
		AvPacket->GetNaluFrame(nal_unit_type_h264_sps, (av_uchar *&)DataSps, LenSps);
		AvPacket->GetNaluFrame(nal_unit_type_h264_pps, (av_uchar *&)DataPps, LenPps);
		assert(LenSps + LenPps + 16 < sizeof(predata));
		predata[prelen++] = 0x17;
		predata[prelen++] = 0x00;

		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;

		predata[prelen++] = 0x01;
		predata[prelen++] = DataSps[1];
		predata[prelen++] = DataSps[2];
		predata[prelen++] = DataSps[3];
		predata[prelen++] = 0xff;


		predata[prelen++] = 0xe1;
		predata[prelen++] = (LenSps >> 8) & 0xff;
		predata[prelen++] = LenSps & 0xff;
		memcpy(&predata[prelen], DataSps, LenSps);
		prelen += LenSps;

		predata[prelen++] = 0x01;
		predata[prelen++] = (LenPps >> 8) & 0xff;
		predata[prelen++] = (LenPps)& 0xff;
		memcpy(&predata[prelen], DataPps, LenPps);
		prelen += LenPps;
		//av_msg("Send sps prelen = %d, DataLen = %d\n", prelen, DataLen);
		ret = SendRtmpData(nPacketType, predata, prelen, NULL, 0, RTMP_PACKET_SIZE_MEDIUM);
	}
		break;
	case nal_unit_type_h264_sei:
	{
		predata[prelen++] = 0x17;// 1:Iframe  7:AVC   
		predata[prelen++] = 0x01;// AVC NALU   
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;
		predata[prelen++] = 0x00;

		predata[prelen++] = (DataLen >> 24) & 0xff;
		predata[prelen++] = (DataLen >> 16) & 0xff;
		predata[prelen++] = (DataLen >> 8) & 0xff;
		predata[prelen++] = DataLen & 0xff;
		//av_msg("Send sei prelen = %d, DataLen = %d\n", prelen, DataLen);
		ret = SendRtmpData(nPacketType, predata, prelen, (unsigned char *)pData, DataLen);
	}
		break;
	default:
		assert(0);
		break;
	}
	return ret;
}
av_int  AvRtmp::PushMediaData(CAvPacket *AvPacket)
{
	char *pData = NULL;
	int   Len = 0;
	int ret = 0;
	av_nal_unit_type_h264 nalutype;

	for (int i = 0; i < (int)AvPacket->GetNaluCount(); i++){
		AvPacket->GetNaluFrame(i, (av_int &)nalutype, (av_uchar *&)pData, Len);
		switch (nalutype)
		{
		case nal_unit_type_h264_idr:
			ret = SendRtmpPacket(RTMP_PACKET_TYPE_VIDEO, nalutype, AvPacket);
			break;
		case nal_unit_type_h264_p:
			ret = SendRtmpPacket(RTMP_PACKET_TYPE_VIDEO, nalutype, AvPacket);
			break;
		case nal_unit_type_h264_sei:
			ret = 1;
			//ret = SendRtmpPacket(RTMP_PACKET_TYPE_VIDEO, nalutype, AvPacket);
			break;
		case nal_unit_type_h264_pps://P帧带PPS  windows intel media sdk , H264
			if (AvPacket->FrameType() != avFrameT_I){
				ret = SendRtmpPacket(RTMP_PACKET_TYPE_VIDEO, nalutype, AvPacket);
			}
			break;
		case nal_unit_type_h264_sps:
			if (m_PicHeigh != AvPacket->ImageHeigh() || m_PicWidth != AvPacket->ImageWidth()){
				m_PicWidth = AvPacket->ImageWidth();
				m_PicHeigh = AvPacket->ImageHeigh();
				av_msg("##################SendMetaPacket\n");
				ret  = SendMetaPacket(AvPacket->Comp(), AvPacket->ImageWidth(), AvPacket->ImageHeigh(),
					AvPacket->FrameRate(), AvComp_G711A, 8000, 16);
				SendAacSpac();
			}
			ret = SendRtmpPacket(RTMP_PACKET_TYPE_VIDEO, nalutype, AvPacket);
			break;

		default:
			ret = 1;
			break;
		}
		if (ret <= 0){
			return -1;
		}
	}
	if (m_nVBasePts != 0){
		m_nVTimestamp = (av_u32)(AvPacket->TimeStamp() - m_nVBasePts)/1000;
	}
	else{
		m_nVBasePts = AvPacket->TimeStamp();
	}

	return 1;
}


av_int AvRtmp::SendAacSpac()
{
	// af 00 11 90   ;;aac    48000 16bits 
	// af 00 12 10   ;;aac    44100 16bits
	// af 00 13 90	 ;;aac    22050 16bits
	// 72 00 04 88   ;;g711-a 8000 16bits

//	unsigned char preData[32];
	int  preLen = 0;
	int SampleRate = 0;
	int SampleBits = 0;

	av_uchar PreData[4];

	switch (m_AudioComp)
	{
	case AvComp_AAC:
	{
		switch (m_AudioCfgSampleRate)
		{
		case 48000:
			PreData[0] = 0XAF;
			PreData[1] = 0X00;
			PreData[2] = 0x11;
			PreData[3] = 0x90;
			break;
		case 44100:
			PreData[0] = 0xAF;
			PreData[1] = 0x00;
			PreData[2] = 0x12;
			PreData[3] = 0x10;
			break;
		case 22050:
			PreData[0] = 0XAF;
			PreData[1] = 0X00;
			PreData[2] = 0x13;
			PreData[3] = 0x90;
			break;
		default:
			assert(0);
			break;
		}
	}
		break;
	case AvComp_G711A:
	{
		switch (m_AudioCfgSampleRate)
		{
		case 8000:
			PreData[0] = 0x72;
			PreData[1] = 0x00;
			PreData[2] = 0x04;
			PreData[3] = 0x88;
			break;
		default:
			av_error("SampleRate = %d\n", m_AudioCfgSampleRate);
			assert(0);

		}
	}
		break;
	case AvComp_G711U:
		break;
	default:
		assert(0);
		break;
	}


	return SendRtmpData(RTMP_PACKET_TYPE_AUDIO, PreData, sizeof(PreData), NULL, 0);
}

av_int AvRtmp::SendAudioData(CAvPacket *Packet)
{
	int ret = 0;
	switch (Packet->Comp())
	{
	case AvComp_G711A:
	{
		unsigned char preData[1] = { 0x72 };
		ret = SendRtmpData(RTMP_PACKET_TYPE_AUDIO, preData, sizeof(preData),
			(unsigned char *)Packet->GetRawBuffer() + 4, Packet->GetRawLength() - 4);
}
		break;
	case AvComp_G711U:
		break;
	case AvComp_AAC:
{
	unsigned char preData[2] = { 0xAF, 0x01 };
		ret = SendRtmpData(RTMP_PACKET_TYPE_AUDIO, preData, sizeof(preData),
			(unsigned char *)Packet->GetRawBuffer() + 7, Packet->GetRawLength() - 7);
	}
		break;

	default:
		av_warning("audio type = %d\n", Packet->Comp());
		break;
	}
	if (m_nABasePts != 0){
		m_nATimestamp = (av_uint)(Packet->TimeStamp() - m_nABasePts) / 1000;
	}
	else{
		m_nABasePts = Packet->TimeStamp();
	}

	return 1;
}
av_bool AvRtmp::RtmpPushTask()
{
	CAvPacket *AvPacket = NULL;
	int iRet = 0;


	while (m_Loop == av_true)
	{
		while (m_Loop == av_true)
		{
	
			if (!RTMP_Connect(m_RtmpHandle, NULL)){
				av_msleep(2 * 1000);
				av_msg("Drop clent reconnect\n");
				continue;
			}

			if (!RTMP_ConnectStream(m_RtmpHandle, 0)){
				av_msleep(2 * 1000);
				av_msg("Drop ConnectStream reconnect\n");
				continue;
			}
			else{
				
				Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(m_Channel);
				pCapture->Start(m_Slave, this, (Capture::SIG_PROC_ONDATA)&AvRtmp::OnStream);
			}
			
			break;
		}
		RTMP_SendChunkSize(m_RtmpHandle, 8192);

		while (m_Loop == av_true)
		{
			m_Mutex.Enter();
			if (m_Avpacket.empty() == true){
				m_Mutex.Leave();
				av_msleep(50);
			}
			else{
				AvPacket = m_Avpacket.front();
				m_Avpacket.pop();
				m_Mutex.Leave();
				
				if (avStreamT_V == AvPacket->StreamType()){
				AvPacket->GetNaluSplit();
					iRet = PushMediaData(AvPacket);
				}
				else if (avStreamT_A == AvPacket->StreamType()){
					SendAudioData(AvPacket);
				}
				else{
					assert(0);
				}
				
				AvPacket->Release();

				if (iRet <= 0){
					RTMP_DeleteStream(m_RtmpHandle);
					RTMP_Close(m_RtmpHandle);
				}
			}

		}
	}

	return av_true;
}

av_void AvRtmp::ThreadProc()
{

	if (m_RtmpMode == RTMP_PULL){
		RtmpPullTask();
	}
	else if (m_RtmpMode == RTMP_PUSH){
		RtmpPushTask();
	}
	else{
		assert(0);
	}

	av_error("%s Task Exit\n", __FUNCTION__);
}
