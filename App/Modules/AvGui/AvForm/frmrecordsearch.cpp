#include "frmrecordsearch.h"
#include "ui_frmrecordsearch.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvSource/QAvEvent.h"
#include "AvGuiApplication.h"

FrmRecordSearch::FrmRecordSearch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmRecordSearch)
{
    ui->setupUi(this);
	ui->calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	m_ChannelItemModel = NULL;
	FixUiDlg();
}

FrmRecordSearch::~FrmRecordSearch()
{
    delete ui;
}

#define D_RECORDSEARCH_NO 0
#define D_RECORDSEARCH_CHANEL 1
#define D_RECORDSEARCH_NR	2


void FrmRecordSearch::FixUiDlg()
{
	m_ChannelItemModel = new QStandardItemModel;
	m_ChannelItemModel->setColumnCount(D_RECORDSEARCH_NR);
	m_ChannelItemModel->setHeaderData(D_RECORDSEARCH_NO, Qt::Horizontal, tr("No"));
	m_ChannelItemModel->setHeaderData(D_RECORDSEARCH_CHANEL, Qt::Horizontal, tr("Channel"));

	ui->TViewRecordSearchChannel->setModel(m_ChannelItemModel);
	ui->TViewRecordSearchChannel->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TViewRecordSearchChannel->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TViewRecordSearchChannel->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TViewRecordSearchChannel->verticalHeader()->hide();

	{

		for (int i = 0; i < SYS_CHN_NUM; i++){
			m_ChannelItemModel->setItem(i, D_RECORDSEARCH_NO, new QStandardItem(QString::number(i)));
			m_ChannelItemModel->item(i, D_RECORDSEARCH_NO)->setCheckable(true);
			QString ChannelName(tr("Channel_"));
			ChannelName += QString::number(i);
			m_ChannelItemModel->setItem(i, D_RECORDSEARCH_CHANEL, new QStandardItem(ChannelName));
		}
	}
}
void FrmRecordSearch::on_BtnClose_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_RecordPlay_Close);
	g_AvGuiApp.PostQAvEvent(AvEvent);
}
