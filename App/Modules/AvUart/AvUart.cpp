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
#include "AvUart/AvUart.h"
#include "Apis/AvEnuminline.h"

#define CHECK_SUM_START 1
#define CHECK_SUM_SPOT  5
#define BUILD_STR_LEN   7


static av_32 BuildCmdCheckSum(char *CmdBuffer, av_32 start_idx, av_32 stop_idx)
{
	if (NULL == CmdBuffer)
	{
		av_error("input param is error...\n");
		return av_false;
	}

	int idx = 0;
    int check_sum = 0;

    for (idx = start_idx; idx <= stop_idx; idx++)
    {
        check_sum += *(CmdBuffer + idx);
    }

    check_sum = check_sum % 0x100;
    check_sum = (check_sum & 0xFF);

    return check_sum;
}


SINGLETON_IMPLEMENT(CAvUart)

static av_bool PtzCmd2Buffer(C_PtzCmd &PtzCmd, av_buf &buf)
{
	char  *pData = buf.base;
	const char PELCO_D[10] = { 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01 };
	memcpy(pData, PELCO_D, sizeof(PELCO_D));

	av_msg("PtzCmd[%s], Value[%d], Speed[%d]\n", 
		EnumNamePtzCommand((PtzCommand)PtzCmd.PtzCmd), PtzCmd.Value, PtzCmd.PtzSpeed);
	switch (PtzCmd.PtzCmd)
	{
	case PtzCommand_NONE:
		break;
			///方向
	case PtzCommand_STOP:
		break;
	case PtzCommand_UP:
		pData[3] = 0x08;
		pData[4] = PtzCmd.PtzSpeed;
		break;
	case PtzCommand_DOWN:
		pData[3] = 0x10;
		pData[4] = PtzCmd.PtzSpeed;
		break;
	case PtzCommand_LEFT:
		pData[3] = 0x04;
		pData[5] = PtzCmd.PtzSpeed;
		break;
	case PtzCommand_RIGHT:
		pData[3] = 0x02;
		pData[5] = PtzCmd.PtzSpeed;
		break;
	case PtzCommand_LEFTUP:
		break;
	case PtzCommand_LEFTDOWN:
		break;
	case PtzCommand_RIGHTUP:
		break;
	case PtzCommand_RIGHTDOWN:
		break;
			///绝对运动
	case PtzCommand_ABSOLUTE_MOVE:
		break;
			///相对运动
	case PtzCommand_RELATIVE_MOVE:
		break;
			///绝对变焦
	case PtzCommand_ABSOLUTE_FOCUS:
		break;
			///变倍
	case PtzCommand_ZOOM_WIDE:
		pData[3] = 0x40;
		pData[4] = PtzCmd.PtzSpeed/4;
		break;
	case PtzCommand_ZOOM_TELE:
		pData[3] = 0x20;
		pData[4] = PtzCmd.PtzSpeed/4;
		break;
			///聚焦
	case PtzCommand_FOCUS_FAR:
		pData[2] = 0x01;
		break;
	case PtzCommand_FOCUS_NEAR:
		pData[3] = 0x80;
		break;
			///光圈
	case PtzCommand_IRIS_LARGE:
		pData[1] = 0x01;
		pData[2] = 0x02;
		break;
	case PtzCommand_IRIS_SMALL:
		pData[1] = 0x01;
		pData[2] = 0x04;
		break;
			///报警功能
	case PtzCommand_ALARM:
		break;
			///灯光
	case PtzCommand_LIGHT_ON:
		break;
	case PtzCommand_LIGHT_OFF:
		break;
			///设置预置点
	case PtzCommand_SETPRESET:
		break;
			///清除预置点
	case PtzCommand_CLEARPRESET:
		break;
			///转至预置点
	case PtzCommand_GOTOPRESET:
		break;
			///水平开始 
	case PtzCommand_AUTOPANON:
		break;
			///水平结束
	case PtzCommand_AUTOPANOFF:
		break;
			///设置边界 
	case PtzCommand_SETLIMIT:
		break;
			///自动扫描开始
	case PtzCommand_AUTOSCANON:
		break;
			///自动扫描开停止
	case PtzCommand_AUTOSCANOFF:
		break;
			///增加巡航点
	case PtzCommand_ADDTOUR:
		break;
			///删除巡航点 	
	case PtzCommand_DELETETOUR:
		break;
			///开始巡航
	case PtzCommand_STARTTOUR:
		break;
			///结束巡航
	case PtzCommand_STOPTOUR:
		break;
			///删除巡航
	case PtzCommand_CLEARTOUR:
		break;
			///设置模式
	case PtzCommand_SETPATTERN:
		break;
			///开始模式
	case PtzCommand_STARTPATTERN:
		break;
			///停止模式
	case PtzCommand_STOPPATTERN:
		break;
			///清除模式
	case PtzCommand_CLEARPATTERN:
		break;
			///快速定位
	case PtzCommand_POSITION:
		break;
			///辅助开关
	case PtzCommand_AUX_ON:
		break;
	case PtzCommand_AUX_OFF:
		pData[3] = 0x0b;
		pData[5] = PtzCmd.Value;
		av_msg("Auto Cali [%d]\n", pData[5]);
		break;
			///球机菜单
	case PtzCommand_MENU:
		break;
			///退出球机菜单
	case PtzCommand_EXIT:
		break;
			///确认
	case PtzCommand_ENTER:
		break;
			///取消
	case PtzCommand_ESC:
		break;
			///菜单上下操作
	case PtzCommand_MENUUPDOWN:
		break;
			///菜单左右操作
	case PtzCommand_MENULEFTRIGHT:
		break;
			///矩阵切换
	case PtzCommand_MATRIXSWITCH:
		break;
			/// 镜头翻转
	case PtzCommand_FLIP:
		break;
			///云台复位
	case PtzCommand_RESET:
		break;
			///菜单左右操作 （主要用于网络对灯光控制器的控制）
	case PtzCommand_LIGHTCONTROLLER:
		break;

	default:
		return av_false;
		break;
	}

	pData[6] = BuildCmdCheckSum(pData, CHECK_SUM_START, CHECK_SUM_SPOT);
	buf.len = BUILD_STR_LEN;
	
	return av_true;
}
av_bool CAvUart::Initialize()
{
	AvSerialInit();
	UartCaps();
	int i = 0;
	{
		m_Config.Attach(this, (AvConfigCallBack)&CAvUart::OnConfigsModify);
		m_ConfigPtzCameraLensProfile.Attach(this, (AvConfigCallBack)&CAvUart::OnConfigPtzCameralensModify);
	m_Config.Update();
		m_ConfigPtzCameraLensProfile.Update();
	}

	
	for (i = 0; i < m_SerialCaps.TotalSerials; i++){
		if (av_true != UartOpen(i)){
			av_error("open Uart index  = %d Error\n", i);
			continue;
		}
		ConfigUart &SerialAttr = m_Config.GetConfig(i);
		SetAttr(i, SerialAttr.Attribute);
	}
	return av_true;
}
av_bool CAvUart::Write(av_uchar index, av_buf &buf)
{
	if (index >= m_SerialCaps.TotalSerials) {
		av_error("Ptz Write Index[%d]  TotalSerials [%d]\n", index, m_SerialCaps.TotalSerials);
		return av_false;
	}
	if (buf.len <= 0 || buf.base == NULL) {
		av_error("Ptz Write buf.size[%d]  buf.base [%p]\n", buf.len, buf.base);
		return av_false;
	}
	return AvSerialWrite(index, &buf);
}
av_bool CAvUart::Read(av_uchar index, av_buf &buf)
{
	if (index >= m_SerialCaps.TotalSerials) {
		av_error("Ptz Write Index[%d]  TotalSerials [%d]\n", index, m_SerialCaps.TotalSerials);
		return av_false;
	}
	if (buf.size <= 0 || buf.base == NULL) return av_false;
	return AvSerialRead(index, &buf);
}
av_bool CAvUart::SetAttr(av_uchar index, C_SerialAttr &Attr)
{
	if (index >= m_SerialCaps.TotalSerials) {
		av_error("Ptz Write Index[%d]  TotalSerials [%d]\n", index, m_SerialCaps.TotalSerials);
		return av_false;
	}
	av_bool ret =  AvSerialSetAttribute(index, &Attr);
	return ret;
}

