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
#ifndef _PROTOENUM_H_
#define _PROTOENUM_H_


enum ProtoBool {
	ProtoBool_Enable = 0,
	ProtoBool_Disable = 1,
	ProtoBool_LAST = 2
};

inline const char **EnumNamesProtoBool() {
	static const char *names[] = { "Enable", "Disable", "LAST", nullptr };
	return names;
}

inline const char *EnumNameProtoBool(ProtoBool e) { return EnumNamesProtoBool()[e]; }

enum ProtoSwitch {
	ProtoSwitch_NONE = 0,
	ProtoSwitch_OPEN = 1,
	ProtoSwitch_CLOSE = 2,
	ProtoSwitch_DisSupport = 3,
	ProtoSwitch_LAST = 4
};

inline const char **EnumNamesProtoSwitch() {
	static const char *names[] = { "NONE", "OPEN", "CLOSE", "DisSupport", "LAST", nullptr };
	return names;
}

inline const char *EnumNameProtoSwitch(ProtoSwitch e) { return EnumNamesProtoSwitch()[e]; }

///自动升级部分
enum Platform {
	Platform_NONE = 0,
	Platform_DEV_MANAGER = 1,
	Platform_PLUGIN = 2,
	Platform_AULOGPULL = 3,
	Platform_IPC_HISI = 10,
	Platform_IPC_AMBA = 11,
	Platform_IPC_TI = 12,
	Platform_NVR = 20,
	Platform_LAST = 21
};

inline const char **EnumNamesPlatform() {
	static const char *names[] = { "NONE", "DEV_MANAGER", "PLUGIN", "AULOGPULL", "", "", "", "", "", "", "IPC_HISI", "IPC_AMBA", "IPC_TI", "", "", "", "", "", "", "", "NVR", "LAST", nullptr };
	return names;
}

inline const char *EnumNamePlatform(Platform e) { return EnumNamesPlatform()[e]; }

enum BitRateCtrl {
	BitRateCtrl_NONE = 0,
	BitRateCtrl_BRC_CBR = 1,
	BitRateCtrl_BRC_VBR = 2,
	BitRateCtrl_BRC_VCBR = 3,
	BitRateCtrl_LAST = 4
};

inline const char **EnumNamesBitRateCtrl() {
	static const char *names[] = { "NONE", "BRC_CBR", "BRC_VBR", "BRC_VCBR", "LAST", nullptr };
	return names;
}

inline const char *EnumNameBitRateCtrl(BitRateCtrl e) { return EnumNamesBitRateCtrl()[e]; }

enum StreamContent {
	StreamContent_NONE = 0,
	StreamContent_VIDEO = 1,
	StreamContent_AUDIO = 2,
	StreamContent_AUDIO_PLAY = 3,
	StreamContent_LAST = 4
};

inline const char **EnumNamesStreamContent() {
	static const char *names[] = { "NONE", "VIDEO", "AUDIO", "AUDIO_PLAY", "LAST", nullptr };
	return names;
}

inline const char *EnumNameStreamContent(StreamContent e) { return EnumNamesStreamContent()[e]; }

enum TransportMode {
	TransportMode_NONE = 0,
	TransportMode_PUSH = 1,
	TransportMode_PULL = 2,
	TransportMode_LAST = 3
};

inline const char **EnumNamesTransportMode() {
	static const char *names[] = { "NONE", "PUSH", "PULL", "LAST", nullptr };
	return names;
}

inline const char *EnumNameTransportMode(TransportMode e) { return EnumNamesTransportMode()[e]; }

enum Resolution {
	Resolution_NONE = 0,
	Resolution_Res320_160 = 1,
	Resolution_Res320_240 = 2,
	Resolution_Res640_320 = 3,
	Resolution_Res640_480 = 4,
	Resolution_Res704_576 = 5,
	Resolution_Res1280_720 = 6,
	Resolution_Res1280_960 = 7,
	Resolution_Res1920_1080 = 8,
	///300w
	Resolution_Res2048_1536 = 9,
	///500w
	Resolution_Res2592_1944 = 10,
	///600w
	Resolution_Res3072_2048 = 11,
	///800w
	Resolution_Res3840_2160 = 12,
	///4k
	Resolution_Res4096_2160 = 13,
	///8k
	Resolution_Res7680_4320 = 14,
	Resolution_LAST = 15
};

