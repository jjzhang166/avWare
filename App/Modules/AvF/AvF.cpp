#include "AvF/AvF.h"

CAvF::CAvF()
{
	m_FilePatch.clear();
	m_Mode = 0x00;
	m_FileStartT = 0x00;
	m_FileEndT = 0x00;
	m_FrameRate = 0x00;

	m_File = NULL;
}
CAvF::~CAvF()
{
	m_FilePatch.clear();
	m_Mode = 0x00;
	m_FileStartT = 0x00;
	m_FileEndT = 0x00;
	m_FrameRate = 0x00;
	
	close();
	m_File = NULL;
}

av_bool CAvF::open(av_char *FilePatch, OpenMode _mode)
{
	char mode[12];
	if (_mode == AvF_OR){
		sprintf(mode, "rb");
	}
	else if (_mode == AvF_OW){
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
av_bool CAvF::close()
{
	if (m_File != NULL){
		fclose(m_File);
	}
	m_File = NULL;
	return av_true;
}
av_bool CAvF::read(CAvPacket &Packet)
{
	return av_true;
}
av_bool CAvF::write(CAvPacket &Packet)
{
	return av_true;
}
av_bool CAvF::seek(SeekPlace p, SeekContext c, av_32 offset)
{
	return av_true;
}
