#ifndef _AVENUM_H_
#define _AVENUM_H_



typedef enum {
	IRet_succeed,
	IRet_faild,
	IRet_uninitialized,
	IRet_initialized,
	IRet_timeout,
	IRet_unlogin,
	IRet_unsupport,
	IRet_linking,
	IRet_droplinked,
	IRet_closeed,
}I_RET;


typedef enum {
	CHL_MAIN_T = 0,
	CHL_SUB1_T = 1,
	CHL_SUB2_T = 2,
	CHL_SUB3_T = 3,
	CHL_SNAP_T = 4,
	CHL_JPEG_T = 5,
	CHL_NR_T = 6,
}E_EncodeCHL;

typedef enum {
	CHL_ACAP_T = 0,
	CHL_APLY_T = 1,
	CHL_A_NUM = 2,
}E_AUDIO_CHL;



typedef enum{
	COMMAND_ACK_REQ = 0x80000000,
	COMMAND_ACK_RESP = 0x40000000,
	COMMAND_ACK_SET = 0X20000000,
	COMMAND_ACK_SETRESP = 0X10000000,

	COMMAND_ACK_ADD = 0x08000000,
	COMMAND_ACK_ADDRESP = 0x04000000,
	COMMAND_ACK_DEL = 0x02000000,
	COMMAND_ACK_DELRESP = 0x01000000,

	COMMAND_ACK_MODIFY = 0x00800000,
	COMMAND_ACK_MODIFYRESP = 0x00400000,

	COMMAND_ACK_DELCMD = 0x0000ffff,
	COMMAND_ACK_DELMSG = 0xffff0000,
}COMMAND_E;


typedef enum  {
  BitRateCtrl_NONE = 0,
  BitRateCtrl_CBR = 1,
  BitRateCtrl_VBR = 2,
  BitRateCtrl_VCBR = 3,
  BitRateCtrl_LAST = 4
}BitRateCtrl;

typedef enum  {
  StreamContent_NONE = 0,
  StreamContent_VIDEO = 1,
  StreamContent_AUDIO = 2,
  StreamContent_AUDIO_PLAY = 3,
  StreamContent_LAST = 4
}StreamContent;


typedef enum  {
  TransportMode_NONE = 0,
  TransportMode_PUSH = 1,
  TransportMode_PULL = 2,
  TransportMode_LAST = 3
}TransportMode;


typedef enum  {
  CaptureSize_Self = 0,
  CaptureSize_QVGAEX = 1,
  CaptureSize_QVGA = 2,
  CaptureSize_VGAEX = 3,
  CaptureSize_VGA = 4,
  CaptureSize_D1 = 5,
  CaptureSize_HD720P = 6,
  CaptureSize_HD960P = 7,
  CaptureSize_HD1080P = 8,
  CaptureSize_HC300W = 9,
  CaptureSize_HC400W = 10,
  CaptureSize_HC500W = 11,
  CaptureSize_HC600W = 12,
  CaptureSize_HC800W = 13,
  CaptureSize_HC4K = 14,
  CaptureSize_HC8K = 15,
  CaptureSize_CNR = 16,
  CaptureSize_S0800_0600 = 17,
  CaptureSize_S1024_0768 = 18,
  CaptureSize_S1280_0768 = 19,
  CaptureSize_S1280_0800 = 20,
  CaptureSize_S1280_1024 = 21,
  CaptureSize_S1366_0768 = 22,
  CaptureSize_S1400_0900 = 23,
  CaptureSize_S1440_0900 = 24,
  CaptureSize_S1400_1050 = 25,
  CaptureSize_S1600_0900 = 26,
  CaptureSize_S1680_1050 = 27,
  CaptureSize_S1920_1080 = 28,
  CaptureSize_S4K = 29,
  CaptureSize_SNR = 30,
  CaptureSize_LAST = 31
}CaptureSize;


typedef enum  {
  AvComp_H264 = 0,
  AvComp_H265 = 1,
  AvComp_MJPEG = 2,
  AvComp_JPEG = 3,
  AvComp_PCM = 4,
  AvComp_G711A = 5,
  AvComp_G711U = 6,
  AvComp_AAC = 7,
  AvComp_AMR_NB = 8,
  AvComp_AMR_WB = 9,
  AvComp_MP3 = 10,
  AvComp_LAST = 11
}AvComp;

