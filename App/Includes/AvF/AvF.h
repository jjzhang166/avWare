#ifndef _AV_F_H_
#define _AV_F_H_
#include "Apis/AvWareCplusplus.h"
#include "Apis/LibEncode.h"
#include "AvPacket/AvPacket.h"
#include "AvRecord/AvRecordCom.h"
class CAvF :public CAvRecFCom
{
public:
#define _AVF_TAG_VERSION 0x00010101
	typedef struct {
		av_uint TagVer;
		av_u32  FrameT;
		av_u32	StreamContent;
		av_u32	FrameSecond;
		av_u32	AlarmMark;

		av_uint PrewFrameLen;
		av_uint NextFrameLen;
	}CAvFTag;

public:
	CAvF();
	~CAvF();

public:
	av_64			AvRecFOpen(av_char *FilePatch, AvRecFOpenMode _mode);
	av_int			AvRecFNextFrameLength();
	av_bool			AvRecFReadPacket(CAvPacket &Packet);
	CAvPacket *		AvRecFReadPacket();
	av_bool			AvRecFWritePacket(CAvPacket &Packet);
	av_bool			AvRecFSeek(AvRecFSeekPlace seek, AvRecFSeekContext c, av_32 offset);
	av_bool			AvRecFSeek(av_u32 AbsoluteSecond);
	av_bool			AvRecFRename(av_char *newName);
	av_bool			AvRecFRemove();
	av_bool			AvRecFClose();
	av_bool			AvRecRepair(av_u32 &FileLen, av_u32 &EndTmSec, av_u32 &FileMask);

private:
	std::string m_FilePatch;
	av_u32		m_Mode;
	av_u32		m_FileStartT;
	av_u32		m_FileEndT;
	av_u32		m_FrameRate;

	FILE*		m_File;
	CAvFTag		m_AvfTag;
	av_uchar	*m_ReadCache;
};














#endif