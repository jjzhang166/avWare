#include "AvProc/AvProc.h"
#include "Apis/AvWareCplusplus.h"


SINGLETON_IMPLEMENT(CAvProc);
av_int CAvProc::m_ProcHandle = -1;
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
	m_ProcHandle = open("avWare.log", _O_WRONLY | _O_CREAT );
	if (m_ProcHandle <= 0){
		av_error("open %s error\n", logAvWarePath);
	}
	else{
		dup2(m_ProcHandle, 1);
		dup2(m_ProcHandle, 2);
	}
#else
	m_ProcHandle = open(logAvWarePath, O_WRONLY);
	if (m_ProcHandle <= 0){
		av_error("open %s error\n", logAvWarePath);
	}
	else{
		dup2(m_ProcHandle, STDOUT_FILENO);
		dup2(m_ProcHandle, STDERR_FILENO);
	}
#endif

	av_msg("%-20s Over\n", __FUNCTION__);
	return av_true;
}
av_bool CAvProc::avProcSet(IOCTRL_CMD cmd, av_void *data)
{
	int iRet = 0;
#if defined(WIN32)

#else
	iRet = ioctl(m_ProcHandle, cmd, data);
#endif
	return iRet == 0 ? av_true : av_false;
}