av_void CAvUart::OnConfigsModify(CAvConfigUart *Config, int &result)
{
	for (int i = 0; i < m_SerialCaps.TotalSerials; i++){
		ConfigUart &newConf = Config->GetConfig(i);
		ConfigUart &oldConf = m_Config.GetConfig(i);
		if (0 != memcmp(&newConf.Attribute, &oldConf.Attribute, sizeof(C_SerialAttr))){
			SetAttr(i, newConf.Attribute);
			newConf.Attribute = oldConf.Attribute;
			newConf.Enable = oldConf.Enable;
		}
	}
}

av_bool CAvUart::PtzGetCaps(C_PtzCaps &Ptzcaps)
{
	return av_true;
}
av_bool CAvUart::PtzSetProfile(C_PtzProfile &PtzProfile)
{
	return av_true;
}
av_bool CAvUart::PtzGetProfile(C_PtzProfile &PtzProfile)
{
	return av_true;
}

av_bool CAvUart::PtzSetCommand(C_PtzCmd &PtzCmd)
{
	av_buf buf;
	char buffer[16] = {0};
	buf.base = buffer;
	buf.size = sizeof(buffer);
	buf.len = 0;
	if (av_false == PtzCmd2Buffer(PtzCmd, buf)){
		return av_false;
	}
	av_uchar index = -1;
	for (int i = 0; i < m_SerialCaps.TotalSerials; i++){
		if (COM_PTZ == m_SerialCaps.Com_t[i]){
			index = i;
			break;
		}
	}
	return Write(index, buf);
}


