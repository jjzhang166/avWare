#include "dlgdeviceset.h"
#include "ui_dlgdeviceset.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvCapture/AvManCapture.h"
#include "AvSource/AvQStringsLanguage.h"
#include "AvForm/dlgareaselect.h"
#include "AvForm/dlgoverlay.h"
//#include "AvForm/ctimearea.h"
#include "AvForm/dlgtimeselect.h"
#include "AvUiComm/AvUiConfigIni.h"
typedef enum {
	TabWidgetDeviceSet_Capture,
	TabWidgetDeviceSet_Encode,
	TabWidgetDeviceSet_Audio,
	TabWidgetDeviceSet_Version,
	TabWidgetDeviceSet_Record,
	TabWidgetDeviceSet_NetWork,
	TabWidgetDeviceSet_Alarm,
	TabWidgetDeviceSet_UserManager,
	TabWidgetDeviceSet_Log,
	TabWidgetDeviceSet_Ptz,
}TabWidgetDeviceSet;

typedef enum {
	TboxNetWorkEnum_NetSet,
	TboxNetWorkEnum_Ftp,
	TboxNetWorkEnum_Email,
	TboxNetWorkEnum_Upnp,
	TboxNetWorkEnum_Rtsp,
	TboxNetWorkEnum_Ddns,
	TboxNetWorkEnum_P2p,
	TboxNetWorkEnum_Rtmp,
	TboxNetWorkEnum_Ntp,
}TboxNetWorkEnum;


DlgDeviceSet::DlgDeviceSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgDeviceSet)
{
	m_UiInitedOver = false;
    ui->setupUi(this);
	
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnMenu, QChar(0xf0c9), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool);

	FixDlgUi();
	ui->StWidgetNetWork->setCurrentIndex(0);
	ui->TBoxNetWork->setCurrentIndex(0);

	ui->TabWidgetDeviceSet->setCurrentIndex(TabWidgetDeviceSet_Version);
	FillInVersion();

	m_UiInitedOver = true;

	m_mouseRtmpRightMenu = NULL;
	m_RtmpItemModelcolumn = -1;
	m_RtmpItemModelrow = -1;
	m_RtmpListStandardItemModel = NULL;

	memset(&m_CurrentAudioCaps, 0x00, sizeof(m_CurrentAudioCaps));
	memset(&m_CurrentAudioProfile, 0x00, sizeof(m_CurrentAudioProfile));
	memset(&m_AlarmTimeArea, 0x00, sizeof(m_AlarmTimeArea));
	memset(&m_AlarmMotionArea, 0x00, sizeof(m_AlarmMotionArea));
}

void DlgDeviceSet::SelectModifyChannel(int Channel)
{
	QString String;
	String = String.sprintf("CHN%02d", Channel);
	ui->CBoxSettingChannel->setCurrentText(String);
}
void DlgDeviceSet::FixDlgUi()
{
	IconComm::Instance()->SetIcon(ui->BtnNetWorkRTMPAdd, QChar(0xf067), 10);
	IconComm::Instance()->SetIcon(ui->BtnNetWorkRTMPDel, QChar(0xf068), 10);

	ui->CBoxSettingChannel->clear();
	int MaxValidChns = g_AvManCapture.GetAvCaptureTotal();
	QString CboxSettingChanelItem;
	for (int i = 0; i < MaxValidChns; i++){
		CboxSettingChanelItem.clear();
		CboxSettingChanelItem = CboxSettingChanelItem.sprintf("CHN%02d", i);
		ui->CBoxSettingChannel->addItem(CboxSettingChanelItem);
	}

	//ui->CBoxSettingChannel->setStyleSheet("color:black");
	FixDlgUiCapture();
	FixDlgUiEncode();
	FixDlgUiNetWork();
	FixDlgUiAudio();
	FixDlgUiRecord();
	//QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}

void DlgDeviceSet::FixDlgUiCapture()
{
// 	ui->CBoxIrCut->setStyleSheet("color:black");
// 	ui->CBoxAnitFlicker->setStyleSheet("color:black");
// 	ui->CBoxExposureMode->setStyleSheet("color:black");
// 	ui->CBoxWhitBalance->setStyleSheet("color:black");
// 	ui->CBoxRotateMode->setStyleSheet("color:black");
// 	ui->CBoxCvbs->setStyleSheet("color:black");
}
void DlgDeviceSet::FixDlgUiEncode()
{
// 	ui->CBoxMBitRateCtrl->setStyleSheet("color:black");
// 	ui->CBoxMBitRateQlevel->setStyleSheet("color:black");
// 	ui->CBoxMEnFormat->setStyleSheet("color:black");
// 	ui->CBoxMFrameRate->setStyleSheet("color:black");
// 	ui->CBoxMImageSize->setStyleSheet("color:black");
// 	
// 	ui->CBoxSBitRateCtrl->setStyleSheet("color:black");
// 	ui->CBoxSBitRateQlevel->setStyleSheet("color:black");
// 	ui->CBoxSSlaveNumber->setStyleSheet("color:black");
// 	ui->CBoxSFrameRate->setStyleSheet("color:black");
// 	ui->CBoxSImageSize->setStyleSheet("color:black");

}
void DlgDeviceSet::FixDlgUiAudio()
{
// 	ui->CBoxAiSoundMode->setStyleSheet("color:black");
// 	ui->CBoxAiEncodeType->setStyleSheet("color:black");
// 	ui->CBoxAiSampleBits->setStyleSheet("color:black");
// 	ui->CBoxAiSampleRate->setStyleSheet("color:black");
}
void DlgDeviceSet::FixDlgUiVersion()
{

}
void DlgDeviceSet::FixDlgUiRecord()
{
// 	QStringList headers;
// 	headers.push_back(QString(tr("No")));
// 	headers.push_back(QString(tr("FileName")));
// 	headers.push_back(QString(tr("Chn")));
// 	headers.push_back(QString(tr("Slave")));
// 	headers.push_back(QString(tr("Length")));
// 	TableModel *ListTableModel = new TableModel;
// 	ListTableModel->setHorizontalHeader(headers);
// 
// 	ui->TViewRecordFileList->setModel(ListTableModel);
// 	ui->TViewRecordFileList->setSelectionBehavior(QAbstractItemView::SelectRows);
// 	ui->TViewRecordFileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
// 	ui->TViewRecordFileList->setContextMenuPolicy(Qt::CustomContextMenu);
// 	ui->TViewRecordFileList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);


//	QObject::connect((const QObject *)ui->TViewRecordFileList, SIGNAL(resize(int, int)), this, SLOT(SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged(int, int)));
	
	//emit ListTableModel->layoutChanged();
}

#define D_RTMPITEMMODEL_HEADSECTION_NO			0
#define D_RTMPITEMMODEL_HEADSECTION_RTMPURL		1
#define D_RTMPITEMMODEL_HEADSECTION_CHN			2
#define D_RTMPITEMMODEL_HEADSECTION_SLAVE		3
#define D_RTMPITEMMODEL_HEADSECTION_AUDIO		4
#define D_RTMPITEMMODEL_HEADSECTION_STATUS		5
#define D_RTMPITEMMODEL_HEADSECTION_TOTAL		6	


#define D_RTMP_INI_SECTION			"RTMP"
#define D_RTMP_INI_KEY_ADDR			"Addr"
#define D_RTMP_INI_KEY_STRING		"Strings"
#define D_RTMP_INI_KEY_WITH_AUDIO	"Audio"


void DlgDeviceSet::FixDlgUiNetWork()
{
	m_RtmpListStandardItemModel = new QStandardItemModel;
	m_RtmpListStandardItemModel->setColumnCount(D_RTMPITEMMODEL_HEADSECTION_TOTAL);
	m_RtmpListStandardItemModel->setHeaderData(D_RTMPITEMMODEL_HEADSECTION_NO, Qt::Horizontal, tr("No"));
	m_RtmpListStandardItemModel->setHeaderData(D_RTMPITEMMODEL_HEADSECTION_RTMPURL, Qt::Horizontal, tr("RtmpUrl"));
	m_RtmpListStandardItemModel->setHeaderData(D_RTMPITEMMODEL_HEADSECTION_CHN, Qt::Horizontal, tr("Chn"));
	m_RtmpListStandardItemModel->setHeaderData(D_RTMPITEMMODEL_HEADSECTION_SLAVE, Qt::Horizontal, tr("Slave"));
	m_RtmpListStandardItemModel->setHeaderData(D_RTMPITEMMODEL_HEADSECTION_AUDIO, Qt::Horizontal, tr("Audio"));
	m_RtmpListStandardItemModel->setHeaderData(D_RTMPITEMMODEL_HEADSECTION_STATUS, Qt::Horizontal, tr("Status"));

	ui->TviewRtmpLinkInfo->setModel(m_RtmpListStandardItemModel);
	ui->TviewRtmpLinkInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewRtmpLinkInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewRtmpLinkInfo->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TviewRtmpLinkInfo->verticalHeader()->hide();


	QObject::connect((const QObject *)ui->TviewRtmpLinkInfo->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged(int, int)));
	{
		QVariant Var = CAvUiConfigIni::Instance()->InigetValue(QString(D_RTMP_INI_SECTION), QString(D_RTMP_INI_KEY_ADDR));
		if (Var.toString().size() != 0){
			ui->LiEditNetWorkRTMPAddr->setText(Var.toString());
		}
		else{
			ui->LiEditNetWorkRTMPAddr->setText(QString("rtmp://rtmp.pushserver.com/live"));
		}
		

		 Var = CAvUiConfigIni::Instance()->InigetValue(QString(D_RTMP_INI_SECTION), QString(D_RTMP_INI_KEY_STRING));
		 if (Var.toString().size() != 0){
			 ui->LiEditNetWorkRTMPString->setText(Var.toString());
		 }
		 else{
			 ui->LiEditNetWorkRTMPString->setText(QString("sample"));
		 }


		 Var = CAvUiConfigIni::Instance()->InigetValue(QString(D_RTMP_INI_SECTION), QString(D_RTMP_INI_KEY_WITH_AUDIO));
		 if (Var.toBool() == true){
			 ui->CBoxNetWorkRTMPAudio->setChecked(true);
		 }
		 else{
			 ui->CBoxNetWorkRTMPAudio->setChecked(false);
		 }

		 ui->CBoxNetWorkRTMPStream->clear();

		 ui->CBoxNetWorkRTMPStream->addItem(AvUiLangsCHL(CHL_MAIN_T));
		 ui->CBoxNetWorkRTMPStream->addItem(AvUiLangsCHL(CHL_SUB1_T));
	}


}
void DlgDeviceSet::FixDlgUiAlarm()
{

}
void DlgDeviceSet::FixDlgUiUserManager()
{

}
void DlgDeviceSet::FixDlgUiLog()
{

}
void DlgDeviceSet::FixDlgPtz()
{

}

void DlgDeviceSet::ResizeRecord()
{
// 
// 	QSize viewSize = ui->TViewRecordFileList->size();
// 	int TviewWidth = viewSize.width();
// 	TviewWidth = 550;
// 	AvQDebug("Size [%d, %d]\n", viewSize.width(), viewSize.height());
// 	ui->TViewRecordFileList->setColumnWidth(0, 0.1*TviewWidth);
// 	ui->TViewRecordFileList->setColumnWidth(1, 0.55*TviewWidth);
// 	ui->TViewRecordFileList->setColumnWidth(2, 0.075*TviewWidth);
// 	ui->TViewRecordFileList->setColumnWidth(3, 0.075*TviewWidth);
// 	ui->TViewRecordFileList->setColumnWidth(4, 0.2*TviewWidth);
}

void DlgDeviceSet::ResizeNetWorkRtmp()
{
	QSize viewSize = ui->TviewRtmpLinkInfo->viewport()->size();
	int TviewWidth = viewSize.width() + 3;
	ui->TviewRtmpLinkInfo->setColumnWidth(D_RTMPITEMMODEL_HEADSECTION_NO,		0.08*TviewWidth);
	ui->TviewRtmpLinkInfo->setColumnWidth(D_RTMPITEMMODEL_HEADSECTION_RTMPURL,	0.55*TviewWidth);
	ui->TviewRtmpLinkInfo->setColumnWidth(D_RTMPITEMMODEL_HEADSECTION_CHN,		0.07*TviewWidth);
	ui->TviewRtmpLinkInfo->setColumnWidth(D_RTMPITEMMODEL_HEADSECTION_SLAVE,	0.10*TviewWidth);
	ui->TviewRtmpLinkInfo->setColumnWidth(D_RTMPITEMMODEL_HEADSECTION_AUDIO,	0.10*TviewWidth);
	ui->TviewRtmpLinkInfo->setColumnWidth(D_RTMPITEMMODEL_HEADSECTION_STATUS,	0.10*TviewWidth);
}

