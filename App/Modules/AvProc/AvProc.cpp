#include "AvProc/AvProc.h"
#include "Apis/AvWareType.h"


SINGLETON_IMPLEMENT(CAvProc);

CAvProc::CAvProc()
{
	m_ProcHandle = -1;
}
CAvProc::~CAvProc()
{
	if (m_ProcHandle > 0){
		close(m_ProcHandle);
		m_ProcHandle = -1;
	}
}
av_bool CAvProc::Initialize()
{
#if defined(WIN32)

#else
	m_ProcHandle = open("/dev/logApp", O_WRONLY);
	if (m_ProcHandle <= 0){

	}
	else{
		dup2(m_ProcHandle, STDOUT_FILENO);
		dup2(m_ProcHandle, STDERR_FILENO);
	}
#endif

	return av_true;
}
av_bool CAvProc::avProcSet(IOCTRL_CMD cmd, av_void *data, av_u32 datalen)
{
	int iRet = 0;
#if defined(WIN32)

#else
	iRet = ioctl(m_ProcHandle, cmd, data);
#endif
	return iRet == 0 ? av_true : av_false;
}