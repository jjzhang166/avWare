#include "formptzadvanced.h"
#include "ui_formptzadvanced.h"
#include "AvUiComm.h"
#include "AvQStringsLanguage.h"

FormPtzAdvanced::FormPtzAdvanced(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPtzAdvanced)
{
    ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	this->setAttribute(Qt::WA_DeleteOnClose);
	raise();
	m_Capture = NULL;

}

FormPtzAdvanced::~FormPtzAdvanced()
{
    delete ui;
}

void	FormPtzAdvanced::SetCurrentChannel(Capture *pCapture)
{
	m_Capture = pCapture;
	FillInCaps();
	FillInProfile();
}

void	FormPtzAdvanced::FillInCaps()
{
	if (m_Capture == NULL){
		return;
	}
	//C_PtzAdvancedCaps PtzAdvancedCaps;
	C_AdvancedSystemCaps AdvancedSystemCaps;
	AdvancedSystemCaps._msg = __MsgPtzCameraLensCaps;
	C_PtzAdvancedCameraLensCaps &CameraLensCaps = AdvancedSystemCaps.PtzCameralensCaps;

	av_bool bRet = m_Capture->AdvancedSystemGetCaps(AdvancedSystemCaps);
	if (bRet == av_false){
		return;
	}
	if (av_true == CameraLensCaps.bSupportFigureZoom){
		ui->CBoxPtzASceneDZoom->setCheckable(true);
	}
	else{
		ui->CBoxPtzASceneDZoom->setCheckable(false);
	}

	ui->CBoxPtzASceneFocuMode->clear();
	for (int i = PtzCameraLensFocusCtrlMode_None; i < PtzCameraLensFocusCtrlMode_Last; i++){
		if (!(AvMask(i) & CameraLensCaps.FocusCtrlModeMask)) continue;
		ui->CBoxPtzASceneFocuMode->addItem(AvUiLangsPtzCameraLensFocusCtrlMode((PtzCameraLensFocusCtrlMode)i));
	}

	ui->CBoxPtzASceneFocuArea->clear();
	for (int i = PtzCameraLensFocusRegion_None; i < PtzCameraLensFocusRegion_Last; i++){
		if (!(AvMask(i) & CameraLensCaps.FocusRegionMask)) continue;
		ui->CBoxPtzASceneFocuArea->addItem(AvUiLangsPtzCameraLensFocusRegion((PtzCameraLensFocusRegion)i));
	}

	ui->CBoxPtzASceneFocuSearch->clear();
	for (int i = PtzCameraLensFocusSearchMode_None; i < PtzCameraLensFocusSearchMode_Last; i++){
		if (!(AvMask(i) & CameraLensCaps.FocusSearchModeMask)) continue;
		ui->CBoxPtzASceneFocuSearch->addItem(AvUiLangsPtzCameraLensFocusSearchMode((PtzCameraLensFocusSearchMode)i));
	}

	ui->CBoxPtzASceneIrisMode->clear();
	for (int i = PtzCameraLensIrisCtrlMode_None; i < PtzCameraLensIrisCtrlMode_Last; i++){
		if (!(AvMask(i) & CameraLensCaps.IrisCtrlModeMask)) continue;
		ui->CBoxPtzASceneIrisMode->addItem(AvUiLangsPtzCameraLensIrisCtrlMode((PtzCameraLensIrisCtrlMode)i));
	}

	QString FocuSpeedMax;
	FocuSpeedMax = FocuSpeedMax.sprintf("(1-%d)", CameraLensCaps.FocusSensitivityMax);
	ui->LabPtzASceneFocuSpeedMax->setText(FocuSpeedMax);


	ui->HSliderPtzASceneZoomSpeed->setRange(1, CameraLensCaps.ZoomSpeedMax <= 0 ? 100 : CameraLensCaps.ZoomSpeedMax);

}
void	FormPtzAdvanced::FillInProfile()
{
	if (m_Capture == NULL){
		av_error("m_Capture == NULL\n");
		return;
	}
	
	//C_PtzAdvancedProfile PtzAdvancedProfile;
	C_AdvancedSystemProfile AdvancedSystemProfile;
	AdvancedSystemProfile._msg = __MsgPtzCameraLensProfile;
	C_PtzAdvancedCameraLensProfile &CameraLensProfile = AdvancedSystemProfile.PtzAdvancedCameraLensProfile;


	av_bool bRet = m_Capture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (bRet == av_false){
		av_error("AdvancedSystemGetProfile == error\n");
		return;
	}

	ui->CBoxPtzASceneIrisMode->setCurrentText(AvUiLangsPtzCameraLensIrisCtrlMode(CameraLensProfile.IrisCtrlMode));
	ui->CBoxPtzASceneFocuSearch->setCurrentText(AvUiLangsPtzCameraLensFocusSearchMode(CameraLensProfile.FocusSearchMode));
	ui->CBoxPtzASceneFocuMode->setCurrentText(AvUiLangsPtzCameraLensFocusCtrlMode(CameraLensProfile.FocusCtrlMode));
	ui->CBoxPtzASceneFocuArea->setCurrentText(AvUiLangsPtzCameraLensFocusRegion(CameraLensProfile.FocusRegion));
	ui->LiEditPtzASceneFocuSpeed->setText(QString::number(CameraLensProfile.FocusSensitivity));
	if (CameraLensProfile.bFigureZoom == av_true){
		ui->CBoxPtzASceneDZoom->setChecked(true);
	}
	else{
		ui->CBoxPtzASceneDZoom->setChecked(false);
	}
	ui->HSliderPtzASceneZoomSpeed->setValue(CameraLensProfile.ZoomSpeed);

}