void DlgDeviceSet::FillInCapture()
{

	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
	av_bool bRet = av_false;
	C_CaptureCaps CaptureCaps;
	bRet = pCapture->CaptureGetCaps(CaptureCaps);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}


	ui->CBoxIrCut->clear();
	ui->CBoxAnitFlicker->clear();
	ui->CBoxExposureMode->clear();
	ui->CBoxWhitBalance->clear();
	
	
	{
		//wdr
		ui->CBoxWdr->clear();
		if (CaptureCaps.WdrModeMask == 0){
			ui->CBoxWdr->hide();
			ui->LabWdr->hide();
			ui->LabWdrValueRegion->hide();
			ui->LiEditWdrValue->hide();
		}
		else{
			ui->CBoxWdr->show();
			ui->LabWdr->show();
			ui->LabWdrValueRegion->show();
			ui->LiEditWdrValue->show();
		}
		for (int i = WdrMode_None; i < WdrMode_Last; i++){
			if (CaptureCaps.WdrModeMask & AvMask(i)){
				ui->CBoxWdr->addItem(AvUiLangsWdrMode((WdrMode)i));
			}
		}
	}

	{
		ui->CBoxShutter->clear();
		if (CaptureCaps.ShutterModeMask == 0){
			ui->CBoxShutter->hide();
			ui->LabShutter->hide();
			ui->LabShutterValueRegion->hide();
			ui->LiEditShutter->hide();
		}
		else{
			ui->CBoxShutter->show();
			ui->LabShutter->show();
			ui->LabShutterValueRegion->show();
			ui->LiEditShutter->show();
		}

		if (CaptureCaps.ShutterModeMask & AvMask(ShutterMode_MaxShutter)){

		}
		else{
			ui->LabShutterValueRegion->hide();
			ui->LiEditShutter->hide();
		}
	}
	for (int i = IrCutMode_NONE; i < IrCutMode_LAST; i++){
		if (CaptureCaps.IrCutMask & AvMask(i)){
			ui->CBoxIrCut->addItem(AvUiLangsIrCutMode((IrCutMode)i));
		}
	}


	for (int i = AntiFlckerMode_NONE; i < AntiFlckerMode_LAST; i++){
		if (CaptureCaps.AntiFlckerMask & AvMask(i)){
			ui->CBoxAnitFlicker->addItem(AvUiLangsAntiFlckerMode((AntiFlckerMode)i));
		}
	}
	for (int i = WhiteBalanceMode_NONE; i < WhiteBalanceMode_LAST; i++){
		if (CaptureCaps.WhiteBalanceMask & AvMask(i)){
			ui->CBoxWhitBalance->addItem(AvUiLangsWhiteBalanceMode((WhiteBalanceMode)i));
		}
	}

	for (int i = ExposureMode_NONE; i < ExposureMode_LAST; i++){
		if (CaptureCaps.ExposureMask & AvMask(i)){
			ui->CBoxExposureMode->addItem(AvUiLangsExposureMode((ExposureMode)i));
		}
	}
	bRet = av_false;
	ui->CBoxHorizontal->setEnabled(false);
	ui->CBoxVertical->setEnabled(false);
	for (int i = MirrorMode_NONE; i < MirrorMode_LAST; i++){
		if (CaptureCaps.MirrorMask & AvMask(i)){
			unsigned int MirrorMask = ~AvMask(i);//��ʱ��������CaptureCaps.MirrorMask ������ÿ�ζ�������if (MirrorMask & AvMask(MirrorMode_HOR)){
			if (MirrorMask & AvMask(MirrorMode_HOR)){
				ui->CBoxHorizontal->setEnabled(true);
			}
			else if (MirrorMask & AvMask(MirrorMode_VER)){
				ui->CBoxVertical->setEnabled(true);
			}else{
				bRet = av_true;
				ui->CBoxRotateMode->addItem(AvUiLangsMirrorMode((MirrorMode)i));
			}
		}
	}
	if (bRet == av_false){
		ui->CBoxRotateMode->hide();
		ui->LabRotateMode->hide();
	}
	else{
		ui->CBoxRotateMode->show();
		ui->LabRotateMode->show();
	}

	ui->CBoxCvbs->clear();
	ui->LabCvbs->hide();
	ui->CBoxCvbs->hide();
	if (CaptureCaps.CvbsMask & AvMask(av_false)){
		ui->CBoxCvbs->addItem(AvUiLangsOpenClose(av_false));
		ui->LabCvbs->show();
		ui->CBoxCvbs->show();
	}
	if (CaptureCaps.CvbsMask &AvMask(av_true)){
		ui->CBoxCvbs->addItem(AvUiLangsOpenClose(av_true));
		ui->LabCvbs->show();
		ui->CBoxCvbs->show();
	}



	C_CaptureProfile CaptureProfile;
	bRet = pCapture->CaptureGetProfile(CaptureProfile);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}

	ui->CBoxIrCut->setCurrentText(AvUiLangsIrCutMode(CaptureProfile.IrCut));
	//ui->TEditIrCutStime->setTime(QTime())
	ui->CBoxAnitFlicker->setCurrentText(AvUiLangsAntiFlckerMode(CaptureProfile.AntiFlcker));
	ui->LiEditAnitFlickerCustom->setText(QString::number(CaptureProfile.AntiFlckerValue));
	
	ui->CBoxExposureMode->setCurrentText(AvUiLangsExposureMode(CaptureProfile.Exposure));
	ui->LiEditExposureModeCustom->setText(QString::number(CaptureProfile.ExposureValue));

	ui->CBoxWhitBalance->setCurrentText(AvUiLangsWhiteBalanceMode(CaptureProfile.WhiteBalance));
	ui->LiEditWhitBalanceCustom->setText(QString::number(CaptureProfile.WhiteBalanceValue));

	ui->CBoxWdr->setCurrentText(AvUiLangsWdrMode(CaptureProfile.Wdr));
	ui->LiEditWdrValue->setText(QString::number(CaptureProfile.WdrValue));

	ui->CBoxShutter->setCurrentText(AvUiLangsShutterMode(CaptureProfile.Shutter));
	ui->LiEditShutter->setText(QString::number(CaptureProfile.ShutterMax));

	if (CaptureProfile.MirrorMaskValue & AvMask(MirrorMode_HOR)){
		ui->CBoxHorizontal->setChecked(true);
	}
	else{
		ui->CBoxHorizontal->setChecked(false);
	}
	if (CaptureProfile.MirrorMaskValue & AvMask(MirrorMode_VER)){
		ui->CBoxVertical->setChecked(true);
	}
	else{
		ui->CBoxVertical->setChecked(false);
	}


	for (int i = MirrorMode_Rotate_000; i <= MirrorMode_Rotate_270; i++){
		if (AvMask(i) & CaptureProfile.MirrorMaskValue){
			ui->CBoxRotateMode->setCurrentText(AvUiLangsMirrorMode((MirrorMode)i));
			break;
		}
	}

	if (CaptureProfile.bOpenCvbs == av_true){
		ui->CBoxCvbs->setCurrentText(AvUiLangsOpenClose(av_true));
	}
	else{
		ui->CBoxCvbs->setCurrentText(AvUiLangsOpenClose(av_false));
	}
}
void DlgDeviceSet::FillInEncode()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_EncodeCaps EncodeCaps;
	C_EnCodeProfile EnCodeProfile_M;
	C_EnCodeProfile EnCodeProfile_S;

	av_bool bRet = av_false;
	bRet = pCapture->EncodeGetCaps(EncodeCaps);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}

	bRet = pCapture->EncodeGetProfile(CHL_MAIN_T, EnCodeProfile_M);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}

	bRet = pCapture->EncodeGetProfile(CHL_SUB1_T, EnCodeProfile_S);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}



	ui->CBoxMEnFormat->clear();
	for (int i = AvComp_H264; i < AvComp_LAST; i++){
		if (EncodeCaps.CompMask & AvMask(i)){
			ui->CBoxMEnFormat->addItem(AvUiLangsAvComp((AvComp)i));
		}
	}

	ui->CBoxMImageSize->clear();
	for (int i = CaptureSize_Self; i < CaptureSize_SNR; i++){
		if (EncodeCaps.ImageSizeMask & AvMask(i)){
			ui->CBoxMImageSize->addItem(AvUiLangsCaptureSize((CaptureSize)i));
		}
	}
	ui->CBoxMFrameRate->clear();
	int Start, End;
	Start = EncodeCaps.FrameRateMask[EnCodeProfile_M.ImageSize] >> 16;
	End = EncodeCaps.FrameRateMask[EnCodeProfile_M.ImageSize] & 0xffff;
	for (int i = Start; i <= End; i++){
		ui->CBoxMFrameRate->addItem(QString::number(i));
	}

	ui->CBoxMBitRateCtrl->clear();
	for (int i = BitRateCtrl_NONE; i < BitRateCtrl_LAST; i++){
		if (EncodeCaps.BitRateCtrlMask & AvMask(i)){
			ui->CBoxMBitRateCtrl->addItem(AvUiLangsBitRateCtrl((BitRateCtrl)i));
		}
	}

	ui->CBoxMBitRateQlevel->clear();
	switch (EnCodeProfile_M.BitRateCtl)
	{
	case BitRateCtrl_CBR:
	{
		for (int i = 1; i * 512 <= 8192; i++){
			char str[12];
			sprintf(str, "%04d", i * 512);
			ui->CBoxMBitRateQlevel->addItem(QString(str));
		}
		ui->CBoxMBitRateQlevel->addItem(AvUiLangsCustom());
		ui->LabMVcbr->hide();
		ui->LiEditMBitRateQlevel->show();
	}
	break;
	case BitRateCtrl_VBR:
	{
		for (int i = 1; i < 5; i++){
			ui->CBoxMBitRateQlevel->addItem(QString::number(i));
		}
		ui->LabMVcbr->hide();
		ui->LiEditMBitRateQlevel->hide();
	}
	break;
	case BitRateCtrl_VCBR:
	{
		for (int i = 1; i < 5; i++){
			ui->CBoxMBitRateQlevel->addItem(QString::number(i));
		}
		ui->LabMVcbr->show();
		ui->LiEditMBitRateQlevel->show();
	}
	break;
	default:
		break;
	}




	ui->CBoxSSlaveNumber->clear();
	for (int i = CHL_SUB1_T; i < CHL_NR_T; i++){
		if (EncodeCaps.ExtChannelMask &AvMask(i)){
			ui->CBoxSSlaveNumber->addItem(AvUiLangsCHL((E_EncodeCHL)i));
		}
	}


	ui->CBoxSImageSize->clear();
	for (int i = CaptureSize_Self; i < CaptureSize_SNR; i++){
		if (EncodeCaps.ExtImageSizeMask[EnCodeProfile_M.ImageSize] & AvMask(i)){
			ui->CBoxSImageSize->addItem(AvUiLangsCaptureSize((CaptureSize)i));
		}
	}

	ui->CBoxSFrameRate->clear();

	Start = EncodeCaps.FrameRateMask[EnCodeProfile_S.ImageSize] >> 16;
	End = EncodeCaps.FrameRateMask[EnCodeProfile_S.ImageSize] & 0xffff;
	for (int i = Start; i <= End; i++){
		ui->CBoxSFrameRate->addItem(QString::number(i));
	}

	ui->CBoxSBitRateCtrl->clear();
	for (int i = BitRateCtrl_NONE; i < BitRateCtrl_LAST; i++){
		if (EncodeCaps.BitRateCtrlMask & AvMask(i)){
			ui->CBoxSBitRateCtrl->addItem(AvUiLangsBitRateCtrl((BitRateCtrl)i));
		}
	}

	ui->CBoxSBitRateQlevel->clear();
	switch (EnCodeProfile_S.BitRateCtl)
	{
	case BitRateCtrl_CBR:
	{
		for (int i = 1; i * 64 <= 2048; i++){
			char str[12];
			sprintf(str, "%04d", i * 64);
			ui->CBoxSBitRateQlevel->addItem(QString(str));
		}
		ui->CBoxSBitRateQlevel->addItem(AvUiLangsCustom());
		ui->LabSVcbr->hide();
		ui->LiEditSBitRateQlevel->show();
	}
		break;
	case BitRateCtrl_VBR:
	{
		for (int i = 1; i < 5; i++){
			ui->CBoxSBitRateQlevel->addItem(QString::number(i));
		}
		ui->LabSVcbr->hide();
		ui->LiEditSBitRateQlevel->hide();
	}
		break;
	case BitRateCtrl_VCBR:
	{
		for (int i = 1; i < 5; i++){
			ui->CBoxSBitRateQlevel->addItem(QString::number(i));
		}
		ui->LabSVcbr->show();
		ui->LiEditSBitRateQlevel->show();
	}
		break;
	default:
		break;
	}

	////profile
	ui->CBoxMEnFormat->setCurrentText(AvUiLangsAvComp(EnCodeProfile_M.Comp));
	ui->CBoxMImageSize->setCurrentText(AvUiLangsCaptureSize(EnCodeProfile_M.ImageSize));
	ui->CBoxMFrameRate->setCurrentText(QString::number(EnCodeProfile_M.FrameRate));
	ui->CBoxMBitRateCtrl->setCurrentText(AvUiLangsBitRateCtrl(EnCodeProfile_M.BitRateCtl));
	switch (EnCodeProfile_M.BitRateCtl)
	{
	case BitRateCtrl_CBR:
	{
		if (EnCodeProfile_M.BitRateValue % 512 != 0){
			ui->CBoxMBitRateQlevel->setCurrentText(AvUiLangsCustom());
			ui->LiEditMBitRateQlevel->setText(QString::number(EnCodeProfile_M.BitRateValue));
		}
		else{
			char str[12];
			sprintf(str, "%04d", EnCodeProfile_M.BitRateValue);
			ui->CBoxMBitRateQlevel->setCurrentText(QString(str));
			ui->LiEditMBitRateQlevel->setText(QString(str));
		}
	}
	break;
	case BitRateCtrl_VBR:
	{
		ui->CBoxMBitRateQlevel->setCurrentText(QString::number(EnCodeProfile_M.Qlevel));
	}
	break;
	case BitRateCtrl_VCBR:
	{
		ui->CBoxMBitRateQlevel->setCurrentText(QString::number(EnCodeProfile_M.Qlevel));
		ui->LiEditMBitRateQlevel->setText(QString::number(EnCodeProfile_M.BitRateValue));
	}
	break;
	default:
		break;
	}
	ui->LiEditMIFrameSpan->setText(QString::number(EnCodeProfile_M.Gop));

	///proflie slvae
	ui->CBoxSSlaveNumber->setCurrentText(AvUiLangsCHL(CHL_SUB1_T));
	ui->CBoxSImageSize->setCurrentText(AvUiLangsCaptureSize(EnCodeProfile_S.ImageSize));
	ui->CBoxSFrameRate->setCurrentText(QString::number(EnCodeProfile_S.FrameRate));
	ui->CBoxSBitRateCtrl->setCurrentText(AvUiLangsBitRateCtrl(EnCodeProfile_S.BitRateCtl));


	switch (EnCodeProfile_S.BitRateCtl)
	{
	case BitRateCtrl_CBR:
	{
		if (EnCodeProfile_M.BitRateValue % 64 != 0){
			ui->CBoxSBitRateQlevel->setCurrentText(AvUiLangsCustom());
			ui->LiEditSBitRateQlevel->setText(QString::number(EnCodeProfile_S.BitRateValue));
		}
		else{
			char str[12];
			sprintf(str, "%04d", EnCodeProfile_S.BitRateValue);
			ui->CBoxSBitRateQlevel->setCurrentText(QString(str));
			ui->LiEditSBitRateQlevel->setText(QString(str));
		}
	}
		break;
	case BitRateCtrl_VBR:
	{
		ui->CBoxSBitRateQlevel->setCurrentText(QString::number(EnCodeProfile_S.Qlevel));
	}
		break;
	case BitRateCtrl_VCBR:
	{
		ui->CBoxMBitRateQlevel->setCurrentText(QString::number(EnCodeProfile_S.Qlevel));
		ui->LiEditSBitRateQlevel->setText(QString::number(EnCodeProfile_S.BitRateValue));
	}
		break;
	default:
		break;
	}
	
	ui->LiEditSIFrameSpan->setText(QString::number(EnCodeProfile_S.Gop));
}
void DlgDeviceSet::FillInAudio()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
// 	C_AudioCaps AudioCaps;
// 	C_AudioProfile AudioProfile;

	av_bool bRet = av_false;
	bRet = pCapture->AudioGetCaps(m_CurrentAudioCaps);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}
	bRet = pCapture->AudioGetProfile(m_CurrentAudioProfile);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}
	QString AoCapsStr;

	ui->CBoxAiSoundMode->clear();
	AoCapsStr.clear();
	for (int i = AudioSoundMode_NONE; i < AudioSoundMode_LAST; i++){
		if (AvMask(i) & m_CurrentAudioCaps.SoundModeMask){
			ui->CBoxAiSoundMode->addItem(AvUiLangsAudioSoundMode((AudioSoundMode)i));
			AoCapsStr += AvUiLangsAudioSoundMode((AudioSoundMode)i);
			AoCapsStr += QString(" ");
		}
	}
	ui->LabAoSoundModeText->setText(AoCapsStr);

	QString SamplebitsStr;
	QString SampleRateStr;
	char StrFomat[64] = { 0 };
	for (int i = AvComp_H264; i < AvComp_LAST; i++){
		if (AvMask(i) & m_CurrentAudioCaps.CompMask){
			sprintf(StrFomat, "%6s", AvUiLangsAvComp((AvComp)i).toStdString().c_str());
			SamplebitsStr += QString(StrFomat);
			SamplebitsStr += QString(" [ ");

			SampleRateStr += QString(StrFomat);
			SampleRateStr += QString(" [ ");
			for (int j = AudioSampleBits_NONE; j < AudioSampleBits_LAST; j++){
				if (AvMask(j) & m_CurrentAudioCaps.SampleBitsMask[i]){
					SamplebitsStr += AvUiLangsAudioSampleBits((AudioSampleBits)j);
					SamplebitsStr += QString(" ");
				}
			}

			for (int j = AudioSampleRate_NONE; j < AudioSampleRate_LAST; j++){
				if (AvMask(j) & m_CurrentAudioCaps.SampleRateMask[i]){
					SampleRateStr += AvUiLangsAudioSampleRate((AudioSampleRate)j);
					SampleRateStr += QString(" ");
				}
			}

			SamplebitsStr += QString("]\n");
			SampleRateStr += QString("]\n");
		}
	}
	ui->LabAoSampleBitsText->setText(SamplebitsStr);
	ui->LabAoSampleRateText->setText(SampleRateStr);




	AoCapsStr.clear();
	ui->CBoxAiEncodeType->clear();
	for (int i = AvComp_H264; i < AvComp_LAST; i++){
		if (AvMask(i) & m_CurrentAudioCaps.CompMask){
			ui->CBoxAiEncodeType->addItem(AvUiLangsAvComp((AvComp)i));
			AoCapsStr += AvUiLangsAvComp((AvComp)i);
			AoCapsStr += QString(" ");
		}
	}
	ui->LabAoEncodeTypeText->setText(AoCapsStr);



	if (AvMask(AudioCaptureDevice_LINE_IN) & m_CurrentAudioCaps.CaptureDeviceMask){
		ui->RBtnAiLineIn->setCheckable(true);
	}
	else{
		ui->RBtnAiLineIn->setCheckable(false);
	}
	if (AvMask(AudioCaptureDevice_MIC_IN) & m_CurrentAudioCaps.CaptureDeviceMask){
		ui->RBtnAiMicIn->setCheckable(true);
	}
	else{
		ui->RBtnAiMicIn->setCheckable(false);
	}


	if (AvMask(AudioVoiceQualityEnhancement_ANR) & m_CurrentAudioCaps.VoiceQualityMask){
		ui->CBoxAiAnr->setEnabled(true);
	}
	else{
		ui->CBoxAiAnr->setEnabled(false);
	}

	if (AvMask(AudioVoiceQualityEnhancement_AEC) & m_CurrentAudioCaps.VoiceQualityMask){
		ui->CBoxAiAec->setEnabled(true);
	}
	else{
		ui->CBoxAiAec->setEnabled(false);
	}



	//profile
	ui->CBoxAiSoundMode->setCurrentText(AvUiLangsAudioSoundMode(m_CurrentAudioProfile.SoundMode));
	ui->CBoxAiEncodeType->setCurrentText(AvUiLangsAvComp(m_CurrentAudioProfile.Comp));

	ui->HSliderAiVolume->setValue(m_CurrentAudioProfile.VolumeCapture);
	ui->HSliderAoVolume->setValue(m_CurrentAudioProfile.VolumePlay);

	if (m_CurrentAudioProfile.CaptureDevice == AudioCaptureDevice_LINE_IN){
		ui->RBtnAiLineIn->setChecked(true);
		ui->RBtnAiMicIn->setChecked(false);
	}
	else if (m_CurrentAudioProfile.CaptureDevice == AudioCaptureDevice_MIC_IN){
		ui->RBtnAiLineIn->setChecked(false);
		ui->RBtnAiMicIn->setChecked(true);
	}

}
void DlgDeviceSet::FillInVersion()
{
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		//ShowErrorConnectMesageBox();
		return ;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	AdvancedSystemProfile._msg = __MsgFirmwareVersion;
	av_bool bRet = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (bRet != av_true){
		av_error("AdvancedSystemGetProfile __MsgFirmwareVersion error\n");
		return;
	}
	unsigned int &Version = AdvancedSystemProfile.FirmwareInfo.Version;
	QString Qstr;
	Qstr.clear();
	Qstr = Qstr.sprintf("%d.%d.%d.%d", Version>>24&0xff, Version>>16&0xff, Version>>8&0xff, Version&0xff);
	ui->LabSoftWareShow->setText(Qstr);
	
	ui->LabBuildTimeShow->setText(QString(AdvancedSystemProfile.FirmwareInfo.BuildTime));
	ui->LabKernelShow->setText(QString(AdvancedSystemProfile.FirmwareInfo.KernelVersion));
	ui->LabFileSysShow->setText(QString(AdvancedSystemProfile.FirmwareInfo.FilesystemVerion));
	ui->LabProtoShow->setText(QString(AdvancedSystemProfile.FirmwareInfo.ProtoVersion));

	AdvancedSystemProfile._msg = __MsgManufacturerInfo;
	bRet = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (bRet != av_true){
		av_error("AdvancedSystemGetProfile __MsgManufacturerInfo error\n");
		return;
	}
	ui->LabFactoryNameShow->setText(AdvancedSystemProfile.ManufacturerInfo.FacManufacturer);
	struct tm factm;
	time_t facTime = AdvancedSystemProfile.ManufacturerInfo.FacTime;
#if defined(WIN32)
	localtime_s(&factm, &facTime);
#else
	localtime_r((time_t*)&facTime, &factm);
#endif
	Qstr.clear();
	Qstr = Qstr.sprintf("%d-%02d-%02d", factm.tm_year + 1900, factm.tm_mon + 1, factm.tm_mday);
	ui->LabFactoryTimeShow->setText(Qstr);

	ui->LabFactoryNameShow->setText(AdvancedSystemProfile.ManufacturerInfo.FacManufacturer);
	ui->LabProductModelShow->setText(AdvancedSystemProfile.ManufacturerInfo.FacProductionModel);
	ui->LabHardWareShow->setText(AdvancedSystemProfile.ManufacturerInfo.HardWareVersion);
	ui->LabIDNumberShow->setText(AdvancedSystemProfile.ManufacturerInfo.FacProductionSerialNo);

}
void DlgDeviceSet::FillInRecord()
{

	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}




}
void DlgDeviceSet::FillInNetWork()
{
	ui->TBoxNetWork->setCurrentIndex(TboxNetWorkEnum_NetSet);
	FillInNetWorkNetSet();
}
void DlgDeviceSet::FillInAlarm()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	AdvancedSystemProfile._msg = __MsgAlarmProfile;

	C_AdvancedSystemCaps AdvancedSystemCaps;
	AdvancedSystemCaps._msg = __MsgAlarmCaps;

	{
		//ui show item
		C_AlarmCaps &AlarmCaps = AdvancedSystemCaps.AlarmCaps;
		av_bool abRet = pCapture->AdvancedSystemGetCaps(AdvancedSystemCaps);
		if (abRet != av_true){
			AvQDebug("AdvancedSystemGetCaps error  return\n");
			return;
		}

		if (AlarmCaps.AlarmEventMask & AvMask(AlarmEvent_PORT_In)){
			ui->CBoxAlmLEnablePortIn->show();
		}
		else{
			ui->CBoxAlmLEnablePortIn->hide();
		}
		if (AlarmCaps.AlarmEventMask & AvMask(AlarmEvent_VIDEO_MotionDetection)){
			ui->CBoxAlmEnableMotionDetection->show();
			ui->LabAlmMotionLevel->show();
			ui->CBoxAlmMotionLevel->show();
		}
		else{
			ui->CBoxAlmEnableMotionDetection->hide();
			ui->LabAlmMotionLevel->hide();
			ui->CBoxAlmMotionLevel->hide();
		}

		ui->CBoxalmPreRcordSecond->clear();
		for (int i = 0; i < 4; i++){
			ui->CBoxalmPreRcordSecond->addItem(QString::number(i * 5));
		}

		ui->CBoxAlmMotionLevel->clear();
		for (int i = 1; i < 5; i++){
			ui->CBoxAlmMotionLevel->addItem(QString::number(i));
		}
	}


	{
		//profile
		C_AlarmProfile &AlarmProfile = AdvancedSystemProfile.AlarmProfile;
		av_bool abRet = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
		if (abRet != av_true){
			AvQDebug("AdvancedSystemGetCaps error  return\n");
			return;
		}

		if (AlarmProfile.AlarmbEnable == av_true){
			ui->CBoxAlmEnable->setChecked(true);
			on_CBoxAlmEnable_clicked(true);
		}
		else{
			ui->CBoxAlmEnable->setChecked(false);
			on_CBoxAlmEnable_clicked(false);
		}

		if (AlarmProfile.AlarmEventMask & AvMask(AlarmEvent_PORT_In)){
			ui->CBoxAlmLEnablePortIn->setChecked(true);
		}
		else{
			ui->CBoxAlmLEnablePortIn->setChecked(false);
		}
		
		if (AlarmProfile.AlarmEventMask & AvMask(AlarmEvent_VIDEO_MotionDetection)){
			ui->CBoxAlmEnableMotionDetection->setChecked(true);
		}
		else{
			ui->CBoxAlmEnableMotionDetection->setChecked(false);
		}

		if (AlarmProfile.AlarmbLinkageEmail == av_true){
			ui->CBoxAlmLinkageEmail->setChecked(true);
		}
		else{
			ui->CBoxAlmLinkageEmail->setChecked(false);
		}

		if (AlarmProfile.AlarmbLinkageFtp == av_true){
			ui->CBoxAlmLinkageFtp->setChecked(true);
		}
		else{
			ui->CBoxAlmLinkageFtp->setChecked(false);
		}

		if (AlarmProfile.AlarmLinkagePrerecordSec == 0){
			ui->CBoxAlmLinkageRecord->setChecked(false);
		}
		else{
			ui->CBoxAlmLinkageRecord->setChecked(true);
		}

		if (AlarmProfile.AlarmbLinkageSnapshot == av_true){
			ui->CBoxAlmLinkageSnapshort->setChecked(true);
		}
		else{
			ui->CBoxAlmLinkageSnapshort->setChecked(false);
		}

		if (AlarmProfile.AlarmbLinkageBeep == av_true){
			ui->CBoxAlmLinkageBeep->setChecked(true);
		}
		else{
			ui->CBoxAlmLinkageBeep->setChecked(false);
		}

		if (AlarmProfile.AlarmbLinkagePtz == av_true){
			ui->CBoxAlmLinkagePtz->setChecked(true);
		}
		else{
			ui->CBoxAlmLinkagePtz->setChecked(false);
		}

		if (AlarmProfile.AlarmbLinkageAlmOut == av_true){
			ui->CBoxAlmLinkagePortOut->setChecked(true);
		}
		else{
			ui->CBoxAlmLinkagePortOut->setChecked(false);
		}
		ui->CBoxalmPreRcordSecond->setCurrentText(QString::number(AlarmProfile.AlarmLinkagePrerecordSec));
		ui->CBoxAlmMotionLevel->setCurrentText(QString::number(AlarmProfile.AlarmMotionLevel));
		
		memcpy(m_AlarmMotionArea, AlarmProfile.AlarmMotionArea, sizeof(m_AlarmMotionArea));
		memcpy(m_AlarmTimeArea, AlarmProfile.AlarmTimeArea, sizeof(m_AlarmTimeArea));
	}




}
void DlgDeviceSet::FillInUserManager()
{
// 	Capture *pCapture = GetChannelHandle();
// 	if (pCapture == NULL){
// 		return;
// 	}
}
void DlgDeviceSet::FillInLog()
{
// 	Capture *pCapture = GetChannelHandle();
// 	if (pCapture == NULL){
// 		return;
// 	}
}
void DlgDeviceSet::FillInPtz()
{
// 	Capture *pCapture = GetChannelHandle();
// 	if (pCapture == NULL){
// 		return;
// 	}
}