inline const char **EnumNamesResolution() {
	static const char *names[] = { "NONE", "Res320_160", "Res320_240", "Res640_320", "Res640_480", "Res704_576", "Res1280_720", "Res1280_960", "Res1920_1080", "Res2048_1536", "Res2592_1944", "Res3072_2048", "Res3840_2160", "Res4096_2160", "Res7680_4320", "LAST", nullptr };
	return names;
}

inline const char *EnumNameResolution(Resolution e) { return EnumNamesResolution()[e]; }

enum StreamCompress {
	StreamCompress_NONE = 0,
	StreamCompress_JPEG = 1,
	StreamCompress_MJPEG = 2,
	StreamCompress_H264 = 3,
	StreamCompress_H265 = 4,
	StreamCompress_G711A = 5,
	StreamCompress_G711U = 6,
	StreamCompress_AAC = 7,
	StreamCompress_PCM = 8,
	StreamCompress_LAST = 9
};

inline const char **EnumNamesStreamCompress() {
	static const char *names[] = { "NONE", "JPEG", "MJPEG", "H264", "H265", "G711A", "G711U", "AAC", "PCM", "LAST", nullptr };
	return names;
}

inline const char *EnumNameStreamCompress(StreamCompress e) { return EnumNamesStreamCompress()[e]; }

enum Sensor {
	Sensor_NONE = 0,
	Sensor_OV9712 = 1,
	Sensor_AR0130 = 2,
	Sensor_MT9P006 = 3,
	Sensor_AR0330 = 4,
	Sensor_AR0331 = 5,
	Sensor_IMX122 = 6,
	Sensor_IMX222 = 7,
	Sensor_IMX322 = 8,
	Sensor_OV4689 = 9,
	Sensor_OV5658 = 10,
	Sensor_IMX178 = 11,
	Sensor_IMX290 = 12,
	Sensor_LAST = 13
};

inline const char **EnumNamesSensor() {
	static const char *names[] = { "NONE", "OV9712", "AR0130", "MT9P006", "AR0330", "AR0331", "IMX122", "IMX222", "IMX322", "OV4689", "OV5658", "IMX178", "IMX290", "LAST", nullptr };
	return names;
}

inline const char *EnumNameSensor(Sensor e) { return EnumNamesSensor()[e]; }

enum VideoSliceType {
	VideoSliceType_NONE = 0,
	VideoSliceType_SPS = 1,
	VideoSliceType_PPS = 2,
	VideoSliceType_SEI = 4,
	VideoSliceType_P = 8,
	VideoSliceType_B = 16,
	VideoSliceType_IDR = 32,
	VideoSliceType_LAST = 33
};

inline const char **EnumNamesVideoSliceType() {
	static const char *names[] = { "NONE", "SPS", "PPS", "", "SEI", "", "", "", "P", "", "", "", "", "", "", "", "B", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "IDR", "LAST", nullptr };
	return names;
}

inline const char *EnumNameVideoSliceType(VideoSliceType e) { return EnumNamesVideoSliceType()[e]; }

enum Chip {
	Chip_NONE = 0,
	Chip_WINDOWS_32 = 1,
	Chip_WINDOWS_64 = 2,
	Chip_LINUX_32 = 3,
	Chip_LINUX_64 = 4,
	Chip_MAC_32 = 5,
	Chip_MAC_64 = 6,
	Chip_IPC_H18E = 7,
	Chip_IPC_H18C = 8,
	Chip_IPC_H18A = 9,
	Chip_IPC_H16C = 10,
	Chip_IPC_H16A = 11,
	Chip_IPC_H16D = 12,
	Chip_IPC_A22M = 13,
	Chip_IPC_A33M = 14,
	Chip_IPC_A55M = 15,
	Chip_IPC_A65 = 16,
	Chip_IPC_A66 = 17,
	Chip_NVR_H20D = 18,
	Chip_NVR_H21 = 19,
	Chip_NVR_H31 = 20,
	Chip_NVR_H35 = 21,
	Chip_NVR_H36 = 22,
	Chip_LAST = 23
};

