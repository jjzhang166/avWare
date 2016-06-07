/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef __DH_WEB_HTTP_TASK_H__
#define __DH_WEB_HTTP_TASK_H__


#include "Apis/AvWareType.h"
#include "CObject.h"
#define WebPrintf av_msg
#define WebPerror av_error

#include <pthread.h>
#include <vector>
#include <string>
#include <semaphore.h>
#include <errno.h>



#include "CObject.h"
#define RECV_BUF_SIZE 2*1024



typedef enum __EnFileType
{
	EnFileType_Html,
	EnFileType_Text,
	EnFileType_Jpeg,
	EnFileType_Gif,
	EnFileType_Binary,
	EnFileType_Png,
	EnFileType_Cgi,
	EnFileType_Json,
	EnFileType_Xml,
	EnFileType_Default,
} EnFileType;

typedef int(CObject::*WebCallBack)(std::string InBuf, std::string &OutBuf);

class CWorkTask:public CObject
{   
public:   
 	CWorkTask();
	~CWorkTask();
	int Run();
	int SetSocket(int fdSock);//将socket传进来
	int SetWebAttr(std::string WebRoot, std::string WebIndex);

	int SetCallBack(CObject *obj, WebCallBack proc);
	int GetSocket();
	int HandleConnect(int fdSock,int iTimeout = 5000000);
	int ParseAndHandReq(int fdSock, char *pData, int iLen);
	int PacketHttpHeader(EnFileType content_type,std::string &strBuf,int fileLength);
	int SendNoBlock(int sockfd, char *buf,int len,unsigned int iTimeout = 5000000);
	int DoDir(int fdSock, char *name);
	int DoScriptFile(int fdSock, char *name, EnFileType EnFileType);
	int DoStaticFile(int fdsock, char *name, EnFileType EnFileType);
	int DoDataFile(int fdSock, std::string jsondata);
	int cpFile(int fdSock,char *name, EnFileType EnFileType);
	int CheckValidMsg(char* bufMsg,int lenMsg);

	int RunLongCon();
	int HandleLongConnect(int fdSock,int iTimeout = 5000000);


private:
	char m_chRecvBuf[RECV_BUF_SIZE];
	int m_ProcSock;//要处理的socket

	std::string m_WebRoot;
	std::string m_IndexHtml;

	CObject *m_CbObj;
	WebCallBack  m_CbProc;
};   


/**  
//线程池
**/  
   

class CWebThreadPool
{  
public:  
	CWebThreadPool();
	CWebThreadPool(int threadNum);
	void InitPool(int threadNum);
	int AddTask(CWorkTask *task);      //把任务添加到线程池中   
	int StopAll(); 
	int Create();          //创建所有的线程 
	static void* ThreadProc(void * arg); //新线程的线程函数
	void ProcessTaskProc();
private:   
	std::vector <CWorkTask*> m_vecTaskList;         //任务列表   
	int m_iThreadNum;                            //线程池中启动的线程数 
	pthread_mutex_t m_pthreadMutex;    //线程同步锁   
	sem_t m_Semaphore;

  	
};  
#endif

