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
#include "AvNetService/AvNetService.h"
#include "AvNetService/AvRtmp.h"
#include "AvNetService/AvOnvif.h"
#include "AvNetService/AvRtsp.h"
#include "AvNetService/AvWeb.h"
#include "AvNetService/AvDdnsCli.h"
#include "AvNetService/AvNtpCli.h"
//#include "AvProtocol/Moon/AvMoon.h"
#include "AvNetService/AvProtoMoon.h"



SINGLETON_IMPLEMENT(CAvNetService);
CAvNetService::CAvNetService()
{
}
CAvNetService::~CAvNetService()
{

}
av_bool CAvNetService::Initialize()
{
	av_msg("%s Started\n", __FUNCTION__);
	{
		m_ConfigRtmp.Update();
		m_ConfigRtmp.Attach(this, (AvConfigCallBack)&CAvNetService::OnConfigRtmpModify);
	}
	av_bool ret = Start();


	//CThread::run();
	return ret;
}

av_bool CAvNetService::Start()
{
	av_bool ret = av_false;
	g_AvProtoMoon.Start();
	ret = g_AvOnvifSer.Start();
	ret = g_AvRtspServer.Start();
	ret = g_AvWebSer.Start();

	{
		AvRtmp *pRtmp = NULL;
		std::string RtmpUrl;
		for (int i = 0; i < SYS_CHN_NUM; i++){
			RtmpUrl.clear();
			ConfigRtmp &RtmpConfig = m_ConfigRtmp.GetConfig(i);
			if (RtmpConfig.RtmpFormats.bEnable == av_true){
				pRtmp = new AvRtmp(AvRtmp::RTMP_PUSH);
				RtmpUrl.append(RtmpConfig.RtmpFormats.RtmpAddress);
				RtmpUrl.append("/");
				RtmpUrl.append(RtmpConfig.RtmpFormats.RtmpString);
				pRtmp->Start(i, RtmpConfig.RtmpFormats.Slave, RtmpUrl);
				m_AvRtmpMap[i] = pRtmp;
			}
		}
	}

	return ret;
}
av_bool CAvNetService::Stop()
{
	av_bool ret = av_false;
	g_AvProtoMoon.Stop();
	ret = g_AvOnvifSer.Stop();
	ret = g_AvRtspServer.Stop();
	ret = g_AvWebSer.Stop();
	return ret;
}
av_bool CAvNetService::Restart()
{
	av_bool ret = av_false;
	Stop();
	Start();
	return ret;
}
av_void CAvNetService::ThreadProc()
{
	while (m_Loop == av_true)
	{
		av_msleep(100);
	}
}
av_void CAvNetService::OnConfigRtmpModify(CAvConfigRtmp *configRtmp, int &result)
{
	AvRtmp *pRtmp = NULL;
	std::string RtmpUrl;
	av_warning("OnConfigRtmpModify\n");
	for (int i = 0; i < SYS_CHN_NUM; i++){
		ConfigRtmp &OldConfig = m_ConfigRtmp.GetConfig(i);
		ConfigRtmp &NewConfig = configRtmp->GetConfig(i);
		if (0 != memcmp(&OldConfig, &NewConfig, sizeof(ConfigRtmp))){
			pRtmp = m_AvRtmpMap[i];
			m_AvRtmpMap[i] = NULL;

			if (pRtmp != NULL){
				pRtmp->Stop();
				av_warning("Stop i = %d RTMP\n", i);
				delete pRtmp;
			}

			if (NewConfig.RtmpFormats.bEnable == av_true){
				RtmpUrl.clear();
				pRtmp = new AvRtmp(AvRtmp::RTMP_PUSH);
				RtmpUrl.append(NewConfig.RtmpFormats.RtmpAddress);
				RtmpUrl.append("/");
				RtmpUrl.append(NewConfig.RtmpFormats.RtmpString);
				pRtmp->Start(i, NewConfig.RtmpFormats.Slave, RtmpUrl);
				m_AvRtmpMap[i] = pRtmp;

				OldConfig = NewConfig;
			}

		}
	}

	result = 1;
}