typedef enum {
		AvSensor_NONE = 0,
		AvSensor_OV9712 = 1,
		AvSensor_OV9732 = 2,
		AvSensor_OV9750 = 3,
		AvSensor_OV9752 = 4,
		AvSensor_OV2718 = 5,
		AvSensor_OV4689 = 6,
		AvSensor_OV5658 = 7,
		AvSensor_AR0130 = 8,
		AvSensor_AR0230 = 9,
		AvSensor_AR0237 = 10,
		AvSensor_AR0330 = 11,
		AvSensor_AR9M034 = 12,
		AvSensor_MN34222 = 13,
		AvSensor_IMX104 = 14,
		AvSensor_IMX138 = 15,
		AvSensor_IMX123 = 16,
		///IMX122 222 322 统属IMX222
		AvSensor_IMX222 = 17,
		AvSensor_IMX236 = 18,
		AvSensor_IMX117 = 19,
		AvSensor_IMX178 = 20,
		AvSensor_IMX185 = 21,
		AvSensor_IMX290 = 22,
		AvSensor_IMX291 = 23,
		AvSensor_BT1120 = 24,
		AvSensor_BT656 = 25,
		AvSensor_CVBS = 26,
		AvSensor_USBCAMERA = 27,
		AvSensor_SCREEN = 28,
		AvSensor_LAST = 31
}AvSensor;


typedef enum  {
  avFrameT_NONE = 0,
  avFrameT_P = 1,
  avFrameT_B = 2,
  avFrameT_I = 4,
  avFrameT_JPEG = 8,
  avFrameT_AUDIO = 16,
  avFrameT_LAST = 33
}avFrameT;


typedef enum  {
  nal_unit_type_h264_nr = 0,
  nal_unit_type_h264_p = 1,
  nal_unit_type_h264_dataA = 2,
  nal_unit_type_h264_dataB = 3,
  nal_unit_type_h264_dataC = 4,
  nal_unit_type_h264_idr = 5,
  nal_unit_type_h264_sei = 6,
  nal_unit_type_h264_sps = 7,
  nal_unit_type_h264_pps = 8,
  nal_unit_type_h264_delimiter = 9,
  nal_unit_type_h264_nalend = 10,
  nal_unit_type_h264_streamend = 11,
  nal_unit_type_h264_pading = 12
}nal_unit_type_h264;


typedef enum  {
  nal_unit_type_hevc_trail_n = 0,
  nal_unit_type_hevc_trail_r = 1,
  nal_unit_type_hevc_idr = 19,
  nal_unit_type_hevc_vps = 32,
  nal_unit_type_hevc_sps = 33,
  nal_unit_type_hevc_pps = 34,
  nal_unit_type_hevc_sei = 39
}nal_unit_type_hevc;

enum AvChip {
	AvChip_H18EV100 = 0,
	AvChip_H18EV200 = 1,
	AvChip_H18EV201 = 2,
	AvChip_H18C = 3,
	AvChip_H18A = 4,
	AvChip_H16CV100 = 5,
	AvChip_H16CV200 = 6,
	AvChip_H16CV300 = 7,
	AvChip_H16A = 8,
	AvChip_H16D = 9,
	AvChip_H19 = 10,
	AvChip_HIPC_RES_1 = 11,
	AvChip_HIPC_RES_2 = 12,
	AvChip_H20D = 13,
	AvChip_H21 = 14,
	AvChip_H31 = 15,
	AvChip_H35 = 16,
	AvChip_H36 = 17,
	AvChip_H3798MV100 = 18,
	AvChip_HNVR_RES_2 = 19,
	AvChip_S2L22M = 20,
	AvChip_S2L33M = 21,
	AvChip_S2L55M = 22,
	AvChip_S2L65 = 23,
	AvChip_S2L66 = 24,
	AvChip_WINDOWS_32 = 25,
	AvChip_WINDOWS_64 = 26,
	AvChip_LINUX_32 = 27,
	AvChip_LINUX_64 = 28,
	AvChip_MAC_32 = 29,
	AvChip_MAC_64 = 30,
	AvChip_LAST = 31
};