inline const char **EnumNamesChip() {
	static const char *names[] = { "NONE", "WINDOWS_32", "WINDOWS_64", "LINUX_32", "LINUX_64", "MAC_32", "MAC_64", "IPC_H18E", "IPC_H18C", "IPC_H18A", "IPC_H16C", "IPC_H16A", "IPC_H16D", "IPC_A22M", "IPC_A33M", "IPC_A55M", "IPC_A65", "IPC_A66", "NVR_H20D", "NVR_H21", "NVR_H31", "NVR_H35", "NVR_H36", "LAST", nullptr };
	return names;
}

inline const char *EnumNameChip(Chip e) { return EnumNamesChip()[e]; }

enum TimeFMT {
	TimeFMT_NONE = 0,
	TimeFMT_YMD = 1,
	TimeFMT_MDY = 2,
	TimeFMT_DMY = 3,
	TimeFMT_H24 = 4,
	TimeFMT_H12 = 5,
	TimeFMT_LAST = 6
};

inline const char **EnumNamesTimeFMT() {
	static const char *names[] = { "NONE", "YMD", "MDY", "DMY", "H24", "H12", "LAST", nullptr };
	return names;
}

inline const char *EnumNameTimeFMT(TimeFMT e) { return EnumNamesTimeFMT()[e]; }

enum Profile {
	Profile_NONE = 0,
	Profile_High = 1,
	Profile_Main = 2,
	Profile_BaseLine = 3,
	Profile_LAST = 4
};

inline const char **EnumNamesProfile() {
	static const char *names[] = { "NONE", "High", "Main", "BaseLine", "LAST", nullptr };
	return names;
}

inline const char *EnumNameProfile(Profile e) { return EnumNamesProfile()[e]; }

enum IrCutMode {
	IrCutMode_NONE = 0,
	IrCutMode_OPEN = 1,
	IrCutMode_CLOSE = 2,
	IrCutMode_AUTO = 3,
	IrCutMode_TIMER = 4,
	IrCutMode_LAST = 5
};

inline const char **EnumNamesIrCutMode() {
	static const char *names[] = { "NONE", "OPEN", "CLOSE", "AUTO", "TIMER", "LAST", nullptr };
	return names;
}

inline const char *EnumNameIrCutMode(IrCutMode e) { return EnumNamesIrCutMode()[e]; }

enum MirrorMode {
	MirrorMode_NONE = 0,
	MirrorMode_HOR = 1,
	MirrorMode_VER = 2,
	MirrorMode_HALFMIRROR = 3,
	MirrorMode_LAST = 4
};

inline const char **EnumNamesMirrorMode() {
	static const char *names[] = { "NONE", "HOR", "VER", "HALFMIRROR", "LAST", nullptr };
	return names;
}

inline const char *EnumNameMirrorMode(MirrorMode e) { return EnumNamesMirrorMode()[e]; }

enum ExposureMode {
	ExposureMode_NONE = 0,
	ExposureMode_AUTO = 1,
	ExposureMode_MANUAL = 2,
	ExposureMode_TRAFFIC = 3,
	ExposureMode_LAST = 4
};

inline const char **EnumNamesExposureMode() {
	static const char *names[] = { "NONE", "AUTO", "MANUAL", "TRAFFIC", "LAST", nullptr };
	return names;
}

inline const char *EnumNameExposureMode(ExposureMode e) { return EnumNamesExposureMode()[e]; }

