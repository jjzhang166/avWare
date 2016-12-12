/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: FootMan [FootMan@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef __INCLUDE__COMMON_H__
#define __INCLUDE__COMMON_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "pthread.h"
#include "list.h"

#if  defined(WIN32)
#include <io.h>
#include <WS2tcpip.h>
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <dirent.h>

#endif


#if defined(WIN32)
#ifndef inline
#define inline __inline
#endif
#define snprintf _snprintf
#ifndef strcasecmp
#define strcasecmp _stricmp
#endif

#ifndef strncasecmp
#define strncasecmp _strnicmp
#endif
#else
#define sprintf_s sprintf
#endif


#if defined(WIN32)
#define OnvifCloseSock(fd) closesocket(fd)
#else
#define OnvifCloseSock(fd) close(fd)
#endif

#ifdef  WIN32
#ifndef PATH_HR
#define PATH_HR '\\'
#endif
#ifndef DF_DEBUG
#define DF_DEBUG(fmt,...) do {printf("[%s][%d]=>", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__);SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);printf(fmt, ##__VA_ARGS__); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}while(0)
#endif

#else

#ifndef DF_DEBUG
#define DF_DEBUG(msg...) printf("[ %s,%d]=> ",__FILE__,__LINE__);printf(msg);printf("\r\n");
#endif

#endif

#ifndef KEY_FALSE
#define KEY_FALSE				-1
#endif

#ifndef KEY_TRUE
#define KEY_TRUE				0
#endif

#ifndef KEY_PROBE
#define KEY_PROBE				1
#endif



#ifndef WIN32
#define  SleepMs(ms) usleep((ms)*1000)
#define _printd(fmt, ...)	printf ("[%s][%d]"fmt"\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
#define SleepMs(ms) Sleep(ms)
#define _printd(fmt, ...)	printf ("[%s][%d]"fmt"\n", (char *)(strrchr(__FILE__, '\\') + 1), __LINE__ - 2, ##__VA_ARGS__)
#endif

#endif