typedef enum  {
	HardInterface_NONE = 0,
	HardInterface_SD = 1,
	HardInterface_WIFI = 2,
	HardInterface_PTZ = 3,
	HardInterface_IO = 4,
	HardInterface_AUDIO_PLAY = 5,
	HardInterface_AUDIO_CAPTURE = 6,
	HardInterface_SIM_4G = 7,
	HardInterface_LAST = 8
}HardInterface;

typedef enum  {
  TimeFMT_NONE = 0,
  TimeFMT_YMD = 1,
  TimeFMT_MDY = 2,
  TimeFMT_DMY = 3,
  TimeFMT_H24 = 4,
  TimeFMT_H12 = 5,
  TimeFMT_LAST = 6
}TimeFMT;

typedef enum  {
  EnCodeProfile_NONE = 0,
  EnCodeProfile_High = 1,
  EnCodeProfile_Main = 2,
  EnCodeProfile_BaseLine = 3,
  EnCodeProfile_LAST = 4
}EnCodeProfile;


typedef enum  {
  IrCutMode_NONE = 0,
  IrCutMode_OPEN = 1,
  IrCutMode_CLOSE = 2,
  IrCutMode_AUTO = 3,
  IrCutMode_TIMER = 4,
  IrCutMode_LAST = 5
}IrCutMode;


typedef enum  {
  MirrorMode_NONE = 0,
  MirrorMode_HOR = 1,
  MirrorMode_VER = 2,
  MirrorMode_Rotate_000 = 3,
  MirrorMode_Rotate_090 = 4,
  MirrorMode_Rotate_180 = 5,
  MirrorMode_Rotate_270 = 6,
  MirrorMode_LAST = 7
}MirrorMode;

typedef enum  {
  ExposureMode_NONE = 0,
  ExposureMode_AUTO = 1,
  ExposureMode_MANUAL = 2,
  ExposureMode_TRAFFIC = 3,
  ExposureMode_LAST = 4
}ExposureMode;

typedef enum  {
  WhiteBalanceMode_NONE = 0,
  WhiteBalanceMode_OFF = 1,
  WhiteBalanceMode_AUTO = 3,
  ///室外白天
  WhiteBalanceMode_DAYTIME = 4,
  ///室外黄昏
  WhiteBalanceMode_EVENING = 5,
  ///室外多去
  WhiteBalanceMode_CLOUDY = 6,
  ///室内办公室
  WhiteBalanceMode_OFFICE = 7,
  ///荧光灯
  WhiteBalanceMode_FLUORESCENT = 8,
  /// 白炽灯
  WhiteBalanceMode_INCANDESCENT = 9,
  /// 手动调节
  WhiteBalanceMode_MANUAL = 10,
  WhiteBalanceMode_LAST = 11
}WhiteBalanceMode;


typedef enum  {
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
}AntiFlckerMode;


typedef enum  {
	WdrMode_None = 0,
	WdrMode_Open = 1,
	WdrMode_Close = 2,
	WdrMode_Last = 3
}WdrMode;

typedef enum  {
	ShutterMode_None = 0,
	ShutterMode_Auto = 1,
	ShutterMode_MaxShutter = 2,
	ShutterMode_Last = 3,
}ShutterMode;

typedef enum  {
  NetCommGetMode_NONE = 0,
  NetCommGetMode_MANUAL = 1,
  NetCommGetMode_AUTO = 2,
  NetCommGetMode_PPPOE = 3,
  NetCommGetMode_LAST = 4
}NetCommGetMode;

typedef enum {
	WirelessMode_NONE = 0,
	WirelessMode_AP = 1,
	WirelessMode_STATION = 2,
	WirelessMode_LAST = 3
}WirelessMode;


typedef enum  {
  NetCommT_LAN0			= 0,
  NetCommT_LAN1			= 1,
  NetCommT_Wireless		= 2,
  NetCommT_SIM			= 3,
  NetCommT_BlueTooth	= 4,
  NetCommT_Wireless1	= 5,
  NetCommT_SIM1			= 6,
  NetCommT_LAST			= 7
}NetCommT;

typedef enum  {
  NetCommWirelessMode_NONE = 0,
  NetCommWirelessMode_AP = 1,
  NetCommWirelessMode_Station = 2,
  NetCommWirelessMode_LAST,
}NetCommWirelessModeT;


