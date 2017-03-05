#ifndef _AVQSTRINGSLANGUAGE_H_
#define _AVQSTRINGSLANGUAGE_H_

#include <QString>
#include <QObject>
#include "Apis/AvEnum.h"


inline QString AvUiLangsCustom()
{
	return QObject::tr("Custom");
}

inline QString AvUiLangsOpenClose(av_bool b)
{
	static QString names[] = {
		QObject::tr("Close"),
		QObject::tr("Open"),
	};
	return names[b];
}

inline QString AvUiLangsBitRateCtrl(BitRateCtrl e) {

	static QString names[] = {
		QObject::tr("NULL"),
		QObject::tr("CBR"),
		QObject::tr("VBR"),
		QObject::tr("VCBR"),
		QObject::tr("LAST")
	};
	return names[e];
}

inline QString AvUiLangsCHL(E_EncodeCHL e)
{
	static QString names[] = {
		QObject::tr("MAIN"),
		QObject::tr("SUB1"),
		QObject::tr("SUB2"),
		QObject::tr("SUB3"),
		QObject::tr("SUB4"),
		QObject::tr("JPEG"),
		QObject::tr("NR"),
	};

	return names[e];
}

inline QString AvUiLangsStreamContent(StreamContent e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("VIDEO"),
		QObject::tr("AUDIO"),
		QObject::tr("AUDIO_PLAY"),
		QObject::tr("LAST"),
	};
	return names[e];
}




inline QString AvUiLangsTransportMode(TransportMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("PUSH"),
		QObject::tr("PULL"),
		QObject::tr("LAST"),
	};
	return names[e];
}



inline QString AvUiLangsCaptureSize(CaptureSize e) {
	static QString names[] = {
		QObject::tr("Self"),
		QObject::tr("QVGAEX"),
		QObject::tr("QVGA"),
		QObject::tr("VGAEX"),
		QObject::tr("VGA"),
		QObject::tr("D1"),
		QObject::tr("HD720P"),
		QObject::tr("HD960P"),
		QObject::tr("HD1080P"),
		QObject::tr("HC300W"),
		QObject::tr("HC400W"),
		QObject::tr("HC500W"),
		QObject::tr("HC600W"),
		QObject::tr("HC800W"),
		QObject::tr("HC4K"),
		QObject::tr("HC8K"),
		QObject::tr("CNR"),
		QObject::tr("S0800_0600"),
		QObject::tr("S1024_0768"),
		QObject::tr("S1280_0768"),
		QObject::tr("S1280_0800"),
		QObject::tr("S1280_1024"),
		QObject::tr("S1366_0768"),
		QObject::tr("S1400_0900"),
		QObject::tr("S1440_0900"),
		QObject::tr("S1400_1050"),
		QObject::tr("S1600_0900"),
		QObject::tr("S1680_1050"),
		QObject::tr("S1920_1080"),
		QObject::tr("S4K"),
		QObject::tr("SNR"),
		QObject::tr("LAST"),

	};
	return names[e];
}



inline QString AvUiLangsAvComp(AvComp e) {
	static QString names[] = {
		QObject::tr("H264"),
		QObject::tr("H265"),
		QObject::tr("MJPEG"),
		QObject::tr("JPEG"),
		QObject::tr("PCM"),
		QObject::tr("G711A"),
		QObject::tr("G711U"),
		QObject::tr("AAC"),
		QObject::tr("AMR_NB"),
		QObject::tr("AMR_WB"),
		QObject::tr("MP3"),
		QObject::tr("LAST"),
	};
	return names[e];
}
 

