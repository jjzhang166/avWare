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
#include "AvDevice/AvDevice.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvLua/AvLua.h"
#include "AvProc/AvProc.h"
#include "Apis/AvEnuminline.h"
C_DeviceFactoryInfo CAvDevice::m_FactoryInfo = {0};
std::string CAvDevice::m_SStartGUID;
CAvConfigNetComm CAvDevice::m_ConfigNetComm;

SINGLETON_IMPLEMENT(CAvDevice);


CAvDevice::CAvDevice()
{
	m_TimerDeviceCount = 0;
	m_TimerDeviceRebootTime = 0;
}
CAvDevice::~CAvDevice()
{ 


}



av_bool CAvDevice::Initialize()
{
	av_msg("CAvDevice::Initialize Started\n");
	AvSystemInit();
	
	{//FACTOORY
		C_DeviceFactoryInfo FactoryInfo = { 0 };
		GetDeviceInfo(FactoryInfo);
	}

	{
		AvWatchDogInit(10);
	}

	{//一定要放在fac 后面，因为lua 里面有可能有auto 的选项，此时需要FAC 参数 配合。
		InitializeLua();
	}
	{// GPIO
		AvGpioInit();
	}
	{// RTC
		AvRtcInit();
		AvTimeRtc2System();
	}
	{//GUID
		av_timeval timeval;
		AvGetTimeOfDay(&timeval);
		char guid[128];
		srand((unsigned int)timeval.tv_usec);
		sprintf(guid, "%s-%04X-%04X-%08X", m_FactoryInfo.ProductMacAddr, timeval.tv_sec, timeval.tv_usec, rand());
		m_SStartGUID.clear();
		m_SStartGUID.assign(guid);
	}

#if defined(_AV_WARE_M_HAVE_PROC)
	{//wirte proc
		avWare_Device_Info DeviceInfo;
		DeviceInfo.MaxNetProtocolCapture = SYS_CHN_NUM - SYS_LOCALCAPTURE_CHN;
		DeviceInfo.MaxDecode = SYS_CHN_NUM;
		DeviceInfo.MaxDeviceCapture = SYS_LOCALCAPTURE_CHN;
		sprintf(DeviceInfo.avWareVersion, "%d.%d.%d.%s.%s", _AV_WARE_VERSION_MAJOR, _AV_WARE_VERSION_MINOR, _AV_WARE_VERSION_PATCH,
			_AV_WARE_VERSION_OEM, _AV_WARE_VERSION_RUNTIME);
		AvProcSet(IOCTRL_CMD_SET_DEVINFO, &DeviceInfo);
	}
#endif
	return av_true;
}

