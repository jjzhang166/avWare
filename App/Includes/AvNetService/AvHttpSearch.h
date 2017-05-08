#ifndef _AV_HTTPSEARCH_H_
#define _AV_HTTPSEARCH_H_
#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvThread/AvThread.h"
#include "CAvObject.h"
#include "AvNetService/AvRtmp.h"


/*
	1 线程体未使用系统，debug 时，需要特殊考虑。
	2 线程体未分离，线程体未标名。
	3 json 消息写到WebProcess 共同处理。
*/

class CHttpSearchServer{
public:
	SINGLETON_DECLARE(CHttpSearchServer);
	CHttpSearchServer();
	~CHttpSearchServer();

	av_int	Start();
	av_void Stop();


private:
	enum{
		TYPE_ALL,
		TYPE_IPC,
		TYPE_NVR,
		TYPE_PLATFORM,
		TYPE_NR,
	};

private:
	av_void			ThreadProc();
	av_bool			codec(const char* recv_buff, std::string& resp_buff);
	av_void			NetUdpSendMsg(const struct sockaddr *addr, const char *msg, int len);
	av_int			NetMulicastServerSocketInit(const char *mulicastip, const av_u16 mulicastPort);
private:
	static av_char*	SearchType[TYPE_NR];
	static av_void*	ThreadProcProxy(void *param);

private:
	const av_char *const	mulicastip_		= "239.255.255.250";
	const av_u16			mulicastPort	= 3703;
	av_int					serverSocketFd_;
	pthread_t				pid_;
	av_bool					thread_run_;
	DISALLOW_COPY_AND_ASSIGN_11(CHttpSearchServer);
};

#define g_HttpSearchServer (*CHttpSearchServer::instance())


#endif
