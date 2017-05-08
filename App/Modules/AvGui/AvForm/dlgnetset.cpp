#include "dlgnetset.h"
#include "ui_dlgnetset.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvSource/AvQStringsLanguage.h"
#include "AvGui/AvGui.h"


DlgNetSet::DlgNetSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgNetSet)
{
    ui->setupUi(this);



	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	//this->setAttribute(Qt::WA_DeleteOnClose);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnMenu, QChar(0xf0c9), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);
	FixDlgUi();
}

DlgNetSet::~DlgNetSet()
{
	AvQDebug("clear up\n");
    delete ui;
}
void DlgNetSet::SetModifyDeviceList(std::list<C_DevSearch> &ModifyList)
{
	m_ModifyList = &ModifyList;

	int width_temp = 0;
	std::list <C_DevSearch>::iterator ilist;
	C_DevSearch DevSearchInfo;
	int i = 0;
	for (ilist = m_ModifyList->begin(); ilist != m_ModifyList->end(); ilist++){
		DevSearchInfo = *ilist;
		m_TviewDeviceTableModel->setItem(i, 0, new QStandardItem(QString::number(i)));
		m_TviewDeviceTableModel->setItem(i, 1, new QStandardItem(QString(ilist->DevStatusInfo.DeviceName)));
		m_TviewDeviceTableModel->setItem(i, 2, new QStandardItem(QString(ilist->NetWorkProfile.Mac)));
		QIcon icon(":/image/info.png");
		m_TviewDeviceTableModel->setItem(i, 3, new QStandardItem(icon, QString(tr("Wait For Modify"))));
		i++;
	}

	if (m_ModifyList->size() == 1){
		DevSearchInfo = *m_ModifyList->begin();
		ui->LiEditDns1->settext(QString(DevSearchInfo.NetWorkProfile.DnsI));
		ui->LiEditDns2->settext(QString(DevSearchInfo.NetWorkProfile.DnsII));
		ui->LiEditAddrIp->settext(QString(DevSearchInfo.NetWorkProfile.Ipv4));
		ui->LiEditAddrMac->setText(QString(DevSearchInfo.NetWorkProfile.Mac));
		ui->LiEditAddrMac->setEnabled(false);
		ui->LiEditGateWay->settext(QString(DevSearchInfo.NetWorkProfile.GateWay));
		ui->LiEditSubWay->settext(QString(DevSearchInfo.NetWorkProfile.SubMask));
		ui->LiEditPort->setText(QString::number(DevSearchInfo.NetWorkProfile.ServicePort));
	}
	else{
		ui->LabAddrIp->setText(QString(tr("StartIp")));
		ui->LiEditAddrIp->settext(QString("192.168.1.10"));
	}
}

void DlgNetSet::FixDlgUi()
{
	{
		ui->CBoxGetMode->setStyleSheet("color:black");
		ui->CBoxiFrname->setStyleSheet("color:black");
		ui->CBoxWifiList->setStyleSheet("color:black");
		for (int i = NetCommT_LAN0; i < NetCommT_LAST; i++){
			ui->CBoxiFrname->addItem(AvUiLangsNetCommT((NetCommT)i));
		}
		for (int i = NetCommGetMode_NONE + 1; i < NetCommGetMode_LAST; i++){
			ui->CBoxGetMode->addItem(AvUiLangsNetCommGetMode((NetCommGetMode)i));
		}

		ui->LiEditDns1->settext(QString("8.8.8.8"));
		ui->LiEditDns2->settext(QString("8.8.8.8"));
		ui->LiEditAddrIp->settext(QString("192.168.1.90"));
		ui->LiEditAddrMac->setText(QString("08:01:02:03:04:06"));
		ui->LiEditAddrMac->setEnabled(false);
		ui->LiEditGateWay->settext(QString("192.168.1.1"));
		ui->LiEditSubWay->settext(QString("255.255.255.0"));
		ui->LiEditPort->setText(QString::number(5000));
		
	}
	m_TviewDeviceTableModel = new QStandardItemModel;
	m_TviewDeviceTableModel->setColumnCount(4);
	m_TviewDeviceTableModel->setHeaderData(0, Qt::Horizontal, tr("Nu"));
	m_TviewDeviceTableModel->setHeaderData(1, Qt::Horizontal, tr("DevName"));
	m_TviewDeviceTableModel->setHeaderData(2, Qt::Horizontal, tr("StrMac"));
	m_TviewDeviceTableModel->setHeaderData(3, Qt::Horizontal, tr("status"));


	ui->TviewDeviceList->setModel(m_TviewDeviceTableModel);
	ui->TviewDeviceList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewDeviceList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewDeviceList->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TviewDeviceList->verticalHeader()->hide();
	QObject::connect((const QObject *)ui->TviewDeviceList->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(SlotsHorizontalScrollBarRangChanged(int, int)));
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}

void DlgNetSet::resizeEvent(QResizeEvent * event)
{
	QRect Rect = this->rect();
	ui->TviewDeviceList->setFixedWidth(Rect.width() / 3 * 2);

	QSize viewSize = ui->TviewDeviceList->viewport()->size();
	ui->TviewDeviceList->setColumnWidth(0, 0.10*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(1, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(2, 0.40*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(3, 0.30*viewSize.width());
	AvQDebug("resizeEvent\n");
}
void DlgNetSet::SlotsHorizontalScrollBarRangChanged(int min, int max)
{
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}
void DlgNetSet::on_BtnClose_clicked()
{
	QTimer::singleShot(200, this, SLOT(close()));
}


void DlgNetSet::on_BtnSave_clicked()
{
	C_NetWorkProfile NetWorkProfile;
	NetWorkProfile.NetDevice = (NetCommT)ui->CBoxiFrname->currentIndex();
	NetWorkProfile.NetGetType = (NetCommGetMode)ui->CBoxiFrname->currentIndex();
	sprintf(NetWorkProfile.Ipv4, ui->LiEditAddrIp->text().toStdString().c_str());
	sprintf(NetWorkProfile.SubMask, ui->LiEditSubWay->text().toStdString().c_str());
	sprintf(NetWorkProfile.GateWay, ui->LiEditGateWay->text().toStdString().c_str());
	sprintf(NetWorkProfile.DnsI, ui->LiEditDns1->text().toStdString().c_str());
	sprintf(NetWorkProfile.DnsII, ui->LiEditDns2->text().toStdString().c_str());
	

	std::list <C_DevSearch>::iterator ilist;
	C_DevSearch DevSearchInfo;
	int i = 0;
	av_bool ret;
	struct in_addr Base_in_addr;
	Base_in_addr.s_addr = inet_addr(NetWorkProfile.Ipv4);
	for (ilist = m_ModifyList->begin(); ilist != m_ModifyList->end(); ilist++){
		DevSearchInfo = *ilist;
		sprintf(NetWorkProfile.Mac, DevSearchInfo.NetWorkProfile.Mac);
		sprintf(NetWorkProfile.Ipv4, inet_ntoa(Base_in_addr));
		Base_in_addr.s_addr += 1;
		ret = CAvGui::NetCaptureSetNetProfile(NetWorkProfile);
		if (ret == av_true){
			QIcon icon(":/image/info.png");
			m_TviewDeviceTableModel->setItem(i, 3, new QStandardItem(icon, QString("succeed")));
		}
		else{
			QIcon icon(":/image/info.png");
			m_TviewDeviceTableModel->setItem(i, 3, new QStandardItem(icon, QString("failed")));
		}
		i++;
	}
}

void DlgNetSet::on_BtnCannel_clicked()
{
	QTimer::singleShot(200, this, SLOT(close()));
}