typedef enum  {
  UserCategory_NONE = 0,
  ///超级管理员
  UserCategory_SUPERADMIN = 1,
  ///管理员
  UserCategory_ADMIN = 2,
  ///普通用户
  UserCategory_NORMALUSER = 3,
  UserCategory_LAST = 4
}UserCategory;

typedef enum {
	EncryptionAlgorithm_NONE = 0,
	EncryptionAlgorithm_AUTO = 1,
	EncryptionAlgorithm_ASCII = 2,
	EncryptionAlgorithm_HEXADECIMAL = 3,
	EncryptionAlgorithm_AES = 4,
	EncryptionAlgorithm_TKIP = 5,
	EncryptionAlgorithm_LAST = 6
}EncryptionAlgorithm;

typedef enum {
	CertificationType_NONE = 0,
	CertificationType_AUTO = 1,
	CertificationType_WPA_PSK = 2,
	CertificationType_WPA2_PSK = 3,
	CertificationType_WPA = 4,
	CertificationType_WPA2 = 5,
	CertificationType_WEP = 6,
	CertificationType_LAST = 7
}CertificationType;


typedef enum  {
  LogType_NONE = 0,
  LogType_SYSTEM_OPERATE = 1,
  LogType_DATA_MANAGE = 2,
  LogType_RECORD_MANAGE = 3,
  LogType_LOG_MANAGE = 4,
  LogType_PROFILE_OPERATE = 5,
  LogType_ALARM_EVENT = 6,
  LogType_LAST = 7
}LogType;


typedef enum  {
  UserAccess_NONE = 0,
  UserAccess_VISIT_VIDEO = 1,
  UserAccess_VISIT_AUDIO = 2,
  UserAccess_TALK_BACK = 3,
  UserAccess_VISIT_PTZ = 4,
  UserAccess_VISIT_LOG = 5,
  UserAccess_MODIFY_PROFILE = 6,
  UserAccess_VISIT_PROFILE = 7,
  UserAccess_LAST = 8
}UserAccess;

typedef enum {
	AlarmStat_None = 0,
	AlarmStat_Start = 1,
	AlarmStat_Stop = 2,
	AlarmStat_Ongoing = 3,
	AlarmStat_Nr = 4
}AlarmStat;




typedef enum  {
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
  //ftp上传图片,测试ftp能否成功
  AlarmEvent_TestFtpUpload = 14,
  AlarmEvent_LAST
}AlarmEvent;


typedef enum  {
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
}DiskStatus;


typedef enum  {
  RecordFileType_NONE = 0,
  RecordFileType_ALARM_PORTIN = 1,
  RecordFileType_ALARM_FACERECOGNITION = 2,
  RecordFileType_ALARM_FACEDETECTION = 3,
  RecordFileType_ALARM_PLATENUMBERRECOGNITION = 4,
  RecordFileType_ALARM_MOTIONDETECTION = 5,
  RecordFileType_ALARM_MOTIONTRACE = 6,
  RecordFileType_TIMER = 7,
  RecordFileType_REALTIME = 8,
  RecordFileType_ALARM = 9,
  RecordFileType_LAST = 10
}RecordFileType;


typedef enum  {
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
}RecordFileDownloadCtrl;


typedef enum  {
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
}TransportStreamCtrl;


typedef enum  {
  PtzBaudrate_RNONE = 0,
  PtzBaudrate_R2400 = 1,
  PtzBaudrate_R4800 = 2,
  PtzBaudrate_R9600 = 3,
  PtzBaudrate_R19200 = 4,
  PtzBaudrate_R38400 = 5,
  PtzBaudrate_R57600 = 6,
  PtzBaudrate_R115200 = 7,
  PtzBaudrate_RLAST = 8
}PtzBaudrate;


typedef enum  {
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
}PtzVeriFy;


typedef enum  {
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
}PtzProtocol;


typedef enum  {
  AudioSampleRate_NONE = 0,
  AudioSampleRate_R8000 = 1,
  AudioSampleRate_R11025 = 2,
  AudioSampleRate_R12000 = 3,
  AudioSampleRate_R16000 = 4,
  AudioSampleRate_R22050 = 5,
  AudioSampleRate_R24000 = 6,
  AudioSampleRate_R32000 = 7,
  AudioSampleRate_R44100 = 8,
  AudioSampleRate_R48000 = 9,
  AudioSampleRate_R64000 = 10,
  AudioSampleRate_R88200 = 11,
  AudioSampleRate_R96000 = 12,
  AudioSampleRate_R192000 = 13,
  AudioSampleRate_LAST = 14
}AudioSampleRate;


