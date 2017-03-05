#include "dlgptzwindows.h"
#include "ui_dlgptzwindows.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "formptzadvanced.h"
DlgPtzWindows::DlgPtzWindows(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgPtzWindows)
{
    ui->setupUi(this);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
	this->setAttribute(Qt::WA_DeleteOnClose);

	IconComm::Instance()->SetIcon(ui->BtnPtzAuto, QChar(0xf01e), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzLeft, QChar(0xf053), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzRight, QChar(0xf054), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzUp, QChar(0xf077), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzDown, QChar(0xf078), 10);

	IconComm::Instance()->SetIcon(ui->BtnPtzZoomAdd, QChar(0xf067), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzZoomReduce, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzFocusReduce, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzFocusAdd, QChar(0xf067), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzIrisReduce, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnPtzIrisAdd, QChar(0xf067), 10);


	m_WidowsMax = false;
	m_mousePressed = false;
	m_bAutoTour = false;
	m_bAutoScan = false;
	m_bAutoPan = false;
	m_PtzValue = 0xff;
	m_PtzSpeed = 50;
	m_FormPtzAdvanced = NULL;
	m_bFromPtzAdvancedShow = false;
	FixUi();
}

DlgPtzWindows::~DlgPtzWindows()
{
    delete ui;
}

void DlgPtzWindows::FixUi()
{
	ui->CBoxChannels->setStyleSheet("color:black");
	ui->CBoxChannels->clear();
	for (int i = 0; i < 32; i++){
		ui->CBoxChannels->addItem(QString::number(i));
	}
	ui->CBoxChannels->setCurrentText(QString::number(0));
	m_Channel = 0;
	m_Capture = g_AvManCapture.GetAvCaptureInstance(m_Channel);

	m_Capture->ImageGetProfile(m_ImageProfile);
	{
		ui->HSliderBrightness->setTickInterval(5);
		ui->HSliderContrast->setTickInterval(5);
		ui->HSliderHue->setTickInterval(5);
		ui->HSliderSaturation->setTickInterval(5);
	}

	ui->BtnAutoPan->setText(QObject::tr("Pan On"));
	ui->BtnAutoScan->setText(QObject::tr("Scan On"));
	ui->BtnAutoTour->setText(QObject::tr("Tour On"));
	ui->HSliderPtzSpeed->setValue(m_PtzSpeed);
	
	ui->LiEditPtzValue->setText(QString::number(m_PtzValue));
	
}
void DlgPtzWindows::mouseMoveEvent(QMouseEvent *e)
{
	if (m_mousePressed && (e->buttons() && Qt::LeftButton) && !m_WidowsMax) {
		this->move(e->globalPos() - m_mousePoint);
		if (m_bFromPtzAdvancedShow == true){
			movePtzAdvanced();
		}
		e->accept();
	}
}
void DlgPtzWindows::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - this->pos();
		e->accept();
	}

}
void DlgPtzWindows::mouseReleaseEvent(QMouseEvent *e)
{
	if (m_mousePressed == true){
		m_mousePressed = false;
		e->ignore();
	}

}

void DlgPtzWindows::movePtzAdvanced()
{
	if (m_bFromPtzAdvancedShow == true){
		QRect PtzWindwosRect = geometry();
		QPoint newPoint; 
		QSize PtzWindowsSize = size();
		newPoint.setX(PtzWindwosRect.x() + PtzWindowsSize.width());
		newPoint.setY(PtzWindwosRect.y());
		m_FormPtzAdvanced->move(newPoint);
	}
	
}
void DlgPtzWindows::on_BtnClose_clicked()
{
	if (m_FormPtzAdvanced != NULL){
		delete m_FormPtzAdvanced;
		m_FormPtzAdvanced = NULL;
	}
    this->close();
}
void DlgPtzWindows::PtzStop()
{
	C_PtzCmd Command ;
	memset(&Command, 0x00, sizeof(C_PtzCmd));
	Command.PtzCmd = PtzCommand_STOP;
	Command.bEnAck = av_false;
	Command.Value = m_PtzValue;
	Command.PtzSpeed = m_PtzSpeed;

	m_Capture->PtzSetCommand(Command);
}
void DlgPtzWindows::PtzStart(PtzCommand Command, C_PtzCmd *pCmd)
{
	m_PtzValue = ui->LiEditPtzValue->text().toInt();
	C_PtzCmd Cmd ;
	memset(&Cmd, 0x00, sizeof(C_PtzCmd));
	Cmd.PtzCmd = Command;
	Cmd.PtzSpeed = m_PtzSpeed;
	Cmd.bEnAck = av_false;
	Cmd.Value = m_PtzValue;
	if (pCmd != NULL){
		Cmd = *pCmd;
		Cmd.Value = m_PtzValue;
		Cmd.PtzSpeed = m_PtzSpeed;
	}

	m_Capture->PtzSetCommand(Cmd);
}
void DlgPtzWindows::on_BtnPtzLU_pressed()
{
	PtzStart(PtzCommand_LEFTUP);
}

