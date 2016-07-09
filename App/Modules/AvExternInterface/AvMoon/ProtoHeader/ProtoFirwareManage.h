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
#ifndef _PROTOFIRWAREMANAGE_H_
#define _PROTOFIRWAREMANAGE_H_

#include "ProtoStruct.h"

#ifdef __MOONPROTO_ //FOR VFWARE
#include "system/Object.h"
#include "system/SignalS.h"
#else
#include "Object.h"
#include "SignalS.h"
#endif


#ifdef PROTOSHARE_EXPORTS

#if defined (WIN32)

#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllexport)
#endif

#else //else define win32
#define PROTOSHARE_API __attribute__((visibility("default")))
#endif //end define win32

#else //else PROTOSHARE_EXPORTS

#if defined(WIN32)
#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllimport)
#endif

#else
#define PROTOSHARE_API
#endif

#endif //end PROTOSHARE_EXPORTS

typedef struct{
	char FirmWarePath[128];
	char FirmWareName[64];
	C_FirmwareInfo FirmwareInfo;
	int  FirmwareSize;
	unsigned char *FirmwareAddr;
}C_FirmWareManageMem;

class PROTOSHARE_API CFirmWareManage
{
public:
	CFirmWareManage();
	~CFirmWareManage();
	SINGLETON_DECLARE(CFirmWareManage);
public:
	//外部使用
	int FirmWareManageReSet();
	int FirmWareManageAdd(const char *Path);
	int FirmWareManageDel(const char *Path);
	int FirmWareManageGetFirmWareInfo(C_FirmwareInfo &FirmwareInfo, const char *FileName);
	//协议内部使用
	int FirmWareManageGetFirmWareInfoList(std::list <C_FirmWareManageMem> FirmWaremanageList);

private:
	std::list <C_FirmWareManageMem> m_FirmWaremanageMem;
};


#define g_StreamManager (*CFirmWareManage::instance())











#endif