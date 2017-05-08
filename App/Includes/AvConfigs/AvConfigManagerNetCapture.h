/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2017年3月31日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/

#ifndef _AVCONFIG_MANAGER_NETCAPTURE_H_
#define _AVCONFIG_MANAGER_NETCAPTURE_H_

#include "AvConfigs/AvConfigManagerBase.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvConfigs/AvConfigIndex.h"

class CAvConfigManagerNetCapture :public CAvConfigManagerBase
{
public:
	SINGLETON_DECLARE(CAvConfigManagerNetCapture);
	CAvConfigManagerNetCapture();
	~CAvConfigManagerNetCapture();
	virtual void Initialize();

	int RestoreDefaultAllGeneralConfig();

private:
	CAvConfigProtocol				m_ConfigNetProtocol;
};


#endif //_AVCONFIG_MANAGER_NETCAPTURE_H_