#include "AvConfigs/AvConfigManagerGeneral.h"
#include "AvConfigs/AvConfigIndex.h"
#include "AvDevice/AvDevice.h"
#ifdef WIN32
#define  AvConfigFileName "avWare1.json"
#define  AvConfigFileNameBak "avWare2.json"
#else
#define  AvConfigFileName "avWare1.conf"
#define  AvConfigFileNameBak "avWare2.conf"
#endif

SINGLETON_IMPLEMENT(CAvConfigManagerGeneral);

CAvConfigManagerGeneral::CAvConfigManagerGeneral() :
	CAvConfigManagerBase(AvConfigFileName, AvConfigFileNameBak)
{
}

CAvConfigManagerGeneral::~CAvConfigManagerGeneral()
{
}

void CAvConfigManagerGeneral::Initialize()
{
	std::string ConfigPath; 
	bool bLoadConfigFlag = false;
	av_bool abRet = CAvDevice::GetEnv(std::string(EKey_ConfigsPath), ConfigPath);
	if (abRet == av_true){
		SetAvConfigPath(ConfigPath, AvConfigFileName, AvConfigFileNameBak);
	}

	if (av_false == LoadConfigFromFile()) {
		av_warning("Load config from config file failed\n");
	}
	else{
		bLoadConfigFlag = true;
	}

	SetUpConfig("DeviceUart", m_config_device_uart, CONF_DEVICE_UART);

	SetUpConfig("Capture", m_ConfigCapture, CONF_CAPTURE_FORMATS);
	SetUpConfig("Image", m_ConfigImage, CONF_IMAGE_FORMATS);

	SetUpConfig("Enocde", m_ConfigEncode, CONF_ENCODE_FORMATS);
	/*
	//for test zh 20170301
	m_ConfigEncode.Update();
	ConfigEncodeProfile &Formats = m_ConfigEncode.GetConfig(0);
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++) {
		av_warning("Enable[%d], FrameRate[%d], Gop[%d] ImageSize[%d] BitRateValue[%d]\n",
			Formats.CHLProfile[i].Enable,
			Formats.CHLProfile[i].Profile.FrameRate,
			Formats.CHLProfile[i].Profile.Gop,
			Formats.CHLProfile[i].Profile.ImageSize,
			Formats.CHLProfile[i].Profile.BitRateValue);
	}
	*/
	
	SetUpConfig("Cover", m_ConfigCover, CONF_ENCODE_COVER);
	SetUpConfig("OverLay", m_ConfigOverLay, CONF_ENCODE_OverLay);

	SetUpConfig("Alarm", m_ConfigAlarm, CONF_ALARM);


	SetUpConfig("NetComm", m_ConfigNetComm, CONF_NET_COMM);
	SetUpConfig("NetProtocol", m_ConfigNetProtocol, CONF_NETPROTOCOL);

	SetUpConfig("Audio", m_ConfigAudio, CONF_AUDIO);
	
	SetUpConfig("UserManager", m_config_user_manager, CONF_USER_MANAGER);
	SetUpConfig("NetServerSmtp", m_confignet_smtp, CONF_NET_SER_EMAIL);
	SetUpConfig("NetServerFtp", m_confignet_ftp, CONF_NET_SER_FTP);
	SetUpConfig("NetServerDdns", m_confignet_ddns, CONF_NET_SER_DDNS);
	SetUpConfig("NetServerUpnp", m_confignet_upnp, CONF_NET_SER_UPNP);
	SetUpConfig("NetServerNtp", m_confignet_ntp, CONF_NET_SER_NTP);
	SetUpConfig("NetProtoRtmp", m_config_rtmp,CONF_PROTO_RTMP);
	SetUpConfig("NetP2p", m_config_p2p, CONF_NET_P2P);
	SetUpConfig("NetRtsp", m_config_rtsp, CONF_NET_RTSP);
	SetUpConfig("Record", m_config_record, CONF_RECORD);
	SetUpConfig("RecordCtrl", m_config_recordctrl, CONF_RECORDCTRL);
	SetUpConfig("PtzAdvancedCameraLens", m_config_ptzCameraLens, CONF_PTZADVANCEDCAMERSLENS);

	if (bLoadConfigFlag == false){
		av_msg("bLoadConfigFlag == false, save file\n");
		WriteFile();
	}
}


int CAvConfigManagerGeneral::RestoreDefaultAllGeneralConfig()
{
	return RestoreDefaultAll();
}
