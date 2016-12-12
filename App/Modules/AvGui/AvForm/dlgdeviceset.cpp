#include "dlgdeviceset.h"
#include "ui_dlgdeviceset.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvCapture/AvManCapture.h"
#include "AvSource/AvQStringsLanguage.h"


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

	this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_DeleteOnClose);




	m_TviewRtmpListTableModel = new TableModel;
	FixDlgUi();
	ui->StWidgetNetWork->setCurrentIndex(0);
	ui->TBoxNetWork->setCurrentIndex(0);

	ui->TabWidgetDeviceSet->setCurrentIndex(TabWidgetDeviceSet_Version);
	FillInVersion();

	m_UiInitedOver = true;
	memset(&m_CurrentAudioCaps, 0x00, sizeof(m_CurrentAudioCaps));
	memset(&m_CurrentAudioProfile, 0x00, sizeof(m_CurrentAudioProfile));
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

	ui->CBoxSettingChannel->setStyleSheet("color:black");
	FixDlgUiCapture();
	FixDlgUiEncode();
	FixDlgUiNetWork();
	FixDlgUiAudio();

	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}

void DlgDeviceSet::FixDlgUiCapture()
{
	ui->CBoxIrCut->setStyleSheet("color:black");
	ui->CBoxAnitFlicker->setStyleSheet("color:black");
	ui->CBoxExposureMode->setStyleSheet("color:black");
	ui->CBoxWhitBalance->setStyleSheet("color:black");
	ui->CBoxRotateMode->setStyleSheet("color:black");
	ui->CBoxCvbs->setStyleSheet("color:black");
}
void DlgDeviceSet::FixDlgUiEncode()
{
	ui->CBoxMBitRateCtrl->setStyleSheet("color:black");
	ui->CBoxMBitRateQlevel->setStyleSheet("color:black");
	ui->CBoxMEnFormat->setStyleSheet("color:black");
	ui->CBoxMFrameRate->setStyleSheet("color:black");
	ui->CBoxMImageSize->setStyleSheet("color:black");
	
	ui->CBoxSBitRateCtrl->setStyleSheet("color:black");
	ui->CBoxSBitRateQlevel->setStyleSheet("color:black");
	ui->CBoxSSlaveNumber->setStyleSheet("color:black");
	ui->CBoxSFrameRate->setStyleSheet("color:black");
	ui->CBoxSImageSize->setStyleSheet("color:black");

}
void DlgDeviceSet::FixDlgUiAudio()
{
	ui->CBoxAiSoundMode->setStyleSheet("color:black");
	ui->CBoxAiEncodeType->setStyleSheet("color:black");
	ui->CBoxAiSampleBits->setStyleSheet("color:black");
	ui->CBoxAiSampleRate->setStyleSheet("color:black");
}
void DlgDeviceSet::FixDlgUiVersion()
{

}
void DlgDeviceSet::FixDlgUiRecord()
{

}
void DlgDeviceSet::FixDlgUiNetWork()
{

	QStringList headers;
	headers.push_back(QString(tr("No")));
	headers.push_back(QString(tr("RtmpUrl")));
	headers.push_back(QString(tr("Chn")));
	headers.push_back(QString(tr("Slave")));
	headers.push_back(QString(tr("Audio")));
	headers.push_back(QString(tr("Status")));
	m_TviewRtmpListTableModel->setHorizontalHeader(headers);
	ui->TviewRtmpLinkInfo->setModel(m_TviewRtmpListTableModel);
	ui->TviewRtmpLinkInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewRtmpLinkInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewRtmpLinkInfo->setContextMenuPolicy(Qt::CustomContextMenu);
	emit m_TviewRtmpListTableModel->layoutChanged();

	QObject::connect((const QObject *)ui->TviewRtmpLinkInfo->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged(int, int)));

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

void DlgDeviceSet::FillInCapture()
{
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
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
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
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
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
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
	//caps
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
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
	for (int i = AvComp_H264; i < AvComp_LAST; i++){
		if (AvMask(i) & m_CurrentAudioCaps.CompMask){
			SamplebitsStr += AvUiLangsAvComp((AvComp)i);
			SamplebitsStr += QString(" [ ");
			SampleRateStr += AvUiLangsAvComp((AvComp)i);
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

}
void DlgDeviceSet::FillInRecord()
{

}
void DlgDeviceSet::FillInNetWork()
{

}
void DlgDeviceSet::FillInAlarm()
{

}
void DlgDeviceSet::FillInUserManager()
{

}
void DlgDeviceSet::FillInLog()
{

}
void DlgDeviceSet::FillInPtz()
{

}


void DlgDeviceSet::SubmitCapture()
{
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
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
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
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
	int Channel = ui->CBoxSettingChannel->currentIndex();

	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	Capture::EAvCaptureStatus stat = pCapture->GetCaptureStatus();
	if (stat != Capture::EAvCapture_ING){
		ShowErrorConnectMesageBox();
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

}

void DlgDeviceSet::SubmitNetWork()
{

}

void DlgDeviceSet::SubmitAlarm()
{

}

void DlgDeviceSet::SubmitUserManager()
{

}

void DlgDeviceSet::SubmitLog()
{

}

void DlgDeviceSet::SubmitPtz()
{

}

void DlgDeviceSet::ShowErrorConnectMesageBox()
{
	CAvUiComm::ShowMessageBoxError(QString(tr("This Channel does't Connect!")));
}
void DlgDeviceSet::ShowErrorGetArgs()
{
	CAvUiComm::ShowMessageBoxError(QString(tr("Failed to get parameters!")));
}
void DlgDeviceSet::ShowSucceedSetArgs()
{
	CAvUiComm::ShowMessageBoxInfo(QString(tr("set parameters succeed!")));
}
void DlgDeviceSet::ShowErrorSetArgs()
{
	CAvUiComm::ShowMessageBoxError(QString(tr("Failed to set parameters!")));
}

DlgDeviceSet::~DlgDeviceSet()
{
    delete ui;
}

void DlgDeviceSet::SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged(int min, int max)
{
	AvQDebug("SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged min = %d, max = %d\n", min, max);
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}

void DlgDeviceSet::resizeEvent(QResizeEvent * event)
{

	AvQDebug("DlgDeviceSet::resizeEvent\n");
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

#if 0
	QSize viewSize = ui->TviewRtmpLinkInfo->viewport()->size();
	ui->TviewRtmpLinkInfo->setColumnWidth(0, 0.10*viewSize.width());
	ui->TviewRtmpLinkInfo->setColumnWidth(1, 0.50*viewSize.width());
	ui->TviewRtmpLinkInfo->setColumnWidth(2, 0.10*viewSize.width());
	ui->TviewRtmpLinkInfo->setColumnWidth(3, 0.10*viewSize.width());
	ui->TviewRtmpLinkInfo->setColumnWidth(4, 0.10*viewSize.width());
	ui->TviewRtmpLinkInfo->setColumnWidth(5, 0.10*viewSize.width());
#endif
}

void DlgDeviceSet::on_BtnNetWorkRTMPAdd_clicked()
{

}

void DlgDeviceSet::on_BtnNetWorkRTMPDel_clicked()
{

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