av_bool CAvUart::PtzGetAdvancedCaps(C_AdvancedSystemCaps &PtzAdvancedCaps)
{
	//这里面有一个  共用体不可以消除
	//memset(&PtzCameraLensCaps, 0x00, sizeof(C_PtzAdvancedCaps));
	//return AvPtzCameraLensCaps(&PtzCameraLensCaps);

	switch (PtzAdvancedCaps._msg)
	{
	case __MsgPtzCameraLensCaps:
		return AvPtzAdvancedCameraLensCaps(&PtzAdvancedCaps.PtzCameralensCaps);
		break;
	default:
		return av_false;
	}
}

av_bool CAvUart::PtzGetAdvancedProfile(C_AdvancedSystemProfile &AdvancedSystemProfile)
{

	switch (AdvancedSystemProfile._msg)
	{
	case __MsgPtzCameraLensProfile:
	{
		AdvancedSystemProfile.PtzAdvancedCameraLensProfile = m_ConfigPtzCameraLensProfile.GetConfig();
// 		av_msg("PtzSetAdvancedProfile ==> bFigureZoom %d, FocusCtrlMode = %d,FocusRegion=%d,  FocusSearchMode = %d, \
// 			   FocusSensitivity = %d, IrisCtrlMode = %d, ZoomSpeed = %d\n",
// 			   AdvancedSystemProfile.PtzAdvancedCameraLensProfile.bFigureZoom, AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusCtrlMode, AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusRegion,
// 			   AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusSearchMode, AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusSensitivity, AdvancedSystemProfile.PtzAdvancedCameraLensProfile.IrisCtrlMode, AdvancedSystemProfile.PtzAdvancedCameraLensProfile.ZoomSpeed);

	}
	
		break;
	default:
		return av_false;
	}

	return av_true;
}
av_bool CAvUart::PtzSetAdvancedProfile(C_AdvancedSystemProfile &AdvancedSystemProfile)
{
	switch (AdvancedSystemProfile._msg)
	{
	case __MsgPtzCameraLensProfile:
	{
		CAvConfigPtzCameraLensProfile ConfigPtzCameraLens;
		ConfigPtzCameraLens.Update();
		ConfigPtzCameraLensProfile &ConfigProfile = ConfigPtzCameraLens.GetConfig();
		ConfigProfile = AdvancedSystemProfile.PtzAdvancedCameraLensProfile;
		av_msg("PtzSetAdvancedProfile ==> bFigureZoom %d, FocusCtrlMode = %d,FocusRegion=%d,  FocusSearchMode = %d, \
			   			   			   				   FocusSensitivity = %d, IrisCtrlMode = %d, ZoomSpeed = %d\n",
													   ConfigProfile.bFigureZoom, ConfigProfile.FocusCtrlMode, ConfigProfile.FocusRegion,
													   ConfigProfile.FocusSearchMode, ConfigProfile.FocusSensitivity, ConfigProfile.IrisCtrlMode, ConfigProfile.ZoomSpeed);
		ConfigPtzCameraLens.SettingUp();
	}
		AdvancedSystemProfile.PtzAdvancedCameraLensProfile = m_ConfigPtzCameraLensProfile.GetConfig();
		break;
	default:
		return av_false;
	}

	return av_true;
	
}
av_void CAvUart::OnConfigPtzCameralensModify(CAvConfigPtzCameraLensProfile *Config, int &result)
{
	av_msg("%s\n", __FUNCTION__);
	ConfigPtzCameraLensProfile &newPtzCameraLensProfile = Config->GetConfig();
	ConfigPtzCameraLensProfile &oldPtzCameraLensProfile = m_ConfigPtzCameraLensProfile.GetConfig();
	if (0 != memcmp(&newPtzCameraLensProfile, &oldPtzCameraLensProfile, sizeof(ConfigPtzCameraLensProfile))){
		C_PtzAdvancedCameraLensProfile PtzAdvancedProfile;
		PtzAdvancedProfile = newPtzCameraLensProfile;
		AvPtzAdvancedCameraLensProfile(&PtzAdvancedProfile);
		oldPtzCameraLensProfile = newPtzCameraLensProfile;
	}
}

av_bool CAvUart::UartOpen(av_uchar index)
{
	if (index >= m_SerialCaps.TotalSerials) {
		av_error("UartOpen Index[%d]  TotalSerials [%d]\n", index, m_SerialCaps.TotalSerials);
		return av_false;
	}
	return AvSerialOpen(index);
}
av_bool CAvUart::UartClose(av_uchar index)
{
	if (index >= m_SerialCaps.TotalSerials) {
		av_error("UartClose Index[%d]  TotalSerials [%d]\n", index, m_SerialCaps.TotalSerials);
		return av_false;
	}
	return AvSerialClose(index);
}

av_bool CAvUart::UartCaps()
{
	av_bool ret = AvSerialCaps(&m_SerialCaps);
	if (ret != av_true){
		memset(&m_SerialCaps, 0x00, sizeof(C_SerialCaps));
		return av_false;
	}
	return av_true;
}

CAvUart::CAvUart()
{
	memset(&m_SerialCaps, 0x00, sizeof(C_SerialCaps));
}

CAvUart::~CAvUart()
{

}