enum WhiteBalanceMode {
	WhiteBalanceMode_NONE = 0,
	WhiteBalanceMode_OFF = 1,
	WhiteBalanceMode_AUTO = 2,
	///室外白天
	WhiteBalanceMode_DAYTIME = 3,
	///室外黄昏
	WhiteBalanceMode_EVENING = 4,
	///室外多去
	WhiteBalanceMode_CLOUDY = 5,
	///室内办公室
	WhiteBalanceMode_OFFICE = 6,
	///荧光灯
	WhiteBalanceMode_FLUORESCENT = 7,
	/// 白炽灯
	WhiteBalanceMode_INCANDESCENT = 8,
	/// 手动调节
	WhiteBalanceMode_MANUAL = 9,
	WhiteBalanceMode_LAST = 10
};

inline const char **EnumNamesWhiteBalanceMode() {
	static const char *names[] = { "NONE", "OFF", "", "AUTO", "DAYTIME", "EVENING", "CLOUDY", "OFFICE", "FLUORESCENT", "INCANDESCENT", "MANUAL", "LAST", nullptr };
	return names;
}

inline const char *EnumNameWhiteBalanceMode(WhiteBalanceMode e) { return EnumNamesWhiteBalanceMode()[e]; }

enum AntiFlckerMode {
	AntiFlckerMode_NONE = 0,
	AntiFlckerMode_INDOOR_50HZ = 1,
	AntiFlckerMode_OUTDOOR_50HZ = 2,
	AntiFlckerMode_AUTO_50HZ = 3,
	AntiFlckerMode_INDOOR_60HZ = 4,
	AntiFlckerMode_OUTDOOR_60HZ = 5,
	AntiFlckerMode_AUTO_60HZ = 6,
	AntiFlckerMode_THEATER_50HZ = 7,
	AntiFlckerMode_FAST_50HZ = 8,
	AntiFlckerMode_THEATER_60HZ = 9,
	AntiFlckerMode_FAST_60HZ = 10,
	AntiFlckerMode_ANTI_FLICKER_CLOSED = 11,
	AntiFlckerMode_LAST = 12
};

inline const char **EnumNamesAntiFlckerMode() {
	static const char *names[] = { "NONE", "INDOOR_50HZ", "OUTDOOR_50HZ", "AUTO_50HZ", "INDOOR_60HZ", "OUTDOOR_60HZ", "AUTO_60HZ", "THEATER_50HZ", "FAST_50HZ", "THEATER_60HZ", "FAST_60HZ", "ANTI_FLICKER_CLOSED", "LAST", nullptr };
	return names;
}

inline const char *EnumNameAntiFlckerMode(AntiFlckerMode e) { return EnumNamesAntiFlckerMode()[e]; }

enum NetGetMode {
	NetGetMode_NONE = 0,
	NetGetMode_MANUAL = 1,
	NetGetMode_AUTO = 2,
	NetGetMode_PPPOE = 3,
	NetGetMode_LAST = 4
};

inline const char **EnumNamesNetGetMode() {
	static const char *names[] = { "NONE", "MANUAL", "AUTO", "PPPOE", "LAST", nullptr };
	return names;
}

inline const char *EnumNameNetGetMode(NetGetMode e) { return EnumNamesNetGetMode()[e]; }

enum NetWorkDev {
	NetWorkDev_NONE = 0,
	NetWorkDev_LINK = 1,
	NetWorkDev_WIFI = 2,
	NetWorkDev_BlueTooth = 3,
	NetWorkDev_SIMLINK = 4,
	NetWorkDev_LAST = 5
};

inline const char **EnumNamesNetWorkDev() {
	static const char *names[] = { "NONE", "LINK", "WIFI", "BlueTooth", "SIMLINK", "LAST", nullptr };
	return names;
}

inline const char *EnumNameNetWorkDev(NetWorkDev e) { return EnumNamesNetWorkDev()[e]; }

enum UserCategory {
	UserCategory_NONE = 0,
	///超级管理员
	UserCategory_SUPERADMIN = 1,
	///管理员
	UserCategory_ADMIN = 2,
	///普通用户
	UserCategory_NORMALUSER = 3,
	UserCategory_LAST = 4
};