void DlgDeviceSet::FillInNetWorkNetSet()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_NetWorkProfile &NetWorkProfile = AdvancedSystemProfile.NetWorkProfile;
	AdvancedSystemProfile._msg = __MsgNetWorkProfile;
	av_bool ret = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);

	if (ret == av_true){
		ui->CBoxNetWorkIfrName->clear();
		ui->CBoxNetWorkIfrName->addItem(AvUiLangsNetCommT(NetWorkProfile.NetDevice));
		ui->CBoxNetWorkGetMode->clear();
		ui->CBoxNetWorkGetMode->addItem(AvUiLangsNetCommGetMode(NetWorkProfile.NetGetType));
		ui->LiEditNetWorkIpV4Addr->setText(QString(NetWorkProfile.Ipv4));
		
		if (strlen(NetWorkProfile.Ipv6) == 0){
			ui->LiEditNetWorkIpV6Addr->hide();
			ui->LabNetWorkIpV6Addr->hide();
		}
		else{
			ui->LiEditNetWorkIpV6Addr->setText(QString(NetWorkProfile.Ipv6));
		}
		ui->LiEditNetWorkGateWay->setText(QString(NetWorkProfile.GateWay));
		ui->LiEditNetWorkSubMask->setText(QString(NetWorkProfile.SubMask));
		ui->LiEditNetWorkDns1->setText(QString(NetWorkProfile.DnsI));
		ui->LiEditNetWorkDns2->setText(QString(NetWorkProfile.DnsII));
		ui->LiEditNetWorkServicePort->setText(QString::number(NetWorkProfile.ServicePort));
		ui->LiEditNetWorkMacAddr->setText(QString(NetWorkProfile.Mac));
		ui->LiEditNetWorkMacAddr->setEnabled(false);
	}


}
void DlgDeviceSet::FillInNetWorkFtp()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_FtpProfile &FtpProfile = AdvancedSystemProfile.FtpProfile;
	AdvancedSystemProfile._msg = __MsgFtpProfile;
	pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);

	{
		if (FtpProfile.bEnable == av_true){
			ui->CBoxNetWorkFtpEnable->setChecked(true);
// 			ui->LiEditNetWorkFtpServer->setEnabled(true);
// 			ui->LiEditNetWorkFtpPasswd->setEnabled(true);
// 			ui->LiEditNetWorkFtpPort->setEnabled(true);
// 			ui->LiEditNetWorkFtpUserName->setEnabled(true);
		}
		else{
			ui->LiEditNetWorkFtpServer->setEnabled(false);
// 			ui->LiEditNetWorkFtpPasswd->setEnabled(false);
// 			ui->LiEditNetWorkFtpPort->setEnabled(false);
// 			ui->LiEditNetWorkFtpUserName->setEnabled(false);
		}
		ui->LiEditNetWorkFtpServer->setText(FtpProfile.ServerAddress);
		ui->LiEditNetWorkFtpPasswd->setText(FtpProfile.PassWord);
		ui->LiEditNetWorkFtpPort->setText(QString::number(FtpProfile.ServicePort));
		ui->LiEditNetWorkFtpUserName->setText(FtpProfile.UserName);
		ui->LiEditNetWorkFtpRemotePath->setText(FtpProfile.RemotePath);
	}

}
void DlgDeviceSet::FillInNetWorkEmail()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_SmtpProfile &EmailProfile = AdvancedSystemProfile.SmtpProfile;
	AdvancedSystemProfile._msg = __MsgEmailProfile;
	av_bool ret = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (ret != av_true){
		return;
	}

	if (EmailProfile.bEnable == av_true){
		ui->CBoxNetWorkEmailEnable->setChecked(true);
// 		ui->LiEditNetWorkEmailPasswd->setEnabled(true);
// 		ui->LiEditNetWorkEmailPort->setEnabled(true);
// 		ui->LiEditNetWorkEmailReceiver1->setEnabled(true);
// 		ui->LiEditNetWorkEmailReceiver2->setEnabled(true);
// 		ui->LiEditNetWorkEmailSender->setEnabled(true);
// 		ui->LiEditNetWorkEmailSmtpServer->setEnabled(true);
// 		ui->LiEditNetWorkEmailTitle->setEnabled(true);
// 		ui->LiEditNetWorkEmailUserName->setEnabled(true);
// 		ui->CBoxNetWorkEmailEncodeType->setEnabled(true);
	}
	else{

		ui->CBoxNetWorkEmailEnable->setChecked(false);
// 		ui->LiEditNetWorkEmailPasswd->setEnabled(false);
// 		ui->LiEditNetWorkEmailPort->setEnabled(false);
// 		ui->LiEditNetWorkEmailReceiver1->setEnabled(false);
// 		ui->LiEditNetWorkEmailReceiver2->setEnabled(false);
// 		ui->LiEditNetWorkEmailSender->setEnabled(false);
// 		ui->LiEditNetWorkEmailSmtpServer->setEnabled(false);
// 		ui->LiEditNetWorkEmailTitle->setEnabled(false);
// 		ui->LiEditNetWorkEmailUserName->setEnabled(false);
// 		ui->CBoxNetWorkEmailEncodeType->setEnabled(false);
	}

	ui->CBoxNetWorkEmailEncodeType->clear();
	for (int i = EmailEncodeType_None + 1; i < EmailEncodeType_Last; i++){
		ui->CBoxNetWorkEmailEncodeType->addItem(AvUiLangsEmailEncodeType((EmailEncodeType)i));
	}

	ui->LiEditNetWorkEmailPasswd->setText(EmailProfile.PassWord);
	ui->LiEditNetWorkEmailPort->setText(QString::number(EmailProfile.ServicePort));
	ui->LiEditNetWorkEmailReceiver1->setText(EmailProfile.Receiver1);
	ui->LiEditNetWorkEmailReceiver2->setText(EmailProfile.Receiver2);
	ui->LiEditNetWorkEmailSender->setText(EmailProfile.Sender);
	ui->LiEditNetWorkEmailSmtpServer->setText(EmailProfile.ServerAddress);
	ui->LiEditNetWorkEmailTitle->setText(EmailProfile.Title);
	ui->LiEditNetWorkEmailUserName->setText(EmailProfile.UserName);
}
void DlgDeviceSet::FillInNetWorkUpnp()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_UpnpProfile &UpnpProfile = AdvancedSystemProfile.UpnpProfile;
	AdvancedSystemProfile._msg = __MsgUpnpProfile;
	av_bool ret = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (ret != av_true){
		return;
	}

	if (UpnpProfile.bEnableHttpPort == av_true){
		ui->CBoxNetWorkUpnpWeb->setChecked(true);
	}
	else{
		ui->CBoxNetWorkUpnpWeb->setChecked(false);
	}

	if (UpnpProfile.bEnableMoonProtoPort == av_true){
		ui->CBoxNetWorkUpnpProtoPort->setChecked(true);
	}
	else{
		ui->CBoxNetWorkUpnpProtoPort->setChecked(false);
	}

	if (UpnpProfile.bEnableRtspPort == av_true){
		ui->CBoxNetWorkUpnpRtsp->setChecked(true);
	}
	else{
		ui->CBoxNetWorkUpnpRtsp->setChecked(false);
	}
}
void DlgDeviceSet::FillInNetWorkRtsp()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_RtspProfile &RtspProfile = AdvancedSystemProfile.RtspProfile;
	AdvancedSystemProfile._msg = __MsgRtspProfile;
	av_bool ret = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (ret != av_true){
		return;
	}

	if (RtspProfile.bEnable == av_true){
		ui->CBoxNetWorkRtspEnable->setChecked(true);
	}
	else{
		ui->CBoxNetWorkRtspEnable->setChecked(false);
	}

	ui->LiEditNetWorkRtspPort->setText(QString::number(RtspProfile.ServicePort));

}
void DlgDeviceSet::FillInNetWorkDDNS()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_DdnsProfile &DdnsProfile = AdvancedSystemProfile.DdnsProfile;
	AdvancedSystemProfile._msg = __MsgDdnsProfile;
	av_bool ret = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (ret != av_true){
		return;
	}
	
	if (DdnsProfile.bEnable == av_true){
		ui->CBoxNetWorkDdnsEnable->setChecked(true);
	}
	else{
		ui->CBoxNetWorkDdnsEnable->setChecked(false);
	}
	ui->CBoxNetWorkDdnsDdnsType->clear();
	for (int i = DdnsType_None + 1; i < DdnsType_Last; i++){
		ui->CBoxNetWorkDdnsDdnsType->addItem(AvUiLangsDdnsType((DdnsType)i));
	}
	ui->CBoxNetWorkDdnsDdnsType->setCurrentText(AvUiLangsDdnsType(DdnsProfile.Type));

	ui->CBoxNetWorkDdnsUpdateInterval->clear();

	for (int i = RenewalInterval_HalfHour; i < RenewalInterval_SixHour; i++){
		ui->CBoxNetWorkDdnsUpdateInterval->addItem(AvUiLangsRenewalInterval((RenewalInterval)i));
	}
	
	if (DdnsProfile.UpdateInterval == 30 * 60){
		ui->CBoxNetWorkDdnsUpdateInterval->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_HalfHour));
	}
	else if(DdnsProfile.UpdateInterval == 1 * 3600){
		ui->CBoxNetWorkDdnsUpdateInterval->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_OneHour));
	}
	else if (DdnsProfile.UpdateInterval == 3 * 3600){
		ui->CBoxNetWorkDdnsUpdateInterval->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_ThreeHour));
	}
	else{
		ui->CBoxNetWorkDdnsUpdateInterval->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_SixHour));
	}


	ui->LiEditNetWorkDdnsPasswd->setText(QString(DdnsProfile.PassWord));
	ui->LiEditNetWorkDdnsServerAddr->setText(QString(DdnsProfile.ServerAddress));
	ui->LiEditNetWorkDdnsUserName->setText(QString(DdnsProfile.UserName));
	
}
void DlgDeviceSet::FillInNetWorkP2P()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_P2pProfile &P2pProfile = AdvancedSystemProfile.P2pProfile;
	AdvancedSystemProfile._msg = __MsgP2pProfile;
	av_bool ret = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (ret != av_true){
		return;
	}

	if (P2pProfile.bEnable == av_true){
		ui->CBoxNetWorkP2PEnable->setChecked(true);
	}
	else{
		ui->CBoxNetWorkP2PEnable->setChecked(false);
	}

	if (P2pProfile.bUseLinkId == av_true){
		ui->CBoxNetWorkP2PUseLinkID->setChecked(true);
		
		ui->LiEditNetWorkP2PServer->hide();
		ui->LabNetWorkP2PServer->hide();

		ui->LiEditNetWorkP2PPort->hide();
		ui->LabNetWorkP2PPort->hide();

		ui->LiEditNetWorkP2PUsrName->hide();
		ui->LabNetWorkP2PUsrName->hide();

		ui->LiEditNetWorkP2PPasswd->hide();
		ui->LabNetWorkP2PPasswd->hide();

		ui->LiEditNetWorkP2PLinkID->show();
		ui->LabNetWorkP2PLinkID->show();
	}
	else{
		ui->CBoxNetWorkP2PUseLinkID->setChecked(false);

		ui->LiEditNetWorkP2PServer->show();
		ui->LabNetWorkP2PServer->show();

		ui->LiEditNetWorkP2PPort->show();
		ui->LabNetWorkP2PPort->show();

		ui->LiEditNetWorkP2PUsrName->show();
		ui->LabNetWorkP2PUsrName->show();

		ui->LiEditNetWorkP2PPasswd->show();
		ui->LabNetWorkP2PPasswd->show();

		ui->LiEditNetWorkP2PLinkID->hide();
		ui->LabNetWorkP2PLinkID->hide();
	}

	ui->LiEditNetWorkP2PServer->setText(QString(P2pProfile.ServerAddress));
	ui->LiEditNetWorkP2PPort->setText(QString::number(P2pProfile.ServicePort));
	ui->LiEditNetWorkP2PUsrName->setText(QString(P2pProfile.UserName));
	ui->LiEditNetWorkP2PPasswd->setText(QString(P2pProfile.PassWord));
	ui->LiEditNetWorkP2PLinkID->setText(QString(P2pProfile.LinkId));

}