av_bool CAvDevice::InitializeLua()
{
	std::string Value;
	{
		Value = std::string(EnumNameAvChip(((AvChip)m_FactoryInfo.ChipType)));
		SetEnv(std::string(EKey_Chip), Value);
	}
	CAvLua initLua;
	initLua.LuaLoadfile(EInitLuaFileName);
	std::list<std::string> retList;
	retList = initLua.LuaCall2(EKey_Call2avWarePlatformConfigs, EnumNameAvChip(((AvChip)m_FactoryInfo.ChipType)));
	std::list<std::string>::iterator iList;
	if (retList.size() == 0){
		av_error("EKey_Call2avWarePlatformConfigs error\n");
		assert(0);
	}
	int cnt  =0;
	for (iList = retList.begin(); iList != retList.end(); iList++){
		switch (cnt)
		{
		case 0:
			SetEnv(std::string(EKey_DefaultRecordHomeDir), *iList);
			break;
		case 1:
			SetEnv(std::string(EKey_ConfigsPath), *iList);
			break;
		case 2:
			SetEnv(std::string(EKey_WebRoot), *iList);
			break;
		case 3:
			SetEnv(std::string(EKey_WebIndex), *iList);
			break;


		default:
			assert(0);
			break;
		}
		cnt++;
	}
	Value = initLua.LuaGlobal(EKey_DefaultIpaddr);
	if (Value.empty() == false){
		SetEnv(std::string(EKey_DefaultIpaddr), Value);
	}
	else{
		av_error("Get %s File Args[%s] NULL\n", EInitLuaFileName, EKey_DefaultIpaddr);
		assert(0);
	}
	Value = initLua.LuaGlobal(EKey_DefaultNetMask);
	if (Value.empty() == false){
		SetEnv(std::string(EKey_DefaultNetMask), Value);
	}
	else{
		av_error("Get %s File Args[%s] NULL\n", EInitLuaFileName, EKey_DefaultNetMask);
		assert(0);
	}
	Value = initLua.LuaGlobal(EKey_DefaultGateWay);
	if (Value.empty() == false){
		SetEnv(std::string(EKey_DefaultGateWay), Value);
	}
	else{
		av_error("Get %s File Args[%s] NULL\n", EInitLuaFileName, EKey_DefaultGateWay);
		assert(0);
	}
	Value = initLua.LuaGlobal(EKey_DefaultDns1);
	if (Value.empty() == false){
		SetEnv(std::string(EKey_DefaultDns1), Value);
	}
	else{
		av_error("Get %s File Args[%s] NULL\n", EInitLuaFileName, EKey_DefaultDns1);
		assert(0);
	}
	Value = initLua.LuaGlobal(EKey_DefaultDns2);
	if (Value.empty() == false){
		SetEnv(std::string(EKey_DefaultDns2), Value);
	}
	else{
		av_error("Get %s File Args[%s] NULL\n", EInitLuaFileName, EKey_DefaultDns2);
		assert(0);
	}
	Value = initLua.LuaGlobal(EKey_DefaultHostName);
	if (Value.empty() == false){
		SetEnv(std::string(EKey_DefaultHostName), Value);
	}
	else{
		av_error("Get %s File Args[%s] NULL\n", EInitLuaFileName, EKey_DefaultHostName);
		assert(0);
	}

	return av_true;
}
av_bool CAvDevice::InitializeConfigs()
{
	Start();

	CAvTimer::StartTimer(1000, this, (CAvTimer::ONTIMER_PROC)&CAvDevice::OnTimer, av_true);
	
	
	
	return av_true;
}

av_bool CAvDevice::AttachDeviceStatus(CAvObject *obj, SIG_PROC_DEVICESTATUS OnModifyStatus)
{
	int ret = 0;
	ret = m_DeviceStatusSignal.Attach(obj, OnModifyStatus);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}
av_bool CAvDevice::DetachDeviceStatus(CAvObject *obj, SIG_PROC_DEVICESTATUS OnModifyStatus)
{
	int ret = 0;
	ret = m_DeviceStatusSignal.Detach(obj, OnModifyStatus);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}
av_bool CAvDevice::DeviceSignal(EDeviceStatus _status)
{
	switch (_status)
	{
	case CAvDevice::Upgradeing:
		break;
	case CAvDevice::RebootSoon:
		m_TimerDeviceRebootTime = AvGetUpTime() + _D_AVDEVICE_REBOOT_DELAY;
		break;
	case CAvDevice::ModifyNetDevice:
		break;
	case CAvDevice::ModifySysTime:
		break;
	default:
		break;
	}
	m_DeviceStatusSignal(_status);
	return av_true;
}
av_bool CAvDevice::IsEmbeddedSystem()
{
	std::string Value;
	if (av_true == CAvDevice::GetEnv(EKey_Chip, Value)){
		AvChip avChip = AvChipStr2EnumValue(Value.c_str());
		switch (avChip)
		{
		case AvChip_H18EV100:
		case AvChip_H18EV200:
		case AvChip_H18EV201:
		case AvChip_H18C:
		case AvChip_H18A:
		case AvChip_H16CV100:
		case AvChip_H16CV200:
		case AvChip_H16CV300:
		case AvChip_H16A:
		case AvChip_H16D:
		case AvChip_H19:
		case AvChip_HIPC_RES_1:
		case AvChip_HIPC_RES_2:
		case AvChip_H20D:
		case AvChip_H21:
		case AvChip_H31:
		case AvChip_H35:
		case AvChip_H36:
		case AvChip_H3798MV100:
		case AvChip_HNVR_RES_2:
		case AvChip_S2L22M:
		case AvChip_S2L33M:
		case AvChip_S2L55M:
		case AvChip_S2L65:
		case AvChip_S2L66:
			return av_true;

		case AvChip_WINDOWS_32:
		case AvChip_WINDOWS_64:
		case AvChip_LINUX_32:
		case AvChip_LINUX_64:
		case AvChip_MAC_32:
		case AvChip_MAC_64:
			return av_false;

		case AvChip_LAST:
			break;
		default:
			break;
		}
	}

	return av_false;
}



