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
		av_uint AvRecFContMask;
		av_uint PrewFrameLen;
		av_uint NextFrameLen;
	}CAvFTag;

public:
	CAvF();
	~CAvF();

public:
	av_bool AvRecFOpen(av_char *FilePatch, AvRecFOpenMode _mode);
	av_int  AvRecFNextFrameLength();
	av_bool AvRecFReadPacket(CAvPacket &Packet);
	av_bool AvRecFWritePacket(CAvPacket &Packet);
	av_bool AvRecFSeek(AvRecFSeekPlace seek, AvRecFSeekContext c, av_32 offset);
	av_bool AvRecFRename(av_char *newName);
	av_bool AvRecFRemove();
	av_bool AvRecFClose();
	av_bool AvRecRepair();
private:
	std::string m_FilePatch;
	av_u32		m_Mode;
	av_u32		m_FileStartT;
	av_u32		m_FileEndT;
	av_u32		m_FrameRate;

	FILE*		m_File;
	CAvFTag		m_AvfTag;
};














#endif