typedef enum  {
  AudioSampleBits_NONE = 0,
  AudioSampleBits_B8 = 1,
  AudioSampleBits_B16 = 2,
  AudioSampleBits_B24 = 3,
  AudioSampleBits_B32 = 4,
  AudioSampleBits_LAST = 5
}AudioSampleBits;


typedef enum  {
  AudioSoundMode_NONE = 0,
  AudioSoundMode_MONO = 1,
  AudioSoundMode_STEREO = 2,
  AudioSoundMode_LAST = 3
}AudioSoundMode;


typedef enum  {
  AudioCaptureDevice_NONE = 0,
  AudioCaptureDevice_LINE_IN = 1,
  AudioCaptureDevice_MIC_IN = 2,
  AudioCaptureDevice_LAST = 3
}AudioCaptureDevice;

typedef enum  {
	AudioVoiceQualityEnhancement_NONE = 0,
	///回声消除
	AudioVoiceQualityEnhancement_AEC = 1,
	///语音降噪
	AudioVoiceQualityEnhancement_ANR = 2,
	///录音降噪
	AudioVoiceQualityEnhancement_RNR = 3,
	///高通滤波
	AudioVoiceQualityEnhancement_HPF = 4,
	///自动增益控制
	AudioVoiceQualityEnhancement_AGC = 5,
	///均衡处理器
	AudioVoiceQualityEnhancement_EQ = 6,
	///音量调节模块
	AudioVoiceQualityEnhancement_GAIN = 7,
	///重采样
	AudioVoiceQualityEnhancement_RES = 8,
	AudioVoiceQualityEnhancement_LAST = 9
}AudioVoiceQualityEnhancement;

 typedef enum{
	OverLayType_Title = 0,
	OverLayType_Time = 1,
	OverLayType_Last = 2
}OverLayType;


 typedef enum {
	FontSize_None = 0,
	FontSize__8 = 1,
	FontSize__16 = 2,
	FontSize__24 = 3,
	FontSize__32 = 4,
	FontSize__48 = 5,
	FontSize__64 = 6,
	FontSize__128 = 7,
	FontSize_Last = 8
 }FontSize;


 typedef enum {
	OverLayDateTimeStyle_None = 0,
	OverLayDateTimeStyle_Y_M_D_H_M_S = 1,
	OverLayDateTimeStyle_Y_M_D_M_H_M_S = 2,
	OverLayDateTimeStyle_H_M_S_Y_M_D = 3,
	OverLayDateTimeStyle_Last = 4
 }OverLayDateTimeStyle;

typedef enum {
	OverLayDateTimeSplit_None = 0,
	OverLayDateTimeSplit_Split_Symbol = 1,
	OverLayDateTimeSplit_Split_Word = 2,
	OverLayDateTimeSplit_Last = 3
} OverLayDateTimeSplit;