static int GetCompileDateTime(char *szDateTime)
{

	#define MONTH_PRE_YEAR 12
	const char szEnglishMonth[MONTH_PRE_YEAR][6] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	char szMonth[12] = { 0 };
	int iYear =  0, iMonth = 0, iDay = 0, iHour  = 0, iMin = 0, iSec = 0;
	int ret = 0;
	ret = sscanf(__DATE__, "%s %d %d", szMonth, &iDay, &iYear);
	if (ret != 3){
		av_warning("sscanf error\n");
	}
	ret = sscanf(__TIME__, "%d:%d:%d", &iHour, &iMin, &iSec);
	if (ret != 3){
		av_warning("sscanf error\n");
	}
	for (int i = 0; i < MONTH_PRE_YEAR; i++)
	{
		if (strncmp(szMonth, szEnglishMonth[i], 3) == 0)
		{
			iMonth = i + 1;
			break;
		}
	}
	sprintf(szDateTime, "%02d%02d%02d", iYear%100, iMonth, iDay);
	return 0;
}

std::string CAvDevice::GetSoftVersionString()
{
	std::string Version;
	av_char ver[128];
	av_char _complileDate[128];
	GetCompileDateTime(_complileDate);

	sprintf(ver, "%d.%d.%d.%d %s %s", _AV_WARE_VERSION_MAJOR, _AV_WARE_VERSION_MINOR,
		_AV_WARE_VERSION_PATCH, _AV_WARE_VERSION_INWARD, _AV_WARE_VERSION_RUNTIME, _complileDate);
	if (0 != strlen(_AV_WARE_VERSION_OEM)){
		strcat(ver, " ");
		strcat(ver, _AV_WARE_VERSION_OEM);
	}

	Version.assign(ver);
	return Version;
}
av_u32      CAvDevice::GetSoftVersionU32()
{
	return _AV_WARE_VERSION_MAJOR << 24 | _AV_WARE_VERSION_MINOR << 16 | _AV_WARE_VERSION_PATCH << 8| _AV_WARE_VERSION_INWARD;
}



CMutex CAvDevice::m_SEnvMutex;
std::map<std::string, std::string> CAvDevice::m_SEnv;
av_bool CAvDevice::GetEnv(std::string key, std::string &value)
{
	CGuard m(m_SEnvMutex);
	if (key.size() == 0){
		return av_false;
	}
	if (m_SEnv[key].size() == 0){
		value.clear();
		return av_false;
	}
	value = m_SEnv[key];
	return av_true;
}
av_bool CAvDevice::SetEnv(std::string key, std::string &value)
{
	CGuard m(m_SEnvMutex);
	if (key.size() == 0){
		return av_false;
	}
	
	if (value == std::string("")){
		std::map<std::string, std::string>::iterator i;
		i = m_SEnv.find(key);
		if (i != m_SEnv.end()){
			m_SEnv.erase(i);
		}
	}
	else{
		m_SEnv[key] = value;
	}
	return av_true;
}


av_bool CAvDevice::Start()
{
	
	{
		//## Set NetWork
		m_ConfigNetComm.Update();
		m_ConfigNetComm.Attach(this, (AvConfigCallBack)&CAvDevice::OnConfigsNetComm);

		C_NetCommCaps NetCommCaps;
		GetNetCommCaps(NetCommCaps);
		for (int i = 0; i < NetCommT_LAST; i++)
		{
			if (AvMask(i) & NetCommCaps.NetCommMask){
		
				ConfigNetComm &Formats = m_ConfigNetComm.GetConfig(i);
				//C_NetCommAttribute NetCommAttr;
				//NetCommAttr.Enable = Formats.Enable;
				switch ((NetCommT)i)
				{
				case NetCommT_LAN0:
				case NetCommT_LAN1:
					//NetCommAttr.LanAttr = Formats.LanAttr;
					break;
				case NetCommT_Wireless:
				case NetCommT_Wireless1:
					//NetCommAttr.WirelessAttr = Formats.WirelessAttr;
					break;
				case NetCommT_SIM:
				case NetCommT_SIM1:
					//NetCommAttr.SimAttr = Formats.SimAttr;
					break;

				default:
					break;
				}
				av_bool ret = avNetCommSet(&Formats);
				if (ret != av_true){
					av_error("Set %d NetCommDev Error\n", i);
				}
			}
		}
	}	

	return av_true;
}
av_bool CAvDevice::Stop()
{
	AvSystemDeInit();

	AvTimeSystem2Rtc();



	return av_true;
}

