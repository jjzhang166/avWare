#include "AvRecord/AvRecord.h"
#include "AvFile/AvFile.h"
#include "AvDevice/AvDevice.h"





CAvRecord::CAvRecord(av_u32 Channel)
{
	m_Channel	= Channel;
	m_MaxPacket = AVRECORD_MAX_PACKET;
	
	m_RecSlave	= 0;

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		m_RecF[i] = NULL;
		m_RecStatus[i] = CAvRecStoped;
		m_RecordFInfo[i].FileChannel = m_Channel;
		memset(m_RecordFInfo[i].FilePath, 0, sizeof(m_RecordFInfo[i].FilePath));
		m_RecordFInfo[i].FileRecTmE = 0;
		m_RecordFInfo[i].FileRecTmS = 0;
		m_RecordFInfo[i].FileSize = 0;
		m_RecordFInfo[i].FileSlave = i;
		m_RecordFInfo[i].FileTypeMask = 0;
	}

	m_ConfigRecord.Update(m_Channel);
	m_ConfigRecordFormat = m_ConfigRecord.GetConfig(m_Channel);
	m_ConfigRecord.Attach(this, (AvConfigCallBack)&CAvRecord::OnConfigRecordModify);


	m_ConfigRecordCtrl.Update();
	m_ConfigRecordCtrlFormat = m_ConfigRecordCtrl.GetConfig();
	m_ConfigRecordCtrl.Attach(this, (AvConfigCallBack)&CAvRecord::OnConfigRecordCtrlModify);
	m_Capture = g_AvManCapture.GetAvCaptureInstance(m_Channel);
}
CAvRecord::~CAvRecord()
{
	m_ConfigRecord.Detach(this, (AvConfigCallBack)&CAvRecord::OnConfigRecordModify);
	m_ConfigRecordCtrl.Detach(this, (AvConfigCallBack)&CAvRecord::OnConfigRecordCtrlModify);

	CAvPacket *pack = NULL;
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		while (m_AvPacketQue[i].empty() == false){
			pack = m_AvPacketQue[i].front();
			pack->Release();
			m_AvPacketQue[i].pop();
		}
	}
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		delete m_RecF[i];
		m_RecF[i] = NULL;
	}


}

av_void CAvRecord::OnStream(av_int Channel, av_int Slave, CAvPacket *AvPacket)
{
	assert(Channel == m_Channel);
	CGuard m(m_AvPacketQueMutex[Slave]);
	if (m_AvPacketQue[Slave].size() >= m_MaxPacket && m_MaxPacket != 0){
		CAvPacket *pAvPacket = NULL;
		pAvPacket = m_AvPacketQue[Slave].front();
		m_AvPacketQue[Slave].pop();
		pAvPacket->Release();
	}
	AvPacket->AddRefer();
	m_AvPacketQue[Slave].push(AvPacket);
}

