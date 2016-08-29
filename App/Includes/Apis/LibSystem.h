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
#ifndef _LIBSYSTEM_H_
#define _LIBSYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "AvWareType.h"
	typedef enum {
		AvSensor_OV9712		= 0,
		AvSensor_OV4689,
		AvSensor_OV5658,

		AvSensor_AR0130		= 5,
		AvSensor_AR0330,
		AvSensor_AR0237,

		AvSensor_IMX138		= 10,
		AvSensor_IMX122,
		AvSensor_IMX236,
		AvSensor_IMX178,
		AvSensor_IMX185,
		AvSensor_IMX117,
		AvSensor_IMX123,

		AvSensor_BT1120		= 25,
		AvSensor_BT656,

		AvSensor_AUTO		= 30,
	}E_AvSensor;

	typedef enum {
		AvChip_S2L22M,
		AvChip_S2L33M,
		AvChip_S2L55M,
		AvChip_S2L65,
		AvChip_S2L66,

		AvChip_H18EV100,
		AvChip_H18EV200,
		AvChip_H18EV201,
		AvChip_H18C,
		AvChip_H18A,
		AvChip_H16CV100,
		AvChip_H16CV200,
		AvChip_H16A,
		AvChip_H16D,
		AvChip_H19,

		AvChip_H20D,
		AvChip_H35,
		AvChip_H36,

		AvChip_WIN32,
		AvChip_WIN64,

		AvChip_LIN32,
		AvChip_LIN64,
	}E_AvChip;

	typedef struct {
	av_u32		nMaxEncodePower;
	av_uchar	nMaxEncodeChannel;
	av_uchar	nMaxEncodeExtChannel;
	av_uchar	nMaxDecodeChannel;
	av_uchar	nMaxDecodeExtChannel;
}C_DspCaps;

av_bool AvGetDspCaps(C_DspCaps *DspCaps);

av_bool AvSystemInit();
av_bool AvSystemDeInit();


//sleep 
av_void av_msleep(int ms);

//GPIO 操作
av_bool  AvGpioInit();
av_bool  AvGpioDeInit();
av_u32   AvGpioRead(av_uchar GpioNo);
av_bool  AvGpioSet1(av_uchar GpioNo);
av_bool  AvGpioSet0(av_uchar GpioNo);

//串口 操作

typedef enum {
	SerialParity_NOPARITY,	///< No parity
	SerialParity_ODDPARITY,	///< Odd parity
	SerialParity_EVENPARITY,///< Even parity
	SerialParity_MARK,		///< 
	SerialParity_SPACE		///< 
}SerialParity_E;

typedef struct {
	av_u32		baudrate;
	av_uchar	databits;
	av_uchar	parity;
	av_uchar	stopbits;
	av_uchar	reserved;
}C_SerialAttr;

typedef enum {
	COM_PTZ,
	COM_SERIAL,
}av_com_type;

typedef struct {
	av_uchar	TotalSerials;
	av_com_type Com_t[8];
}C_SerialCaps;

av_bool	 AvSerialInit();
av_bool  AvSerialDeInit();
av_bool  AvSerialCaps(C_SerialCaps *SerialCaps);
av_bool  AvSerialOpen(av_uchar index);
av_bool  AvSerialClose(av_uchar index);
av_bool  AvSerialSetAttribute(av_uchar index, C_SerialAttr *SerialAttr);
av_bool  AvSerialSetNonBlock(av_uchar index, av_bool iFlag);
av_bool  AvSerialRead(av_uchar index, av_buf *buf);
av_bool  AvSerialWrite(av_uchar index, av_buf *buf);


//watch dog
av_bool AvWatchDogInit(const int FeedSec);
av_bool AvWatchDogDeInit();
av_bool AvWatchDogFeed();

//rtc 操作
typedef struct {
	av_u32 tv_sec;		//设置时间时必添 1900 至今的秒数
	av_u32 tv_usec;		//设置时间时必添
	
	av_u32 t_year;		//实际的年 2016
	av_u32 t_mon;		//实际的月 1-12
	av_u32 t_day;		//实际的日 1-31

	av_uchar t_week;		//rang 1-7
	av_uchar t_hour;
	av_uchar t_min;
	av_uchar t_sec;

	av_u32 t_todaysec;	//当天的秒数
}av_timeval;


av_u32  AvGetUpTime();
av_bool AvRtcInit();
av_bool AvRtcDeInit();
av_bool AvTimeRtc2System();
av_bool AvTimeSystem2Rtc();

av_bool AvGetTimeOfDay(av_timeval *tv);
av_bool AvSetTimeofDay(av_timeval *tv);
//线程操作
typedef void *(*AvTask)(void *args);
av_bool AvCreateTask(AvTask TaskFunc, void *args, av_thread_t *threadid);

//锁操作
av_mutex *AvMutexCreate();
av_bool AvMutexDestroy(av_mutex *);
av_bool AvMutexLock(av_mutex *);
av_bool AvMutexUnLock(av_mutex *);
av_bool AvMutexTryLock(av_mutex *);

//sem
av_sem *AvSemCreate();
av_bool AvSemDestroy(av_sem *);
av_bool AvSemWait(av_sem *);
av_bool AvSemPost(av_sem *);
av_bool AvSemTryWait(av_sem *);


