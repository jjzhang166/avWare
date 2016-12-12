#ifndef _AV_RECORD_H_
#define _AV_RECORD_H_
#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "AvPacket/AvPacket.h"
#include "AvCapture/AvManCapture.h"
#include "AvF/AvF.h"
#include "AvRecord/AvRecordCom.h"
#include "AvConfigs/AvConfigRecord.h"
#include "AvRecord/AvRecordDB.h"

#define  AVRECORD_MAX_PACKET 10
class CAvRecord:public CAvObject
{
public:
	typedef enum {
		CAvRecRecording,
		CAvRecStoped,
	}CAvRecStatus;
public:
	CAvRecord(av_u32 Channel);
	~CAvRecord();

public:
	av_void OnStream(av_int Channel, av_int Slave, CAvPacket *AvPacket);

public:
	av_void SyncWrite();
	av_void CheckOut();
	av_bool GetStatus(av_int Slave, CAvRecStatus &RecStatus);
private:
	av_bool CheckConfig(av_timeval &aTv);


private:
	RecordFileInfo m_RecordFInfo[CHL_NR_T];
private:
	av_bool RecordFileOpen(av_int Slave, av_timeval &AvTv);
	av_bool RecordFileClose(av_int Slave, av_timeval &AvTv);

public:
	av_void OnConfigRecordModify(CAvConfigRecord *ConfigRecord, int &result);
	av_void OnConfigRecordCtrlModify(CAvConfigRecordCtrl *CofnigRecordCtrl, int &result);

private:
	CAvConfigRecord				m_ConfigRecord;
	ConfigRecordFormats			m_ConfigRecordFormat;

	CAvConfigRecordCtrl			m_ConfigRecordCtrl;
	ConfigRecordCtrlFormats		m_ConfigRecordCtrlFormat;

private:
	av_u32						m_Channel;
	av_u32						m_MaxPacket;
	std::queue <CAvPacket *>	m_AvPacketQue[CHL_NR_T];
	CMutex						m_AvPacketQueMutex[CHL_NR_T];
	CAvRecFCom		*			m_RecF[CHL_NR_T];
	CAvRecStatus				m_RecStatus[CHL_NR_T];
	av_u32						m_RecSlave;
	Capture			*			m_Capture;
};


class CAvRecordManager :public CThread
{
public:
	SINGLETON_DECLARE(CAvRecordManager);
	av_bool Initialize();

public:
	av_bool RecordDbInit();
	av_bool RecordDbDeInit();
	av_bool RecordDbRepair();


	av_bool RecordDbAddItem(RecordFileInfo &RecordFInfo);
	av_bool RecordDbOverItem(RecordFileInfo &RecordFInfo);
private:
	CAvRecordDB m_RecordDb;


private:
	CAvRecordManager();
	~CAvRecordManager();

private:
	void			ThreadProc();
	av_bool			GetRecordStatus(int Channel, CAvRecord::CAvRecStatus &RecStatus);
private:
	CAvRecord	*	m_AvRecord[SYS_CHN_NUM];
};


#define g_AvRecordMan (*CAvRecordManager::instance())








#endif