void DlgDeviceSet::FillInNetWorkRTMP()
{

	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_RtmpProfile &RtmpProfile = AdvancedSystemProfile.RtmpProfile;
	C_AdvancedSystemCaps AdvancedSystemCaps;
	C_DeviceCaps  &DeviceCaps = AdvancedSystemCaps.DspCaps;
	AdvancedSystemCaps._msg = __MsgDspCaps;
	pCapture->AdvancedSystemGetCaps(AdvancedSystemCaps);
	m_DeviceMaxChannels = DeviceCaps.nMaxDecodeChannel + DeviceCaps.nMaxEncodeChannel;
	ui->CBoxNetWorkRTMPChannel->clear();
	QString  String;

	for (int i = 0; i < m_DeviceMaxChannels; i++){
		String.clear();
		String += tr("CHN");
		String += QString::number(i + 1);
		ui->CBoxNetWorkRTMPChannel->addItem(String);
	}

	AdvancedSystemProfile._msg = __MsgRtmpProfile;
	pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);

	m_RtmpNodeProfileList = RtmpProfile.RtmpNodeProfileList;
	DrawRtmpConfigTableView();

}

void DlgDeviceSet::DrawRtmpConfigTableView()
{

	std::list<C_RtmpNodeProfile>::iterator iList;
	int i = 0;
	QString Strings;
	m_RtmpListStandardItemModel->removeRows(0, m_RtmpListStandardItemModel->rowCount());
	for (iList = m_RtmpNodeProfileList.begin(); iList != m_RtmpNodeProfileList.end(); iList++, i++){
		m_RtmpListStandardItemModel->setItem(i, D_RTMPITEMMODEL_HEADSECTION_NO, new QStandardItem(QString::number(i + 1)));
		m_RtmpListStandardItemModel->item(i, D_RTMPITEMMODEL_HEADSECTION_NO)->setCheckable(true);
		Strings.clear();
		Strings = Strings.sprintf("%s/%s", iList->PushServer, iList->PushStrings);
		m_RtmpListStandardItemModel->setItem(i, D_RTMPITEMMODEL_HEADSECTION_RTMPURL, new QStandardItem(Strings));
		m_RtmpListStandardItemModel->setItem(i, D_RTMPITEMMODEL_HEADSECTION_CHN, new QStandardItem(QString::number(iList->PushChannel)));
		Strings.clear();
		if (iList->PushStream == CHL_MAIN_T){
			Strings = QString(tr("Main"));
		}
		else{
			Strings = QString(tr("Sub"));
		}
		m_RtmpListStandardItemModel->setItem(i, D_RTMPITEMMODEL_HEADSECTION_SLAVE, new QStandardItem(Strings));
		Strings.clear();
		if (av_true == iList->bEnableAudio){
			Strings = QString(tr("WithAudio"));
		}
		else{
			Strings = QString(tr("WithoutAudio"));
		}
		m_RtmpListStandardItemModel->setItem(i, D_RTMPITEMMODEL_HEADSECTION_AUDIO, new QStandardItem(Strings));
		QIcon icon(":/image/info.png");
		m_RtmpListStandardItemModel->setItem(i, D_RTMPITEMMODEL_HEADSECTION_STATUS, new QStandardItem(icon, QString("")));
	}
}
void DlgDeviceSet::FillInNetWorkNTP()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_NtpProfile &NtpProfile = AdvancedSystemProfile.NtpProfile;
	AdvancedSystemProfile._msg = __MsgNtpProfile;
	av_bool ret = pCapture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (ret != av_true){
		return;
	}
	
	ui->CBoxNetWorkTimetimeZone->clear();
	for (int i = TimeZone_UTC_None + 1; i < TimeZone_UTC_Last; i++){
		ui->CBoxNetWorkTimetimeZone->addItem(AvUiLangsTimeZone((TimeZone)i));
	}


	ui->CBoxNetWorkTimeUpdateTime->clear();

	for (int i = RenewalInterval_HalfHour; i < RenewalInterval_SixHour; i++){
		ui->CBoxNetWorkTimeUpdateTime->addItem(AvUiLangsRenewalInterval((RenewalInterval)i));
	}

	if (NtpProfile.UpdateInterval == 30 * 60){
		ui->CBoxNetWorkTimeUpdateTime->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_HalfHour));
	}
	else if (NtpProfile.UpdateInterval == 1 * 3600){
		ui->CBoxNetWorkTimeUpdateTime->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_OneHour));
	}
	else if (NtpProfile.UpdateInterval == 3 * 3600){
		ui->CBoxNetWorkTimeUpdateTime->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_ThreeHour));
	}
	else{
		ui->CBoxNetWorkTimeUpdateTime->setCurrentText(AvUiLangsRenewalInterval(RenewalInterval_SixHour));
	}
	ui->CBoxNetWorkTimeServer->clear();
	ui->CBoxNetWorkTimeServer->addItem(QString("time.windows.com"));
	ui->CBoxNetWorkTimeServer->addItem(QString("time.nist.gov"));
	ui->CBoxNetWorkTimeServer->addItem(QString("s2g.time.edu.cn"));
	
	ui->CBoxNetWorkTimeServer->setCurrentText(QString(NtpProfile.ServerAddress));
}

