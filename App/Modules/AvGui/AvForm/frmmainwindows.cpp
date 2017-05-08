#include "frmmainwindows.h"
#include "ui_frmmainwindows.h"
#include "AvUiComm/IconComm.h"
#include "AvUiComm/AvUiComm.h"
#include "AvForm/frmdevice.h"
#include "AvForm/dlgnetset.h"
#include "AvUiComm/FloatingButton.h"
#include "AvDevice/AvDevice.h"
#include "AvForm/dlgabout.h"
#include "AvForm/dlgrecordset.h"
#include "AvForm/frmmainmenu.h"
#include "AvSource/AvGuiStatusMachine.h"

#if defined(_AV_WARE_CODE_OPENSOURCE)
#else
#include "AvForm/dlgmkfirmware.h"
#endif
FrmMainWindows::FrmMainWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMainWindows)
{
    ui->setupUi(this);
	CAvUiComm::FormInCenter(this);
	setWindowTitle("avWare");
	m_WidowsMax = false;
	m_VideoWidowsMax = false;
	m_mousePressed = false;

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 8);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 8);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 8);
	IconComm::Instance()->SetIcon(ui->BtnAbout, QChar(0xf06b), 8);
	IconComm::Instance()->SetIcon(ui->BtnVideoWindowInfo, QChar(0xf129), 8);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 8);
	IconComm::Instance()->SetIcon(ui->BtnAlarm, QChar(0xf0f3), 8);

	m_localRect = this->geometry();


	
	ui->LabTitle->installEventFilter(this);
	m_bOpenedMenu = false;
	ui->FrmMenu->hide();
	m_FrmMainMenu = NULL;
	

	ui->RecordShowBar->hide();
	ui->RecordSearch->hide();

	{
		m_frmDevices = new FrmDevice;
		QObject::connect(m_frmDevices, SIGNAL(SignalPreviewStart(int, int, bool)), ui->FrmVideoArea, SLOT(SlotOnPreviewStart(int, int, bool)), Qt::QueuedConnection);

		m_DlgAlarmMessage = new DlgAlarmMessage;
	}
	
	m_DlgRecordSet = new DlgRecordSet;

}

FrmMainWindows::~FrmMainWindows()
{
	AvQDebug("clear up\n");

    delete ui;
}
bool FrmMainWindows::SetWidgetWindowsMax()
{
	setWindowState(Qt::WindowMaximized);
	m_WidowsMax = true;
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf079), 10);
	ui->BtnMax->setToolTip(tr("MaxWindows"));
	return true;
}
bool FrmMainWindows::SetWidgetVideoAreaMax()
{
	m_VideoWidowsMax = true;
	ui->FrmVideoArea->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	ui->FrmVideoArea->showMaximized();
	QRect cur = CAvUiComm::GetWindowsOnScreen();
	ui->FrmVideoArea->setGeometry(cur);
	return true;
}

QPoint	 FrmMainWindows::GetVideoAreaToGlobal()
{

	if (m_VideoWidowsMax == true){
		return QPoint(0, 0);
	}
	QPoint FrmmainWindows = pos();
	QPoint FrmUsrWindowsPoint = ui->FrmUsrWindows->pos();
	QPoint VideoAreaPoint = ui->FrmVideoArea->pos();

	QPoint GlobalPoint;
	GlobalPoint.setX(FrmmainWindows.x() + FrmUsrWindowsPoint.x() + VideoAreaPoint.x());
	GlobalPoint.setY(FrmmainWindows.y() + FrmUsrWindowsPoint.y() + VideoAreaPoint.y());
	return GlobalPoint;
}

