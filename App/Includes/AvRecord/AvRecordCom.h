#ifndef _AVRECORDCOM_H_
#define _AVRECORDCOM_H_

#include "Apis/AvWareType.h"
#include "AvPacket/AvPacket.h"

class CAvRecFCom
{
public:
	typedef enum {
		RecfT_AVF,
		RecfT_AVI,
		RecfT_MP4,
	}RecfType;

	typedef enum{
		AvRecF_OR	= 1 << 0,
		AvRecF_OW	= 1 << 1,
	}AvRecFOpenMode;

	typedef enum {
		AvRecF_CUR,
		AvRecF_HEAD,
		AvRecF_TAIL,
	}AvRecFSeekPlace;

	typedef enum {
		AvRecF_Audio,
		AvRecF_Video,
		AvRecF_Alarm,
		AvRecF_IFrame,
	}AvRecFSeekContext;
	
public:
	CAvRecFCom(){};
	virtual ~CAvRecFCom(){};
	
public:	
	virtual av_bool AvRecFOpen(av_char *FilePatch, AvRecFOpenMode _mode) 		= 0;
	virtual av_bool AvRecFReadPacket(CAvPacket &Packet) 						= 0;
	virtual av_bool AvRecFWritePacket(CAvPacket &Packet) 						= 0;
	virtual av_bool AvRecFSeek(AvRecFSeekPlace seek, AvRecFSeekContext c, av_32 offset) 		= 0;
	virtual av_bool AvRecFRename(av_char *newName) 								= 0;
	virtual av_bool AvRecFRemove() 												= 0;
	virtual av_bool AvRecFClose() = 0;
	

};


#endif