inline const char **EnumNamesUserCategory() {
	static const char *names[] = { "NONE", "SUPERADMIN", "ADMIN", "NORMALUSER", "LAST", nullptr };
	return names;
}

inline const char *EnumNameUserCategory(UserCategory e) { return EnumNamesUserCategory()[e]; }

enum LogType {
	LogType_NONE = 0,
	LogType_SYSTEM_OPERATE = 1,
	LogType_DATA_MANAGE = 2,
	LogType_RECORD_MANAGE = 3,
	LogType_LOG_MANAGE = 4,
	LogType_PROFILE_OPERATE = 5,
	LogType_ALARM_EVENT = 6,
	LogType_LAST = 7
};

inline const char **EnumNamesLogType() {
	static const char *names[] = { "NONE", "SYSTEM_OPERATE", "DATA_MANAGE", "RECORD_MANAGE", "LOG_MANAGE", "PROFILE_OPERATE", "ALARM_EVENT", "LAST", nullptr };
	return names;
}

inline const char *EnumNameLogType(LogType e) { return EnumNamesLogType()[e]; }

enum UserAccess {
	UserAccess_NONE = 0,
	UserAccess_VISIT_VIDEO = 1,
	UserAccess_VISIT_AUDIO = 2,
	UserAccess_TALK_BACK = 3,
	UserAccess_VISIT_PTZ = 4,
	UserAccess_VISIT_LOG = 5,
	UserAccess_MODIFY_PROFILE = 6,
	UserAccess_VISIT_PROFILE = 7,
	UserAccess_LAST = 8
};

inline const char **EnumNamesUserAccess() {
	static const char *names[] = { "NONE", "VISIT_VIDEO", "VISIT_AUDIO", "TALK_BACK", "VISIT_PTZ", "VISIT_LOG", "MODIFY_PROFILE", "VISIT_PROFILE", "LAST", nullptr };
	return names;
}

inline const char *EnumNameUserAccess(UserAccess e) { return EnumNamesUserAccess()[e]; }

enum AlarmEvent {
	AlarmEvent_NONE = 0,
	AlarmEvent_PORT_In = 1,
	AlarmEvent_PORT_Out = 2,
	///人脸识别
	AlarmEvent_VIDEO_FaceRecognition = 3,
	///人脸检测
	AlarmEvent_VIDEO_FaceDetection = 4,
	///移动帧测
	AlarmEvent_VIDEO_MotionDetection = 5,
	///移动跟踪
	AlarmEvent_VIDEO_MotionFollow = 6,
	///车牌识别
	AlarmEvent_VIDEO_PlatenumberRecognition = 7,
	///设备掉线
	AlarmEvent_VIDEO_DevOffline = 8,
	///视频丢失
	AlarmEvent_VIDEO_Lost = 9,
	///视频遮挡
	AlarmEvent_VIDEO_Blind = 10,
	///物品遗失
	AlarmEvent_VIDEO_SomethingLoss = 11,
	///硬盘
	AlarmEvent_DISK_ERROR = 12,
	AlarmEvent_DISK_LOST = 13,
	AlarmEvent_LAST = 14
};

inline const char **EnumNamesAlarmEvent() {
	static const char *names[] = { "NONE", "PORT_In", "PORT_Out", "VIDEO_FaceRecognition", "VIDEO_FaceDetection", "VIDEO_MotionDetection", "VIDEO_MotionFollow", "VIDEO_PlatenumberRecognition", "VIDEO_DevOffline", "VIDEO_Lost", "VIDEO_Blind", "VIDEO_SomethingLoss", "DISK_ERROR", "DISK_LOST", "LAST", nullptr };
	return names;
}

inline const char *EnumNameAlarmEvent(AlarmEvent e) { return EnumNamesAlarmEvent()[e]; }

enum DiskStatus {
	DiskStatus_NONE = 0,
	DiskStatus_RUN = 1,
	///休眠
	DiskStatus_DORMANCY = 2,
	///损坏
	DiskStatus_BROKE = 3,
	///未格式化
	DiskStatus_UNFORMATTED = 4,
	///不可识别
	DiskStatus_UNIDENTIFICATION = 5,
	DiskStatus_LAST = 6
};