void FormPtzAdvanced::on_BtnPtzASceneSubmit_clicked()
{
	QString data;
	C_AdvancedSystemProfile AdvancedSystemProfile;
	AdvancedSystemProfile._msg = __MsgPtzCameraLensProfile;
	data = ui->CBoxPtzASceneFocuMode->currentText();
	for (int i = PtzCameraLensFocusCtrlMode_None; i < PtzCameraLensFocusCtrlMode_Last; i++){
		if (data == AvUiLangsPtzCameraLensFocusCtrlMode((PtzCameraLensFocusCtrlMode)i)){
			AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusCtrlMode = (PtzCameraLensFocusCtrlMode)i;
			break;
		}
	}

	data = ui->CBoxPtzASceneFocuSearch->currentText();
	for (int i = PtzCameraLensFocusSearchMode_None; i < PtzCameraLensFocusSearchMode_Last; i++){
		if (data == AvUiLangsPtzCameraLensFocusSearchMode((PtzCameraLensFocusSearchMode)i)){
			AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusSearchMode = (PtzCameraLensFocusSearchMode)i;
			break;
		}
	}

	data = ui->CBoxPtzASceneFocuArea->currentText();
	for (int i = PtzCameraLensFocusRegion_None; i < PtzCameraLensFocusRegion_Last; i++){
		if (data == AvUiLangsPtzCameraLensFocusRegion((PtzCameraLensFocusRegion)i)){
			AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusRegion = (PtzCameraLensFocusRegion)i;
			break;
		}
	}
	
	data = ui->CBoxPtzASceneIrisMode->currentText();
	for (int i = PtzCameraLensIrisCtrlMode_None; i < PtzCameraLensIrisCtrlMode_Last; i++){
		if (data == AvUiLangsPtzCameraLensIrisCtrlMode((PtzCameraLensIrisCtrlMode)i)){
			AdvancedSystemProfile.PtzAdvancedCameraLensProfile.IrisCtrlMode = (PtzCameraLensIrisCtrlMode)i;
			break;
		}
	}

	data = ui->LiEditPtzASceneFocuSpeed->text();
	AdvancedSystemProfile.PtzAdvancedCameraLensProfile.FocusSensitivity = data.toInt();
	if (ui->CBoxPtzASceneDZoom->checkState() == Qt::Checked){
		AdvancedSystemProfile.PtzAdvancedCameraLensProfile.bFigureZoom = av_true;
	}
	else{
		AdvancedSystemProfile.PtzAdvancedCameraLensProfile.bFigureZoom = av_false;
	}


	AdvancedSystemProfile.PtzAdvancedCameraLensProfile.ZoomSpeed = ui->HSliderPtzASceneZoomSpeed->value();

	av_bool ret = m_Capture->AdvancedSystemSetProfile(AdvancedSystemProfile);
	if (ret == av_true){

	}
	else{

	}
}
