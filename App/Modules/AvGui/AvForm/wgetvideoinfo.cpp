#include "wgetvideoinfo.h"
#include "ui_wgetvideoinfo.h"
#include "AvUiComm/IconComm.h"
#include "AvUiComm/AvUiComm.h"


WgetVideoInfo::WgetVideoInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgetVideoInfo)
{
    ui->setupUi(this);

	raise();
	m_Palette = new QPalette;
	m_Palette->setColor(QPalette::WindowText, Qt::green);
	m_Palette->setColor(QPalette::Background, QColor(0x00, 0xff, 0x00, 0x00));

	m_PaletteTrigger = new QPalette;
	m_PaletteTrigger->setColor(QPalette::WindowText, QColor(0x00, 0xff, 0x00, 0x00));
	m_PaletteTrigger->setColor(QPalette::Background, QColor(0x00, 0xff, 0x00, 0x00));
	
	ui->LabAlarm->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
	ui->LabStreamInfo->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
	ui->LabListen->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
	ui->LabRecord->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
	ui->LabSpeak->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));

	m_bBtnEnableVideoClicked = false;
	m_bBtnListenClicked = false;
	m_bBtnRecordClicked = false;
	m_bBtnSnapClicked = false;
	m_bBtnSpeakListenCliecked = false;
	ResetOSDVideoInfo();
	ResizeIconSize(12);

	startTimer(800);
	
	m_bLabListenStatus = true;
	m_bLabAlarmtatus = true;
	m_bLabSpeakStatus = true;
	m_bLabRecordStatus = true;

}

WgetVideoInfo::~WgetVideoInfo()
{
    delete ui;
}

void WgetVideoInfo::ResizeIconSize(int size)
{
	m_IconSize = size;
	IconComm::Instance()->SetIcon(ui->BtnEnableVideo, QChar(0xf04b), m_IconSize);
	IconComm::Instance()->SetIcon(ui->BtnListen, QChar(0xf025), m_IconSize);
	IconComm::Instance()->SetIcon(ui->BtnRecord, QChar(0xf0c7), m_IconSize);
	IconComm::Instance()->SetIcon(ui->BtnSnap, QChar(0xf03e), m_IconSize);
	IconComm::Instance()->SetIcon(ui->BtnSpeakListen, QChar(0xf130), m_IconSize);


	IconComm::Instance()->SetIcon(ui->LabAlarm, QChar(0xf0a2), m_IconSize);
	IconComm::Instance()->SetIcon(ui->LabListen, QChar(0xf025), m_IconSize);
	IconComm::Instance()->SetIcon(ui->LabRecord, QChar(0xf03d), m_IconSize);

	IconComm::Instance()->SetIcon(ui->LabSpeak, QChar(0xf130), m_IconSize);
	QFont font = QApplication::font();
	font.setPointSize(m_IconSize);
	ui->LabStreamInfo->setFont(font);
}
void  WgetVideoInfo::ResetOSDVideoInfo()
{
	SetOSDVideoInfo(QString("0 kbps"), QString("0*0"), QString("0 FPS"), QString("H264"));
}
void WgetVideoInfo::SetOSDVideoInfo(QString kbps, QString ImageSize, QString FrameRate, QString EncodeType)
{

	QString Text;
	Text = ImageSize;
	if (FrameRate.size() != 0){
		Text += "\n";
		Text += FrameRate;
	}
	if (kbps.size() != 0){
		Text += "\n";
		Text += kbps;
	}
	if (EncodeType.size() != 0){
		Text += "\n";
		Text += EncodeType;
	}
	ui->LabStreamInfo->setText(Text);

	
}
void WgetVideoInfo::mousePressEvent(QMouseEvent *event)
{
	emit SignalsMousePress(event);
}
void WgetVideoInfo::mouseDoubleClickEvent(QMouseEvent *event)
{
	emit SignalsMouseDoubleClickEvent(event);
}

void WgetVideoInfo::contextMenuEvent(QContextMenuEvent *event)
{
	emit SignalsContextMenuEvent(event);
}