av_bool CAvRecord::GetStatus(av_int Slave, CAvRecStatus &RecStatus)
{
	RecStatus = m_RecStatus[Slave];
	return av_true;
}
av_bool CAvRecord::CheckConfig(av_timeval &aTv)
{
	if (m_ConfigRecordFormat.RecordChlMask != 0
		&& m_ConfigRecordFormat.RecordTypeMask != 0){
		if (m_ConfigRecordFormat.RecordTypeMask == EAvRecord_Manul){
			return av_true;
		}
		av_bool bTIn = av_false;
		for (int i = 0; i < RECORD_MAX_TIME_RANGE; i++){
			if (aTv.t_todaysec >= m_ConfigRecordFormat.RecordTm[i].tStart
				&& aTv.t_todaysec <= m_ConfigRecordFormat.RecordTm[i].tEnd
				&& m_ConfigRecordFormat.RecordTm[i].bEnable == av_true){
				bTIn = av_true;
				break;
			}
		}
		return bTIn;
	}
	else{
		return av_false;
	}
}
av_void CAvRecord::CheckOut()
{
	av_bool bStartRec = av_true;
	av_bool bRet = av_false;
	av_timeval AvTv;
	AvGetTimeOfDay(&AvTv);

	bStartRec = CheckConfig(AvTv);
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (!(m_ConfigRecordFormat.RecordChlMask & AvMask(i))) {
			if (NULL != m_RecF[i]){
				RecordFileClose(i, AvTv);
				m_RecStatus[i] = CAvRecStoped;
			}
			continue;
		}

		if (bStartRec == av_true && m_RecStatus[i] != CAvRecRecording){
			if (m_Capture->GetCaptureStatus(i) != Capture::EAvCapture_ING){
				continue;
			}
			bRet = RecordFileOpen(i, AvTv);
			m_Capture->Start(i, this, (Capture::SIG_PROC_ONDATA)&CAvRecord::OnStream);
			m_RecStatus[i] = CAvRecRecording;
		}
		else if (bStartRec == av_false && m_RecStatus[i] != CAvRecStoped){
			if (m_RecF[i] != NULL){
				m_Capture->Stop(i, this, (Capture::SIG_PROC_ONDATA)&CAvRecord::OnStream);
				RecordFileClose(i, AvTv);
				m_RecStatus[i] = CAvRecStoped;
			}

		}else{
			av_bool bChangeFile = av_false;
			if (m_RecStatus[i] != CAvRecRecording){
				continue;
			}
			m_ConfigRecordCtrlFormat.LimitArgs = 6 * 1024 * 1024;
			switch (m_ConfigRecordCtrlFormat.LimitType)
			{
				case EAvRecord_LimitTime:
				{
					if (AvTv.t_todaysec - m_RecordFInfo[i].FileRecTmS >= m_ConfigRecordCtrlFormat.LimitArgs){
						bChangeFile = av_true;
					}
				}
				break;

				case EAvRecord_LimitSize:
				{
					if (m_RecordFInfo[i].FileSize >= m_ConfigRecordCtrlFormat.LimitArgs){
						bChangeFile = av_true;
					}
				}
				break;

				default:
					assert(0);
					break;
			}

			if (bChangeFile == av_true){
				RecordFileClose(i, AvTv);
				RecordFileOpen(i, AvTv);
			}
		}
	}
}
av_void CAvRecord::SyncWrite()
{
	CAvPacket *pack = NULL;
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (m_AvPacketQue[i].empty() == false){
			
			m_AvPacketQueMutex[i].Enter();
			pack = m_AvPacketQue[i].front();
			m_AvPacketQue[i].pop();
			m_AvPacketQueMutex[i].Leave();

			m_RecF[i]->AvRecFWritePacket(*pack);
			
			m_RecordFInfo[i].FileSize += pack->GetLength();
			pack->Release();
		}
	}
}

av_void CAvRecord::OnConfigRecordModify(CAvConfigRecord *ConfigRecord, int &result)
{
	result = 0;
	m_ConfigRecordFormat = m_ConfigRecord.GetConfig(m_Channel);
}

av_void CAvRecord::OnConfigRecordCtrlModify(CAvConfigRecordCtrl *CofnigRecordCtrl, int &result)
{
	m_ConfigRecordCtrlFormat = m_ConfigRecordCtrl.GetConfig();
	result = 0;
}

av_bool CAvRecord::RecordFileOpen(av_int Slave, av_timeval &AvTv)
{
	m_RecF[Slave] = new CAvF;
	av_bool bRet = av_false;
	std::string RecordHomeDir;
	CAvDevice::GetEnv(std::string(EKey_DefaultRecordHomeDir), RecordHomeDir);
	sprintf(m_RecordFInfo[Slave].FilePath, "%s/%d_%02d/%02d/CHN%02d_%d_%02d_%02d_%02d.Avf",  RecordHomeDir.c_str(),AvTv.t_year, AvTv.t_mon, AvTv.t_day, m_Channel, Slave, AvTv.t_hour, AvTv.t_min, AvTv.t_sec);
	CFile::MakeDeepDirectory(m_RecordFInfo[Slave].FilePath);
	bRet = m_RecF[Slave]->AvRecFOpen(m_RecordFInfo[Slave].FilePath, CAvRecFCom::AvRecF_OW);
	if (bRet != av_true){
		av_error("AvRecFOpen return av_flase\n");
		assert(0);
	}


	m_RecordFInfo[Slave].FileRecTmE = 0;
	m_RecordFInfo[Slave].FileRecTmS = AvTv.t_todaysec;
	m_RecordFInfo[Slave].FileSize = 0;
	m_RecordFInfo[Slave].FileTypeMask = 0;

	g_AvRecordMan.RecordDbAddItem(m_RecordFInfo[Slave]);
	return av_true;
}

