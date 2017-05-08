#include "AvConfigs/AvConfigManagerNetCapture.h"
#include "AvDevice/AvDevice.h"

SINGLETON_IMPLEMENT(CAvConfigManagerNetCapture);



CAvConfigManagerNetCapture::CAvConfigManagerNetCapture() 
	:CAvConfigManagerBase(AvConfigFileNameNetCapture, AvConfigFileNameNetCaptureBak)
{

}


CAvConfigManagerNetCapture::~CAvConfigManagerNetCapture()
{

}

void CAvConfigManagerNetCapture::Initialize()
{
	std::string ConfigPath;
	bool bLoadConfigFlag = false;
	av_bool abRet = CAvDevice::GetEnv(std::string(EKey_ConfigsPath), ConfigPath);
	if (abRet == av_true){
		SetAvConfigPath(ConfigPath, AvConfigFileNameNetCapture, AvConfigFileNameNetCaptureBak);
	}

	if (av_false == LoadConfigFromFile()) {
		av_warning("Load config from config file failed\n");
	}
	else{
		bLoadConfigFlag = true;
	}
	SetUpConfig("NetProtocol", m_ConfigNetProtocol, CONF_NETPROTOCOL);

	if (bLoadConfigFlag == false){
		av_msg("bLoadConfigFlag == false, save file\n");
		WriteFile();
	}


}


int CAvConfigManagerNetCapture::RestoreDefaultAllGeneralConfig()
{
	return 0;
}