typedef enum  {
		PtzCommand_NONE = 0,
		///方向
		PtzCommand_STOP = 1,
		PtzCommand_UP = 2,
		PtzCommand_DOWN = 3,
		PtzCommand_LEFT = 4,
		PtzCommand_RIGHT = 5,
		PtzCommand_LEFTUP = 6,
		PtzCommand_LEFTDOWN = 7,
		PtzCommand_RIGHTUP = 8,
		PtzCommand_RIGHTDOWN = 9,
		///绝对运动
		PtzCommand_ABSOLUTE_MOVE = 10,
		///相对运动
		PtzCommand_RELATIVE_MOVE = 11,
		///绝对变焦
		PtzCommand_ABSOLUTE_FOCUS = 12,
		///变倍
		PtzCommand_ZOOM_WIDE = 13,
		PtzCommand_ZOOM_TELE = 14,
		///聚焦
		PtzCommand_FOCUS_FAR = 15,
		PtzCommand_FOCUS_NEAR = 16,
		///光圈
		PtzCommand_IRIS_LARGE = 17,
		PtzCommand_IRIS_SMALL = 18,
		///报警功能
		PtzCommand_ALARM = 19,
		///灯光
		PtzCommand_LIGHT_ON = 20,
		PtzCommand_LIGHT_OFF = 21,
		///设置预置点
		PtzCommand_SETPRESET = 22,
		///清除预置点
		PtzCommand_CLEARPRESET = 23,
		///转至预置点
		PtzCommand_GOTOPRESET = 24,
		///水平开始 
		PtzCommand_AUTOPANON = 25,
		///水平结束
		PtzCommand_AUTOPANOFF = 26,
		///设置边界 
		PtzCommand_SETLIMIT = 27,
		///自动扫描开始
		PtzCommand_AUTOSCANON = 28,
		///自动扫描开停止
		PtzCommand_AUTOSCANOFF = 29,
		///增加巡航点
		PtzCommand_ADDTOUR = 30,
		///删除巡航点 	
		PtzCommand_DELETETOUR = 31,
		///开始巡航
		PtzCommand_STARTTOUR = 32,
		///结束巡航
		PtzCommand_STOPTOUR = 33,
		///删除巡航
		PtzCommand_CLEARTOUR = 34,
		///设置模式
		PtzCommand_SETPATTERN = 35,
		///开始模式
		PtzCommand_STARTPATTERN = 36,
		///停止模式
		PtzCommand_STOPPATTERN = 37,
		///清除模式
		PtzCommand_CLEARPATTERN = 38,
		///快速定位
		PtzCommand_POSITION = 39,
		///辅助开关
		PtzCommand_AUX_ON = 40,
		PtzCommand_AUX_OFF = 41,
		///球机菜单
		PtzCommand_MENU = 42,
		///退出球机菜单
		PtzCommand_EXIT = 43,
		///确认
		PtzCommand_ENTER = 44,
		///取消
		PtzCommand_ESC = 45,
		///菜单上下操作
		PtzCommand_MENUUPDOWN = 46,
		///菜单左右操作
		PtzCommand_MENULEFTRIGHT = 47,
		///矩阵切换
		PtzCommand_MATRIXSWITCH = 48,
		/// 镜头翻转
		PtzCommand_FLIP = 49,
		///云台复位
		PtzCommand_RESET = 50,
		///菜单左右操作 （主要用于网络对灯光控制器的控制）
		PtzCommand_LIGHTCONTROLLER = 51
}PtzCommand;


typedef enum  {
	ProgressStatus_None = 0,
	ProgressStatus_UpgradeStart = 1,
	ProgressStatus_UpgradeRecvData = 2,
	ProgressStatus_UpgradeWipePartition = 3,
	ProgressStatus_UpgradeWriteData = 4,
	ProgressStatus_UpgradeOver = 5,
	ProgressStatus_UpgradeExtract = 6,
	ProgressStatus_UpgradeCopy = 7,
	ProgressStatus_UpgradeInstall = 8,
	ProgressStatus_Rebootting = 9,
	ProgressStatus_ModifyOver = 10,
	ProgressStatus_HaveNoDiskResource = 11,
	ProgressStatus_HaveNoRightResource = 12,
	ProgressStatus_LinkFailed = 13,
	ProgressStatus_UpgradeFirmwareCheckout = 14,
	ProgressStatus_UpgradeFirmwareCheckoutSucceed = 15,
	ProgressStatus_UpgradeFirmwareCheckoutFailed = 16,
	ProgressStatus_Last = 17
}ProgressStatus;





typedef enum  {
	PtzCameraLensFocusCtrlMode_None = 0,
	PtzCameraLensFocusCtrlMode_Auto = 1,
	PtzCameraLensFocusCtrlMode_SemiAutomatic = 2,
	PtzCameraLensFocusCtrlMode_Manual = 3,
	PtzCameraLensFocusCtrlMode_Last = 4
}PtzCameraLensFocusCtrlMode;

///PtzCameraLensFocusSensitivity
typedef enum  {
	PtzCameraLensFocusRegion_None = 0,
	PtzCameraLensFocusRegion_AllRegion = 1,
	PtzCameraLensFocusRegion_CentralRegion = 2,
	PtzCameraLensFocusRegion_Last = 3
}PtzCameraLensFocusRegion;