inline const char **EnumNamesDiskStatus() {
	static const char *names[] = { "NONE", "RUN", "DORMANCY", "BROKE", "UNFORMATTED", "UNIDENTIFICATION", "LAST", nullptr };
	return names;
}

inline const char *EnumNameDiskStatus(DiskStatus e) { return EnumNamesDiskStatus()[e]; }

enum RecordFileType {
	RecordFileType_NONE = 0,
	RecordFileType_ALARM_PORTIN = 1,
	RecordFileType_ALARM_FACERECOGNITION = 2,
	RecordFileType_ALARM_FACEDETECTION = 3,
	RecordFileType_ALARM_PLATENUMBERRECOGNITION = 4,
	RecordFileType_ALARM_MOTIONDETECTION = 5,
	RecordFileType_ALARM_MOTIONTRACE = 6,
	RecordFileType_TIMER = 7,
	RecordFileType_REALTIME = 8,
	RecordFileType_LAST = 9
};

inline const char **EnumNamesRecordFileType() {
	static const char *names[] = { "NONE", "ALARM_PORTIN", "ALARM_FACERECOGNITION", "ALARM_FACEDETECTION", "ALARM_PLATENUMBERRECOGNITION", "ALARM_MOTIONDETECTION", "ALARM_MOTIONTRACE", "TIMER", "REALTIME", "LAST", nullptr };
	return names;
}

inline const char *EnumNameRecordFileType(RecordFileType e) { return EnumNamesRecordFileType()[e]; }

enum RecordFileDownloadCtrl {
	RecordFileDownloadCtrl_NONE = 0,
	RecordFileDownloadCtrl_FastX1 = 1,
	RecordFileDownloadCtrl_FastX2 = 2,
	RecordFileDownloadCtrl_FastX3 = 3,
	RecordFileDownloadCtrl_FastX4 = 4,
	RecordFileDownloadCtrl_LowX1 = 5,
	RecordFileDownloadCtrl_LowX2 = 6,
	RecordFileDownloadCtrl_LowX3 = 7,
	RecordFileDownloadCtrl_LowX4 = 8,
	RecordFileDownloadCtrl_Stop = 9,
	RecordFileDownloadCtrl_Pause = 10,
	RecordFileDownloadCtrl_Single = 11,
	RecordFileDownloadCtrl_LAST = 12
};

inline const char **EnumNamesRecordFileDownloadCtrl() {
	static const char *names[] = { "NONE", "FastX1", "FastX2", "FastX3", "FastX4", "LowX1", "LowX2", "LowX3", "LowX4", "Stop", "Pause", "Single", "LAST", nullptr };
	return names;
}

inline const char *EnumNameRecordFileDownloadCtrl(RecordFileDownloadCtrl e) { return EnumNamesRecordFileDownloadCtrl()[e]; }

enum TransportStreamCtrl {
	TransportStreamCtrl_NONE = 0,
	///只发I帧
	TransportStreamCtrl_SINGLE_I = 1,
	///将发送不出去的P帧丢弃
	TransportStreamCtrl_DROP_LAST_P = 2,
	///调节码率
	TransportStreamCtrl_CHANGE_CBR = 3,
	///调节帧率
	TransportStreamCtrl_CHANGE_FPS = 4,
	///chuckSize
	TransportStreamCtrl_CHUCKSIZE = 5,
	TransportStreamCtrl_LAST = 6
};

inline const char **EnumNamesTransportStreamCtrl() {
	static const char *names[] = { "NONE", "SINGLE_I", "DROP_LAST_P", "CHANGE_CBR", "CHANGE_FPS", "CHUCKSIZE", "LAST", nullptr };
	return names;
}

inline const char *EnumNameTransportStreamCtrl(TransportStreamCtrl e) { return EnumNamesTransportStreamCtrl()[e]; }

