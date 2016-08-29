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
#ifndef AVWARE_AVNETSERVICE_AVWEB_H_
#define AVWARE_AVNETSERVICE_AVWEB_H_

#include <map>
#include <list>
#include "Apis/AvWareType.h"
#include "CAvObject.h"
#include "WebService/LibWebService.h"
#include "AvNetService/AvWebProcess.h"

class CAvPacket;
class CAvWebSnap;

class CAvWebSer:public CWebService
{
public:
	SINGLETON_DECLARE(CAvWebSer);
	av_bool Start();
	av_bool Stop();

private:
	CAvWebSer();
	~CAvWebSer();

private:

	int WebCallBackDataProc(std::string InBuf, std::string &OutBuf);
	int WebCallBackGetSnap(int Channel, std::string &SnapData);

	void RegisterMsgProc();
	typedef std::map<std::string, WebMsgProc> MAP_LIST;
	MAP_LIST map_webmsgproc_;
};

#define g_AvWebSer (*CAvWebSer::instance())

#endif //AVWARE_AVNETSERVICE_AVWEB_H_