av_bool CAvDevice::GetDspCaps(C_DspCaps &DspCaps)
{
	memset(&DspCaps, 0x00, sizeof(C_DspCaps));
	return AvGetDspCaps(&DspCaps);
}
av_bool CAvDevice::GetEncodeCaps(av_ushort Channel, C_EncodeCaps &EncodeCaps)
{
	memset(&EncodeCaps, 0x00, sizeof(C_EncodeCaps));
	av_bool ret = AvEncodeGetCaps((av_uchar)Channel, &EncodeCaps);
	return ret;
}
av_bool CAvDevice::GetDecodeCaps(av_ushort Channel)
{
	return av_true;
}
av_bool CAvDevice::GetSerialCaps(C_SerialCaps &SerialCaps)
{
	memset(&SerialCaps, 0x00, sizeof(C_SerialCaps));
	return AvSerialCaps(&SerialCaps);
}


av_bool CAvDevice::GetNetCommCaps(C_NetCommCaps &NetCommCaps)
{
	memset(&NetCommCaps, 0x00, sizeof(C_NetCommCaps));
	return avNetCommCaps(&NetCommCaps);
}
av_bool CAvDevice::SetNetCommAttribute(NetCommT comt, C_NetCommAttribute &NetCommAttribute)
{
	CAvConfigNetComm TmpConfigNetComm;
	TmpConfigNetComm.Update();

	ConfigNetComm &Formats = TmpConfigNetComm.GetConfig(comt);
	Formats = NetCommAttribute;
	TmpConfigNetComm.SettingUp();
	return av_true;
}
av_bool CAvDevice::GetNetCommAttribute(NetCommT comt, C_NetCommAttribute &NetCommAttribute)
{
	ConfigNetComm &Formats = m_ConfigNetComm.GetConfig(comt);
	switch (comt)
	{
	case NetCommT_LAN0:
		NetCommAttribute = Formats;
		break;
	case NetCommT_LAN1:
		break;
	case NetCommT_Wireless:
		break;
	case NetCommT_SIM:
		break;
	case NetCommT_BlueTooth:
		break;
	default:
		break;
	}
	return av_true;
}