typedef enum  {
	PtzCameraLensFocusSearchMode_None = 0,
	///全景
	PtzCameraLensFocusSearchMode_FullView = 1,
	PtzCameraLensFocusSearchMode_FAR_1_5M = 2,
	PtzCameraLensFocusSearchMode_FAR_3_0M = 3,
	PtzCameraLensFocusSearchMode_FAR_6_0M = 4,
	PtzCameraLensFocusSearchMode_FAR_8_0M = 5,
	///无穷远
	PtzCameraLensFocusSearchMode_FAR_Infinity = 6,
	PtzCameraLensFocusSearchMode_Last = 7
}PtzCameraLensFocusSearchMode;



typedef enum  {
	PtzCameraLensIrisCtrlMode_None = 0,
	PtzCameraLensIrisCtrlMode_Auto = 1,
	PtzCameraLensIrisCtrlMode_Manual = 2,
	PtzCameraLensIrisCtrlMode_Last = 3
}PtzCameraLensIrisCtrlMode;


typedef enum {
	EmailEncodeType_None = 0,
	EmailEncodeType_TLS = 1,
	EmailEncodeType_SSL = 2,
	EmailEncodeType_NULL = 3,
	EmailEncodeType_Last = 4
}EmailEncodeType;

typedef enum {
	DdnsType_None = 0,
	DdnsType_DynDns = 1,
	DdnsType_Ddns3322 = 2,
	DdnsType_DnsDynMaic = 3,
	DdnsType_DdnsExt1 = 4,
	DdnsType_DdnsExt2 = 5,
	DdnsType_Last = 6
}DdnsType;
typedef enum  {
	RenewalInterval_None = 0,
	RenewalInterval_HalfHour = 1,
	RenewalInterval_OneHour = 2,
	RenewalInterval_ThreeHour = 3,
	RenewalInterval_SixHour = 4,
	RenewalInterval_TwelveHour = 5,
	RenewalInterval_OneDay = 6,
	RenewalInterval_OneWeek = 7,
	RenewalInterval_OneMonth = 8,
	RenewalInterval_Last = 9
}RenewalInterval;

typedef enum  {
	LinkProtocol_None = 0,
	LinkProtocol_Moon = 1,
	LinkProtocol_Onvif = 2,
	LinkProtocol_GBT28181 = 3,
	LinkProtocol_RTSP = 4,
	LinkProtocol_RTMP = 5,
	LinkProtocol_Last = 6
}LinkProtocol;


typedef enum  {
	TimeZone_UTC_None = 0,
	TimeZone_UTC_W12 = 1,
	TimeZone_UTC_W11 = 2,
	TimeZone_UTC_W10 = 3,
	TimeZone_UTC_W9_30 = 4,
	TimeZone_UTC_W9 = 5,
	TimeZone_UTC_W8 = 6,
	TimeZone_UTC_W7 = 7,
	TimeZone_UTC_W6 = 8,
	TimeZone_UTC_W5 = 9,
	TimeZone_UTC_W4 = 10,
	TimeZone_UTC_W3_30 = 11,
	TimeZone_UTC_W3 = 12,
	TimeZone_UTC_W2 = 13,
	TimeZone_UTC_W1 = 14,
	TimeZone_UTC_0 = 15,
	TimeZone_UTC_E1 = 16,
	TimeZone_UTC_E2 = 17,
	TimeZone_UTC_E3 = 18,
	TimeZone_UTC_E3_30 = 19,
	TimeZone_UTC_E4 = 20,
	TimeZone_UTC_E4_30 = 21,
	TimeZone_UTC_E5 = 22,
	TimeZone_UTC_E5_30 = 23,
	TimeZone_UTC_E5_45 = 24,
	TimeZone_UTC_E6 = 25,
	TimeZone_UTC_E6_30 = 26,
	TimeZone_UTC_E7 = 27,
	TimeZone_UTC_E8 = 28,
	TimeZone_UTC_E8_45 = 29,
	TimeZone_UTC_E9 = 30,
	TimeZone_UTC_E9_30 = 31,
	TimeZone_UTC_E10 = 32,
	TimeZone_UTC_E10_30 = 33,
	TimeZone_UTC_E11 = 34,
	TimeZone_UTC_E12 = 35,
	TimeZone_UTC_E13 = 36,
	TimeZone_UTC_E14 = 37,
	TimeZone_UTC_Last = 38
}TimeZone;



