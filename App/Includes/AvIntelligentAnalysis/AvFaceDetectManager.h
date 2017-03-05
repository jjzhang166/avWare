#ifndef _AV_FACEDETECTMANAGER_H_
#define _AV_FACEDETECTMANAGER_H_

#ifdef FACE_DETECT
#include <queue>

#include "CAvObject.h"
#include "Signals.h"
#include "Apis/AvWareCplusplus.h"
#include "AvThread/AvThread.h"
#include "Apis/LibSystem.h"
#include "AvThread/AvTask.h"

class CAvPacket;
class CAvFtp;

class AvFaceDetectManager:public CThread{
public:
	SINGLETON_DECLARE(AvFaceDetectManager);
	bool Start();
	bool Stop();

private:
	AvFaceDetectManager();
	~AvFaceDetectManager();
	
	av_void OnStream(av_uchar Channel, av_uchar Slave, CAvPacket *AvPacket);
	av_void ThreadProc();
	CAvFtp *m_ftp;
	
private:
	MutexLock mutex_;
	PthreadCondition cond_;
	std::queue<CAvPacket*> msg_queue_;
	void PushMsg(CAvPacket **AvPacket);
	void PopMsg(CAvPacket **AvPacket);
private:
	DISALLOW_COPY_AND_ASSIGN_11(AvFaceDetectManager);
};

#define g_AvIS_FaceDetect (*AvFaceDetectManager::instance())

#endif //FACE_DETECT

#endif //_AV_FACEDETECTMANAGER_H_
