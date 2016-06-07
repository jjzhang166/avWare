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
#ifndef _AVUART_H_
#define _AVUART_H_
#include "Apis/AvWareType.h"
#include "Apis/LibSystem.h"
#include "AvConfigs/AvConfigDevice.h"
#include "CObject.h"


typedef struct {
	av_u32	 Cmd;
	av_uchar HSpeed;
	av_uchar VSpeed;
	av_uchar arg1[2];
}PtzCmd;

class CAvUart
{
public:
	PATTERN_SINGLETON_DECLARE(CAvUart)
public:
	av_bool Initialize();
	av_bool Write(av_uchar index, av_buf &buf);
	av_bool Read(av_uchar index, av_buf &buf);
public:
	av_bool PtzStart(PtzCmd &cmd);

private:
	av_bool UartOpen(av_uchar index);
	av_bool UartClose(av_uchar index);
	av_bool UartCaps();
	av_bool SetAttr(av_uchar index, C_SerialAttr &Attr);
	av_void OnConfigsModify(CAvConfigUart *Config, int &result);
private:
	C_SerialCaps m_SerialCaps;

private:
	CAvConfigUart m_Config;

private:
	CAvUart();
	~CAvUart();

public:
	enum PtzCommand {
		PtzCommand_NONE = 0,
		PtzCommand_PTZ_STOP = 1,
		PtzCommand_LEFT_START = 2,
		PtzCommand_LEFT_STOP = 3,
		PtzCommand_RIGHT_START = 4,
		PtzCommand_RIGHT_STOP = 5,
		PtzCommand_UP_START = 6,
		PtzCommand_UP_STOP = 7,
		PtzCommand_DOWN_START = 8,
		PtzCommand_DOWN_STOP = 9,
		////
		PtzCommand_LEFT_UP_START = 10,
		PtzCommand_LEFT_UP_STOP = 11,
		PtzCommand_RIGHT_UP_START = 12,
		PtzCommand_RIGHT_UP_STOP = 13,
		PtzCommand_LEFT_DOWN_START = 14,
		PtzCommand_LEFT_DOWN_STOP = 15,
		PtzCommand_RIGHT_DOWN_START = 16,
		PtzCommand_RIGHT_DOWN_STOP = 17,
		//// 线扫描开
		PtzCommand_SCAN_ON = 18,
		PtzCommand_SCAN_OFF = 19,
		//// 轨迹扫描开
		PtzCommand_TRACK_ON = 20,
		PtzCommand_TRACK_OFF = 21,
		//// 绝对运动
		PtzCommand_ABSOLUTE_MOVE = 22,
		//// 相对运动
		PtzCommand_RELATIVE_MOVE = 23,
		//// 绝对变焦
		PtzCommand_ABSOLUTE_FOCUS = 24,
		//// 开光圈开始
		PtzCommand_IRIS_OPEN_START = 100,
		//// 开光圈停止
		PtzCommand_IRIS_OPEN_STOP = 101,
		//// 关光圈开始
		PtzCommand_IRIS_CLOSE_START = 102,
		//// 关光圈停止
		PtzCommand_IRIS_CLOSE_STOP = 103,
		//// 焦距近开始
		PtzCommand_FOCUS_NEAR_START = 104,
		//// 焦距近停止
		PtzCommand_FOCUS_NEAR_STOP = 105,
		//// 焦距远开始
		PtzCommand_FOCUS_FAR_START = 106,
		//// 焦距远停止
		PtzCommand_FOCUS_FAR_STOP = 107,
		//// 图象变小开始
		PtzCommand_ZOOM_WIDE_START = 108,
		//// 图象变小停止
		PtzCommand_ZOOM_WIDE_STOP = 109,
		//// 图象变大开始
		PtzCommand_ZOOM_TELE_START = 110,
		//// 图象变大停止
		PtzCommand_ZOOM_TELE_STOP = 111,
		//// 摄像机电源开
		PtzCommand_CAMERA_ON = 200,
		//// 摄像机电源关
		PtzCommand_CAMERA_OFF = 201,
		//// 灯光开
		PtzCommand_LAMP_ON = 202,
		//// 灯光关
		PtzCommand_LAMP_OFF = 203,
		//// 雨刷开
		PtzCommand_WIPER_ON = 204,
		//// 雨刷关
		PtzCommand_WIPER_OFF = 205,
		//// 打开继电器输出
		PtzCommand_IO_ON = 206,
		//// 关闭继电器输出
		PtzCommand_IO_OFF = 207,
		//// 设置预置位
		PtzCommand_PRESET_POINT = 300,
		//// 调用预置位
		PtzCommand_SET_POINT = 301,
		//// 清除预置位	
		PtzCommand_CLEAR_POINT = 302
	};

};

#define g_AvUart (*CAvUart::instance())






#endif