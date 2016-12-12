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
#ifndef __USRMEM_H__
#define __USRMEM_H__
#include "list.h"

typedef struct _MemHandle_S{
	void *mem_addr;
	struct listnode _list;
}MemHandle_S;

MemHandle_S *CreateMemHandle();
char *usr_malloc(MemHandle_S *handle,int len);
char *usr_free(MemHandle_S *handle);
char *usr_strdup(MemHandle_S *handle,char *str);
char* usr_inttostr(MemHandle_S *handle,int value);
char* usr_floattostr(MemHandle_S *handle,float value);
char* usr_addstr_intostr(MemHandle_S *handle,int size,char *str1,char *str2);
#endif