void DlgDeviceSet::SubmitCapture()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_CaptureProfile CaptureProfile;
	for (int i = IrCutMode_NONE; i < IrCutMode_LAST; i++){
		if (ui->CBoxIrCut->currentText() == AvUiLangsIrCutMode((IrCutMode)i)){
			CaptureProfile.IrCut = (IrCutMode)i;
			//ircut timer stime etime;
			{
				if (CaptureProfile.IrCut == IrCutMode_TIMER){
					CaptureProfile.IrCutTimer.Start = 6 * 3600;
					CaptureProfile.IrCutTimer.End = 18 * 3600;
				}
				else {
					CaptureProfile.IrCutTimer.Start = 6 * 3600;
					CaptureProfile.IrCutTimer.End = 18 * 3600;
				}
			}
			break;
		}
	}
	

	for (int i = AntiFlckerMode_NONE; i < AntiFlckerMode_LAST; i++){
		if (ui->CBoxAnitFlicker->currentText() == AvUiLangsAntiFlckerMode((AntiFlckerMode)i)){
			CaptureProfile.AntiFlcker = (AntiFlckerMode)i;
			CaptureProfile.AntiFlckerValue = ui->LiEditAnitFlickerCustom->text().toInt();
			break;
		}
	}

	for (int i = ExposureMode_NONE; i < ExposureMode_LAST; i++){
		if (ui->CBoxExposureMode->currentText() == AvUiLangsExposureMode((ExposureMode)i)){
			CaptureProfile.Exposure = (ExposureMode)i;
			CaptureProfile.ExposureValue = ui->LiEditExposureModeCustom->text().toInt();
			break;
		}
	}

	for (int i = WhiteBalanceMode_NONE; i < WhiteBalanceMode_LAST; i++){
		if (ui->CBoxWhitBalance->currentText() == AvUiLangsWhiteBalanceMode((WhiteBalanceMode)i)){
			CaptureProfile.WhiteBalance = (WhiteBalanceMode)i;
			CaptureProfile.WhiteBalanceValue = ui->LiEditWhitBalanceCustom->text().toInt();
			break;
		}
	}

	CaptureProfile.MirrorMaskValue = 0;
	if (ui->CBoxHorizontal->isChecked() == true){
		CaptureProfile.MirrorMaskValue |= AvMask(MirrorMode_HOR);
	}
	if (ui->CBoxVertical->isChecked() == true){
		CaptureProfile.MirrorMaskValue |= AvMask(MirrorMode_VER);
	}

	for (int i = MirrorMode_Rotate_000; i <= MirrorMode_Rotate_270; i++){
		if (ui->CBoxRotateMode->currentText() == AvUiLangsMirrorMode((MirrorMode)i)){
			CaptureProfile.MirrorMaskValue |= AvMask(i);
			break;
		}
	}

	if (AvUiLangsOpenClose(av_true) == ui->CBoxCvbs->currentText()){
		CaptureProfile.bOpenCvbs = av_true;
	}
	else{
		CaptureProfile.bOpenCvbs = av_false;
	}

	for (int i = WdrMode_None; i < WdrMode_Last; i++){
		if (ui->CBoxWdr->currentText() == AvUiLangsWdrMode((WdrMode)i)){
			CaptureProfile.Wdr = (WdrMode)i;
		}
	}
	CaptureProfile.WdrValue = ui->LiEditWdrValue->text().toInt();

	for (int i = ShutterMode_None; i < ShutterMode_Last; i++){
		if (ui->CBoxShutter->currentText() == AvUiLangsShutterMode((ShutterMode)i)){
			CaptureProfile.Shutter = (ShutterMode)i;
		}
	}
	CaptureProfile.ShutterMax = ui->LiEditShutter->text().toInt();

	av_bool bRet = av_false;
	bRet = pCapture->CaptureSetProfile(CaptureProfile);
	if (bRet == av_true){
		ShowSucceedSetArgs();
	}
	else{
		ShowErrorSetArgs();
	}



}

void DlgDeviceSet::SubmitEncode()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_EnCodeProfile EnCodeProfile_M;
	C_EnCodeProfile EnCodeProfile_S;

	QString IndexString = ui->CBoxMEnFormat->currentText();

	for (int i = AvComp_H264; i < AvComp_LAST; i++){
		if (IndexString == AvUiLangsAvComp((AvComp)i)){
			EnCodeProfile_M.Comp = (AvComp)i;
			EnCodeProfile_S.Comp = (AvComp)i;
			break;
		}
	}

	IndexString = ui->CBoxMImageSize->currentText();
	for (int i = CaptureSize_Self; i < CaptureSize_LAST; i++){
		if (IndexString == AvUiLangsCaptureSize((CaptureSize)i)){
			EnCodeProfile_M.ImageSize = (CaptureSize)i;
			EnCodeProfile_M.ImageSelfHeigh = 128;
			EnCodeProfile_M.ImageSelfWidth = 128;
			break;
		}
	}

	IndexString = ui->CBoxMFrameRate->currentText();
	EnCodeProfile_M.FrameRate = IndexString.toInt();

	IndexString = ui->CBoxMBitRateCtrl->currentText();
	for (int i = BitRateCtrl_NONE; i < BitRateCtrl_LAST; i++){
		if (IndexString == AvUiLangsBitRateCtrl((BitRateCtrl)i)){
			EnCodeProfile_M.BitRateCtl = (BitRateCtrl)i;
			break;
		}
	}
	if (IndexString == AvUiLangsBitRateCtrl(BitRateCtrl_CBR)){
		EnCodeProfile_M.Qlevel = 4;
	}
	else{
		EnCodeProfile_M.Qlevel = ui->CBoxMBitRateQlevel->currentText().toInt();
	}
	EnCodeProfile_M.BitRateValue = ui->LiEditMBitRateQlevel->text().toInt();
	EnCodeProfile_M.Gop = ui->LiEditMIFrameSpan->text().toInt();



	///sub encodeprofile
	IndexString = ui->CBoxSImageSize->currentText();
	for (int i = CaptureSize_Self; i < CaptureSize_LAST; i++){
		if (IndexString == AvUiLangsCaptureSize((CaptureSize)i)){
			EnCodeProfile_S.ImageSize = (CaptureSize)i;
			EnCodeProfile_S.ImageSelfHeigh = 128;
			EnCodeProfile_S.ImageSelfWidth = 128;
			break;
		}
	}

	IndexString = ui->CBoxSFrameRate->currentText();
	EnCodeProfile_S.FrameRate = IndexString.toInt();

	IndexString = ui->CBoxSBitRateCtrl->currentText();
	for (int i = BitRateCtrl_NONE; i < BitRateCtrl_LAST; i++){
		if (IndexString == AvUiLangsBitRateCtrl((BitRateCtrl)i)){
			EnCodeProfile_S.BitRateCtl = (BitRateCtrl)i;
			break;
		}
	}
	if (IndexString == AvUiLangsBitRateCtrl(BitRateCtrl_CBR)){
		EnCodeProfile_S.Qlevel = 4;
	}
	else{
		EnCodeProfile_S.Qlevel = ui->CBoxMBitRateQlevel->currentText().toInt();
	}

	EnCodeProfile_S.BitRateValue = ui->LiEditSBitRateQlevel->text().toInt();
	EnCodeProfile_S.Gop = ui->LiEditSIFrameSpan->text().toInt();


	av_bool bRet = av_false;
	bRet = pCapture->EncodeSetProfile(CHL_MAIN_T, EnCodeProfile_M);
	if (bRet != av_true){
		ShowErrorSetArgs();
		return;
	}
	bRet = pCapture->EncodeSetProfile(CHL_SUB1_T, EnCodeProfile_S);
	if (bRet != av_true){
		ShowErrorSetArgs();
		return;
	}
	else{
		ShowSucceedSetArgs();
	}


}
void DlgDeviceSet::SubmitAudio()
{
	C_AudioProfile AudioProfile;
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	QString currentText;
	currentText = ui->CBoxAiSoundMode->currentText();
	for (int i = AudioSoundMode_NONE; i < AudioSoundMode_LAST; i++){
		if (currentText == AvUiLangsAudioSoundMode((AudioSoundMode)i)){
			AudioProfile.SoundMode = (AudioSoundMode)i;
			break;
		}
	}

	currentText = ui->CBoxAiEncodeType->currentText();
	for (int i = AvComp_H264; i < AvComp_LAST; i++){
		if (currentText == AvUiLangsAvComp((AvComp)i)){
			AudioProfile.Comp = (AvComp)i;
			break;
		}
	}

	currentText = ui->CBoxAiSampleRate->currentText();
	for (int i = AudioSampleRate_NONE; i < AudioSampleRate_LAST; i++){
		if (currentText == AvUiLangsAudioSampleRate((AudioSampleRate)i)){
			AudioProfile.SampleRate = (AudioSampleRate)i;
			break;
		}
	}

	currentText = ui->CBoxAiSampleBits->currentText();
	for (int i = AudioSampleBits_NONE; i < AudioSampleBits_LAST; i++){
		if (currentText == AvUiLangsAudioSampleBits((AudioSampleBits)i)){
			AudioProfile.SampleBits = (AudioSampleBits)i;
			break;
		}
	}
	if (ui->RBtnAiLineIn->isChecked() == true){
		AudioProfile.CaptureDevice = AudioCaptureDevice_LINE_IN;
	}
	else{
		AudioProfile.CaptureDevice = AudioCaptureDevice_MIC_IN;
	}
	AudioProfile.VolumeCapture = ui->HSliderAiVolume->value();
	AudioProfile.VolumePlay = ui->HSliderAoVolume->value();


	AudioProfile.VoiceQualityMask = 0x00;
	if (ui->CBoxAiAnr->isChecked() == true){
		AudioProfile.VoiceQualityMask |= AvMask(AudioVoiceQualityEnhancement_ANR);
	}
	if (ui->CBoxAiAec->isChecked() == true){
		AudioProfile.VoiceQualityMask |= AvMask(AudioVoiceQualityEnhancement_AEC);
	}


	av_bool bRet = pCapture->AudioSetProfile(AudioProfile);
	if (bRet != av_true){
		ShowErrorSetArgs();
		return;
	}
	else{
		ShowSucceedSetArgs();
	}
}

void DlgDeviceSet::SubmitVersion()
{

}

void DlgDeviceSet::SubmitRecord()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
}

void DlgDeviceSet::SubmitNetWork()
{
	int TabToolsIndex = ui->TBoxNetWork->currentIndex();
	switch (TabToolsIndex)
	{
	case TboxNetWorkEnum_NetSet:
		SubmitNetWorkNetSet();
		break;
	case TboxNetWorkEnum_Ftp:
		SubmitNetWorkFtp();
		break;
	case TboxNetWorkEnum_Email:
		SubmitNetWorkEmail();
		break;
	case TboxNetWorkEnum_Upnp:
		SubmitNetWorkUpnp();
		break;
	case TboxNetWorkEnum_Rtsp:
		SubmitNetWorkRtsp();
		break;
	case TboxNetWorkEnum_Ddns:
		SubmitNetWorkDDNS();
		break;
	case TboxNetWorkEnum_P2p:
		SubmitNetWorkP2P();
		break;
	case TboxNetWorkEnum_Rtmp:
		SubmitNetWorkRTMP();
		break;
	case TboxNetWorkEnum_Ntp:
		SubmitNetWorkNTP();
		break;
	default:
		break;
	}
}