//av_buf 操作
av_buf * avBufMalloc	(int size);
av_buf * avBufRelloc	(av_buf *src, int newsize);
av_bool avBufCopy		(av_buf *dest, av_buf *src);
av_bool avBufAppend		(av_buf *dest, char *data, int len);
av_bool avBufSplice		(av_buf *dest, av_buf *src);
av_bool avBufSpliceData (av_buf *dest, char *data, int len);
av_bool avBufFree		(av_buf *);
av_bool avBufFreeHead	(av_buf *);

#define avBufGetLen(avbuf) (avbuf)->len
#define avBufGetSize(avbuf) (avbuf)->size
#define avBufGetData(avbuf) (avbuf)->base
#define avBufGetLeftSize(avbuf) ((avbuf)->size - (avbuf)->len)


//network

typedef enum{
	NetCommT_Lan0,
	NetCommT_Lan1,
	NetCommT_Wireless,
	NetCommT_SIM,
	NetCommT_BlueTooth,
	NetCommT_Nr,
}E_NetComm_Type;

typedef enum {
	NetCommGetMode_MANUAL,
	NetCommGetMode_AUTO,
	NetCommGetMode_PPPOE,
	NetCommGetMode_NR,
}E_NetComm_Mode;


typedef struct{
	av_u32 NetCommMask;
	av_u32 NetCommGetModeMask[ConfMaxNetComm];
}C_NetCommCaps;

#define NetCommMaxStringLen 32

typedef struct{
	char IpAddr[NetCommMaxStringLen];
	char Gateway[NetCommMaxStringLen];
	char Submask[NetCommMaxStringLen];
	char Dns1[NetCommMaxStringLen];
	char Dns2[NetCommMaxStringLen];
	char Host[NetCommMaxStringLen];
}C_LanAttribute;

typedef struct {
	char MarkCode[NetCommMaxStringLen];
}C_BlueToothAttribute;

typedef struct {
	char SSID[NetCommMaxStringLen];
	char Passwd[NetCommMaxStringLen];
}C_WirelessAttribute;

typedef struct  {
	char PhoneCode[32];
}C_SimAttribute;

typedef struct 
{
	av_bool Enable;
	union
	{
		C_LanAttribute			LanAttr;
		C_BlueToothAttribute	BluteToothAttr;
		C_WirelessAttribute		WirelessAttr;
		C_SimAttribute			SimAttr;
	};
}C_NetCommAttribute;




av_bool avNetCommCaps(C_NetCommCaps *NetCommCaps);

av_bool avNetCommSet(E_NetComm_Type type, C_NetCommAttribute *Attr);
av_bool avNetCommGet(E_NetComm_Type type, C_NetCommAttribute *Attr);



unsigned int  /*av_capture_size*/NmSize2AvCaptureSize(int higth, int width);

int AvCaptureSize2NmSize( unsigned int /*av_capture_size*/capsize, int *hight, int *width);


typedef struct {
	av_char		SerialNumber[64];
	av_char		FactoryName[64];
	av_char		HardWareVersion[64];
	av_char		ProductModel[64];
	av_char		ProductMacAddr[32];
	av_uint		ChipType;
	av_uint		SensorType;
	av_uint		FActoryTime;
	av_uint		MaxChannel;
	av_uint		Res;
}C_DeviceFactoryInfo;

typedef struct {
	av_uint		Totoal;
	av_uint		TotoalUse;
}C_MemoryLoadInfo;

typedef struct{
	av_uint NetType;
	av_uint Totoal;
	av_uint TotoalUse;
}C_NetLoadInfo;

typedef struct {
	av_uint mainHZ;
	av_uint Totoal;
	av_uint TotoalUse;
}C_CpuLoadInfo;

av_bool AvReboot();
av_bool AvGetDeviceInfo(C_DeviceFactoryInfo *FactoryInfo);
av_bool AvSetDeviceInfo(C_DeviceFactoryInfo *FactoryInfo);
av_uint AvGetDeviceStartUp();
av_bool AvSystemBeep(av_uint dwFrequence, av_uint dwDuration);

/*
UpgradeStart = 1,
UpgradeRecvData = 2,
UpgradeWipePartition = 3,
UpgradeWriteData = 4,
UpgradeOver = 5,
Rebootting = 6,
ModifyOver = 7,
HaveNoResource = 8,
*/
typedef struct{
	av_ushort ProgressCmd;
	av_ushort ProgressValue;
}C_UpgradeProgress;

av_bool AvSystemUpgradeFile(const av_char * UpgradeFilePath, C_UpgradeProgress *Progress);
av_bool AvSystemUpgradeMemory(av_uchar *ptr, av_uint length, C_UpgradeProgress *Progress);
av_bool AvGetMemLoadInfo(C_MemoryLoadInfo *MemLoadInfo);
av_bool AvGetNetLoadInfo(C_NetLoadInfo *NetLoadInfo);
av_bool AvGetCpuLoadInfo(C_CpuLoadInfo *CpuLoadInfo);

#ifdef __cplusplus
}
#endif

#endif