void DlgPtzWindows::on_BtnPtzLU_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzUp_pressed()
{
	PtzStart(PtzCommand_UP);
}

void DlgPtzWindows::on_BtnPtzUp_released()
{

	PtzStop();
}

void DlgPtzWindows::on_BtnPtzRU_pressed()
{
	PtzStart(PtzCommand_RIGHTUP);
}

void DlgPtzWindows::on_BtnPtzRU_released()
{
	PtzStop();

}

void DlgPtzWindows::on_BtnPtzLeft_pressed()
{
	PtzStart(PtzCommand_LEFT);
}

void DlgPtzWindows::on_BtnPtzLeft_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzAuto_pressed()
{
	
}

void DlgPtzWindows::on_BtnPtzAuto_released()
{
	
}

void DlgPtzWindows::on_BtnPtzRight_pressed()
{
	PtzStart(PtzCommand_RIGHT);
}

void DlgPtzWindows::on_BtnPtzRight_released()
{
	PtzStop();

}

void DlgPtzWindows::on_BtnPtzLD_pressed()
{
	PtzStart(PtzCommand_LEFTDOWN);

}

void DlgPtzWindows::on_BtnPtzLD_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzDown_pressed()
{	
	PtzStart(PtzCommand_DOWN);
}

void DlgPtzWindows::on_BtnPtzDown_released()
{
	PtzStop();
	
}

void DlgPtzWindows::on_BtnPtzRD_pressed()
{
	PtzStart(PtzCommand_RIGHTDOWN);

}

void DlgPtzWindows::on_BtnPtzRD_released()
{
	PtzStop();

}

void DlgPtzWindows::on_CBoxChannels_currentIndexChanged(const QString &arg1)
{
	m_Channel = arg1.toInt();
	m_Capture = g_AvManCapture.GetAvCaptureInstance(m_Channel);
	if (m_Capture == NULL || m_Capture->GetCaptureStatus() != Capture::EAvCapture_ING){
		CAvUiComm::ShowMessageBoxError(QObject::tr("Channel is Not Working"));
		return;
	}
	m_bAutoTour = false;
	m_Capture->ImageGetProfile(m_ImageProfile);

}

void DlgPtzWindows::on_HSliderBrightness_valueChanged(int value)
{
	if (5 > abs((int)(value - m_ImageProfile.Brightness))){
		av_msg("value = %d, m_ImageProfile Brightness = %d\n", value, m_ImageProfile.Brightness);
		return;
	}
	m_ImageProfile.Brightness = value;
	m_Capture->ImageSetProfile(m_ImageProfile);
}

void DlgPtzWindows::on_HSliderContrast_valueChanged(int value)
{
	if (5 > abs((int)(value - m_ImageProfile.Contrast))){
		av_msg("value = %d, m_ImageProfile Contrast = %d\n", value, m_ImageProfile.Contrast);
		return;
	}
	m_ImageProfile.Contrast = value;
	m_Capture->ImageSetProfile(m_ImageProfile);
}

void DlgPtzWindows::on_HSliderHue_valueChanged(int value)
{
	if (5 > abs((int)(value - m_ImageProfile.Hue))){
		av_msg("value = %d, m_ImageProfile Hue = %d\n", value, m_ImageProfile.Hue);
		return;
	}
	m_ImageProfile.Hue = value;
	m_Capture->ImageSetProfile(m_ImageProfile);
}

void DlgPtzWindows::on_HSliderSaturation_valueChanged(int value)
{
	if (5 > abs((int)(value - m_ImageProfile.Saturation))){
		av_msg("value = %d, m_saturation = %d\n", value, m_ImageProfile.Saturation);
		return;
	}
	m_ImageProfile.Saturation = value;
	m_Capture->ImageSetProfile(m_ImageProfile);
}