void DlgDeviceSet::SubmitAlarm()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	//memset(&AdvancedSystemProfile, 0x00, sizeof(C_AdvancedSystemProfile));
	AdvancedSystemProfile._msg = __MsgAlarmProfile;
	C_AlarmProfile &AlarmProfile = AdvancedSystemProfile.AlarmProfile;
	if (true == ui->CBoxAlmEnable->isChecked()){
		AlarmProfile.AlarmbEnable = av_true;
	}
	else{
		AlarmProfile.AlarmbEnable = av_false;
	}

	if (true == ui->CBoxAlmLEnablePortIn->isChecked()){
		AlarmProfile.AlarmEventMask |= AvMask(AlarmEvent_PORT_In);
	}

	if (true == ui->CBoxAlmEnableMotionDetection->isChecked()){
		AlarmProfile.AlarmEventMask |= AvMask(AlarmEvent_VIDEO_MotionDetection);
	}

	if (true == ui->CBoxAlmLinkageEmail->isChecked()){
		AlarmProfile.AlarmbLinkageEmail = av_true;
	}
	else{
		AlarmProfile.AlarmbLinkageEmail = av_false;
	}

	if (true == ui->CBoxAlmLinkageFtp->isChecked()){
		AlarmProfile.AlarmbLinkageFtp = av_true;
	}
	else{
		AlarmProfile.AlarmbLinkageFtp = av_false;
	}

	if (true == ui->CBoxAlmLinkageRecord->isChecked()){
		AlarmProfile.AlarmbLinkageRecord = av_true;
	}
	else{
		AlarmProfile.AlarmbLinkageRecord = av_false;
	}

	if (true == ui->CBoxAlmLinkageSnapshort->isChecked()){
		AlarmProfile.AlarmbLinkageSnapshot = av_true;
	}
	else{
		AlarmProfile.AlarmbLinkageSnapshot = av_false;
	}

	if (true == ui->CBoxAlmLinkageBeep->isChecked()){
		AlarmProfile.AlarmbLinkageBeep = av_true;
	}
	else{
		AlarmProfile.AlarmbLinkageBeep = av_false;
	}

	if (true == ui->CBoxAlmLinkagePtz->isChecked()){
		AlarmProfile.AlarmbLinkagePtz = av_true;
	}
	else{
		AlarmProfile.AlarmbLinkagePtz = av_false;
	}
	if (true == ui->CBoxAlmLinkagePortOut->isChecked()){
		AlarmProfile.AlarmbLinkageAlmOut = av_true;
	}
	else{
		AlarmProfile.AlarmbLinkageAlmOut = av_false;
	}
	
	AlarmProfile.AlarmLinkagePrerecordSec = atoi(ui->CBoxalmPreRcordSecond->currentText().toStdString().c_str());
	AlarmProfile.AlarmMotionLevel = atoi(ui->CBoxAlmMotionLevel->currentText().toStdString().c_str());
	memcpy(AlarmProfile.AlarmTimeArea, m_AlarmTimeArea, sizeof(m_AlarmTimeArea));
	memcpy(AlarmProfile.AlarmMotionArea, m_AlarmMotionArea, sizeof(m_AlarmMotionArea));

	av_bool abRet = pCapture->AdvancedSystemSetProfile(AdvancedSystemProfile);
	if (abRet == av_true){

	}
	else{

	}



}

void DlgDeviceSet::SubmitUserManager()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
}

void DlgDeviceSet::SubmitLog()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
}

void DlgDeviceSet::SubmitPtz()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
}



void DlgDeviceSet::SubmitNetWorkNetSet()
{

}
void DlgDeviceSet::SubmitNetWorkFtp()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
	C_AdvancedSystemProfile AdvancedSystemProfile;
	AdvancedSystemProfile._msg = __MsgFtpProfile;
	C_FtpProfile &FtpProfile = AdvancedSystemProfile.FtpProfile;

	if (true == ui->CBoxNetWorkFtpEnable->isChecked()){
		FtpProfile.bEnable = av_true;
	}
	else{
		FtpProfile.bEnable = av_false;
	}

	sprintf(FtpProfile.ServerAddress, "%s", ui->LiEditNetWorkFtpServer->text().toStdString().c_str());
	sprintf(FtpProfile.UserName, "%s", ui->LiEditNetWorkFtpUserName->text().toStdString().c_str());
	sprintf(FtpProfile.PassWord, "%s", ui->LiEditNetWorkFtpPasswd->text().toStdString().c_str());
	sprintf(FtpProfile.RemotePath, "%s", ui->LiEditNetWorkFtpRemotePath->text().toStdString().c_str());
	FtpProfile.ServicePort = atoi(ui->LiEditNetWorkFtpPort->text().toStdString().c_str());

	pCapture->AdvancedSystemSetProfile(AdvancedSystemProfile);

}
void DlgDeviceSet::SubmitNetWorkEmail()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}
	C_AdvancedSystemProfile AdvancedSystemProfile;
	AdvancedSystemProfile._msg = __MsgFtpProfile;
	C_SmtpProfile &EmailProfile = AdvancedSystemProfile.SmtpProfile;

	if (ui->CBoxNetWorkEmailEnable->isChecked() == true){
		EmailProfile.bEnable = av_true;
	}
	else{
		EmailProfile.bEnable = av_false;
	}

	for (int i = EmailEncodeType_None; i < EmailEncodeType_Last; i++){
		if (AvUiLangsEmailEncodeType((EmailEncodeType)i) == ui->CBoxNetWorkEmailEncodeType->currentText()){
			EmailProfile.EncodeType = (EmailEncodeType)i;
			break;
		}
	}
	EmailProfile.ServicePort = atoi(ui->LiEditNetWorkEmailPort->text().toStdString().c_str());
	sprintf(EmailProfile.Receiver1, ui->LiEditNetWorkEmailReceiver1->text().toStdString().c_str());
	sprintf(EmailProfile.Receiver2, ui->LiEditNetWorkEmailReceiver2->text().toStdString().c_str());
	sprintf(EmailProfile.Sender, ui->LiEditNetWorkEmailSender->text().toStdString().c_str());
	sprintf(EmailProfile.ServerAddress, ui->LiEditNetWorkEmailSmtpServer->text().toStdString().c_str());
	sprintf(EmailProfile.Title, ui->LiEditNetWorkEmailTitle->text().toStdString().c_str());
	sprintf(EmailProfile.UserName, ui->LiEditNetWorkEmailUserName->text().toStdString().c_str());
	sprintf(EmailProfile.PassWord, ui->LiEditNetWorkEmailPasswd->text().toStdString().c_str());

	pCapture->AdvancedSystemSetProfile(AdvancedSystemProfile);
}

void DlgDeviceSet::SubmitNetWorkUpnp()
{

}
void DlgDeviceSet::SubmitNetWorkRtsp()
{

}
void DlgDeviceSet::SubmitNetWorkDDNS()
{

}
void DlgDeviceSet::SubmitNetWorkP2P()
{

}
void DlgDeviceSet::SubmitNetWorkRTMP()
{
	Capture *pCapture = GetChannelHandle();
	if (pCapture == NULL){
		return;
	}

	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_RtmpProfile &RtmpProfile = AdvancedSystemProfile.RtmpProfile;
	AdvancedSystemProfile._msg = __MsgRtmpProfile;
	RtmpProfile.RtmpNodeProfileList = m_RtmpNodeProfileList;
	av_bool bRet = pCapture->AdvancedSystemSetProfile(AdvancedSystemProfile);
	if (bRet == av_true){

		CAvUiComm::ShowNotificationBox(QString(tr("Set Rtmp OK")));
	}
	else{
		AvQDebug("AdvancedSystemSetProfile rtmp error\n");
	}
}
void DlgDeviceSet::SubmitNetWorkNTP()
{

}

void DlgDeviceSet::ShowErrorConnectMesageBox()
{
	//CAvUiComm::ShowMessageBoxError(QString(tr("This Channel does't Connect!")));
	CAvUiComm::ShowNotificationBox(QString(tr("This Channel does't Connect!")));
}
void DlgDeviceSet::ShowErrorGetArgs()
{
	//CAvUiComm::ShowMessageBoxError(QString(tr("Failed to get parameters!")));
	CAvUiComm::ShowNotificationBox(QString(tr("Failed to get parameters!")));
}
void DlgDeviceSet::ShowSucceedSetArgs()
{
	//CAvUiComm::ShowMessageBoxInfo(QString(tr("set parameters succeed!")));
	CAvUiComm::ShowNotificationBox(QString(tr("set parameters succeed!")));
}
void DlgDeviceSet::ShowErrorSetArgs()
{
	//CAvUiComm::ShowMessageBoxError(QString(tr("Failed to set parameters!")));
	CAvUiComm::ShowNotificationBox(QString(tr("Failed to set parameters!")));
}

DlgDeviceSet::~DlgDeviceSet()
{
    delete ui;
}

Capture * DlgDeviceSet::GetChannelHandle()
{
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
		return NULL;
	}
	return pCapture;
}

void DlgDeviceSet::SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged(int min, int max)
{
	AvQDebug("SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged min = %d, max = %d\n", min, max);
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}

void DlgDeviceSet::resizeEvent(QResizeEvent * event)
{
	ResizeRecord();
	ResizeNetWorkRtmp();
	AvQDebug("DlgDeviceSet::resizeEvent\n");
}

void DlgDeviceSet::showEvent(QShowEvent *event)
{
	ui->TabWidgetDeviceSet->setCurrentIndex(TabWidgetDeviceSet_Version);
	FixDlgUiVersion();
	FillInVersion();
}

void DlgDeviceSet::on_BtnClose_clicked()
{
	hide();
}

void DlgDeviceSet::on_TBoxNetWork_objectNameChanged(const QString &objectName)
{
    printf("TBoxNetWork_objectNameChanged %s\n", objectName.toStdString().c_str());
}

void DlgDeviceSet::on_TBoxNetWork_currentChanged(int index)
{
	printf("TBoxNetWork_currentChanged %d\n", index);
	ui->StWidgetNetWork->setCurrentIndex(index);

	switch (index)
	{
	case 	TboxNetWorkEnum_NetSet:
		FillInNetWorkNetSet();
		break;
	case 		TboxNetWorkEnum_Ftp:
		FillInNetWorkFtp();
		break;
	case 		TboxNetWorkEnum_Email:
		FillInNetWorkEmail();
		break;
	case 		TboxNetWorkEnum_Upnp:
		FillInNetWorkUpnp();
		break;
	case 		TboxNetWorkEnum_Rtsp:
		FillInNetWorkRtsp();
		break;
	case 		TboxNetWorkEnum_Ddns:
		FillInNetWorkDDNS();
		break;
	case 		TboxNetWorkEnum_P2p:
		FillInNetWorkP2P();
		break;
	case 		TboxNetWorkEnum_Rtmp:
		FillInNetWorkRTMP();
		break;
	case 		TboxNetWorkEnum_Ntp:
		FillInNetWorkNTP();
		break;

	default:
		break;
	}

}

void DlgDeviceSet::on_BtnNetWorkRTMPAdd_clicked()
{
	QString RtmpAddr = ui->LiEditNetWorkRTMPAddr->text();
	QString RtmpString = ui->LiEditNetWorkRTMPString->text();
	bool bWithAudio = ui->CBoxNetWorkRTMPAudio->isChecked();
	int  Channel = ui->CBoxNetWorkRTMPChannel->currentIndex();
	int	 Stream = ui->CBoxNetWorkRTMPStream->currentIndex();
	{//
		CAvUiConfigIni::Instance()->InisetValue(QString(D_RTMP_INI_SECTION), QString(D_RTMP_INI_KEY_ADDR), RtmpAddr);
		CAvUiConfigIni::Instance()->InisetValue(QString(D_RTMP_INI_SECTION), QString(D_RTMP_INI_KEY_STRING), RtmpString);
		CAvUiConfigIni::Instance()->InisetValue(QString(D_RTMP_INI_SECTION), QString(D_RTMP_INI_KEY_WITH_AUDIO), bWithAudio);
	}

	
	C_RtmpNodeProfile NodeProfile;
	NodeProfile.bEnable = av_true;
	NodeProfile.bEnableAudio = bWithAudio == true ? av_true : av_false;
	NodeProfile.PushChannel = Channel;
	NodeProfile.PushStream = Stream;
	sprintf(NodeProfile.PushServer, "%s", RtmpAddr.toStdString().c_str());
	sprintf(NodeProfile.PushStrings, "%s", RtmpString.toStdString().c_str());
	NodeProfile.RtmpbPushStreamAdaptiveNetSpeed = av_true;

	m_RtmpNodeProfileList.push_back(NodeProfile);
	DrawRtmpConfigTableView();
}

void DlgDeviceSet::on_BtnNetWorkRTMPDel_clicked()
{
	std::list<C_RtmpNodeProfile>::iterator iList;
	int i = 0;
	for (iList = m_RtmpNodeProfileList.begin(); iList != m_RtmpNodeProfileList.end(); i++){
		if (m_RtmpListStandardItemModel->item(i, D_RTMPITEMMODEL_HEADSECTION_NO)->checkState() == Qt::Checked){
			iList = m_RtmpNodeProfileList.erase(iList);
			continue;
		}
		iList++;
	}

	DrawRtmpConfigTableView();
}

void DlgDeviceSet::on_GBoxNetWork_RTMP_objectNameChanged(const QString &objectName)
{
	printf("TBoxNetWork_currentChanged %s\n", objectName.toStdString().c_str());
}

void DlgDeviceSet::on_StWidgetNetWork_currentChanged(int arg1)
{
	printf("StWidgetNetWork_currentChanged %d\n", arg1);
}

void DlgDeviceSet::on_StWidgetNetWork_objectNameChanged(const QString &objectName)
{
	printf("StWidgetNetWork_objectNameChanged %s\n", objectName.toStdString().c_str());
}

