#ifndef _AVENUMINLINE_H_
#define _AVENUMINLINE_H_
#include "Apis/AvEnum.h"
inline const char **EnumNamesav_bool() {
  static const char *names[] = { "_false", "_true", nullptr };
  return names;
}

inline const char *EnumNameav_bool(av_bool e) { return EnumNamesav_bool()[e]; }
//////////////////////////////////////////////////////////////////////////

inline const int EnumName2EnumComm(const char *EnumName, const char **EnumNames, int s, int e)
{
	for (; s <= e; s++){
		if (0 == strcasecmp(EnumNames[s], EnumName)){
			return s;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesBitRateCtrl() {
  static const char *names[] = { "NONE", "CBR", "VBR", "VCBR", "LAST", nullptr };
  return names;
}

inline const char *EnumNameBitRateCtrl(BitRateCtrl e) { return EnumNamesBitRateCtrl()[e]; }

inline BitRateCtrl BitRateCtrlStr2EnumValue(const char *EName)
{
	return (BitRateCtrl)EnumName2EnumComm(EName, EnumNamesBitRateCtrl(), BitRateCtrl_NONE, BitRateCtrl_LAST);
}

//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesStreamContent() {
  static const char *names[] = { "NONE", "VIDEO", "AUDIO", "AUDIO_PLAY", "LAST", nullptr };
  return names;
}

inline const char *EnumNameStreamContent(StreamContent e) { return EnumNamesStreamContent()[e]; }
//////////////////////////////////////////////////////////////////////////

inline const char **EnumNamesTransportMode() {
  static const char *names[] = { "NONE", "PUSH", "PULL", "LAST", nullptr };
  return names;
}

inline const char *EnumNameTransportMode(TransportMode e) { return EnumNamesTransportMode()[e]; }
//////////////////////////////////////////////////////////////////////////

inline const char **EnumNamesCaptureSize() {
  static const char *names[] = { "Self", "QVGAEX", "QVGA", "VGAEX", "VGA", "D1", "HD720P", "HD960P", "HD1080P", "HC300W", "HC400W", "HC500W", "HC600W", "HC800W", "HC4K", "HC8K", "CNR", "S0800_0600", "S1024_0768", "S1280_0768", "S1280_0800", "S1280_1024", "S1366_0768", "S1400_0900", "S1440_0900", "S1400_1050", "S1600_0900", "S1680_1050", "S1920_1080", "S4K", "SNR", "LAST", nullptr };
  return names;
}

inline const char *EnumNameCaptureSize(CaptureSize e) { return EnumNamesCaptureSize()[e]; }

inline CaptureSize CaptureSizeStr2EnumValue(const char *EName)
{
	return (CaptureSize)EnumName2EnumComm(EName, EnumNamesCaptureSize(), CaptureSize_Self, CaptureSize_LAST);
}
//////////////////////////////////////////////////////////////////////////

inline const char **EnumNamesAvComp() {
  static const char *names[] = { "H264", "H265", "MJPEG", "JPEG", "PCM", "G711A", "G711U", "AAC", "AMR_NB", "AMR_WB", "MP3", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAvComp(AvComp e) { return EnumNamesAvComp()[e]; }

inline AvComp AvCompStr2EnumValue(const char *EName)
{
	return (AvComp)EnumName2EnumComm(EName, EnumNamesAvComp(), AvComp_H264, AvComp_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesAvSensor() {
	static const char *names[] = { "NONE", "OV9712", "OV9732", "OV9750", "OV9752", "OV2718", "OV4689", "OV5658", "AR0130", "AR0230", "AR0237", "AR0330", "AR9M034", "MN34222", "IMX104", "IMX138", "IMX123", "IMX222", "IMX236", "IMX117", "IMX178", "IMX185", "IMX290", "IMX291", "BT1120", "BT656", "CVBS", "USBCAMERA", "SCREEN", "", "", "LAST", nullptr };
	return names;
}

inline const char *EnumNameAvSensor(AvSensor e) { return EnumNamesAvSensor()[e]; }


inline AvSensor AvSensorStr2EnumValue(const char *EName)
{
	return (AvSensor)EnumName2EnumComm(EName, EnumNamesAvSensor(), AvSensor_NONE, AvSensor_LAST);
}

//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesavFrameT() {
  static const char *names[] = { "NONE", "P", "B", "", "I", "", "", "", "JPEG", "", "", "", "", "", "", "", "AUDIO", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "LAST", nullptr };
  return names;
}

inline const char *EnumNameavFrameT(avFrameT e) { return EnumNamesavFrameT()[e]; }
inline const char **EnumNamesnal_unit_type_h264() {
  static const char *names[] = { "nr", "p", "dataA", "dataB", "dataC", "idr", "sei", "sps", "pps", "delimiter", "nalend", "streamend", "pading", nullptr };
  return names;
}


inline const char **EnumNamesAvChip() {
  static const char *names[] = { "H18EV100", "H18EV200", "H18EV201", "H18C", "H18A", "H16CV100", "H16CV200", "H16CV300", "H16A", "H16D", "H19", "HIPC_RES_1", "HIPC_RES_2", "H20D", "H21", "H31", "H35", "H36", "H3798MV100", "HNVR_RES_2", "A22M", "A33M", "A55M", "A65", "A66", "WINDOWS_32", "WINDOWS_64", "LINUX_32", "LINUX_64", "MAC_32", "MAC_64", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAvChip(AvChip e) { return EnumNamesAvChip()[e]; }


inline AvChip AvChipStr2EnumValue(const char *EName)
{
	return (AvChip)EnumName2EnumComm(EName, EnumNamesAvChip(), AvChip_H18EV100, AvChip_LAST);
}


//////////////////////////////////////////////////////////////////////////

inline const char **EnumNamesTimeFMT() {
  static const char *names[] = { "NONE", "YMD", "MDY", "DMY", "H24", "H12", "LAST", nullptr };
  return names;
}

inline const char *EnumNameTimeFMT(TimeFMT e) { return EnumNamesTimeFMT()[e]; }


inline TimeFMT TimeFMTStr2EnumValue(const char *EName)
{
	return (TimeFMT)EnumName2EnumComm(EName, EnumNamesTimeFMT(), TimeFMT_NONE, TimeFMT_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesEnCodeProfile() {
  static const char *names[] = { "NONE", "High", "Main", "BaseLine", "LAST", nullptr };
  return names;
}

inline const char *EnumNameEnCodeProfile(EnCodeProfile e) { return EnumNamesEnCodeProfile()[e]; }

inline EnCodeProfile EnCodeProfileStr2EnumValue(const char *EName)
{
	return (EnCodeProfile)EnumName2EnumComm(EName, EnumNamesEnCodeProfile(), EnCodeProfile_NONE, EnCodeProfile_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesIrCutMode() {
  static const char *names[] = { "NONE", "OPEN", "CLOSE", "AUTO", "TIMER", "LAST", nullptr };
  return names;
}

inline const char *EnumNameIrCutMode(IrCutMode e) { return EnumNamesIrCutMode()[e]; }

inline IrCutMode IrCutModeStr2EnumValue(const char *EName)
{
	return (IrCutMode)EnumName2EnumComm(EName, EnumNamesIrCutMode(), IrCutMode_NONE, IrCutMode_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesMirrorMode() {
  static const char *names[] = { "NONE", "HOR", "VER", "Rotate_000", "Rotate_090", "Rotate_180", "Rotate_270", "LAST", nullptr };
  return names;
}

inline const char *EnumNameMirrorMode(MirrorMode e) { return EnumNamesMirrorMode()[e]; }

inline MirrorMode MirrorModeStr2EnumValue(const char *EName)
{
	return (MirrorMode)EnumName2EnumComm(EName, EnumNamesMirrorMode(), MirrorMode_NONE, MirrorMode_LAST);
}

//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesExposureMode() {
  static const char *names[] = { "NONE", "AUTO", "MANUAL", "TRAFFIC", "LAST", nullptr };
  return names;
}

inline const char *EnumNameExposureMode(ExposureMode e) { return EnumNamesExposureMode()[e]; }

inline ExposureMode ExposureModeStr2EnumValue(const char *EName)
{
	return (ExposureMode)EnumName2EnumComm(EName, EnumNamesExposureMode(), ExposureMode_NONE, ExposureMode_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesWhiteBalanceMode() {
  static const char *names[] = { "NONE", "OFF", "", "AUTO", "DAYTIME", "EVENING", "CLOUDY", "OFFICE", "FLUORESCENT", "INCANDESCENT", "MANUAL", "LAST", nullptr };
  return names;
}

inline const char *EnumNameWhiteBalanceMode(WhiteBalanceMode e) { return EnumNamesWhiteBalanceMode()[e]; }

inline WhiteBalanceMode WhiteBalanceModeStr2EnumValue(const char *EName)
{
	return (WhiteBalanceMode)EnumName2EnumComm(EName, EnumNamesWhiteBalanceMode(), WhiteBalanceMode_NONE, WhiteBalanceMode_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesAntiFlckerMode() {
  static const char *names[] = { "NONE", "INDOOR_50HZ", "OUTDOOR_50HZ", "AUTO_50HZ", "INDOOR_60HZ", "OUTDOOR_60HZ", "AUTO_60HZ", "THEATER_50HZ", "FAST_50HZ", "THEATER_60HZ", "FAST_60HZ", "ANTI_FLICKER_CLOSED", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAntiFlckerMode(AntiFlckerMode e) { return EnumNamesAntiFlckerMode()[e]; }

inline AntiFlckerMode AntiFlckerModeStr2EnumValue(const char *EName)
{
	return (AntiFlckerMode)EnumName2EnumComm(EName, EnumNamesAntiFlckerMode(), AntiFlckerMode_NONE, AntiFlckerMode_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesNetCommGetMode() {
  static const char *names[] = { "NONE", "MANUAL", "AUTO", "PPPOE", "LAST", nullptr };
  return names;
}

inline const char *EnumNameNetCommGetMode(NetCommGetMode e) { return EnumNamesNetCommGetMode()[e]; }

inline NetCommGetMode NetCommGetModeStr2EnumValue(const char *EName)
{
	return (NetCommGetMode)EnumName2EnumComm(EName, EnumNamesNetCommGetMode(), NetCommGetMode_NONE, NetCommGetMode_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesNetCommT() {
  static const char *names[] = { "LAN0", "LAN1", "Wireless", "SIM", "BlueTooth", "LAST", nullptr };
  return names;
}
inline const char *EnumNameNetCommT(NetCommT e) { return EnumNamesNetCommT()[e]; }
inline NetCommT NetCommTStr2EnumValue(const char *EName)
{
	return (NetCommT)EnumName2EnumComm(EName, EnumNamesNetCommT(), NetCommT_LAN0, NetCommT_LAST);
}

//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesUserCategory() {
  static const char *names[] = { "NONE", "SUPERADMIN", "ADMIN", "NORMALUSER", "LAST", nullptr };
  return names;
}

inline const char *EnumNameUserCategory(UserCategory e) { return EnumNamesUserCategory()[e]; }

inline const char **EnumNamesLogType() {
  static const char *names[] = { "NONE", "SYSTEM_OPERATE", "DATA_MANAGE", "RECORD_MANAGE", "LOG_MANAGE", "PROFILE_OPERATE", "ALARM_EVENT", "LAST", nullptr };
  return names;
}

inline const char *EnumNameLogType(LogType e) { return EnumNamesLogType()[e]; }

inline const char **EnumNamesUserAccess() {
  static const char *names[] = { "NONE", "VISIT_VIDEO", "VISIT_AUDIO", "TALK_BACK", "VISIT_PTZ", "VISIT_LOG", "MODIFY_PROFILE", "VISIT_PROFILE", "LAST", nullptr };
  return names;
}

inline const char *EnumNameUserAccess(UserAccess e) { return EnumNamesUserAccess()[e]; }


inline const char **EnumNamesAlarmEvent() {
  static const char *names[] = { "NONE", "PORT_In", "PORT_Out", "VIDEO_FaceRecognition", "VIDEO_FaceDetection", "VIDEO_MotionDetection", "VIDEO_MotionFollow", "VIDEO_PlatenumberRecognition", "VIDEO_DevOffline", "VIDEO_Lost", "VIDEO_Blind", "VIDEO_SomethingLoss", "DISK_ERROR", "DISK_LOST", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAlarmEvent(AlarmEvent e) { return EnumNamesAlarmEvent()[e]; }


inline const char **EnumNamesDiskStatus() {
  static const char *names[] = { "NONE", "RUN", "DORMANCY", "BROKE", "UNFORMATTED", "UNIDENTIFICATION", "LAST", nullptr };
  return names;
}

inline const char *EnumNameDiskStatus(DiskStatus e) { return EnumNamesDiskStatus()[e]; }


inline const char **EnumNamesRecordFileType() {
  static const char *names[] = { "NONE", "ALARM_PORTIN", "ALARM_FACERECOGNITION", "ALARM_FACEDETECTION", "ALARM_PLATENUMBERRECOGNITION", "ALARM_MOTIONDETECTION", "ALARM_MOTIONTRACE", "TIMER", "REALTIME", "LAST", nullptr };
  return names;
}

inline const char *EnumNameRecordFileType(RecordFileType e) { return EnumNamesRecordFileType()[e]; }


inline const char **EnumNamesRecordFileDownloadCtrl() {
  static const char *names[] = { "NONE", "FastX1", "FastX2", "FastX3", "FastX4", "LowX1", "LowX2", "LowX3", "LowX4", "Stop", "Pause", "Single", "LAST", nullptr };
  return names;
}

inline const char *EnumNameRecordFileDownloadCtrl(RecordFileDownloadCtrl e) { return EnumNamesRecordFileDownloadCtrl()[e]; }


//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesTransportStreamCtrl() {
  static const char *names[] = { "NONE", "SINGLE_I", "DROP_LAST_P", "CHANGE_CBR", "CHANGE_FPS", "CHUCKSIZE", "LAST", nullptr };
  return names;
}

inline const char *EnumNameTransportStreamCtrl(TransportStreamCtrl e) { return EnumNamesTransportStreamCtrl()[e]; }

inline TransportStreamCtrl TransportStreamCtrlStr2EnumValue(const char *EName)
{
	return (TransportStreamCtrl)EnumName2EnumComm(EName, EnumNamesTransportStreamCtrl(), TransportStreamCtrl_NONE, TransportStreamCtrl_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesPtzBaudrate() {
  static const char *names[] = { "RNONE", "R2400", "R4800", "R9600", "R19200", "R38400", "R57600", "R115200", "RLAST", nullptr };
  return names;
}

inline const char *EnumNamePtzBaudrate(PtzBaudrate e) { return EnumNamesPtzBaudrate()[e]; }

inline PtzBaudrate PtzBaudrateStr2EnumValue(const char *EName)
{
	return (PtzBaudrate)EnumName2EnumComm(EName, EnumNamesPtzBaudrate(), PtzBaudrate_RNONE, PtzBaudrate_RLAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesPtzVeriFy() {
  static const char *names[] = { "None", "Odd", "Even", "Mark", "Empty", "Last", nullptr };
  return names;
}

inline const char *EnumNamePtzVeriFy(PtzVeriFy e) { return EnumNamesPtzVeriFy()[e]; }

inline PtzVeriFy PtzVeriFyStr2EnumValue(const char *EName)
{
	return (PtzVeriFy)EnumName2EnumComm(EName, EnumNamesPtzVeriFy(), PtzVeriFy_None, PtzVeriFy_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesPtzProtocol() {
  static const char *names[] = { "None", "Pelcod", "Pelcod1", "Pelcod_s", "Pelcod_s1", "Pelcop", "Pelcop1", "Pelcop5", "SamSung", "Sony", "Last", nullptr };
  return names;
}

inline const char *EnumNamePtzProtocol(PtzProtocol e) { return EnumNamesPtzProtocol()[e]; }

inline PtzProtocol PtzProtocolStr2EnumValue(const char *EName)
{
	return (PtzProtocol)EnumName2EnumComm(EName, EnumNamesPtzProtocol(), PtzProtocol_None, PtzProtocol_Last);
}
//////////////////////////////////////////////////////////////////////////

inline const char **EnumNamesOverLayType() {
	static const char *names[] = { "Title", "Time", "Last", nullptr };
	return names;
}

inline const char *EnumNameOverLayType(OverLayType e) { return EnumNamesOverLayType()[e]; }


inline OverLayType OverLayTypeStr2EnumValue(const char *EName)
{
	return (OverLayType)EnumName2EnumComm(EName, EnumNamesOverLayType(), OverLayType_Title, OverLayType_Last);
}

//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesFontSize() {
	static const char *names[] = { "None", "_8", "_16", "_24", "_32", "_48", "_64", "_128", "Last", nullptr };
	return names;
}

inline const char *EnumNameFontSize(FontSize e) { return EnumNamesFontSize()[e]; }

inline FontSize FontSizeStr2EnumValue(const char *EName)
{
	return (FontSize)EnumName2EnumComm(EName, EnumNamesFontSize(), FontSize_None, FontSize_Last);
}

//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesOverLayDateTimeStyle() {
	static const char *names[] = { "None", "Y_M_D_H_M_S", "Y_M_D_M_H_M_S", "H_M_S_Y_M_D", "Last", nullptr };
	return names;
}

inline const char *EnumNameOverLayDateTimeStyle(OverLayDateTimeStyle e) { return EnumNamesOverLayDateTimeStyle()[e]; }

inline OverLayDateTimeStyle OverLayDateTimeStyleStr2EnumValue(const char *EName)
{
	return (OverLayDateTimeStyle)EnumName2EnumComm(EName, EnumNamesOverLayDateTimeStyle(), OverLayDateTimeStyle_None, OverLayDateTimeStyle_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesOverLayDateTimeSplit() {
	static const char *names[] = { "None", "Split_Symbol", "Split_Word", "Last", nullptr };
	return names;
}

inline const char *EnumNameOverLayDateTimeSplit(OverLayDateTimeSplit e) { return EnumNamesOverLayDateTimeSplit()[e]; }

inline OverLayDateTimeSplit OverLayDateTimeSplitStr2EnumValue(const char *EName)
{
	return (OverLayDateTimeSplit)EnumName2EnumComm(EName, EnumNamesOverLayDateTimeSplit(), OverLayDateTimeSplit_None, OverLayDateTimeSplit_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesPtzCameraLensFocusCtrlMode() {
	static const char *names[] = { "None", "Auto", "SemiAutomatic", "Manual", "Last", nullptr };
	return names;
}
inline const char *EnumNamePtzCameraLensFocusCtrlMode(PtzCameraLensFocusCtrlMode e) { return EnumNamesPtzCameraLensFocusCtrlMode()[e]; }
inline PtzCameraLensFocusCtrlMode PtzCameraLensFocusCtrlModeStr2EnumValue(const char *EName)
{
	return (PtzCameraLensFocusCtrlMode)EnumName2EnumComm(EName, EnumNamesPtzCameraLensFocusCtrlMode(),
		PtzCameraLensFocusCtrlMode_None, PtzCameraLensFocusCtrlMode_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesPtzCameraLensFocusRegion() {
	static const char *names[] = { "None", "AllRegion", "CentralRegion", "Last", nullptr };
	return names;
}

inline const char *EnumNamePtzCameraLensFocusRegion(PtzCameraLensFocusRegion e) { return EnumNamesPtzCameraLensFocusRegion()[e]; }

inline PtzCameraLensFocusRegion PtzCameraLensFocusRegionStr2EnumValue(const char *EName)
{
	return (PtzCameraLensFocusRegion)EnumName2EnumComm(EName, EnumNamesPtzCameraLensFocusRegion(),
		PtzCameraLensFocusRegion_None, PtzCameraLensFocusRegion_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesPtzCameraLensFocusSearchMode() {
	static const char *names[] = { "None", "FullView", "FAR_1_5M", "FAR_3_0M", "FAR_6_0M", "FAR_8_0M", "FAR_Infinity", "Last", nullptr };
	return names;
}

inline const char *EnumNamePtzCameraLensFocusSearchMode(PtzCameraLensFocusSearchMode e) { return EnumNamesPtzCameraLensFocusSearchMode()[e]; }
inline PtzCameraLensFocusSearchMode PtzCameraLensFocusSearchModeStr2EnumValue(const char *EName)
{
	return (PtzCameraLensFocusSearchMode)EnumName2EnumComm(EName, EnumNamesPtzCameraLensFocusSearchMode(),
		PtzCameraLensFocusSearchMode_None, PtzCameraLensFocusSearchMode_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesPtzCameraLensIrisCtrlMode() {
	static const char *names[] = { "None", "Auto", "Manual", "Last", nullptr };
	return names;
}

inline const char *EnumNamePtzCameraLensIrisCtrlMode(PtzCameraLensIrisCtrlMode e) { return EnumNamesPtzCameraLensIrisCtrlMode()[e]; }
inline PtzCameraLensIrisCtrlMode PtzCameraLensIrisCtrlModeStr2EnumValue(const char *EName)
{
	return (PtzCameraLensIrisCtrlMode)EnumName2EnumComm(EName, EnumNamesPtzCameraLensIrisCtrlMode(),
		PtzCameraLensIrisCtrlMode_None, PtzCameraLensIrisCtrlMode_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesAudioSampleRate() {
  static const char *names[] = { "NONE", "R8000", "R11025", "R12000", "R16000", "R22050", "R24000", "R32000", "R44100", "R48000", "R64000", "R88200", "R96000", "R192000", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAudioSampleRate(AudioSampleRate e) { return EnumNamesAudioSampleRate()[e]; }

inline AudioSampleRate AudioSampleRateStr2EnumValue(const char *ENmame)
{
	return (AudioSampleRate)EnumName2EnumComm(ENmame, EnumNamesAudioSampleRate(), AudioSampleBits_NONE, AudioSampleBits_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesAudioSampleBits() {
  static const char *names[] = { "NONE", "B8", "B16", "B24", "B32", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAudioSampleBits(AudioSampleBits e) { return EnumNamesAudioSampleBits()[e]; }
inline AudioSampleBits AudioSampleBitsStr2EnumValue(const char *EName)
{
	return (AudioSampleBits)EnumName2EnumComm(EName, EnumNamesAudioSampleBits(), AudioSampleBits_NONE, AudioSampleBits_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesAudioSoundMode() {
  static const char *names[] = { "NONE", "MONO", "STEREO", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAudioSoundMode(AudioSoundMode e) { return EnumNamesAudioSoundMode()[e]; }

inline AudioSoundMode AudioSoundModeStr2EnumValue(const char *EName)
{
	return (AudioSoundMode)EnumName2EnumComm(EName, EnumNamesAudioSoundMode(), AudioSoundMode_NONE, AudioSoundMode_LAST);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesAudioCaptureDevice() {
  static const char *names[] = { "NONE", "LINE_IN", "MIC_IN", "LAST", nullptr };
  return names;
}

inline const char *EnumNameAudioCaptureDevice(AudioCaptureDevice e) { return EnumNamesAudioCaptureDevice()[e]; }

inline AudioCaptureDevice AudioCaptureDeviceStr2EnumValue(const char *EName)
{
	return (AudioCaptureDevice)EnumName2EnumComm(EName, EnumNamesAudioCaptureDevice(), AudioCaptureDevice_NONE, AudioCaptureDevice_LAST);
}
//////////////////////////////////////////////////////////////////////////


inline const char **EnumNamesAudioVoiceQualityEnhancement() {
	static const char *names[] = { "NONE", "AEC", "ANR", "RNR", "HPF", "AGC", "EQ", "GAIN", "RES", "LAST", nullptr };
	return names;
}

inline const char *EnumNameAudioVoiceQualityEnhancement(AudioVoiceQualityEnhancement e) { return EnumNamesAudioVoiceQualityEnhancement()[e]; }
//////////////////////////////////////////////////////////////////////////

inline const char **EnumNamesProgressStatus() {
  static const char *names[] = { "None", "UpgradeStart", "UpgradeRecvData", "UpgradeWipePartition", "UpgradeWriteData", "UpgradeOver", "UpgradeExtract", "UpgradeCopy", "UpgradeInstall", "Rebootting", "ModifyOver", "Last", nullptr };
  return names;
}
inline const char *EnumNameProgressStatus(ProgressStatus e) { return EnumNamesProgressStatus()[e]; }

inline ProgressStatus ProgressStatusStr2EnumValue(const char *EName)
{
	return (ProgressStatus)EnumName2EnumComm(EName, EnumNamesProgressStatus(), ProgressStatus_None, ProgressStatus_Last);
}
//////////////////////////////////////////////////////////////////////////


inline const char **EnumNamesEmailEncodeType() {
	static const char *names[] = { "None", "TLS", "SSL", "NULL", "Last", nullptr };
	return names;
}

inline const char *EnumNameEmailEncodeType(EmailEncodeType e) { return EnumNamesEmailEncodeType()[e]; }

inline EmailEncodeType EmailEncodeTypeStr2EnumValue(const char *EName)
{
	return (EmailEncodeType)EnumName2EnumComm(EName, EnumNamesEmailEncodeType(), EmailEncodeType_None, EmailEncodeType_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesDdnsType() {
	static const char *names[] = { "None", "DynDns", "Ddns3322", "DnsDynMaic", "DdnsExt1", "DdnsExt2", "Last", nullptr };
	return names;
}

inline const char *EnumNameDdnsType(DdnsType e) { return EnumNamesDdnsType()[e]; }


inline DdnsType DdnsTypeStr2EnumValue(const char *EName)
{
	return (DdnsType)EnumName2EnumComm(EName, EnumNamesDdnsType(), DdnsType_None, DdnsType_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesRenewalInterval() {
	static const char *names[] = { "None", "HalfHour", "OneHour", "ThreeHour", "SixHour", "TwelveHour", "OneDay", "OneWeek", "OneMonth", "Last", nullptr };
	return names;
}

inline const char *EnumNameRenewalInterval(RenewalInterval e) { return EnumNamesRenewalInterval()[e]; }
inline RenewalInterval RenewalIntervalStr2EnumValue(const char *EName)
{
	return (RenewalInterval)EnumName2EnumComm(EName, EnumNamesDdnsType(), RenewalInterval_None, RenewalInterval_Last);
}
//////////////////////////////////////////////////////////////////////////
inline const char **EnumNamesTimeZone() {
	static const char *names[] = { "UTC_None", "UTC_W12", "UTC_W11", "UTC_W10", "UTC_W9_30", "UTC_W9", "UTC_W8", "UTC_W7", "UTC_W6", "UTC_W5", "UTC_W4", "UTC_W3_30", "UTC_W3", "UTC_W2", "UTC_W1", "UTC_0", "UTC_E1", "UTC_E2", "UTC_E3", "UTC_E3_30", "UTC_E4", "UTC_E4_30", "UTC_E5", "UTC_E5_30", "UTC_E5_45", "UTC_E6", "UTC_E6_30", "UTC_E7", "UTC_E8", "UTC_E8_45", "UTC_E9", "UTC_E9_30", "UTC_E10", "UTC_E10_30", "UTC_E11", "UTC_E12", "UTC_E13", "UTC_E14", "UTC_Last", nullptr };
	return names;
}

inline const char *EnumNameTimeZone(TimeZone e) { return EnumNamesTimeZone()[e]; }
inline TimeZone TimeZoneStr2EnumValue(const char *EName)
{
	return (TimeZone)EnumName2EnumComm(EName, EnumNamesTimeZone(), TimeZone_UTC_None, TimeZone_UTC_Last);
}
//////////////////////////////////////////////////////////////////////////

inline const char **EnumNamesLinkProtocol() {
  static const char *names[] = { "None", "Moon", "Onvif", "GBT28181", "RTSP", "Last", nullptr };
  return names;
}

inline const char *EnumNameLinkProtocol(LinkProtocol e) { return EnumNamesLinkProtocol()[e]; }

inline LinkProtocol LinkProtocolStr2EnumValue(const char *EName)
{
	return (LinkProtocol)EnumName2EnumComm(EName, EnumNamesLinkProtocol(), LinkProtocol_None, LinkProtocol_Last);
}
//////////////////////////////////////////////////////////////////////////





inline const char **EnumNamesPtzCommand() {
	static const char *names[] = { "NONE", "STOP", "UP", "DOWN", "LEFT", "RIGHT", "LEFTUP", "LEFTDOWN", "RIGHTUP", "RIGHTDOWN", "ABSOLUTE_MOVE", "RELATIVE_MOVE", "ABSOLUTE_FOCUS", "ZOOM_WIDE", "ZOOM_TELE", "FOCUS_FAR", "FOCUS_NEAR", "IRIS_LARGE", "IRIS_SMALL", "ALARM", "LIGHT_ON", "LIGHT_OFF", "SETPRESET", "CLEARPRESET", "GOTOPRESET", "AUTOPANON", "AUTOPANOFF", "SETLIMIT", "AUTOSCANON", "AUTOSCANOFF", "ADDTOUR", "DELETETOUR", "STARTTOUR", "STOPTOUR", "CLEARTOUR", "SETPATTERN", "STARTPATTERN", "STOPPATTERN", "CLEARPATTERN", "POSITION", "AUX_ON", "AUX_OFF", "MENU", "EXIT", "ENTER", "ESC", "MENUUPDOWN", "MENULEFTRIGHT", "MATRIXSWITCH", "FLIP", "RESET", "LIGHTCONTROLLER", nullptr };
	return names;
}

inline const char *EnumNamePtzCommand(PtzCommand e) { return EnumNamesPtzCommand()[e]; }
//////////////////////////////////////////////////////////////////////////


#endif

