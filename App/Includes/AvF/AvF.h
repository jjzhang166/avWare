#ifndef _AV_F_H_
#define _AV_F_H_
#include "Apis/AvWareType.h"
#include "AvPacket/AvPacket.h"

class CAvF
{
public:
	typedef enum{
		AvF_OR	= 1 << 0,
		AvF_OW	= 1 << 1,
	}OpenMode;

	typedef enum {
		AvSeekP_CUR,
		AvSeekP_HEAD,
	}SeekPlace;

	typedef enum {
		AvSeekC_Audio,
		AvSeekC_Video,
		AvSeekC_Alarm,
		AvSeekC_IFrame,
	}SeekContext;

public:
	CAvF();
	~CAvF();
public:
	av_bool open(av_char *FilePatch, OpenMode _mode);
	av_bool close();
	av_bool read(CAvPacket &Packet);
	av_bool write(CAvPacket &Packet);
	av_bool seek(SeekPlace p, SeekContext c, av_32 offset);
	av_bool rename(av_char *newName);
	av_bool remove();

private:
	std::string m_FilePatch;
	av_u32		m_Mode;
	av_u32		m_FileStartT;
	av_u32		m_FileEndT;
	av_u32		m_FrameRate;

	FILE*		m_File;

};














#endif