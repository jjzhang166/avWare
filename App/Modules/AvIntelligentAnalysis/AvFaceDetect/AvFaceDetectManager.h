#ifndef _AV_FACEDETECTMANAGER_H_
#define _AV_FACEDETECTMANAGER_H_

#include <queue>
#include <memory>

#include "CAvObject.h"
#include "Signals.h"
#include "Apis/AvWareCplusplus.h"
#include "AvThread/AvThread.h"
#include "Apis/LibSystem.h"
#include "AvThread/AvTask.h"
#include "AvConfigs/AvConfigNetService.h"

class CAvFtp;
class BlockQueenFaceDetect;

class AvFaceDetectManager:public CThread{
public:
	SINGLETON_DECLARE(AvFaceDetectManager);
	bool Start();
	bool Stop();

private:
	AvFaceDetectManager();
	~AvFaceDetectManager();
	av_void ThreadProc();

	void onFtpConfigChange(CAvConfigNetFtp *conf, int &req);
	inline void ChangeFtpConfig(const ConfigNetFtp &ftp_conf);
	inline void FtpUploadfile(const char* file_name, const char *buff, const int buffsize);
private:
	std::shared_ptr<CAvFtp> ftp_;
	MutexLock mutex_;
	av_bool is_ftp_enable_;
	BlockQueenFaceDetect *queue_;
	DISALLOW_COPY_AND_ASSIGN_11(AvFaceDetectManager);
};

#define g_AvIntelligent_FaceDetect (*AvFaceDetectManager::instance())

#endif //_AV_FACEDETECTMANAGER_H_
