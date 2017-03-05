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
#ifndef _AVWARETYPE_H_
#define _AVWARETYPE_H_


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#if defined (WIN32)

#include <io.h>

#include <WS2tcpip.h>
#include   <windows.h>
#elif defined (__GNUC__)
#include <sys/sysinfo.h>
#include "sys/prctl.h"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/reboot.h>
#include <sys/uio.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <errno.h>
#else

#endif
#ifdef _AV_WARE_
#include "config.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif


#if defined(WIN32)
#define socklen_t int
#else
#define INVALID_SOCKET -1
#endif


typedef int					av_32,		av_sint, av_int;
typedef unsigned int		av_u32,		av_uint;
typedef short				av_16,		av_sshort, av_short;
typedef unsigned short		av_u16,		av_ushort;
typedef double				av_double;
typedef float				av_float;
typedef char				av_char,	av_s8, av_8;
typedef unsigned char		av_uchar,	av_u8;
typedef void				av_void;
typedef long				av_long;
#ifdef WIN32
typedef __int64                 av_64,	av_slonglong;
typedef unsigned __int64        av_u64, av_ulonglong;
#elif defined(__GNUC__)
typedef long long               av_64,	av_slonglong;
typedef unsigned long long      av_u64, av_ulonglong;
#elif defined(__TCS__)
typedef signed   long long int  av_64,	av_slonglong;
typedef unsigned long long int  av_u64, av_ulonglong;
#endif


//////////////////////////////////////////////////////////////////////////
//##配置表默认信息



#ifndef SYS_LOCALCAPTURE_CHN
#define SYS_LOCALCAPTURE_CHN 1
#endif
#ifndef SYS_CHN_NUM
#define SYS_CHN_NUM 32
#endif

#ifndef _AV_WARE_M_HAVE_UI_CLOSE_CAPUTRE
#define SYS_REMOTECAPTURE_CHN (SYS_CHN_NUM-SYS_LOCALCAPTURE_CHN)
#else
#define SYS_REMOTECAPTURE_CHN SYS_CHN_NUM
#endif


#define AV_WARE_COVER_RECT_WIDTH 10000
#define AV_WARE_COVER_RECT_HEITH 10000

#ifndef ConfMaxSerial
#define ConfMaxSerial			4
#endif

#ifndef ConfMaxCover
#define ConfMaxCover			4
#endif

#ifndef ConfMotionDetectionRows 
#define  ConfMotionDetectionRows 22
#endif

#ifndef ConfMotionDetectionLine
#define ConfMotionDetectionLine 18
#endif

#ifndef ConfMaxNetComm
#define ConfMaxNetComm			5
#endif

#ifndef ConfMaxIoAlarmIn
#define ConfMaxIoAlarmIn		1
#endif

#ifndef ConfMaxUser
#define ConfMaxUser 10
#endif

#ifndef AvMaxWeeks
#define AvMaxWeeks 7
#endif

#ifndef AvMaxTimeArea
#define AvMaxTimeArea 3
#endif


//////////////////////////////////////////////////////////////////////////

//四舍五入 取倍数
#define MultipleRound(target,multiple) (target) = ((((target)+(multiple)/2)/(multiple))*multiple)
//取最大的的倍数
#define MultipleMax(target,multiple) (target) = ((((target)+(multiple - 1))/(multiple))*multiple)
//取最小的倍数
#define MultipleMin(target,multiple) (target) = (((target)/(multiple))*(multiple))


#define AppRunMSec() (long)(clock()*(1000.0/CLOCKS_PER_SEC))

#if defined(WIN32)

#if _MSC_VER < 1400
	#ifndef snprintf
	#define snprintf _snprintf
	#endif
#else
	#ifndef snprintf
	#define snprintf sprintf_s
	#endif
#endif

#ifndef strcasecmp
#define strcasecmp _stricmp
#endif

#ifndef strncasecmp
#define strncasecmp _strnicmp
#endif
#else
#define sprintf_s sprintf
#endif

typedef enum{
	av_false = 0,
	av_true	 = 1,
}av_bool;

typedef struct{
	char *base;
	int   len;
	int   size;
}av_buf;


typedef void av_mutex;
typedef void av_sem;
typedef void av_cond;


#if defined(WIN32)
#include <pthread.h>
#include <semaphore.h>
typedef pthread_t av_thread_t;
#else
typedef pthread_t av_thread_t;
#endif

//avWare func for bsp call
extern av_void *AvMemoryPoolMalloc(size_t size);
extern av_void *AvMemoryPoolRealloc(av_void *ptr, size_t size);
extern av_void  AvMemoryPoolFree(av_void *ptr);


#ifdef HAVE_MemPool
#define AvMalloc AvMemoryPoolMalloc
#define AvFree AvMemoryPoolFree
#define AvRealloc AvMemoryPoolRealloc
#else
#define AvMalloc malloc
#define AvFree free
#define AvRealloc realloc
#endif



#define AvMask(x) (0x01 << (x))

#define av_findMaxMask(result, SupportMask, type) \
	do \
	{\
		for(int i = 31; i >= 0; i--){\
			if (AvMask(i) & (SupportMask)){\
				(result) = (type)(i);\
				break;\
			}\
		}\
	} while (0)

#define av_findMinMask(result, SupportMask, type) \
	do\
	{\
		for(int i = 0; i < 32; i++){\
			if (AvMask(i) & (SupportMask)){\
				(result) = (type)i;\
				break;\
			}\
		}\
	} while (0)






#define AvStrSplice(h,t) (#h##t)
#if defined(WIN32)
#define AvConfigFile(f) "./configs/"##f
#else
#define AvConfigFile(f) f
#endif


#if defined(WIN32)
#pragma warning(disable:32)
#pragma warning(disable:34)
#pragma warning(disable:36)
#endif


#if defined(WIN32)
#ifndef PATH_HR
#define PATH_HR '\\'
#endif
#define av_warning(fmt,...) do {printf("[%s][%d]=>", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__);SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);printf(fmt, ##__VA_ARGS__); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}while(0)
#define av_error(fmt,...) do {printf("[%s][%d]=>", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__);SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);printf(fmt, ##__VA_ARGS__); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); }while(0)
#define av_msg(fmt,...) do {printf("[%s][%d]=>", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__);SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); printf(fmt, ##__VA_ARGS__); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); }while(0)

#else
#define PATH_HR '/'
#define av_warning(fmt,...) printf("[%s][%d]=>\033[1;33m" fmt "\033[m", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__,##__VA_ARGS__)
#define av_error(fmt,...) printf("[%s][%d]=>\033[0;32;31m" fmt "\033[m", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__,##__VA_ARGS__)
#define av_msg(fmt,...) printf("[%s][%d]=>\033[1;32m" fmt "\033[m", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__,##__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif

