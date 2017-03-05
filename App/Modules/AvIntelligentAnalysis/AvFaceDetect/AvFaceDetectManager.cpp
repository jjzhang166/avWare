#ifdef FACE_DETECT
#include "AvIntelligentAnalysis/AvFaceDetectManager.h"
#include "AvPacket/AvPacket.h"
#include "AvCapture/AvManCapture.h"
#include "AvNetService/AvFtp.h"
#include "../../../BspPlatform/HISI_16AD/FaceDetect/av_face_detect.h"

SINGLETON_IMPLEMENT(AvFaceDetectManager);

AvFaceDetectManager::AvFaceDetectManager()
	:CThread(__FUNCTION__),
	m_ftp(nullptr),
	mutex_(),
	cond_(mutex_)	
{
	CThread::ThreadStart();
}

AvFaceDetectManager::~AvFaceDetectManager()
{
}

#define FTP_CONF "ftp.conf"
#define FTP_DEFINE_ADDR "192.168.1.91"
static const char* GetFtpServerAddr()
{
	struct stat st;
	if (-1 == stat(FTP_CONF, &st)) {
		return FTP_DEFINE_ADDR;
	}

	FILE *fp = fopen(FTP_CONF, "rb");
	if (NULL == fp) {
		return FTP_DEFINE_ADDR;
	}
	char addr[32] = {0};
	fread(addr, sizeof(char), 32, fp);
	fclose(fp);
	av_msg("read addr[%s] form ftp.conf\n", addr);
	return addr;
}

bool AvFaceDetectManager::Start()
{

	InitFaceDetect();

	av_msg("AvFaceDetectManager start\n");
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(0);
	pCapture->Start(CHL_SUB2_T, this, (Capture::SIG_PROC_ONDATA)&AvFaceDetectManager::OnStream);

	m_ftp = new CAvFtp();
	m_ftp->SetConf(GetFtpServerAddr(), "21", "admin", "admin");
	m_ftp->Start();
	return true;
}

bool AvFaceDetectManager::Stop()
{

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(0);
	pCapture->Stop(CHL_SUB2_T, this, (Capture::SIG_PROC_ONDATA)&AvFaceDetectManager::OnStream);

	m_ftp->Stop();
	delete m_ftp;
	m_ftp = nullptr;
	return true;
}


av_void AvFaceDetectManager::OnStream(av_uchar Channel, av_uchar Slave, CAvPacket *AvPacket)
{
	AvPacket->AddRefer();
	PushMsg(&AvPacket);
}


void AvFaceDetectManager::PushMsg(CAvPacket **AvPacket)
{
	mutex_.lock();
	msg_queue_.push(*AvPacket);
	cond_.notify();
	mutex_.unlock();
}


void AvFaceDetectManager::PopMsg(CAvPacket **AvPacket)
{
	mutex_.lock();
	if (msg_queue_.empty()) {
		cond_.wait();
	}
	
	*AvPacket = msg_queue_.front();
	msg_queue_.pop();
	mutex_.unlock();
}


av_void AvFaceDetectManager::ThreadProc()
{
	CAvPacket *AvPacket;
	while (m_Loop == av_true) {
		PopMsg(&AvPacket);
		
		static unsigned int index = 0;
		++index;
		char name[32]={0};
		
		sprintf(name, "OnStream_face_index_%d.jpg", index);
		/* save jpg
		FILE *fp = fopen(name, "wb");
		fwrite((char*)AvPacket->GetRawBuffer(), sizeof(char), AvPacket->GetRawLength(), fp);
		fclose(fp);
		//*/
		
		av_msg("FTP upload file name: %s\n", name);
		m_ftp->Upload(name ,const_cast<av_uchar*>(AvPacket->GetRawBuffer()), AvPacket->GetRawLength());
		
		AvPacket->Release();
	}
}
#endif //FACE_DETECT
