#include "frmmainwindows.h"
#include "ui_frmmainwindows.h"
#include "AvUiComm/IconComm.h"
#include "AvUiComm/AvUiComm.h"
#include "AvForm/frmdevice.h"
#include "AvForm/dlgnetset.h"
#include "AvUiComm/FloatingButton.h"
FrmMainWindows::FrmMainWindows(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMainWindows)
{
    ui->setupUi(this);
	CAvUiComm::FormInCenter(this);
	

	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
	m_WidowsMax = false;
	m_mousePressed = false;
	m_localRect = this->geometry();
	ui->LabTitle->installEventFilter(this);

	m_bOpenedMenu = false;
	ui->FrmMenu->hide();
	
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 8);
	IconComm::Instance()->SetIcon(ui->BtnMax,   QChar(0xf096), 8);
	IconComm::Instance()->SetIcon(ui->BtnMin,   QChar(0xf068), 8);
	IconComm::Instance()->SetIcon(ui->BtnMenu,  QChar(0xf0c9), 8);
	IconComm::Instance()->SetIcon(ui->BtnVideoWindowInfo, QChar(0xf129), 8);
	IconComm::Instance()->SetIcon(ui->LabIco,   QChar(0xf015), 8);

// 	m_FloatButton = new FloatingButton(parent);
// 	QPoint point = this->pos();
// 	QRect rect = this->rect();
// 	point.setX(point.x() + rect.width() / 3 * 2);
// 	point.setY(point.y() + rect.height() / 3 * 2);
// 	m_FloatButton->move(point);
// 	QObject::connect(m_FloatButton, SIGNAL(SignalsButtonClick()), this, SLOT(on_BtnOpenMenu_clicked()));
// 	m_FloatButton->show();


}

FrmMainWindows::~FrmMainWindows()
{
	AvQDebug("clear up\n");
    delete ui;
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


void FrmMainWindows::customEvent(QEvent* event)
{
	switch (event->type())
	{
	case QAvEvent::QAvEvent_SearchDevice:{
		AvQDebug("Recv QavEvent \n");
		
	}
		break;

	default:
		break;
	}

}

void FrmMainWindows::on_BtnMenu_clicked()
{
	CAvUiComm::ShowMessageBoxInfo(QString(tr("This is a Test Box info")));
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
	FrmDevice *fWDevices = new FrmDevice;
	QObject::connect(fWDevices, SIGNAL(SignalPreviewStart(int, int, bool)), ui->FrmVideoArea, SLOT(SlotOnPreviewStart(int, int, bool)), Qt::QueuedConnection);
	fWDevices->exec();
}

void FrmMainWindows::on_BtnSysSet_clicked()
{
	DlgNetSet *DlgnetSet = new DlgNetSet;
	DlgnetSet->exec();
}

void FrmMainWindows::on_BtnAlarm_clicked()
{

}

void FrmMainWindows::on_BtnAlm_clicked()
{

}

void FrmMainWindows::on_BtnSysInfo_clicked()
{

}

void FrmMainWindows::on_BtnAlgorithm_clicked()
{

}

void FrmMainWindows::on_BtnVideoWindowInfo_clicked()
{

	ui->FrmVideoArea->setVideoInfoWindows();

}
