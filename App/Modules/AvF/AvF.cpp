#include "AvF/AvF.h"
#include <math.h>

CAvF::CAvF()
{
	m_FilePatch.clear();
	m_Mode = 0x00;
	m_FileStartT = 0x00;
	m_FileEndT = 0x00;
	m_FrameRate = 0x00;

	m_File = NULL;

	m_AvfTag.TagVer = _AVF_TAG_VERSION;
	m_AvfTag.PrewFrameLen = 0;
	m_AvfTag.NextFrameLen = 0;
	m_ReadCache = NULL;

}
CAvF::~CAvF()
{
	AvRecFClose();
	m_FilePatch.clear();
	m_Mode = 0x00;
	m_FileStartT = 0x00;
	m_FileEndT = 0x00;
	m_FrameRate = 0x00;
	if (NULL != m_ReadCache){
		free(m_ReadCache);
		m_ReadCache = NULL;
	}
	
}

av_64 CAvF::AvRecFOpen(av_char *FilePatch, AvRecFOpenMode _mode)
{
	char mode[12];
	if (_mode == CAvRecFCom::AvRecF_OR){
		sprintf(mode, "rb");
		m_ReadCache = (av_uchar *)malloc(sizeof(av_uchar)*AVPACKET_MAX_FRAME);
	}
	else if (_mode == CAvRecFCom::AvRecF_OW){
		m_AvfTag.PrewFrameLen = 0;
		sprintf(mode, "wb");
	}
	else{
		assert(0);
	}
	m_File = fopen(FilePatch, mode);
	if (m_File == NULL){
		return -1;
	}

	int iRet = 0;


	m_Mode = _mode;
	m_FilePatch.assign(FilePatch);

	
	if (_mode == CAvRecFCom::AvRecF_OR){
		iRet = fread(&m_AvfTag, sizeof(CAvFTag), 1, m_File);
		if (iRet != 1){
			fclose(m_File);
			m_File = NULL;
			return -1;
		}
	}
	return 0;
}
av_int  CAvF::AvRecFNextFrameLength()
{
	return m_AvfTag.NextFrameLen;
}
av_bool CAvF::AvRecFReadPacket(CAvPacket &Packet)
{
	if (m_Mode != CAvRecFCom::AvRecF_OR){
		return av_false;
	}
	int iRet = 0;

	
	iRet = fread(m_ReadCache, m_AvfTag.NextFrameLen, 1, m_File);
	if (iRet == 1){
		Packet.PutBuffer(m_ReadCache, m_AvfTag.NextFrameLen);
		Packet.PutBufferOver();
	}
	else{
		return av_false;
	}
	

	iRet = fread(&m_AvfTag, sizeof(CAvFTag), 1, m_File);
	if (iRet != 1){
		return av_false;
	}

	return av_true;
}

CAvPacket *  CAvF::AvRecFReadPacket()
{
	CAvPacket *Packet = NULL;
	if (m_AvfTag.NextFrameLen != 0){
		Packet = g_AvPacketManager.GetAvPacket(m_AvfTag.NextFrameLen);
	}
	else{
		return NULL;
	}
	av_bool avbRet = AvRecFReadPacket(*Packet);
	if (avbRet == av_true){
		return Packet;
	}
	else{
		Packet->Release();
		return NULL;
	}
}
av_bool CAvF::AvRecFWritePacket(CAvPacket &Packet)
{
	if (m_Mode != CAvRecFCom::AvRecF_OW){
		return av_false;
	}
	m_AvfTag.PrewFrameLen = m_AvfTag.NextFrameLen;
	m_AvfTag.NextFrameLen = Packet.GetLength();
	av_u64 TimeStamp = Packet.TimeStamp();
	TimeStamp /= 1000000;
	m_AvfTag.FrameSecond = TimeStamp;
	m_AvfTag.FrameT = Packet.FrameType();
	m_AvfTag.AlarmMark = 0;
	int iRet = 0;
	iRet = fwrite(&m_AvfTag, sizeof(CAvFTag),1, m_File);
	if (iRet != 1){
		return av_false;
	}
	iRet = fwrite(Packet.GetBuffer(), Packet.GetLength(), 1, m_File);
	if (iRet != 1){
		return av_false;
	}
	return av_true;

}
av_bool CAvF::AvRecFSeek(CAvRecFCom::AvRecFSeekPlace seek, CAvRecFCom::AvRecFSeekContext c, av_32 offset)
{
	int iRet = 0;
	switch (seek)
	{
	case AvRecF_CUR:
		break;
	case AvRecF_HEAD:
		iRet = fseek(m_File, 0, SEEK_SET);
		if (iRet == 0){
			return av_false;
		}
		else{
			//return av_true;
		}
		break;
	case AvRecF_TAIL:
		break;
	default:
		break;
	}

	switch (c)
	{
	case AvRecF_Audio:
		break;

	case AvRecF_Video:
		break;

	case AvRecF_Alarm:
		break;

	case AvRecF_IFrame:
		break;

	default:
		break;
	}

	return av_true;
}
av_bool CAvF::AvRecFSeek(av_u32 AbsoluteSecond)
{
	int iRet = 0;
	while (abs((int)(m_AvfTag.FrameSecond - AbsoluteSecond)) > 1 && m_AvfTag.FrameT != avFrameT_I){
		iRet = fseek(m_File, m_AvfTag.NextFrameLen, SEEK_CUR);
		if (iRet != 0){
			return av_false;
		}

		iRet = fread(&m_AvfTag, sizeof(CAvFTag), 1, m_File);
		if (iRet != 1){
			return av_false;
		}
		if (m_AvfTag.NextFrameLen == 0){
			return av_false;
		}
	}

	return av_true;
}
av_bool CAvF::AvRecFRename(av_char *newName)
{
	return av_true;
}
av_bool CAvF::AvRecFRemove()
{
	return av_true;
}
av_bool CAvF::AvRecFClose()
{
	if (m_Mode == AvRecF_OW){
		m_AvfTag.AlarmMark = 0;
		m_AvfTag.FrameSecond = 0;
		m_AvfTag.FrameT = 0;
		m_AvfTag.NextFrameLen = 0;
		m_AvfTag.StreamContent = 0;
		fwrite(&m_AvfTag, sizeof(CAvFTag), 1, m_File);
		// write file index;
	}
	if (m_File != NULL){
		fclose(m_File);
	}
	m_File = NULL;
	return av_true;
}

av_bool CAvF::AvRecRepair(av_u32 &FileLen, av_u32 &EndTmSec, av_u32 &FileMask)
{
	if (NULL == m_File){
		return av_false;
	}
	FileLen = 0;
	FileMask = 0;
	EndTmSec = 0;

	int iRet = 0;
	while (1){
		FileLen += m_AvfTag.NextFrameLen;
		FileMask |= m_AvfTag.AlarmMark;
		EndTmSec = m_AvfTag.FrameSecond;

		iRet = fseek(m_File, m_AvfTag.NextFrameLen, SEEK_CUR);
		if (iRet != 0){
			break;
		}
		iRet = fread(&m_AvfTag, sizeof(CAvFTag), 1, m_File);
		if (iRet != 1){
			break;
		}
		if (m_AvfTag.TagVer != _AVF_TAG_VERSION){
			break;
		}
		if (m_AvfTag.NextFrameLen == 0){
			break;
		}
	}

	if (FileLen == 0){
		return av_false;
	}

	return av_true;

}