void DlgDeviceSet::on_TabWidgetDeviceSet_currentChanged(int index)
{
	av_msg("TabWidgetDeviceSet change %d\n", index);
	switch (index)
	{
	case TabWidgetDeviceSet_Capture:
		FillInCapture();
		break;
	case TabWidgetDeviceSet_Encode:
		FillInEncode();
		break;
	case TabWidgetDeviceSet_Audio:
		FillInAudio();
		break;
	case TabWidgetDeviceSet_Version:
		FillInVersion();
		break;
	case TabWidgetDeviceSet_Record:
		FillInRecord();
		break;
	case TabWidgetDeviceSet_NetWork:
		FillInNetWork();
		break;
	case TabWidgetDeviceSet_Alarm:
		FillInAlarm();
		break;
	case TabWidgetDeviceSet_UserManager:
		FillInUserManager();
		break;
	case TabWidgetDeviceSet_Log:
		FillInLog();
		break;
	case TabWidgetDeviceSet_Ptz:
		FillInPtz();
		break;
	default:
		break;
	}
}

void DlgDeviceSet::on_CBoxIrCut_currentIndexChanged(const QString &arg1)
{
	if (arg1 == AvUiLangsIrCutMode(IrCutMode_TIMER)){
		ui->LabIrCutStime->show();
		ui->LabIrCutEtime->show();
		ui->TEditIrCutEtime->show();
		ui->TEditIrCutStime->show();
	}
	else{
		ui->LabIrCutStime->hide();
		ui->LabIrCutEtime->hide();
		ui->TEditIrCutEtime->hide();
		ui->TEditIrCutStime->hide();
	}
}

void DlgDeviceSet::on_CBoxAnitFlicker_currentIndexChanged(const QString &arg1)
{
	if (arg1 == AvUiLangsAntiFlckerMode(AntiFlckerMode_THEATER_60HZ)){
		ui->LabAnitFlickerCustomPlan->show();
		ui->LiEditAnitFlickerCustom->show();
	}
	else{
		ui->LabAnitFlickerCustomPlan->hide();
		ui->LiEditAnitFlickerCustom->hide();
	}
}

void DlgDeviceSet::on_CBoxExposureMode_currentIndexChanged(const QString &arg1)
{
	if (arg1 == AvUiLangsExposureMode(ExposureMode_MANUAL)){
		ui->LabExposureModePlan->show();
		ui->LiEditExposureModeCustom->show();
	}
	else{
		ui->LabExposureModePlan->hide();
		ui->LiEditExposureModeCustom->hide();
	}
}

void DlgDeviceSet::on_CBoxWhitBalance_currentIndexChanged(const QString &arg1)
{
	if (arg1 == AvUiLangsWhiteBalanceMode(WhiteBalanceMode_MANUAL)){
		ui->LabWhitBalanceCustomPlan->show();
		ui->LiEditWhitBalanceCustom->show();
	}
	else{
		ui->LabWhitBalanceCustomPlan->hide();
		ui->LiEditWhitBalanceCustom->hide();
	}
}

void DlgDeviceSet::on_BtnSubmit_clicked()
{
	int index = ui->TabWidgetDeviceSet->currentIndex();
	av_msg(" Submit TabWidgetDeviceSet %d\n", index);
	switch (index)
	{
	case TabWidgetDeviceSet_Capture:
		SubmitCapture();
		break;
	case TabWidgetDeviceSet_Encode:
		SubmitEncode();
		break;
	case TabWidgetDeviceSet_Audio:
		SubmitAudio();
		break;
	case TabWidgetDeviceSet_Version:
		SubmitVersion();
		break;
	case TabWidgetDeviceSet_Record:
		SubmitRecord();
		break;
	case TabWidgetDeviceSet_NetWork:
		SubmitNetWork();
		break;
	case TabWidgetDeviceSet_Alarm:
		SubmitAlarm();
		break;
	case TabWidgetDeviceSet_UserManager:
		SubmitUserManager();
		break;
	case TabWidgetDeviceSet_Log:
		SubmitLog();
		break;
	case TabWidgetDeviceSet_Ptz:
		SubmitPtz();
		break;
	default:
		break;
	}
}

void DlgDeviceSet::on_BtnUpdate_clicked()
{
	int index = ui->TabWidgetDeviceSet->currentIndex();
	av_msg("Update TabWidgetDeviceSet %d\n", index);
	switch (index)
	{
	case TabWidgetDeviceSet_Capture:
		FillInCapture();
		break;
	case TabWidgetDeviceSet_Encode:
		FillInEncode();
		break;
	case TabWidgetDeviceSet_Audio:
		FillInAudio();
		break;
	case TabWidgetDeviceSet_Version:
		FillInVersion();
		break;
	case TabWidgetDeviceSet_Record:
		FillInRecord();
		break;
	case TabWidgetDeviceSet_NetWork:
		FillInNetWork();
		break;
	case TabWidgetDeviceSet_Alarm:
		FillInAlarm();
		break;
	case TabWidgetDeviceSet_UserManager:
		FillInUserManager();
		break;
	case TabWidgetDeviceSet_Log:
		FillInLog();
		break;
	case TabWidgetDeviceSet_Ptz:
		FillInPtz();
		break;
	default:
		break;
	}
}

void DlgDeviceSet::on_CBoxSettingChannel_currentIndexChanged(int channel)
{
	int index = ui->CBoxSettingChannel->currentIndex();
	av_msg("Update CBoxSettingChannel_currentIndexChanged %d\n", index);
	if (index < 0 || m_UiInitedOver == false) return;
	int tabelIndex = ui->TabWidgetDeviceSet->currentIndex();
	switch (tabelIndex)
	{
	case TabWidgetDeviceSet_Capture:
		FillInCapture();
		break;
	case TabWidgetDeviceSet_Encode:
		FillInEncode();
		break;
	case TabWidgetDeviceSet_Audio:
		FillInAudio();
		break;
	case TabWidgetDeviceSet_Version:
		FillInVersion();
		break;
	case TabWidgetDeviceSet_Record:
		FillInRecord();
		break;
	case TabWidgetDeviceSet_NetWork:
		FillInNetWork();
		break;
	case TabWidgetDeviceSet_Alarm:
		FillInAlarm();
		break;
	case TabWidgetDeviceSet_UserManager:
		FillInUserManager();
		break;
	case TabWidgetDeviceSet_Log:
		FillInLog();
		break;
	case TabWidgetDeviceSet_Ptz:
		FillInPtz();
		break;
	default:
		break;
	}
}


void DlgDeviceSet::on_CBoxMImageSize_currentIndexChanged(const QString &arg1)
{
	CaptureSize indexSize = CaptureSize_LAST;
	for (int i = 0; i < CaptureSize_LAST; i++){
		if (AvUiLangsCaptureSize((CaptureSize)i) == arg1){
			indexSize = (CaptureSize)i;
		}
	}
	if (indexSize == CaptureSize_LAST){
		return;
	}
	int Channel = ui->CBoxSettingChannel->currentIndex();
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
		return;
	}

	C_EncodeCaps EncodeCaps;

	av_bool bRet = av_false;
	bRet = pCapture->EncodeGetCaps(EncodeCaps);
	if (bRet != av_true){
		ShowErrorGetArgs();
		return;
	}
	AvQDebug("current Main Image Size Channge [%s]\n", arg1.toStdString().c_str());
	QString SImageIndexSize = ui->CBoxSImageSize->currentText();
	ui->CBoxSImageSize->clear();
	for (int i = CaptureSize_Self; i < CaptureSize_SNR; i++){
		if (EncodeCaps.ExtImageSizeMask[indexSize] & AvMask(i)){
			ui->CBoxSImageSize->addItem(AvUiLangsCaptureSize((CaptureSize)i));
		}
	}

	ui->CBoxSImageSize->setCurrentText(SImageIndexSize);

	QString MFrameRateIndex = ui->CBoxMFrameRate->currentText();

	ui->CBoxMFrameRate->clear();
	int Start, End;
	Start = EncodeCaps.FrameRateMask[indexSize] >> 16;
	End = EncodeCaps.FrameRateMask[indexSize] & 0xffff;
	int i = 0;
	for (i = Start; i <= End; i++){
		ui->CBoxMFrameRate->addItem(QString::number(i));
	}
	if (i <= MFrameRateIndex.toInt()){
		MFrameRateIndex = QString::number(i);
	}
	ui->CBoxMFrameRate->setCurrentText(MFrameRateIndex);
}

void DlgDeviceSet::on_CBoxMBitRateCtrl_currentIndexChanged(const QString &arg1)
{
	ui->CBoxMBitRateQlevel->clear();
	if (arg1 == AvUiLangsBitRateCtrl(BitRateCtrl_CBR)){
		ui->LabMBitRateQlevel->setText(QObject::tr("BitRate"));

		for (int i = 1; i * 512 <= 8192; i++){
			char str[12];
			sprintf(str, "%04d", i * 512);
			ui->CBoxMBitRateQlevel->addItem(QString(str));
		}
		ui->CBoxMBitRateQlevel->addItem(AvUiLangsCustom());

		ui->CBoxMBitRateQlevel->setCurrentText(QString::number(4096));
		ui->LiEditMBitRateQlevel->setText(QString::number(4096));
		ui->LabMVcbr->hide();
		ui->LiEditMBitRateQlevel->show();
	}
	else if (arg1 == AvUiLangsBitRateCtrl(BitRateCtrl_VBR)){
		for (int i = 1; i < 5; i++){
			ui->CBoxMBitRateQlevel->addItem(QString::number(i));
		}
		ui->CBoxMBitRateQlevel->setCurrentText(QString::number(3));
		ui->LabMVcbr->hide();
		ui->LiEditMBitRateQlevel->hide();
		ui->LabMBitRateQlevel->setText(QObject::tr("Qlevel"));
	}
	else if (arg1 == AvUiLangsBitRateCtrl(BitRateCtrl_VCBR)){
		for (int i = 1; i < 5; i++){
			ui->CBoxMBitRateQlevel->addItem(QString::number(i));
		}

		ui->CBoxMBitRateQlevel->setCurrentText(QString::number(3));
		ui->LabMVcbr->show();
		ui->LiEditMBitRateQlevel->show();
		ui->LiEditMBitRateQlevel->setText(QString::number(4096));
		ui->LabMBitRateQlevel->setText(QObject::tr("Qlevel"));
	}



}

void DlgDeviceSet::on_CBoxSBitRateCtrl_currentIndexChanged(const QString &arg1)
{
	ui->CBoxSBitRateQlevel->clear();
	if (arg1 == AvUiLangsBitRateCtrl(BitRateCtrl_CBR)){
		ui->LabSBitRateQlevel->setText(QObject::tr("BitRate"));

		for (int i = 1; i * 64 <= 2048; i++){
			char str[12];
			sprintf(str, "%04d", i * 64);
			ui->CBoxSBitRateQlevel->addItem(QString(str));
		}
		ui->CBoxSBitRateQlevel->addItem(AvUiLangsCustom());

		ui->CBoxSBitRateQlevel->setCurrentText(QString("0512"));
		ui->LiEditSBitRateQlevel->setText(QString("0512"));
		ui->LabSVcbr->hide();
		ui->LiEditSBitRateQlevel->show();
	}
	else if (arg1 == AvUiLangsBitRateCtrl(BitRateCtrl_VBR)){
		for (int i = 1; i < 5; i++){
			ui->CBoxSBitRateQlevel->addItem(QString::number(i));
		}
		ui->CBoxSBitRateQlevel->setCurrentText(QString::number(3));
		ui->LabSVcbr->hide();
		ui->LiEditSBitRateQlevel->hide();
		ui->LabSBitRateQlevel->setText(QObject::tr("Qlevel"));
	}
	else if (arg1 == AvUiLangsBitRateCtrl(BitRateCtrl_VCBR)){
		for (int i = 1; i < 5; i++){
			ui->CBoxSBitRateQlevel->addItem(QString::number(i));
		}

		ui->CBoxSBitRateQlevel->setCurrentText(QString::number(3));
		ui->LabSVcbr->show();
		ui->LiEditSBitRateQlevel->show();
		ui->LiEditSBitRateQlevel->setText(QString("0512"));
		ui->LabSBitRateQlevel->setText(QObject::tr("Qlevel"));
	}

}

void DlgDeviceSet::on_CBoxMBitRateQlevel_currentIndexChanged(const QString &arg1)
{
	if (ui->CBoxMBitRateCtrl->currentText() == AvUiLangsBitRateCtrl(BitRateCtrl_CBR)){
		ui->LiEditMBitRateQlevel->setText(ui->CBoxMBitRateQlevel->currentText());
	}
}

void DlgDeviceSet::on_CBoxSBitRateQlevel_currentIndexChanged(const QString &arg1)
{
	if (ui->CBoxSBitRateCtrl->currentText() == AvUiLangsBitRateCtrl(BitRateCtrl_CBR)){
		ui->LiEditSBitRateQlevel->setText(ui->CBoxSBitRateQlevel->currentText());
	}
}


void DlgDeviceSet::on_RBtnAiMicIn_clicked(bool checked)
{
    if (checked == false) return;
	ui->RBtnAiLineIn->setChecked(false);
	ui->RBtnAiMicIn->setChecked(true);
}

void DlgDeviceSet::on_RBtnAiLineIn_clicked(bool checked)
{
    if (checked == false) return;
	ui->RBtnAiLineIn->setChecked(true);
	ui->RBtnAiMicIn->setChecked(false);
}

void DlgDeviceSet::on_CBoxAiEncodeType_currentIndexChanged(const QString &arg1)
{

	if (arg1.size() == 0){
		av_warning("on_CBoxAiEncodeType_currentIndexChanged nULL\n");
		return;
	}
	AvComp currentComp;


	for (int i = AvComp_H264; i < AvComp_LAST; i++){
		if (arg1 == AvUiLangsAvComp((AvComp)i)){
			currentComp = (AvComp)i;
			break;
		}
	}
	ui->CBoxAiSampleRate->clear();
	for (int i = AudioSampleRate_NONE; i < AudioSampleRate_LAST; i++){
		if (AvMask(i) & m_CurrentAudioCaps.SampleRateMask[currentComp]){
			ui->CBoxAiSampleRate->addItem(AvUiLangsAudioSampleRate((AudioSampleRate)i));
		}
	}

	ui->CBoxAiSampleBits->clear();
	for (int i = AudioSampleBits_NONE; i < AudioSampleBits_LAST; i++){
		if (AvMask(i) & m_CurrentAudioCaps.SampleBitsMask[currentComp]){
			ui->CBoxAiSampleBits->addItem(AvUiLangsAudioSampleBits((AudioSampleBits)i));
		}
	}

	ui->CBoxAiSampleRate->setCurrentText(AvUiLangsAudioSampleRate(m_CurrentAudioProfile.SampleRate));
	ui->CBoxAiSampleBits->setCurrentText(AvUiLangsAudioSampleBits(m_CurrentAudioProfile.SampleBits));
}