void DlgPtzWindows::on_BtnImageDefault_clicked()
{
	ui->HSliderBrightness->setValue(50);
	ui->HSliderContrast->setValue(50);
	ui->HSliderHue->setValue(50);
	ui->HSliderSaturation->setValue(50);
	m_ImageProfile.Brightness = 50;
	m_ImageProfile.Contrast = 50;
	m_ImageProfile.Hue = 50;
	m_ImageProfile.Saturation = 50;
    m_Capture->ImageSetProfile(m_ImageProfile);
}


void DlgPtzWindows::on_HSliderPtzSpeed_valueChanged(int value)
{
	m_PtzSpeed = ui->HSliderPtzSpeed->value();
}

void DlgPtzWindows::on_BtnAutoTour_clicked()
{
	if (m_bAutoTour == false){
		ui->BtnAutoTour->setText(QObject::tr("Tour Off"));
		PtzStart(PtzCommand_STARTTOUR);
	}
	else{
		ui->BtnAutoTour->setText(QObject::tr("Tour On"));
		PtzStart(PtzCommand_STOPTOUR);
	}
	
	m_bAutoTour = !m_bAutoTour;
}

void DlgPtzWindows::on_BtnPreset_clicked()
{
	PtzStart(PtzCommand_GOTOPRESET);
}

void DlgPtzWindows::on_BtnAutoScan_clicked()
{
	if (m_bAutoScan == false){
		PtzStart(PtzCommand_AUTOSCANON);
		ui->BtnAutoScan->setText(QObject::tr("Scan Off"));
	}
	else {
		PtzStart(PtzCommand_AUTOSCANOFF);
		ui->BtnAutoScan->setText(QObject::tr("Scan On"));
	}
	m_bAutoScan = !m_bAutoScan;
}



void DlgPtzWindows::on_BtnAutoPan_clicked()
{
	if (m_bAutoPan == false){
		ui->BtnAutoPan->setText(QObject::tr("Pan Off"));
		PtzStart(PtzCommand_AUTOPANON);
	}
	else {
		ui->BtnAutoPan->setText(QObject::tr("Pan On"));
		PtzStart(PtzCommand_AUTOPANOFF);
	}
	m_bAutoPan = !m_bAutoPan;
}

void DlgPtzWindows::on_BtnPattem_clicked()
{

}

void DlgPtzWindows::on_BtnPtzConfig_clicked()
{
    if (NULL == m_FormPtzAdvanced){
            m_FormPtzAdvanced = new FormPtzAdvanced(this);
    }
    //m_FormPtzAdvanced->setMouseTracking(true);
    if (m_bFromPtzAdvancedShow == false){
            m_bFromPtzAdvancedShow = !m_bFromPtzAdvancedShow;
            movePtzAdvanced();
            m_FormPtzAdvanced->show();
            m_FormPtzAdvanced->SetCurrentChannel(m_Capture);
    }
    else{
            m_bFromPtzAdvancedShow = !m_bFromPtzAdvancedShow;
            m_FormPtzAdvanced->hide();
    }
}
void DlgPtzWindows::on_BtnAuxOn_clicked()
{
	PtzStart(PtzCommand_AUX_ON);
}

void DlgPtzWindows::on_BtnAuxOff_clicked()
{
	PtzStart(PtzCommand_AUX_OFF);
}

void DlgPtzWindows::on_BtnPtzZoomAdd_pressed()
{
	PtzStart(PtzCommand_ZOOM_TELE);
}

void DlgPtzWindows::on_BtnPtzZoomAdd_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzIrisAdd_pressed()
{
	PtzStart(PtzCommand_IRIS_LARGE);
}

void DlgPtzWindows::on_BtnPtzIrisAdd_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzFocusAdd_pressed()
{
	PtzStart(PtzCommand_FOCUS_FAR);
}

void DlgPtzWindows::on_BtnPtzFocusAdd_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzZoomReduce_pressed()
{
	PtzStart( PtzCommand_ZOOM_WIDE);
}

void DlgPtzWindows::on_BtnPtzZoomReduce_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzIrisReduce_pressed()
{
	PtzStart(PtzCommand_IRIS_SMALL);
}

void DlgPtzWindows::on_BtnPtzIrisReduce_released()
{
	PtzStop();
}

void DlgPtzWindows::on_BtnPtzFocusReduce_pressed()
{
	PtzStart(PtzCommand_FOCUS_NEAR);
}

void DlgPtzWindows::on_BtnPtzFocusReduce_released()
{
	PtzStop();
}
