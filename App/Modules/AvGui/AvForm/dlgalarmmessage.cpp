#include "dlgalarmmessage.h"
#include "ui_dlgalarmmessage.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvSource/AvQStringsLanguage.h"

DlgAlarmMessage::DlgAlarmMessage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAlarmMessage)
{
    ui->setupUi(this);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);

	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
        //this->setAttribute(Qt::WA_DeleteOnClose);

	FixDlgUi();



}

DlgAlarmMessage::~DlgAlarmMessage()
{
    delete ui;
}

#define D_ALARMESSAGE_HEADSECTION_NO 0
#define D_ALARMESSAGE_HEADSECTION_CHANNEL 1
#define D_ALARMESSAGE_HEADSECTION_TIME 2
#define D_ALARMESSAGE_HEADSECTION_ALARMTYPE 3
#define D_ALARMESSAGE_HEADSECTION_STATUS	4
#define D_ALARMESSAGE_HEADSECTION_NR 5
void 	DlgAlarmMessage::FixDlgUi()
{
	m_MessageItemModel = new QStandardItemModel;
	m_MessageItemModel->setColumnCount(D_ALARMESSAGE_HEADSECTION_NR);
	m_MessageItemModel->setHeaderData(D_ALARMESSAGE_HEADSECTION_NO, Qt::Horizontal, tr("Nu"));
	m_MessageItemModel->setHeaderData(D_ALARMESSAGE_HEADSECTION_CHANNEL, Qt::Horizontal, tr("Channel"));
	m_MessageItemModel->setHeaderData(D_ALARMESSAGE_HEADSECTION_TIME, Qt::Horizontal, tr("Time"));
	m_MessageItemModel->setHeaderData(D_ALARMESSAGE_HEADSECTION_ALARMTYPE, Qt::Horizontal, tr("AlmType"));
	m_MessageItemModel->setHeaderData(D_ALARMESSAGE_HEADSECTION_STATUS, Qt::Horizontal, tr("AlmStatus"));

	ui->TViewAlarmMessageAlarmInfo->setModel(m_MessageItemModel);
	ui->TViewAlarmMessageAlarmInfo->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TViewAlarmMessageAlarmInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TViewAlarmMessageAlarmInfo->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TViewAlarmMessageAlarmInfo->verticalHeader()->hide();
}
void DlgAlarmMessage::on_BtnAlarmMessageDeleteAll_clicked()
{
	m_MessageItemModel->removeRows(0, m_MessageItemModel->rowCount());
}

void DlgAlarmMessage::AddAlmItem(C_AlmMsg &Msg)
{
	int index = m_MessageItemModel->rowCount();
	struct tm Tm = {0, 0};
	time_t t = Msg.AlarmTime;
#if defined (_WIN32)
	if (0 != localtime_s(&Tm, &t)){
		AvQError("localtime_s error\n");
		return;
	}
#else
	if (NULL == localtime_r((const time_t *)&t, &Tm)){
		AvQError("localtime_r error\n");
		return;
	}
#endif

	char timeData[128] = {0};
	sprintf(timeData, "%04d-%02d-%02d %02d:%02d:%02d", Tm.tm_year + 1900, Tm.tm_mon + 1, Tm.tm_mday, Tm.tm_hour, Tm.tm_min, Tm.tm_sec);
	AvQDebug("alarm time [%d][%s]\n", Msg.AlarmTime, timeData);
	
	m_MessageItemModel->setItem(index, D_ALARMESSAGE_HEADSECTION_NO, new QStandardItem(QString::number(index + 1)));
	m_MessageItemModel->setItem(index, D_ALARMESSAGE_HEADSECTION_CHANNEL, new QStandardItem(QString::number(Msg.Channel)));
	m_MessageItemModel->setItem(index, D_ALARMESSAGE_HEADSECTION_TIME, new QStandardItem(QString(timeData)));
	m_MessageItemModel->setItem(index, D_ALARMESSAGE_HEADSECTION_ALARMTYPE, new QStandardItem(AvUiLangsAlarmEvent(Msg.AlarmEventName)));
	m_MessageItemModel->setItem(index, D_ALARMESSAGE_HEADSECTION_STATUS, new QStandardItem(AvUiLangsAlarmStat(Msg.AlarmStatus)));
}
void DlgAlarmMessage::ResizeUi()
{
	QSize viewSize = ui->TViewAlarmMessageAlarmInfo->viewport()->size();

	int TviewWidth = viewSize.width() + 3;
	ui->TViewAlarmMessageAlarmInfo->setColumnWidth(D_ALARMESSAGE_HEADSECTION_NO, 0.10*TviewWidth);
	ui->TViewAlarmMessageAlarmInfo->setColumnWidth(D_ALARMESSAGE_HEADSECTION_CHANNEL, 0.10*TviewWidth);
	ui->TViewAlarmMessageAlarmInfo->setColumnWidth(D_ALARMESSAGE_HEADSECTION_TIME, 0.30*TviewWidth);
	ui->TViewAlarmMessageAlarmInfo->setColumnWidth(D_ALARMESSAGE_HEADSECTION_ALARMTYPE, 0.30*TviewWidth);
	ui->TViewAlarmMessageAlarmInfo->setColumnWidth(D_ALARMESSAGE_HEADSECTION_STATUS, 0.20*TviewWidth);
}
void DlgAlarmMessage::on_BtnClose_clicked()
{
    close();
}

void DlgAlarmMessage::showEvent(QShowEvent *e)
{
	ResizeUi();
}

void DlgAlarmMessage::resizeEvent(QResizeEvent *e)
{
	ResizeUi();
}