#ifndef _LIBUVCPP_H_
#define _LIBUVCPP_H_
#include <cstdio>
#include <iostream>
#include <list>
#include "Apis/AvWareLibDef.h"
typedef enum {
	SOCK_TCP	 = 0x01,
	SOCK_UDP	 = 0x02,

	SOCK_LOCAL		= 0x010000,
	SOCK_REMOTE		= 0x020000,
	SOCK_Multicast	= 0x040000,
}SockAttr;

typedef enum{
	SOCK_ENREAD		= 0X01,
	SOCK_ENWRITE	= 0X01 << 1,
	SOCK_CLOSE		= 0X01 << 2,
}SOCKSTATUS;

typedef enum {
	NV_SUCCEED	= 0,
	NV_FAILD	= 1,
	NV_CONNECT	= 2,
	NV_REMOTECONNECT = 3,
	NV_CLOSEOVER	 = 4,
	NV_DROPCONNECT,
	NV_WRITE_ERR,
	NV_READ_ERR,
	
}NetUvStatus;


typedef struct {
	char *base;
	int   len;
	int   size;
}CNetUvBuff_t;

#ifdef WIN32
typedef struct {
	unsigned long len;
	char* base;
}buf_t;
#else
typedef struct {
	char* base;
	size_t len;
}buf_t;
#endif



typedef void (*WorkerFunc_t)(void *args);
typedef void (*WorkerFuncAfter_t)(void *args);

class AVWARE_API CNetUv{
public:
	CNetUv();
	~CNetUv();
public:
	bool				Start(const char *address, const int port, int attr);
	int					WriteAsync(buf_t *bufbase, int nbufs);
	int					WorkerTask(WorkerFunc_t worker, WorkerFuncAfter_t workerafter, void *args);
	virtual int			OnRead(const buf_t *bufbase, int nbufs, struct sockaddr_in *addr = NULL);
	virtual CNetUv *	OnConnect( const char *remoteAddr, const int remotePort);
	virtual void		OnTimer100Ms();
	virtual int			OnEvent(NetUvStatus nvi, const char *message) = 0;
	void				SetUsrData(void *data);
	void *				GetUsrData();

public:
	void				SetMulticaseAddr(const char *addr);
	void				MulticaseGroupJoin(const char *MulticaseAddr, const char *localAddr);
	void				MulticaseGroupLeave(const char *MulticaseAddr, const char *localAddr);
	char *				GetMulticaseAddr();
	void				SetmSock(void *Sock);
	void *				GetmSock();
	void				SetmSockType(int sockattr);
	int					GetmSockType();
	void				SetmLocalAddr(const char *addr);
	char *				GetmLocalAddr();
	void				SetmRemoteAddr(const char *addr);
	char *				GetmRemoteAddr();
	void				SetmLocalPort(int port);
	int					GetmLocalPort();
	void				SetmRemotePort(int port);
	int					GetmRemotePort();
	
	CNetUvBuff_t*		GetmReadBuffer();
	int					SetSockStatus(unsigned int status);
	unsigned int		GetSockStatus();

private:
	CNetUvBuff_t		m_ReadBuffer;
	void *				m_usrdata;
	void *				m_sock;
	void *				m_SendOverlapped;
	int					m_sockAttr;
	unsigned int		m_sockStatus;
	char				m_remoteAddr[32];
	int					m_remotePort;
	char				m_localAddr[32];
	int					m_localPort;
	char				m_MulticastAddr[32];
	void				*m_WriteMutex;

public:
	static void *		 mp_loop;
	static void *		 mp_timer;
	static void *		 mp_async;
	static void *		 mp_async_mutex;
	static void *		 mp_sem_async;
	static void *		 mp_thread;
	static void *		 mp_thread_sem;
	static bool			 muv_inited;
	
	static void *		 mp_CloseSock_async;
	static void *		 mp_CloseSock_sem;
	static void *		 mp_CloseSock_mutex;
	static void *		 mp_timerList;
};





#define CNetUvMutex_t void *
AVWARE_API CNetUvMutex_t		CNetUvMutexCreate();
AVWARE_API void				CNetUvMutexLock(CNetUvMutex_t mutex);
AVWARE_API void				CNetUvMutexUnlock(CNetUvMutex_t mutex);
AVWARE_API int					CNetUvMutexTrylock(CNetUvMutex_t mutex);
AVWARE_API void				CNetUvMutexRelease(CNetUvMutex_t mutex);
AVWARE_API void				uv_msleep(int ms);

#define CNetUvSem_t void *
AVWARE_API CNetUvSem_t			CNetUvSemCreate();
AVWARE_API int					CNetUvSemWait(CNetUvSem_t sem);
AVWARE_API int					CNetUvSemPost(CNetUvSem_t sem);
AVWARE_API int					CNetUvSemRelease(CNetUvSem_t sem);

typedef void(* ThreadTask_t)(void *args);

AVWARE_API int CnetUvThreadCreate(ThreadTask_t Func, void *args);

 
#endif
