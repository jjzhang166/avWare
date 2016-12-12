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
#ifndef __USER_DEFINE_H__
#define __USER_DEFINE_H__

#include "stdio.h"
#include "string.h"
#include "stdlib.h"


#ifdef	WIN32

#ifndef	USER_DEBUG
#define USER_DEBUG
#endif

#else

#ifndef USER_DEBUG
#define USER_DEBUG(msg...) printf("[ %s, %d ]=> ",__FILE__,  __LINE__);printf(msg);printf("\r\n");
#endif

#endif

char *usr_str_btweenpoint(char *pfont,char  *pbehind,char  *str);
int usr_strncasecmp(char *str1,char *str2,int len);
int usr_strcasecmp(char *str1,char *str2);
char* usr_strrstr_nohead(char  *p,char* str);
char* usr_strrstr_btweenpoint(char  *phead,char  *pend,char* str);
int usr_strnum_btweenpoint(char  *pfont,char  *pbehind,char  *str);
int usr_getvalue_btweenpoint(char  *phead,char  *pend, char  *out);
int usr_printf_btweenpoint(char  *ps,char  *pe);

#define STR_BTPOINT(pfont,pbehind,str)		usr_str_btweenpoint(pfont,pbehind,str)
#define STRNCASECMP(str1,str2,len)			usr_strncasecmp(str1,str2,len)
#define STRCASECMP(str1,str2)				usr_strcasecmp(str1,str2)
#define STRRSTR_NOHEAD(p,str)				usr_strrstr_nohead(p,str)
#define STRRSTR_BTPOINT(pfont,pbehind,str)	usr_strrstr_btweenpoint(pfont,pbehind,str)
#define STRNUM_BTPOINT(pfont,pbehind,str)	usr_strnum_btweenpoint(pfont,pbehind,str)
#define STR_GETVALUE_BTPOINT(pfont,pbehind,out) usr_getvalue_btweenpoint(pfont,pbehind,out)
//#define DEBUG_BTPOINT(fileName,lineNum,ps,pe);	usr_printf_btweenpoint(fileName,lineNum,ps,pe);

#ifdef	WIN32
#ifndef	DEBUG_BTPOINT
#define DEBUG_BTPOINT
#endif

#else

#ifndef	DEBUG_BTPOINT	
#define DEBUG_BTPOINT(ps,pe,msg...); printf("[ %s, %d ]=> ",__FILE__,  __LINE__);printf(msg);usr_printf_btweenpoint(ps,pe);printf("\r\n");
#endif

#endif

#endif

