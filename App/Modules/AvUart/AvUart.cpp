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

static av_bool PtzCmd2Buffer(av_32 cmd, av_uchar VSpeed, av_uchar HSpeed, av_buf &buf)
{
	char  *pData = buf.base;
	const char PELCO_D[] = { 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01 };
	memcpy(pData, PELCO_D, sizeof(PELCO_D));

	switch (cmd)
	{
		case CAvUart::PtzCommand_PTZ_STOP:
			break;
		case CAvUart::PtzCommand_LEFT_START:
			pData[3] = 0x04;
			pData[5] = VSpeed;
			break;
		case CAvUart::PtzCommand_LEFT_STOP:
			break;
		case CAvUart::PtzCommand_RIGHT_START:
			pData[3] = 0x02;
			pData[5] = VSpeed;
			break;
		case CAvUart::PtzCommand_RIGHT_STOP:
			break;
		case CAvUart::PtzCommand_UP_START:
			pData[3] = 0x08;
			pData[4] = HSpeed;
			break;
		case CAvUart::PtzCommand_UP_STOP:
			break;
		case CAvUart::PtzCommand_DOWN_START:
			pData[3] = 0x10;
			pData[4] = HSpeed;
			break;
		case CAvUart::PtzCommand_DOWN_STOP:
			break;
		case CAvUart::PtzCommand_LEFT_UP_START:
			break;
		case CAvUart::PtzCommand_LEFT_UP_STOP:
			break;
		case CAvUart::PtzCommand_RIGHT_UP_START:
			break;
		case CAvUart::PtzCommand_RIGHT_UP_STOP:
			break;
		case CAvUart::PtzCommand_LEFT_DOWN_START:
			break;
		case CAvUart::PtzCommand_LEFT_DOWN_STOP:
			break;
		case CAvUart::PtzCommand_RIGHT_DOWN_START:
			break;
		case CAvUart::PtzCommand_RIGHT_DOWN_STOP:
			break;
		case CAvUart::PtzCommand_ZOOM_WIDE_START:
			pData[3] = 0x40;
			break;
		case CAvUart::PtzCommand_ZOOM_WIDE_STOP:
			break;
		case CAvUart::PtzCommand_ZOOM_TELE_START:
			pData[3] = 0x20;
			break;
		case CAvUart::PtzCommand_ZOOM_TELE_STOP:
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
	m_Config.Update();
	
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
av_bool CAvUart::PtzStart(PtzCmd &cmd)
{
	av_buf buf;
	char buffer[16] = {0};
	buf.base = buffer;
	buf.size = sizeof(buffer);
	buf.len = 0;
	if (av_false == PtzCmd2Buffer(cmd.Cmd, cmd.VSpeed, cmd.HSpeed, buf)){
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
