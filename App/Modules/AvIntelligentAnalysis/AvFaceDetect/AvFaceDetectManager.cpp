#include "AvFaceDetectManager.h"
#include "AvNetService/AvFtp.h"
#include "Apis/LibIntelligent.h"

#include "block_queue.h"
#include "bsp_face_detect_demo.h"
using namespace std;

SINGLETON_IMPLEMENT(AvFaceDetectManager);

AvFaceDetectManager::AvFaceDetectManager()
	:CThread(__FUNCTION__),
	is_ftp_enable_(av_false),
	queue_(nullptr)
{
}

AvFaceDetectManager::~AvFaceDetectManager()
{
}

static void FtpDeleter(CAvFtp *ptr)
{
	ptr->Stop();
	delete ptr;
	ptr = nullptr;
	av_msg("Shared_ptr delete ftp\n");
}

bool AvFaceDetectManager::Start()
{
	av_msg("AvFaceDetectManager start\n");

	av_bool b_ret = AvIntelligentGetHandle(IntelligentHandle_FaceDetectResultQueue, (void**)(&queue_));
	if (av_false == b_ret) {
		av_error("AvIntelligentGetHandle failed\n");
		return false;
	}

	ftp_ = shared_ptr<CAvFtp>(new CAvFtp, FtpDeleter);
	CAvConfigNetFtp conf_ftp;
	conf_ftp.Update();
	ConfigNetFtp &ftp_conf = conf_ftp.GetConfig();
	ChangeFtpConfig(ftp_conf);

	conf_ftp.Attach(this, (AvConfigCallBack)&AvFaceDetectManager::onFtpConfigChange);
	CThread::ThreadStart();
	return true;
}

bool AvFaceDetectManager::Stop()
{
	CAvConfigNetFtp conf_ftp;
	conf_ftp.Detach(this, (AvConfigCallBack)&AvFaceDetectManager::onFtpConfigChange);
	ftp_.reset();
	return true;
}


av_void AvFaceDetectManager::ThreadProc()
{
	static int index = 0;
	char ftp_name[88];
	char tmp_time[48];
	while (m_Loop == av_true) {
		pFaceResult result = queue_->take();
		if (result && (av_true == is_ftp_enable_)) {
			index++;
			const time_t t = result->timestamp.tv_sec;
			strftime(tmp_time, 48, "%F_%H-%M-%S", localtime(&t));
			sprintf(ftp_name, "%s_AllView_sync%d.jpg", tmp_time, index);
			
			FtpUploadfile(ftp_name, result->all_view_img.c_str(), result->all_view_img.size());

			int i = 0;
			for (; i < result->face_count; ++i) {
				sprintf(ftp_name, "%s_sync%d_face_%d_rect[%d_%d__%d_%d].jpg", tmp_time, index, i,
					result->face_result[i]->face_rect.Sx,
					result->face_result[i]->face_rect.Sy,
					result->face_result[i]->face_rect.Width,
					result->face_result[i]->face_rect.Heigh);
					FtpUploadfile(ftp_name, result->face_result[i]->face_img.c_str(), result->face_result[i]->face_img.size());
			}
		}
	}
}

void AvFaceDetectManager::FtpUploadfile(const char* file_name, const char *buff, const int buffsize)
{
	shared_ptr<CAvFtp> pftp;
	{
		MutexLockGuard lock(mutex_);
		pftp = ftp_;
	}
	pftp->Upload(file_name, (unsigned char*)buff, buffsize);
}

void AvFaceDetectManager::onFtpConfigChange(CAvConfigNetFtp *conf, int &req)
{
	av_msg("FtpConfigChange\n");
	ConfigNetFtp &ftp_conf = conf->GetConfig();
	ChangeFtpConfig(ftp_conf);
}

//copy-on-write for thread safe
void  AvFaceDetectManager::ChangeFtpConfig(const ConfigNetFtp &ftp_conf)
{
	MutexLockGuard lock(mutex_);
	if (!ftp_.unique()) {
		ftp_.reset(new CAvFtp());
	}
	assert(ftp_.unique());
	is_ftp_enable_ = ftp_conf.bEnable;
	if (av_true == is_ftp_enable_) {
		char tmp_port[8];
		sprintf(tmp_port, "%d", ftp_conf.ServicePort);
		ftp_->SetConf(ftp_conf.ServerAddress, tmp_port, ftp_conf.UserName, ftp_conf.PassWord);
		if (strlen(ftp_conf.RemotePath)) {
			AvFtpSetRemotePath(ftp_.get(), ftp_conf.RemotePath);
		}
	}

}