inline QString AvUiLangsAvSensor(AvSensor e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("OV9712"),
		QObject::tr("OV9732"),
		QObject::tr("OV9750"),
		QObject::tr("OV9752"),
		QObject::tr("OV2718"),
		QObject::tr("OV4689"),
		QObject::tr("OV5658"),
		QObject::tr("AR0130"),
		QObject::tr("AR0230"),
		QObject::tr("AR0237"),
		QObject::tr("AR0330"),
		QObject::tr("AR9M034"),
		QObject::tr("MN34222"),
		QObject::tr("IMX104"),
		QObject::tr("IMX138"),
		QObject::tr("IMX123"),
		QObject::tr("IMX222"),
		QObject::tr("IMX236"),
		QObject::tr("IMX117"),
		QObject::tr("IMX178"),
		QObject::tr("IMX185"),
		QObject::tr("IMX290"),
		QObject::tr("IMX291"),
		QObject::tr("BT1120"),
		QObject::tr("BT656"),
		QObject::tr("CVBS"),
		QObject::tr("USBCAMERA"),
		QObject::tr("SCREEN"),
		QObject::tr("RES1"),
		QObject::tr("RES2"),
		QObject::tr("LAST"),
	};
	return names[e];
}



inline QString AvUiLangsAvChip(AvChip e) {
	static QString names[] = {
		QObject::tr("H18EV100"),
		QObject::tr("H18EV200"),
		QObject::tr("H18EV201"),
		QObject::tr("H18C"),
		QObject::tr("H18A"),
		QObject::tr("H16CV100"),
		QObject::tr("H16CV200"),
		QObject::tr("H16CV300"),
		QObject::tr("H16A"),
		QObject::tr("H16D"),
		QObject::tr("H19"),
		QObject::tr("HIPC1"),
		QObject::tr("HIPC2"),
		QObject::tr("H20D"),
		QObject::tr("H21"),
		QObject::tr("H31"),
		QObject::tr("H35"),
		QObject::tr("H36"),
		QObject::tr("H3798MV100"),
		QObject::tr("HNVR2"),
		QObject::tr("A22M"),
		QObject::tr("A33M"),
		QObject::tr("A55M"),
		QObject::tr("A65"),
		QObject::tr("A66"),
		QObject::tr("WINDOWS_32"),
		QObject::tr("WINDOWS_64"),
		QObject::tr("LINUX_32"),
		QObject::tr("LINUX_64"),
		QObject::tr("MAC_32"),
		QObject::tr("MAC_64"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsTimeFMT(TimeFMT e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("YMD"),
		QObject::tr("MDY"),
		QObject::tr("DMY"),
		QObject::tr("H24"),
		QObject::tr("H12"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsEnCodeProfile(EnCodeProfile e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("High"),
		QObject::tr("Main"),
		QObject::tr("BaseLine"),
		QObject::tr("LAST"),
	};
	return names[e];
}

inline QString AvUiLangsIrCutMode(IrCutMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("OPEN"),
		QObject::tr("CLOSE"),
		QObject::tr("AUTO"),
		QObject::tr("TIMER"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsMirrorMode(MirrorMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("HOR"),
		QObject::tr("VER"),
		QObject::tr("Rotate_000"),
		QObject::tr("Rotate_090"),
		QObject::tr("Rotate_180"),
		QObject::tr("Rotate_270"),
		QObject::tr("LAST"),
	};
	return names[e];
}

inline QString AvUiLangsExposureMode(ExposureMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("AUTO"),
		QObject::tr("MANUAL"),
		QObject::tr("TRAFFIC"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsWhiteBalanceMode(WhiteBalanceMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("OFF"),
		QObject::tr("AUTO"),
		QObject::tr("DAYTIME"),
		QObject::tr("EVENING"),
		QObject::tr("CLOUDY"),
		QObject::tr("OFFICE"),
		QObject::tr("FLUORESCENT"),
		QObject::tr("INCANDESCENT"),
		QObject::tr("MANUAL"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsAntiFlckerMode(AntiFlckerMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("INDOOR_50HZ"),
		QObject::tr("OUTDOOR_50HZ"),
		QObject::tr("AUTO_50HZ"),
		QObject::tr("INDOOR_60HZ"),
		QObject::tr("OUTDOOR_60HZ"),
		QObject::tr("AUTO_60HZ"),
		QObject::tr("THEATER_50HZ"),
		QObject::tr("FAST_50HZ"),
		QObject::tr("THEATER_60HZ"),
		QObject::tr("FAST_60HZ"),
		QObject::tr("ANTI_FLICKER_CLOSED"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsNetCommGetMode(NetCommGetMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("MANUAL"),
		QObject::tr("AUTO"),
		QObject::tr("PPPOE"),
		QObject::tr("LAST"),
	};
	return names[e];
}

inline QString AvUiLangsNetCommT(NetCommT e) {
	static QString names[] = {
		QObject::tr("LAN0"),
		QObject::tr("LAN1"),
		QObject::tr("Wireless"),
		QObject::tr("SIM"),
		QObject::tr("BlueTooth"),
		QObject::tr("LAST"),
	};
	return names[e];
}

inline QString AvUiLangsUserCategory(UserCategory e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("SUPERADMIN"),
		QObject::tr("ADMIN"),
		QObject::tr("NORMALUSER"),
		QObject::tr("LAST"),
	};
	return names[e];
}

inline QString AvUiLangsLogType(LogType e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("SYSTEM_OPERATE"),
		QObject::tr("DATA_MANAGE"),
		QObject::tr("RECORD_MANAGE"),
		QObject::tr("LOG_MANAGE"),
		QObject::tr("PROFILE_OPERATE"),
		QObject::tr("ALARM_EVENT"),
		QObject::tr("LAST"),
	};
	return names[e];
}

inline QString AvUiLangsUserAccess(UserAccess e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("VISIT_VIDEO"),
		QObject::tr("VISIT_AUDIO"),
		QObject::tr("TALK_BACK"),
		QObject::tr("VISIT_PTZ"),
		QObject::tr("VISIT_LOG"),
		QObject::tr("MODIFY_PROFILE"),
		QObject::tr("VISIT_PROFILE"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsAlarmEvent(AlarmEvent e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("PORT_In"),
		QObject::tr("PORT_Out"),
		QObject::tr("VIDEO_FaceRecognition"),
		QObject::tr("VIDEO_FaceDetection"),
		QObject::tr("VIDEO_MotionDetection"),
		QObject::tr("VIDEO_MotionFollow"),
		QObject::tr("VIDEO_PlatenumberRecognition"),
		QObject::tr("VIDEO_DevOffline"),
		QObject::tr("VIDEO_Lost"),
		QObject::tr("VIDEO_Blind"),
		QObject::tr("VIDEO_SomethingLoss"),
		QObject::tr("DISK_ERROR"),
		QObject::tr("DISK_LOST"),
		QObject::tr("LAST"),
	};
	return names[e];
}



inline QString AvUiLangsDiskStatus(DiskStatus e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("RUN"),
		QObject::tr("DORMANCY"),
		QObject::tr("BROKE"),
		QObject::tr("UNFORMATTED"),
		QObject::tr("UNIDENTIFICATION"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsRecordFileType(RecordFileType e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("ALARM_PORTIN"),
		QObject::tr("ALARM_FACERECOGNITION"),
		QObject::tr("ALARM_FACEDETECTION"),
		QObject::tr("ALARM_PLATENUMBERRECOGNITION"),
		QObject::tr("ALARM_MOTIONDETECTION"),
		QObject::tr("ALARM_MOTIONTRACE"),
		QObject::tr("TIMER"),
		QObject::tr("REALTIME"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsRecordFileDownloadCtrl(RecordFileDownloadCtrl e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("FastX1"),
		QObject::tr("FastX2"),
		QObject::tr("FastX3"),
		QObject::tr("FastX4"),
		QObject::tr("LowX1"),
		QObject::tr("LowX2"),
		QObject::tr("LowX3"),
		QObject::tr("LowX4"),
		QObject::tr("Stop"),
		QObject::tr("Pause"),
		QObject::tr("Single"),
		QObject::tr("LAST"),
	};
	return names[e];
}

inline QString AvUiLangsTransportStreamCtrl(TransportStreamCtrl e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("SINGLE_I"),
		QObject::tr("DROP_LAST_P"),
		QObject::tr("CHANGE_CBR"),
		QObject::tr("CHANGE_FPS"),
		QObject::tr("CHUCKSIZE"),
		QObject::tr("LAST"),

	};
	return names[e];
}
inline QString AvUiLangsPtzBaudrate(PtzBaudrate e) {
	static QString names[] = {
		QObject::tr("RNONE"),
		QObject::tr("R2400"),
		QObject::tr("R4800"),
		QObject::tr("R9600"),
		QObject::tr("R19200"),
		QObject::tr("R38400"),
		QObject::tr("R57600"),
		QObject::tr("R115200"),
		QObject::tr("RLAST"),

	};
	return names[e];
}

inline QString AvUiLangsPtzVeriFy(PtzVeriFy e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("Odd"),
		QObject::tr("Even"),
		QObject::tr("Mark"),
		QObject::tr("Empty"),
		QObject::tr("Last"),
	};
	return names[e];
}



inline QString AvUiLangsPtzProtocol(PtzProtocol e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("Pelcod"),
		QObject::tr("Pelcod1"),
		QObject::tr("Pelcod_s"),
		QObject::tr("Pelcod_s1"),
		QObject::tr("Pelcop"),
		QObject::tr("Pelcop1"),
		QObject::tr("Pelcop5"),
		QObject::tr("SamSung"),
		QObject::tr("Sony"),
		QObject::tr("Last"),
	};
	return names[e];
}


inline QString AvUiLangsAudioSampleRate(AudioSampleRate e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("R8000"),
		QObject::tr("R11025"),
		QObject::tr("R12000"),
		QObject::tr("R16000"),
		QObject::tr("R22050"),
		QObject::tr("R24000"),
		QObject::tr("R32000"),
		QObject::tr("R44100"),
		QObject::tr("R48000"),
		QObject::tr("R64000"),
		QObject::tr("R88200"),
		QObject::tr("R96000"),
		QObject::tr("R192000"),
		QObject::tr("LAST"),

	};
	return names[e];
}


inline QString AvUiLangsAudioSampleBits(AudioSampleBits e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("B8"),
		QObject::tr("B16"),
		QObject::tr("B24"),
		QObject::tr("B32"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsPtzCameraLensFocusCtrlMode(PtzCameraLensFocusCtrlMode e){
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("Auto"),
		QObject::tr("SemiAutomatic"),
		QObject::tr("Manual"),
		QObject::tr("Last"),
	};
	return names[e];
}

inline QString AvUiLangsPtzCameraLensIrisCtrlMode(PtzCameraLensIrisCtrlMode e)
{
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("Auto"),
		QObject::tr("Manual"),
		QObject::tr("Last"),
	};
	return names[e];
}
inline QString AvUiLangsPtzCameraLensFocusRegion(PtzCameraLensFocusRegion e)
{
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("AllRegion"),
		QObject::tr("CentralRegion"),
		QObject::tr("Last"),
	};
	return names[e];
}

inline QString AvUiLangsPtzCameraLensFocusSearchMode(PtzCameraLensFocusSearchMode e)
{
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("FullView"),
		QObject::tr("FAR_1_5M"),
		QObject::tr("FAR_3_0M"),
		QObject::tr("FAR_6_0M"),
		QObject::tr("FAR_8_0M"),
		QObject::tr("FAR_Infinity"),
		QObject::tr("Last"),
	};
	return names[e];
}


inline QString AvUiLangsAudioSoundMode(AudioSoundMode e) {
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("MONO"),
		QObject::tr("STEREO"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsAudioCaptureDevice(AudioCaptureDevice e) {
	
	static QString names[] = {
		QObject::tr("NONE"),
		QObject::tr("LINE_IN"),
		QObject::tr("MIC_IN"),
		QObject::tr("LAST"),
	};
	return names[e];
}


inline QString AvUiLangsProgressStatus(ProgressStatus e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("UpgradeStart"),
		QObject::tr("UpgradeRecvData"),
		QObject::tr("UpgradeWipePartition"),
		QObject::tr("UpgradeWriteData"),
		QObject::tr("UpgradeOver"),
		QObject::tr("UpgradeExtract"),
		QObject::tr("UpgradeCopy"),
		QObject::tr("UpgradeInstall"),
		QObject::tr("Rebootting"),
		QObject::tr("ModifyOver"),
		QObject::tr("HaveNoDiskResource"),
		QObject::tr("HaveNoRightResource"),
		QObject::tr("LinkFailed"),
		QObject::tr("Last"),
	};
	return names[e];
}

inline QString AvUiLangsLinkProtocol(LinkProtocol e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("Moon"),
		QObject::tr("Onvif"),
		QObject::tr("GBT28181"),
		QObject::tr("RTSP"),
		QObject::tr("Last"),

	};
	return names[e];
}

inline QString AvUiLangsOverLayDateTimeStyle(OverLayDateTimeStyle e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("Y_M_D_H_M_S"),
		QObject::tr("Y_M_D_M_H_M_S"),
		QObject::tr("H_M_S_Y_M_D"),
		QObject::tr("Last"),

	};
	return names[e];
}
inline QString AvUiLangsOverLayDateTimeSplit(OverLayDateTimeSplit e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("Split_Symbol"),
		QObject::tr("Split_Word"),
		QObject::tr("Last"),
	};
	return names[e];
}

inline QString AvUiLangsFontSize(FontSize e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("_8"),
		QObject::tr("_16"),
		QObject::tr("_24"),
		QObject::tr("_32"),
		QObject::tr("_48"),
		QObject::tr("_64"),
		QObject::tr("_128"),
		QObject::tr("Last"),
	};
	return names[e];
}

inline QString AvUiLangsEmailEncodeType(EmailEncodeType e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("TLS"),
		QObject::tr("SSL"),
		QObject::tr("NULL"),
		QObject::tr("Last"),
	};
	return names[e];
}

inline QString AvUiLangsDdnsType(DdnsType e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("DynDns"),
		QObject::tr("Ddns3322"),
		QObject::tr("DnsDynMaic"),
		QObject::tr("DdnsExt1"),
		QObject::tr("DdnsExt2"),
		QObject::tr("Last"),
	};
	return names[e];
}

inline QString AvUiLangsRenewalInterval(RenewalInterval e) {
	static QString names[] = {
		QObject::tr("None"),
		QObject::tr("HalfHour"),
		QObject::tr("OneHour"),
		QObject::tr("ThreeHour"),
		QObject::tr("SixHour"),
		QObject::tr("TwelveHour"),
		QObject::tr("OneDay"),
		QObject::tr("OneWeek"),
		QObject::tr("OneMonth"),
		QObject::tr("Last"),
	};
	return names[e];
}

  
inline QString AvUiLangsTimeZone(TimeZone e) {
	static QString names[] = {
		QObject::tr("UTC_None"),
		QObject::tr("UTC_W12"),
		QObject::tr("UTC_W11"),
		QObject::tr("UTC_W10"),
		QObject::tr("UTC_W9_30"),
		QObject::tr("UTC_W9"),
		QObject::tr("UTC_W8"),
		QObject::tr("UTC_W7"),
		QObject::tr("UTC_W6"),
		QObject::tr("UTC_W5"),
		QObject::tr("UTC_W4"),
		QObject::tr("UTC_W3_30"),
		QObject::tr("UTC_W3"),
		QObject::tr("UTC_W2"),
		QObject::tr("UTC_W1"),
		QObject::tr("UTC_0"),
		QObject::tr("UTC_E1"),
		QObject::tr("UTC_E2"),
		QObject::tr("UTC_E3"),
		QObject::tr("UTC_E3_30"),
		QObject::tr("UTC_E4"),
		QObject::tr("UTC_E4_30"),
		QObject::tr("UTC_E5"),
		QObject::tr("UTC_E5_30"),
		QObject::tr("UTC_E5_45"),
		QObject::tr("UTC_E6"),
		QObject::tr("UTC_E6_30"),
		QObject::tr("UTC_E7"),
		QObject::tr("UTC_E8"),
		QObject::tr("UTC_E8_45"),
		QObject::tr("UTC_E9"),
		QObject::tr("UTC_E9_30"),
		QObject::tr("UTC_E10"),
		QObject::tr("UTC_E10_30"),
		QObject::tr("UTC_E11"),
		QObject::tr("UTC_E12"),
		QObject::tr("UTC_E13"),
		QObject::tr("UTC_E14"),
		QObject::tr("UTC_Last"),
	};
	return names[e];
}

#endif

