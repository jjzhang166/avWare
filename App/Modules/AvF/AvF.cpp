#include "AvF/AvF.h"


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

}
CAvF::~CAvF()
{
	AvRecFClose();
	m_FilePatch.clear();
	m_Mode = 0x00;
	m_FileStartT = 0x00;
	m_FileEndT = 0x00;
	m_FrameRate = 0x00;
	
	
}



av_bool CAvF::AvRecFOpen(av_char *FilePatch, AvRecFOpenMode _mode)
{
	char mode[12];
	if (_mode == CAvRecFCom::AvRecF_OR){
		sprintf(mode, "rb");
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
		return av_false;
	}

	m_Mode = _mode;
	m_FilePatch.assign(FilePatch);
	return av_true;
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
	iRet = fread(&m_AvfTag, sizeof(CAvFTag), 1, m_File);
	if (iRet != 1){
		return av_false;
	}

	return av_true;
}
av_bool CAvF::AvRecFWritePacket(CAvPacket &Packet)
{
	if (m_Mode != CAvRecFCom::AvRecF_OW){
		return av_false;
	}
	m_AvfTag.PrewFrameLen = m_AvfTag.NextFrameLen;
	m_AvfTag.NextFrameLen = Packet.GetLength();
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
	if (m_File != NULL){
		fclose(m_File);
	}
	m_File = NULL;
	return av_true;
}

av_bool CAvF::AvRecRepair()
{
	if (NULL != m_File);

	return av_true;

}