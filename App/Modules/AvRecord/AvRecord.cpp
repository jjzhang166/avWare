#include "AvRecord/AvRecord.h"





CAvRecord::CAvRecord(av_u32 Channel)
{
	m_Channel = Channel;
	m_MaxPacket = AVRECORD_MAX_PACKET;
}
CAvRecord::~CAvRecord()
{
	CAvPacket *pack = NULL;
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		while (m_AvPacketQue[i].empty() == false){
			pack = m_AvPacketQue[i].front();
			pack->Release();
			m_AvPacketQue[i].pop();
		}
	}


}
av_void CAvRecord::Sync()
{
	//checkout write file;
}
av_bool  CAvRecord::PushPacket(av_u32 Slave, CAvPacket *pack)
{
	CAvPacket *pAvPacket = NULL;
	if (m_AvPacketQue[Slave].size() >= m_MaxPacket && m_MaxPacket != 0){
		pAvPacket = m_AvPacketQue[Slave].front();
		m_AvPacketQue[Slave].pop();
		pAvPacket->Release();
	}
	m_AvPacketQue[Slave].push(pack);
	return av_true;
}

SINGLETON_IMPLEMENT(CAvRecordManager)


CAvRecordManager::CAvRecordManager() :CThread(__FUNCTION__)
{
	m_ChnMax = 0;
}

CAvRecordManager::~CAvRecordManager()
{
	m_ChnMax = 0;
}

av_bool CAvRecordManager::Initialize()
{
	CThread::run();
	m_ChnMax = g_AvManCapture.GetAvCaptureTotal();
	Capture *pCapture = NULL;
	for (int i = 0; i < m_ChnMax; i++){
		pCapture = g_AvManCapture.GetAvCaptureInstance(i);
		pCapture->Start(CHL_MAIN_T, this, (Capture::SIG_PROC_ONDATA)&CAvRecordManager::OnStream);
		pCapture->Start(CHL_SUB1_T, this, (Capture::SIG_PROC_ONDATA)&CAvRecordManager::OnStream);
		m_ChnAvRecord[i] = new CAvRecord(i);
	}

	return av_true;
}


void CAvRecordManager::ThreadProc()
{
	av_int i = 0;
	while (m_Loop == av_true){
		#pragma omp parallel for
		for (i = 0; i < m_ChnMax; i++){
			m_ChnAvRecord[i]->Sync();
		}
		av_msleep(10);
	}
}
av_void CAvRecordManager::OnStream(av_int Channel, av_int Slave, CAvPacket *AvPacket)
{
	//av_msg("Onstream ch[%d]slave[%d]\n", Channel, Slave);
	AvPacket->AddRefer();
	m_ChnAvRecord[Channel]->PushPacket(Slave, AvPacket);
}