av_bool CAvDevice::GetImageCaps(av_ushort Channel, C_ImageCaps &ImageCaps)
{
	memset(&ImageCaps, 0x00, sizeof(C_ImageCaps));
	return AvImageCaps((av_char)Channel, &ImageCaps);
}
av_bool CAvDevice::GetCaputreCaps(av_ushort Channel, C_CaptureCaps &CaptureCaps)
{
	memset(&CaptureCaps, 0x00, sizeof(CaptureCaps));
	return AvCaptureGetCaps((av_char)Channel, &CaptureCaps);
}
av_bool CAvDevice::GetACaptureCaps(E_AUDIO_CHL Chl, C_AudioCaps &AudioCaps)
{
	memset(&AudioCaps, 0x00, sizeof(C_AudioCaps));
	return AvACaptureCaps(Chl, &AudioCaps);
}
static av_bool ConfigNetCommCMP(ConfigNetComm &src, ConfigNetComm &dest)
{
	if (src.bEnable != dest.bEnable || src.bSupport != dest.bSupport || src.iFrNameType != dest.iFrNameType || src.mGetMode != dest.mGetMode){
		return av_false;
	}
	switch (src.iFrNameType)
	{
	case NetCommT_LAN0:
	case NetCommT_LAN1:
	{
		if (0 != strcmp(src.LanAttr.Dns1, dest.LanAttr.Dns1) || 0 != strcmp(src.LanAttr.Dns2, src.LanAttr.Dns2)
			|| 0 != strcmp(src.LanAttr.Gateway, dest.LanAttr.Gateway) || 0 != strcmp(src.LanAttr.IpAddr, dest.LanAttr.IpAddr)
			|| 0 != strcmp(src.LanAttr.Submask, dest.LanAttr.Submask)){
			return av_false;
		}
	}
		break;
	case NetCommT_Wireless:
	case NetCommT_Wireless1:
	{
		if (src.WirelessAttr.Mode != dest.WirelessAttr.Mode || src.WirelessAttr.WirelessPreferred != dest.WirelessAttr.WirelessPreferred){
			return av_false;
		}
		if (0 != strcmp(src.WirelessAttr.WirelessApConf.SSID, dest.WirelessAttr.WirelessApConf.SSID) ||
			0 != strcmp(src.WirelessAttr.WirelessApConf.Passwd, dest.WirelessAttr.WirelessApConf.Passwd)){
			return av_false;
		}
		for (int i = 0; i < MAX_CONF_ROUTER_LINK; i++){
			if (0 != strcmp(src.WirelessAttr.WirelessInfo[i].SSID, dest.WirelessAttr.WirelessInfo[i].SSID) ||
				0 != strcmp(src.WirelessAttr.WirelessInfo[i].Passwd, dest.WirelessAttr.WirelessInfo[i].Passwd)){
				return av_false;
			}
		}
		if (0 != strcmp(src.WirelessAttr.WirelessConf.Dns1, dest.WirelessAttr.WirelessConf.Dns1)
			|| 0 != strcmp(src.WirelessAttr.WirelessConf.Dns2, src.WirelessAttr.WirelessConf.Dns2)
			|| 0 != strcmp(src.WirelessAttr.WirelessConf.Gateway, dest.WirelessAttr.WirelessConf.Gateway)
			|| 0 != strcmp(src.WirelessAttr.WirelessConf.IpAddr, dest.WirelessAttr.WirelessConf.IpAddr)
			|| 0 != strcmp(src.WirelessAttr.WirelessConf.Submask, dest.WirelessAttr.WirelessConf.Submask)){
			return av_false;
		}
	}
		break;
	case NetCommT_SIM:
	case NetCommT_SIM1:
	{
		if (0 != strcmp(src.SimAttr.PhoneCode, dest.SimAttr.PhoneCode)){
			return av_false;
		}
	}
		break;
	case NetCommT_BlueTooth:
	{
		if (0 != strcmp(src.BluteToothAttr.SearchName, dest.BluteToothAttr.SearchName) ||
			0 != strcmp(src.BluteToothAttr.MarkCode, dest.BluteToothAttr.MarkCode)){
			return av_false;
		}
	}
		break;
	default:
		break;
	}
	return av_true;
}
av_void CAvDevice::OnConfigsNetComm(CAvConfigNetComm *NetComm, int &result)
{
	int cmpRet = 0;
	//C_NetCommAttribute NetCommAttr;
	for (int i = 0; i < NetCommT_LAST; i++)
	{
		ConfigNetComm &newConf = NetComm->GetConfig(i);
		ConfigNetComm &oldConf = m_ConfigNetComm.GetConfig(i);

		if (av_true == ConfigNetCommCMP(newConf, oldConf)){
			continue;
		}

		avNetCommSet(&newConf);
		oldConf = newConf;
		m_DeviceStatusSignal(ModifyNetDevice);
	}
}
av_bool CAvDevice::Reboot()
{
	
	g_AvDevice.DeviceSignal(RebootSoon);
	g_AvDevice.Stop();
	return av_true;
}

av_bool CAvDevice::GetSysTime(av_timeval &tv)
{
	return AvGetTimeOfDay(&tv);
}

av_bool CAvDevice::SetSysTime(av_timeval &tv)
{
	struct tm *tmt = localtime((time_t *)&(tv.tv_sec));

	av_warning("Set System time [%s\b]\n", asctime(tmt));
	AvSetTimeofDay(&tv);
	g_AvDevice.DeviceSignal(ModifySysTime);
	return AvTimeSystem2Rtc();
}