void WgetVideoInfo::resizeEvent(QResizeEvent* size)
{
	printf("size %d, %d \n", size->size().width(), size->size().height());
	int height = size->size().height();
	int IconSize = 0;
	if (height > 600){
		IconSize = 16;
	}
	else if (height > 400){
		IconSize = 14;
	}
	else if (height > 200){
		IconSize = 12;
	}
	else {
		IconSize = 10;
	}

	if (m_IconSize != IconSize){
		ResizeIconSize(IconSize);
	}
}
void	WgetVideoInfo::timerEvent(QTimerEvent *event)
{

	if (m_bBtnRecordClicked == true){
		if (m_bLabRecordStatus == true){
			ui->LabRecord->setStyleSheet(QString("color:rgba(0, 255, 0, 0)"));
		}
		else{
			ui->LabRecord->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
		}
		m_bLabRecordStatus = !m_bLabRecordStatus;
	}
	else{
		if (m_bLabRecordStatus == false){
			m_bLabRecordStatus = true;
			ui->LabRecord->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
		}
	}

	if (m_bBtnListenClicked == true){
		if (m_bLabListenStatus == true){
			
			ui->LabListen->setStyleSheet(QString("color:rgba(0, 255, 0, 0)"));
		}
		else{
			
			ui->LabListen->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
		}
		m_bLabListenStatus = !m_bLabListenStatus;
	}
	else{
		if (m_bLabListenStatus == false && m_bBtnSpeakListenCliecked == false){
			m_bLabListenStatus = true;
			ui->LabListen->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
		}
	}
	if (m_bBtnSpeakListenCliecked == true){
		if (m_bLabSpeakStatus == true){
			ui->LabSpeak->setStyleSheet(QString("color:rgba(0, 255, 0, 0)"));
		}
		else{
			ui->LabSpeak->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
		}
		m_bLabSpeakStatus = !m_bLabSpeakStatus;

		if (m_bLabListenStatus == true){
			ui->LabListen->setStyleSheet(QString("color:rgba(0, 255, 0, 0)"));
		}
		else{
			ui->LabListen->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
		}
		m_bLabListenStatus = !m_bLabListenStatus;
	}
	else {
		if (m_bLabSpeakStatus == false){
			ui->LabSpeak->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
			m_bLabSpeakStatus = true;
		}

		if (m_bBtnListenClicked == false && m_bLabListenStatus == false){
			m_bLabListenStatus = true;
			ui->LabSpeak->setStyleSheet(QString("color:rgba(0, 255, 0, 255)"));
		}
	}


}

void	WgetVideoInfo::moveEvent(QMoveEvent *event)
{
	m_bBtnEnableVideoClicked = false;
	m_bBtnListenClicked = false;
	m_bBtnRecordClicked = false;
	m_bBtnSnapClicked = false;
	m_bBtnSpeakListenCliecked = false;
}
void WgetVideoInfo::on_BtnRecord_clicked()
{
	if (m_bBtnRecordClicked == false){
		emit SignalsUiButtonMessage(UIMSG_START_RECORD);
	}
	else{
		emit SignalsUiButtonMessage(UIMSG_STOP_RECORD);
	}

	m_bBtnRecordClicked = !m_bBtnRecordClicked;
}

void WgetVideoInfo::on_BtnSnap_clicked()
{
	m_bBtnSnapClicked = !m_bBtnSnapClicked;
	emit SignalsUiButtonMessage(UIMSG_REQUEST_SNAP);
}

void WgetVideoInfo::on_BtnSpeakListen_clicked()
{
	if (m_bBtnSpeakListenCliecked == false){
		emit SignalsUiButtonMessage(UIMSG_START_SPEAKLISTEN);
		
		
	}
	else{
		emit SignalsUiButtonMessage(UIMSG_STOP_SPEAKLISTEN);
	}
	m_bBtnSpeakListenCliecked = !m_bBtnSpeakListenCliecked;
}

void WgetVideoInfo::on_BtnListen_clicked()
{
	if (m_bBtnListenClicked == false){
		//ui->LabListen->show();
		emit SignalsUiButtonMessage(UIMSG_START_LISTEN);
	}
	else{
		emit SignalsUiButtonMessage(UIMSG_STOP_LISTEN);
		//ui->LabListen->hide();
		
	}
	m_bBtnListenClicked = !m_bBtnListenClicked;
}

void WgetVideoInfo::on_BtnEnableVideo_clicked()
{
	if (m_bBtnEnableVideoClicked == false){
		emit SignalsUiButtonMessage(UIMSG_START_VIDEO);
		IconComm::Instance()->SetIcon(ui->BtnEnableVideo, QChar(0xf0c8), m_IconSize);
	}
	else{
		emit SignalsUiButtonMessage(UIMSG_STOP_VIDEO);
		IconComm::Instance()->SetIcon(ui->BtnEnableVideo, QChar(0xf04b), m_IconSize);
	}
	m_bBtnEnableVideoClicked = !m_bBtnEnableVideoClicked;
}