av_bool CAvRecord::RecordFileClose(av_int Slave, av_timeval &AvTv)
{
	m_RecordFInfo[Slave].FileRecTmE = AvTv.t_todaysec;
	g_AvRecordMan.RecordDbOverItem(m_RecordFInfo[Slave]);
	delete m_RecF[Slave];
	m_RecF[Slave] = NULL;

	return av_true;
}










SINGLETON_IMPLEMENT(CAvRecordManager)
CAvRecordManager::CAvRecordManager() :CThread(__FUNCTION__)
{
	for (int i = 0; i < SYS_CHN_NUM; i++){
		m_AvRecord[i] = NULL;
	}
}

CAvRecordManager::~CAvRecordManager()
{
	for (int i = 0; i < SYS_CHN_NUM; i++){
		if (NULL != m_AvRecord[i]){
			delete m_AvRecord[i];
			m_AvRecord[i] = NULL;
		}
	}
}

av_bool CAvRecordManager::Initialize()
{
	if (av_true != RecordDbInit()){
		assert(0);
	}

	RecordDbRepair();


	for (int i = 0; i < SYS_CHN_NUM; i++){
		m_AvRecord[i] = new CAvRecord(i);
	}

	CThread::ThreadStart();
	return av_true;
}


void CAvRecordManager::ThreadProc()
{
	av_int i = 0;
	while (m_Loop == av_true){
		for (i = 0; i < SYS_CHN_NUM; i++){
			m_AvRecord[i]->CheckOut();
 			m_AvRecord[i]->SyncWrite();
		}
		av_msleep(10);
	}
}

av_bool CAvRecordManager::GetRecordStatus(int Channel, CAvRecord::CAvRecStatus &RecStatus)
{
	return m_AvRecord[Channel]->GetStatus(CHL_MAIN_T, RecStatus);
}


av_bool CAvRecordManager::RecordDbInit()
{
	av_timeval aTv;
	AvGetTimeOfDay(&aTv);
	av_char path[128] = { 0 };
	
	av_timeval AvTv;
	AvGetTimeOfDay(&AvTv);

	std::string RecordHomeDir;
	CAvDevice::GetEnv(std::string(EKey_DefaultRecordHomeDir), RecordHomeDir);
	sprintf(path, "%s/%d_%02d/%02d/Record.db", RecordHomeDir.c_str(), AvTv.t_year, AvTv.t_mon, AvTv.t_day);
	CFile::MakeDeepDirectory(path);

	m_RecordDb.DbOpen(path);

	return av_true;
}
av_bool CAvRecordManager::RecordDbDeInit()
{
	m_RecordDb.DbClose();
	return av_true;
}
av_bool CAvRecordManager::RecordDbRepair()
{
	RecordFileSearch Search;

	Search.FileChannelMask = 0;
	Search.FileSlaveMask = 0;
	Search.FileTypeMask = 0;
	Search.SearchDay = 0;


	Search.FileRecTmS = -1;
	Search.FileRecTmE = -1;
	Search.FileSize = 0;
	
	std::list<RecordFileInfo> result;

	m_RecordDb.DbSearch(Search, result);
	std::list<RecordFileInfo>::iterator i;
	for (i = result.begin(); i != result.end(); i++){

	}
	return av_true;
}
av_bool CAvRecordManager::RecordDbAddItem(RecordFileInfo &RecordFInfo)
{
	m_RecordDb.DbInsert(RecordFInfo);
	return av_true;
}
av_bool CAvRecordManager::RecordDbOverItem(RecordFileInfo &RecordFInfo)
{
	m_RecordDb.DbUpdate(RecordFInfo);
	return av_true;

}