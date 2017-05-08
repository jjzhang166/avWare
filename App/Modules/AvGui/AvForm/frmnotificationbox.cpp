#include "frmnotificationbox.h"
#include "ui_frmnotificationbox.h"
#include <QTimer>
#include "AvUiComm/IconComm.h"
#include "AvUiComm/AvUiComm.h"
FrmNotificationBox::FrmNotificationBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmNotificationBox)
{
	ui->setupUi(this);
	m_ShowTimeOut = 3000;
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
    IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);
    IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	connect(ui->BtnClose, SIGNAL(clicked()), this, SLOT(on_BtnClose_clicked()));
}

FrmNotificationBox::~FrmNotificationBox()
{
    delete ui;
}
void FrmNotificationBox::SetShowInfo(QString &Title, QString &Message, int ShowTimeOut)
{
	m_ShowTimeOut = ShowTimeOut;
	ui->LabMessage->setText(Message);
	ui->LabTitle->setText(Title);
}
void FrmNotificationBox::on_BtnClose_clicked()
{
	AvQDebug("BtnClose\n");
	hide();
}

void FrmNotificationBox::showEvent(QShowEvent *event)
{
	
	QTimer::singleShot(m_ShowTimeOut, this, SLOT(SlotsInitTimer()));
	CAvUiComm::FormInLeftDwon(this);
}

void FrmNotificationBox::SlotsInitTimer()
{
	AvQDebug("Time out\n");
	hide();
}