bool FrmMainWindows::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonDblClick) {
		this->on_BtnMax_clicked();
		return true;
	}
	return QObject::eventFilter(obj, event);
}
void FrmMainWindows::mouseMoveEvent(QMouseEvent *e)
{
	if (m_mousePressed && (e->buttons() && Qt::LeftButton) && !m_WidowsMax) {
		this->move(e->globalPos() - m_mousePoint);
// 		QPoint point = this->pos();
// 		QRect rect = this->rect();
// 		point.setX(point.x() + rect.width()/3*2);
// 		point.setY(point.y() + rect.height()/3*2);
// 		m_FloatButton->move(point);

		QApplication::sendEvent(ui->FrmVideoArea, new QEvent(QEvent::Move));
		e->accept();
	}

}
void FrmMainWindows::mousePressEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - this->pos();
		e->accept();
	}

}
void FrmMainWindows::mouseReleaseEvent(QMouseEvent *e)
{
	if (m_mousePressed == true){
		m_mousePressed = false;
		e->ignore();
	}
	
}
void FrmMainWindows::SetBtnAlarmColor(bool AlmColor)
{
	if (AlmColor == true){
		ui->BtnAlarm->setStyleSheet(QString("color:rgba(255, 0, 0, 255)"));
	}
	else{
		ui->BtnAlarm->setStyleSheet(QString("color:rgba(255, 255, 255, 255)"));
	}
	
}


void FrmMainWindows::customEvent(QEvent* event)
{
	switch (event->type())
	{
	case QAvEvent::QAvEvent_MaxWindows:{
		AvQDebug("max windows\n");
		CAvPreview::SetSpiltScreen(ui->FrmVideoArea->GetSplitCount());
		on_BtnMax_clicked();
	}break;
	case QAvEvent::QAvEvent_SearchDevice:{
		AvQDebug("Recv QavEvent \n");
	}break;

	case QAvEvent::QAvEvent_MediaInfo:{
		AvQDebug("Media Info\n");
		on_BtnVideoWindowInfo_clicked();
	}break;
	case QAvEvent::QAvEvent_ShowMainMenum:{
		if (true == CAvGuiStatus::instance()->IsEmbeddedSystem()){
			AvQDebug("Open main Menum\n");
			if (m_FrmMainMenu == NULL){
				m_FrmMainMenu = new FrmMainMenu;
			}
			m_FrmMainMenu->show();
			CAvUiComm::FormInCenter(m_FrmMainMenu);
		}
		else{
			on_BtnOpenMenu_clicked();
		}

	}break;

	case QAvEvent::QAvEvent_MainMenuDevices_Click:{
		AvQDebug("Recv QavEvent MainMenuDevices\n");
		on_BtnDevices_clicked();
	}break;
	case QAvEvent::QAvEvent_MainMenuSystemSet_Click:{
		AvQDebug("Recv QavEvent MainMenuSystemSet\n");
		on_BtnSysSet_clicked();
	}break;
	case QAvEvent::QAvEvent_MainMenuAlarm_Click:{
		AvQDebug("Recv QavEvent MainMenuAlarm\n");
		//on_BtnAlarm_clicked();
	}break;
	case QAvEvent::QAvEvent_MainMenuSysTools_Click:{
		AvQDebug("Recv QavEvent MainMenuSysTools\n");
		//on_BtnSysInfo_clicked();
	}break;
	case QAvEvent::QAvEvent_MainMenuRecord_Click:{
		AvQDebug("Recv QavEvent MainMenuRecord\n");
	}break;
	case QAvEvent::QAvEvent_MainMenuIntelligenceAlgorthm_Click:{
		AvQDebug("Recv QavEvent MainMenuIntelligenceAlgorthm\n");
	}break;
	case QAvEvent::QAvEvent_MainMenuSystemInfo_Click:{
		AvQDebug("Recv QavEvent MainMenuSystemInfo\n");
	}break;

	case QAvEvent::QAvEvent_MaxVideoAreaWindows:{
		AvQDebug("Recv QAvEvent_MaxVideoAreaWindows\n");
		CAvPreview::SetSpiltScreen(ui->FrmVideoArea->GetSplitCount());
		m_VideoWidowsMax = true;
		ui->FrmVideoArea->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		ui->FrmVideoArea->showMaximized();
		QRect cur = CAvUiComm::GetWindowsOnScreen();
		ui->FrmVideoArea->setGeometry(cur);
		
	}break;
	case QAvEvent::QAvEvent_SysAlarmMsg:{
		SetBtnAlarmColor(true);
		QAvEvent *QAve = (QAvEvent*)event;
		C_AlmMsg Msg = *((C_AlmMsg *)QAve->UsrData());
		m_DlgAlarmMessage->AddAlmItem(Msg);
		//av_error("QAvEvent::QAvEvent_SysAlarmMsg\n");
	}break;
	case QAvEvent::QAvEvent_ShowRecordPlay:{
		on_BtnRecordPlay_clicked();
	}break;

	case QAvEvent::QAvEvent_ShowRecordSet:{
		on_BtnRecordSet_clicked();
	}break;

	case QAvEvent::QAvEvent_ShowDeviceSet:{
		QAvEvent *QAve = (QAvEvent*)event;

		int *pChannel = (int *)QAve->UsrData();
		AvQDebug("IPC Setting Channel = %d\n", *pChannel);
		m_frmDevices->DlgDeviceSetShow(*pChannel);

	}break;

	case QAvEvent::QAvEvent_RecordPlay_Close:{
		AvQDebug("Recv QAvEvent_RecordPlay_Close\n");
		ui->RecordSearch->hide();
		ui->RecordShowBar->hide();
	}
	default:
		break;
	}

}