av_bool CAvDevice::TimeUtc2Local(av_u32 &utcSec, av_u32 &localSec)
{
	localSec = utcSec + 8 * 3600;
	return av_true;
}
av_bool CAvDevice::TimeLocal2Utc(av_u32 &utcSec, av_u32 &localSec)
{
	utcSec = localSec - 8 * 3600;
	return av_true;
}
av_bool CAvDevice::GetDeviceInfo(C_DeviceFactoryInfo &FactoryInfo)
{
	if (0 == strlen(m_FactoryInfo.SerialNumber)){
		if (av_true != AvGetDeviceInfo(&m_FactoryInfo)){
			std::string bVaild = std::string(EnumNameav_bool(av_false));
			SetEnv(std::string(EKey_BvalidFactoryInfo), bVaild);
			av_error("GetFactoryInfo Error use default\n");
		}
		else{
			std::string bVaild = std::string(EnumNameav_bool(av_true));
			SetEnv(std::string(EKey_BvalidFactoryInfo), bVaild);
		}
	}
	FactoryInfo = m_FactoryInfo;
	return av_true;
}
av_bool CAvDevice::SetDeviceInfo(C_DeviceFactoryInfo &FactoryInfo)
{
	m_FactoryInfo = FactoryInfo;
	av_msg( "SetDeviceInfo \n"
			"SerialNumber[%s]\n"
			"FactoryName[%s]\n"
			"HardWareVersion[%s]\n"
			"ProductMacAddr[%s]\n"
			"ChipStr[%s]\n"
			"SensorStr[%s]\n"
			"FActoryTime[%s]\n"
			"MaxChannel[%d]\n"
			"HardInterfaceMask[%x]\n", 
		m_FactoryInfo.SerialNumber, m_FactoryInfo.FactoryName, m_FactoryInfo.HardWareVersion, m_FactoryInfo.ProductMacAddr, 
		EnumNameAvChip((AvChip)m_FactoryInfo.ChipType), EnumNameAvSensor((AvSensor)m_FactoryInfo.SensorType), ctime((time_t*)&m_FactoryInfo.FActoryTime), 
		m_FactoryInfo.MaxChannel, m_FactoryInfo.HardInterfaceMask);

	av_bool bRet = AvSetDeviceInfo(&m_FactoryInfo);
	if (bRet == av_true){
		g_AvDevice.DeviceSignal(CAvDevice::RebootSoon);
		return av_true;
	}
	else{
		return av_false;
	}
}

av_uint CAvDevice::GetDeviceStartUp()
{
	return AvGetUpTime();
}
av_bool CAvDevice::SystemBeep()
{
	return AvSystemBeep(0xff, 100);
}

C_UpgradeProgress CAvDevice::m_SystemUpgradeProgress;

av_bool CAvDevice::SystemUpgrade(std::string UpgradeFilePath)
{
	m_SystemUpgradeProgress.ProgressCmd = ProgressStatus_UpgradeWriteData;
	m_SystemUpgradeProgress.ProgressValue = 0;
	av_bool ret = AvSystemUpgradeFile(UpgradeFilePath.c_str(), &m_SystemUpgradeProgress);
	return ret;
}
av_bool CAvDevice::SystemUpgrade(av_uchar *ptr, av_uint length)
{
	return av_true;
}
av_u32  CAvDevice::SystemUpgradeProgress()
{
	return (av_u32)(m_SystemUpgradeProgress.ProgressCmd << 16 | m_SystemUpgradeProgress.ProgressValue);
}

av_bool CAvDevice::GetNetStatusInfo(C_NetStatusInfo &NetStatusInfo)
{
	return avGetNetStatusInfo(&NetStatusInfo);
}
av_bool CAvDevice::GetCpuMemStatusInfo(C_CpuMemStatusInfo &CpuMemStatusInfo)
{
	return avGetCpuMemStatusInfo(&CpuMemStatusInfo);
}

av_bool CAvDevice::GetStartUpGuid(std::string &guid)
{
	guid = m_SStartGUID;
	return av_true;
}

av_void CAvDevice::OnTimer(CAvTimer &Timer)
{
	m_TimerDeviceCount = m_TimerDeviceCount++ & 0xffffffff;
	av_u32 CurUptime = AvGetUpTime();


	if (m_TimerDeviceCount % 8 == 0){
		AvWatchDogFeed();
	}

	if (m_TimerDeviceRebootTime >= CurUptime && m_TimerDeviceRebootTime != 0){
		AvReboot();
	}

	m_PlatformTimerTask.StartTask(this, (CAvTask::CAvTaskThreadProc)&CAvDevice::PlatformTimerTask, av_false);
}

av_void CAvDevice::PlatformTimerTask()
{
	avSystemOnTime1s();
}