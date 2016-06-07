#ifndef _DDNS_GLOBAL_H
#define _DDNS_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__
#include <malloc.h>

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>
#include <errno.h>
#include <dirent.h> 
#include <netdb.h> 
#include <semaphore.h>

#include <linux/un.h>
//#include <linux/videodev.h>
#include <linux/fb.h>
#include <linux/un.h>
#include <linux/soundcard.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>

#include <arpa/inet.h>
#include <netinet/in.h> 
#include <netinet/tcp.h>

#else
#include <winsock2.h>
#if _MSC_VER < 1400
#define snprintf _snprintf
#else
#define snprintf sprintf_s
#endif
#endif

#ifndef  TRUE  
#define  TRUE    (1)
#endif
#ifndef  FALSE   
#define  FALSE   (0)
#endif

#ifndef  VOID 
#define  VOID    void
#endif
#ifndef  HANDLE
#define  HANDLE  void *
#endif

#if defined(WIN32)
#include   <windows.h>
#define PATH_HR '\\'
#define DDNS_ERROR(fmt,...) do {printf("[%s][%d]=>", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__);SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);printf(fmt, ##__VA_ARGS__); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}while(0)
#define DDNS_DEBUG(fmt,...) do {printf("[%s][%d]=>", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__);SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); printf(fmt, ##__VA_ARGS__); SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);}while(0)

#else
#define PATH_HR '/'
#define DDNS_ERROR(fmt,...) printf("[%s][%d]=>\033[0;32;31m" fmt "\033[m", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__,##__VA_ARGS__)
#define DDNS_DEBUG(fmt,...) printf("[%s][%d]=>\033[1;32m" fmt "\033[m", NULL == strrchr(__FILE__, PATH_HR)? __FILE__:(char *)(strrchr(__FILE__, PATH_HR) + 1), __LINE__,##__VA_ARGS__)
#endif

#define DDNS_EXT_DEBUG  1
#if DDNS_EXT_DEBUG
#define DDNS_TRACE         {printf("[ %s, %d ]=> ",__FILE__,  __LINE__);printf("\r\n");}

#else
#define DDNS_TRACE         

#endif

#endif  //_DDNS_GLOBAL_H

