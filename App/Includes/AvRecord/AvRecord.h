#ifndef _AV_RECORD_H_
#define _AV_RECORD_H_
#include "Apis/AvWareType.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "AvPacket/AvPacket.h"
#include "AvCapture/AvManCapture.h"

#define  AVRECORD_MAX_PACKET 10
class CAvRecord
{
public:
	CAvRecord(av_u32 Channel);
	~CAvRecord();
	av_bool PushPacket(av_u32 Slave, CAvPacket *pack);
	av_void Sync();
private:
	av_u32	m_Channel;
	av_u32  m_MaxPacket;
	std::queue <CAvPacket *> m_AvPacketQue[CHL_NR_T];
};


class CAvRecordManager :public CThread
{
public:
	SINGLETON_DECLARE(CAvRecordManager);
	av_bool Initialize();
private:
	CAvRecordManager();
	~CAvRecordManager();

private:
	void ThreadProc();
	av_void OnStream(av_int Channel, av_int Slave, CAvPacket *AvPacket);

private:
	std::map<av_u32, CAvRecord *> m_ChnAvRecord;
	av_u32						  m_ChnMax;
};


#define g_AvRecordMan (*CAvRecordManager::instance())








#endif