enum PtzBaudrate {
	PtzBaudrate_RNONE = 0,
	PtzBaudrate_R2400 = 1,
	PtzBaudrate_R4800 = 2,
	PtzBaudrate_R9600 = 3,
	PtzBaudrate_R19200 = 4,
	PtzBaudrate_R38400 = 5,
	PtzBaudrate_R57600 = 6,
	PtzBaudrate_R115200 = 7,
	PtzBaudrate_RLAST = 8
};

inline const char **EnumNamesPtzBaudrate() {
	static const char *names[] = { "RNONE", "R2400", "R4800", "R9600", "R19200", "R38400", "R57600", "R115200", "RLAST", nullptr };
	return names;
}

inline const char *EnumNamePtzBaudrate(PtzBaudrate e) { return EnumNamesPtzBaudrate()[e]; }

enum PtzVeriFy {
	///无校验
	PtzVeriFy_None = 0,
	///奇校验
	PtzVeriFy_Odd = 1,
	///偶校验
	PtzVeriFy_Even = 2,
	///标志校验
	PtzVeriFy_Mark = 3,
	///空校验
	PtzVeriFy_Empty = 4,
	///最后
	PtzVeriFy_Last = 5
};

inline const char **EnumNamesPtzVeriFy() {
	static const char *names[] = { "None", "Odd", "Even", "Mark", "Empty", "Last", nullptr };
	return names;
}

inline const char *EnumNamePtzVeriFy(PtzVeriFy e) { return EnumNamesPtzVeriFy()[e]; }

enum PtzProtocol {
	PtzProtocol_None = 0,
	PtzProtocol_Pelcod = 1,
	PtzProtocol_Pelcod1 = 2,
	PtzProtocol_Pelcod_s = 3,
	PtzProtocol_Pelcod_s1 = 4,
	PtzProtocol_Pelcop = 5,
	PtzProtocol_Pelcop1 = 6,
	PtzProtocol_Pelcop5 = 7,
	PtzProtocol_SamSung = 8,
	PtzProtocol_Sony = 9,
	PtzProtocol_Last = 10
};

inline const char **EnumNamesPtzProtocol() {
	static const char *names[] = { "None", "Pelcod", "Pelcod1", "Pelcod_s", "Pelcod_s1", "Pelcop", "Pelcop1", "Pelcop5", "SamSung", "Sony", "Last", nullptr };
	return names;
}

inline const char *EnumNamePtzProtocol(PtzProtocol e) { return EnumNamesPtzProtocol()[e]; }

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

enum ProgressStatus {
	ProgressStatus_None = 0,
	ProgressStatus_UpgradeStart = 1,
	ProgressStatus_UpgradeRecvData = 2,
	ProgressStatus_UpgradeWipePartition = 3,
	ProgressStatus_UpgradeWriteData = 4,
	ProgressStatus_UpgradeOver = 5,
	ProgressStatus_Rebootting = 6,
	ProgressStatus_ModifyOver = 7,
	ProgressStatus_HaveNoResource = 8,
	ProgressStatus_Last = 9
};

inline const char **EnumNamesProgressStatus() {
	static const char *names[] = { "None", "UpgradeStart", "UpgradeRecvData", "UpgradeWipePartition", "UpgradeWriteData", "UpgradeOver", "Rebootting", "ModifyOver", "HaveNoResource", "Last", nullptr };
	return names;
}

inline const char *EnumNameProgressStatus(ProgressStatus e) { return EnumNamesProgressStatus()[e]; }

enum LinkProtocol {
	LinkProtocol_None = 0,
	LinkProtocol_Moon = 1,
	LinkProtocol_Onvif = 2,
	LinkProtocol_GBT28181 = 3,
	LinkProtocol_RTSP = 4,
	LinkProtocol_Last = 5
};

inline const char **EnumNamesLinkProtocol() {
	static const char *names[] = { "None", "Moon", "Onvif", "GBT28181", "RTSP", "Last", nullptr };
	return names;
}

inline const char *EnumNameLinkProtocol(LinkProtocol e) { return EnumNamesLinkProtocol()[e]; }


#endif

