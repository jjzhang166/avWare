#include "AvIntelligentAnalysis/AvIntelligentAnalysis.h"
#include "Apis/LibIntelligent.h"
#include "AvFaceDetect/AvFaceDetectManager.h"


SINGLETON_IMPLEMENT(CAvIntelligentanalysis);

CAvIntelligentanalysis::CAvIntelligentanalysis() :CThread("CAvIntelligentanalysis")
{

}
CAvIntelligentanalysis::~CAvIntelligentanalysis()
{
	AvIntelligentDestory();
}
av_bool CAvIntelligentanalysis::Initialize()
{
	C_IntelligentCaps caps;
	AvIntelligentGetCaps(&caps);
	
	AvIntelligentCreate();
	if (caps.Mask & AvMask(Intelligent_FaceDetect)) {
		av_msg("AvIntelligent_FaceDetect Start()\n");
		g_AvIntelligent_FaceDetect.Start();
	}
	
	//CThread::ThreadStart();
	av_msg("CAvIntelligentanalysis Initialize over\n");
	return av_true;
}

av_void CAvIntelligentanalysis::ThreadProc()
{
	while (m_Loop == av_true)
	{

		av_msleep(1000);
	}
}