void FrmMainWindows::on_BtnMin_clicked()
{
	this->showMinimized();
}

void FrmMainWindows::on_BtnMax_clicked()
{
	if (m_WidowsMax == true){
		this->setGeometry(m_localRect);
		IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
		ui->BtnMax->setToolTip(tr("MaxWindows"));
	}
	else {
		m_localRect = this->geometry();
		this->setGeometry(CAvUiComm::GetWindowsOnScreen());
		IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf079), 10);
		ui->BtnMax->setToolTip(tr("RestoredWindow"));
	}

	m_WidowsMax = !m_WidowsMax;
}

void FrmMainWindows::on_BtnClose_clicked()
{
	//this->close();
	int ret = CAvUiComm::ShowMessageBoxQuesion(QString(tr("Are you sure close windows")));
	if (ret == QDialog::Accepted){
		exit(0);
	}
	
}

void FrmMainWindows::on_BtnOpenMenu_clicked()
{
	std::string Value;
	if (av_true == CAvDevice::GetEnv(EKey_Chip, Value)){
		if (Value == std::string("H3536")){
			return;
		}
		else{
			AvQDebug("GetEnv Key_Chip [%s]\n", Value.c_str());
		}
	}
	else{
		AvQDebug("Get Chip Error return Error\n");
		return;
	}
	if (m_bOpenedMenu == false){
		ui->FrmMenu->show();
	}
	else{
		ui->FrmMenu->hide();
	}

	m_bOpenedMenu = !m_bOpenedMenu;
}

void FrmMainWindows::on_BtnDevices_clicked()
{
	CAvUiComm::FormInCenter(m_frmDevices);
	m_frmDevices->exec();
}

void FrmMainWindows::on_BtnSysSet_clicked()
{

}

// 
// void FrmMainWindows::on_BtnAlm_clicked()
// {
// 
// }


void FrmMainWindows::on_BtnAlgorithm_clicked()
{

}

void FrmMainWindows::on_BtnVideoWindowInfo_clicked()
{

	ui->FrmVideoArea->setVideoInfoWindows();

}

void FrmMainWindows::on_BtnAbout_clicked()
{
    DlgAbout *about = new DlgAbout;
    about->exec();
}

void FrmMainWindows::on_BtnAlarm_clicked()
{
	SetBtnAlarmColor(false);
	m_DlgAlarmMessage->exec();
}
void FrmMainWindows::on_BtnRecordSet_clicked()
{
	 m_DlgRecordSet->exec();
}

void FrmMainWindows::on_BtnRecordPlay_clicked()
{
	ui->FrmMenu->hide();
	ui->RecordSearch->show();
	ui->RecordShowBar->show();
}

void FrmMainWindows::on_BtnTools_clicked()
{
#if defined(_AV_WARE_CODE_OPENSOURCE)
#else
	DlgMkFirmWare *mkPacket = new DlgMkFirmWare(this);
	mkPacket->exec();
#endif

}