void DlgDeviceSet::on_BtnVideoCover_clicked()
{
	DlgAreaSelect *AreaSelect = new DlgAreaSelect(this);
	int Channel = ui->CBoxSettingChannel->currentIndex();
	AreaSelect->SetStyle(DlgAreaSelect::AreaSelect);
	AreaSelect->SetChannel(Channel);
	AreaSelect->exec();
}



void DlgDeviceSet::on_BtnOverlay_clicked()
{
	DlgOverLay *OverLay = new DlgOverLay(this);
	int Channel = ui->CBoxSettingChannel->currentIndex();
	OverLay->SetChannel(Channel);
	OverLay->exec();
}

void DlgDeviceSet::on_BtnAlmTimeSelect_clicked()
{
	DlgTimeSelect *TimeSelect = new DlgTimeSelect(this);

	TimeSelect->CTimeAreaArrary2UiPos(m_AlarmTimeArea);
	TimeSelect->exec();
	TimeSelect->CTimeAreaUi2ArraryPos(m_AlarmTimeArea);
	delete TimeSelect;
}

void DlgDeviceSet::on_BtnAlmAreaSelect_clicked()
{
	DlgAreaSelect *AreaSelect = new DlgAreaSelect (this);
	int Channel = ui->CBoxSettingChannel->currentIndex();
	AreaSelect->SetChannel(Channel);
	AreaSelect->SetStyle(DlgAreaSelect::GriddingSelect);
	AreaSelect->MotionDetectionArrary2UiPos(m_AlarmMotionArea);
	AreaSelect->exec();

	AreaSelect->MotionDetectionUiPos2Arrary(m_AlarmMotionArea);

	delete AreaSelect;

}

void DlgDeviceSet::on_CBoxNetWorkFtpEnable_clicked(bool checked)
{
	if (checked == true){
            ui->CBoxNetWorkFtpEnable->setChecked(true);
            ui->LiEditNetWorkFtpServer->setEnabled(true);
            ui->LiEditNetWorkFtpPasswd->setEnabled(true);
            ui->LiEditNetWorkFtpPort->setEnabled(true);
            ui->LiEditNetWorkFtpUserName->setEnabled(true);
    }
    else{
            ui->LiEditNetWorkFtpServer->setEnabled(false);
            ui->LiEditNetWorkFtpPasswd->setEnabled(false);
            ui->LiEditNetWorkFtpPort->setEnabled(false);
            ui->LiEditNetWorkFtpUserName->setEnabled(false);
    }
}

void DlgDeviceSet::on_CBoxNetWorkEmailEnable_clicked(bool checked)
{
	if (checked == true){
		ui->CBoxNetWorkEmailEnable->setChecked(true);
		ui->LiEditNetWorkEmailPasswd->setEnabled(true);
		ui->LiEditNetWorkEmailPort->setEnabled(true);
		ui->LiEditNetWorkEmailReceiver1->setEnabled(true);
		ui->LiEditNetWorkEmailReceiver2->setEnabled(true);
		ui->LiEditNetWorkEmailSender->setEnabled(true);
		ui->LiEditNetWorkEmailSmtpServer->setEnabled(true);
		ui->LiEditNetWorkEmailTitle->setEnabled(true);
		ui->LiEditNetWorkEmailUserName->setEnabled(true);
		ui->CBoxNetWorkEmailEncodeType->setEnabled(true);
	}
	else{

		ui->CBoxNetWorkEmailEnable->setChecked(false);
		ui->LiEditNetWorkEmailPasswd->setEnabled(false);
		ui->LiEditNetWorkEmailPort->setEnabled(false);
		ui->LiEditNetWorkEmailReceiver1->setEnabled(false);
		ui->LiEditNetWorkEmailReceiver2->setEnabled(false);
		ui->LiEditNetWorkEmailSender->setEnabled(false);
		ui->LiEditNetWorkEmailSmtpServer->setEnabled(false);
		ui->LiEditNetWorkEmailTitle->setEnabled(false);
		ui->LiEditNetWorkEmailUserName->setEnabled(false);
		ui->CBoxNetWorkEmailEncodeType->setEnabled(false);
	}
}
// 
// void DlgDeviceSet::on_CBoxNetWorkUpnpEnable_clicked(bool checked)
// {
// 	if (true == checked){
// 		ui->CBoxNetWorkUpnpProtoPort->setCheckable(true);
// 		ui->CBoxNetWorkUpnpRtsp->setCheckable(true);
// 		ui->CBoxNetWorkUpnpWeb->setCheckable(true);
// 	}
// 	else{
// 		ui->CBoxNetWorkUpnpProtoPort->setCheckable(false);
// 		ui->CBoxNetWorkUpnpRtsp->setCheckable(false);
// 		ui->CBoxNetWorkUpnpWeb->setCheckable(false);
// 	}
// }

void DlgDeviceSet::on_CBoxNetWorkRtspEnable_clicked(bool checked)
{
	if (checked == true){
		ui->LiEditNetWorkRtspPort->setEnabled(true);
	}
	else{
		ui->LiEditNetWorkRtspPort->setEnabled(false);
	}
}

void DlgDeviceSet::on_CBoxNetWorkDdnsEnable_clicked(bool checked)
{
	if (checked == true){
		ui->LiEditNetWorkDdnsPasswd->setEnabled(true);
		ui->LiEditNetWorkDdnsServerAddr->setEnabled(true);
		ui->LiEditNetWorkDdnsUserName->setEnabled(true);
		ui->CBoxNetWorkDdnsDdnsType->setEnabled(true);
	}
	else{
		ui->LiEditNetWorkDdnsPasswd->setEnabled(false);
		ui->LiEditNetWorkDdnsServerAddr->setEnabled(false);
		ui->LiEditNetWorkDdnsUserName->setEnabled(false);
		ui->CBoxNetWorkDdnsDdnsType->setEnabled(false);
	}
}

void DlgDeviceSet::on_CBoxNetWorkP2PEnable_clicked(bool checked)
{
	if (checked == true){
		ui->CBoxNetWorkP2PUseLinkID->setEnabled(true);
		ui->LiEditNetWorkP2PServer->setEnabled(true);
		ui->LiEditNetWorkP2PPort->setEnabled(true);
		ui->LiEditNetWorkP2PUsrName->setEnabled(true);
		ui->LiEditNetWorkP2PPasswd->setEnabled(true);
		ui->LiEditNetWorkP2PLinkID->setEnabled(true);
	}
	else{
		ui->CBoxNetWorkP2PUseLinkID->setEnabled(false);
		ui->LiEditNetWorkP2PServer->setEnabled(false);
		ui->LiEditNetWorkP2PPort->setEnabled(false);
		ui->LiEditNetWorkP2PUsrName->setEnabled(false);
		ui->LiEditNetWorkP2PPasswd->setEnabled(false);
		ui->LiEditNetWorkP2PLinkID->setEnabled(false);
	}
}

void DlgDeviceSet::on_CBoxNetWorkP2PUseLinkID_clicked(bool checked)
{
	if (checked == true){
		ui->LabNetWorkP2PServer->hide();
		ui->LiEditNetWorkP2PServer->hide();
		ui->LabNetWorkP2PPort->hide();
		ui->LiEditNetWorkP2PPort->hide();
		ui->LabNetWorkP2PUsrName->hide();
		ui->LiEditNetWorkP2PUsrName->hide();
		ui->LabNetWorkP2PPasswd->hide();
		ui->LiEditNetWorkP2PPasswd->hide();
		ui->LabNetWorkP2PLinkID->show();
		ui->LiEditNetWorkP2PLinkID->show();
	}
	else{
		ui->LabNetWorkP2PServer->show();
		ui->LiEditNetWorkP2PServer->show();
		ui->LabNetWorkP2PPort->show();
		ui->LiEditNetWorkP2PPort->show();
		ui->LabNetWorkP2PUsrName->show();
		ui->LiEditNetWorkP2PUsrName->show();
		ui->LabNetWorkP2PPasswd->show();
		ui->LiEditNetWorkP2PPasswd->show();
		ui->LabNetWorkP2PLinkID->hide();
		ui->LiEditNetWorkP2PLinkID->hide();
	}
}

void DlgDeviceSet::on_CBoxNetWorkTimeEnable_clicked(bool checked)
{

}

void DlgDeviceSet::on_CBoxNetWorkUpnpWeb_clicked(bool checked)
{

}

void DlgDeviceSet::on_CBoxNetWorkUpnpProtoPort_clicked(bool checked)
{

}

void DlgDeviceSet::on_CBoxNetWorkUpnpRtsp_clicked(bool checked)
{

}

void DlgDeviceSet::on_CBoxAlmEnable_clicked(bool checked)
{
	AvQDebug("on_CBoxAlmEnable_clicked clicked [%s]\n", checked == true ? "true" : "false");
	bool enable = false;
	if (checked == true){
		enable = true;
	}

	ui->CBoxAlmEnableMotionDetection->setCheckable(enable);
	ui->CBoxAlmLEnablePortIn->setCheckable(enable);
	ui->BtnAlmTimeSelect->setEnabled(enable);
	ui->BtnAlmAreaSelect->setEnabled(enable);
	ui->CBoxAlmLinkageBeep->setCheckable(enable);
	ui->CBoxAlmLinkageEmail->setCheckable(enable);
	ui->CBoxAlmLinkageFtp->setCheckable(enable);
	ui->CBoxAlmLinkagePtz->setCheckable(enable);
	ui->CBoxAlmLinkageRecord->setCheckable(enable);
	ui->CBoxAlmLinkageSnapshort->setCheckable(enable);
	ui->CBoxalmPreRcordSecond->setEnabled(enable);

}

void DlgDeviceSet::on_BtnCRecordSetRecordTypeTimeSelect_clicked()
{

}





void DlgDeviceSet::on_CBoxWdr_currentIndexChanged(const QString &arg1)
{
	if (arg1 == AvUiLangsWdrMode(WdrMode_Open)){
		ui->LabWdrValueRegion->show();
		ui->LiEditWdrValue->show();
	}
	else{
		ui->LabWdrValueRegion->hide();
		ui->LiEditWdrValue->hide();
	}
}

void DlgDeviceSet::on_CBoxShutter_currentIndexChanged(const QString &arg1)
{
	if (arg1 == AvUiLangsShutterMode(ShutterMode_MaxShutter)){
		ui->LabShutterValueRegion->show();
		ui->LiEditShutter->show();
	}
	else{
		ui->LabShutterValueRegion->hide();
		ui->LiEditShutter->hide();
	}
}

void DlgDeviceSet::on_TviewRtmpLinkInfo_customContextMenuRequested(const QPoint &pos)
{
	AvQDebug("this is costom menu\n");
	if (NULL == m_mouseRtmpRightMenu){
		m_mouseRtmpRightMenu = new QMenu(ui->TviewRtmpLinkInfo->horizontalHeader());
		m_mouseRtmpRightMenu->addAction(QString(tr("Select")), this, SLOT(SlotsRtmpViewRightMenuSelect()));
		m_mouseRtmpRightMenu->addSeparator();
		m_mouseRtmpRightMenu->addAction(QString(tr("SelectAll")), this, SLOT(SlotsRtmpViewRightMenuSelectAll()));
		m_mouseRtmpRightMenu->addAction(QString(tr("SelectOther")), this, SLOT(SlotsRtmpViewRightMenuSelectOther()));
	}
	if (m_RtmpItemModelrow != -1 && m_RtmpListStandardItemModel->rowCount() > 0){
		m_mouseRtmpRightMenu->exec(QCursor::pos());
	}
}

void DlgDeviceSet::on_TviewRtmpLinkInfo_doubleClicked(const QModelIndex &index)
{
	m_RtmpItemModelIndex	= index;
	m_RtmpItemModelrow		= index.row();
	m_RtmpItemModelcolumn	= index.column();

	if (m_RtmpListStandardItemModel->item(m_RtmpItemModelrow, 0)->checkState() == Qt::Checked){
		m_RtmpListStandardItemModel->item(m_RtmpItemModelrow, 0)->setCheckState(Qt::Unchecked);
	}
	else{
		m_RtmpListStandardItemModel->item(m_RtmpItemModelrow, 0)->setCheckState(Qt::Checked);
	}
}

void DlgDeviceSet::SlotsRtmpViewRightMenuSelect()
{
	m_RtmpListStandardItemModel->item(m_RtmpItemModelrow, 0)->setCheckState(Qt::Checked);
}
void DlgDeviceSet::SlotsRtmpViewRightMenuSelectAll()
{
	for (int i = 0; i < m_RtmpListStandardItemModel->rowCount(); i++){
		m_RtmpListStandardItemModel->item(i, 0)->setCheckState(Qt::Checked);
	}
}
void DlgDeviceSet::SlotsRtmpViewRightMenuSelectOther()
{
	for (int i = 0; i < m_RtmpListStandardItemModel->rowCount(); i++){
		if (m_RtmpListStandardItemModel->item(i, 0)->checkState() == Qt::Checked){
			m_RtmpListStandardItemModel->item(i, 0)->setCheckState(Qt::Unchecked);
		}
		else{
			m_RtmpListStandardItemModel->item(i, 0)->setCheckState(Qt::Checked);
		}
	}
}


void DlgDeviceSet::on_TviewRtmpLinkInfo_pressed(const QModelIndex &index)
{
	m_RtmpItemModelIndex = index;
	m_RtmpItemModelrow = index.row();
	m_RtmpItemModelcolumn = index.column();
}