typedef enum  {
	FirmWareFileFormat_NULL = 0,
	FirmWareFileFormat_Jffs2 = 1,
	FirmWareFileFormat_CramFs = 2,
	FirmWareFileFormat_SquashFs = 3,
	FirmWareFileFormat_Bin = 4,
	FirmWareFileFormat_Yaffs2 = 5,
	FirmWareFileFormat_Tar = 6,
	FirmWareFileFormat_Ubi = 7,
	FirmWareFileFormat_Last = 8
}FirmWareFileFormat;


typedef enum  {
	FirmWarePayload_NULL = 0,
	FirmWarePayload_Uboot = 1,
	FirmWarePayload_UbootConfig = 2,
	FirmWarePayload_Kernel = 3,
	FirmWarePayload_FileSystem = 4,
	FirmWarePayload_App = 5,
	FirmWarePayload_BoardArgs1 = 6,
	FirmWarePayload_BoardArgs2 = 7,
	FirmWarePayload_BoardArgs3 = 8,
	FirmWarePayload_BoardArgs4 = 9,
	FirmWarePayload_Last,
}FirmWarePayload;

typedef enum _ {
  __NONE = 0,
  __MsgConnect = 1,
  __MsgPushLog = 2,
  __MsgLogIn = 3,
  __MsgFirmwareVersion = 4,
  __MsgDevSearch = 5,
  __MsgManufacturerInfo = 6,
  __MsgEcho = 7,
  __MsgDspCaps = 8,
  __MsgSystemTimeProfile = 9,
  __MsgStreamStart = 10,
  __MsgStreamData = 11,
  __MsgStreamStop = 12,
  __MsgEncodeCaps = 13,
  __MsgEncodeProfile = 14,
  __MsgNetWorkCaps = 15,
  __MsgNetWorkProfile = 16,
  __MsgImageCaps = 17,
  __MsgImageProfile = 18,
  __MsgCaptureCaps = 19,
  __MsgCaptureProfile = 20,
  __MsgVideoCoverProfile = 21,
  __MsgAudioCaps = 22,
  __MsgAudioProfile = 23,
  __MsgPtzCaps = 24,
  __MsgPtzProfile = 25,
  __MsgPtzCmd = 26,
  __MsgUserList = 27,
  __MsgUserAdd = 28,
  __MsgUserDel = 29,
  __MsgUserModify = 30,
  __MsgLogList = 31,
  __MsgLogCaps = 32,
  __MsgLogProfile = 33,
  __MsgDeviceStatusInfo = 34,
  __MsgAlarmCaps = 35,
  __MsgAlarmRegister = 36,
  __MsgAlarmProfile = 37,
  __MsgAlarm = 38,
  __MsgDiskStatus = 39,
  __MsgRecordFileSearch = 40,
  __MsgRecordFileDownload = 41,
  __MsgRecordFileDownloadCtrl = 42,
  __MsgRecordFileDownloadData = 43,
  __MsgTransportStreamCtrl = 44,
  __MsgFtpProfile = 45,
  __MsgEmailProfile = 46,
  __MsgUpnpProfile = 47,
  __MsgNtpProfile = 48,
  __MsgPppoeProfile = 49,
  __MsgRtspProfile = 50,
  __MsgDdnsProfile = 51,
  __MsgP2pProfile = 52,
  __MsgRtmpProfile = 53,
  __MsgFirmwareUpgrade = 54,
  __MsgFirmwareData = 55,
  __MsgFirmwareUpgradeProgress = 56,
  __MsgConnectDevice = 57,
  __MsgPtzCameraLensCaps = 58,
  __MsgPtzCameraLensProfile = 59,
  __MsgOverLayProfile = 60,
  __MsgSyncSystemTime = 61,
  __MsgSnapshot = 62,
  __MsgOverLayCaps = 63,
}__Msg;

typedef enum {
	Intelligent_None = 0,
	Intelligent_FaceDetect = 1,
	Intelligent_PlateRecognition = 2,
	Intelligent_Last
}IntelligentCap;

typedef enum {
	IntelligentHandle_None = 0,
	IntelligentHandle_FaceDetectResultQueue = 1,
	IntelligentHandle_Last
}IntelligentHandle;


#endif

