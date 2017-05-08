#include "frmdevice.h"
#include "ui_frmdevice.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvForm/dlgnetset.h"
#include "AvForm/dlgupgrade.h"

#include "AvForm/dlghandadddevice.h"
#include "AvForm/dlgdeviceset.h"
#include "AvGuiSystem.h"
#include <QString>
#include "AvUiComm/StdListDevicesSort.h"
#include "AvGui/AvGui.h"
#include "AvSource/AvQStringsLanguage.h"

#if defined(_AV_WARE_CODE_OPENSOURCE)
#else
#include "AvForm/dlgfactoryset.h"
#endif

FrmDevice::FrmDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrmDevice)
{
    ui->setupUi(this);
    m_WidowsMax = false;
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnMenu, QChar(0xf0c9), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);



	
	m_DeviceSet = NULL;
	m_DeviceSet = new DlgDeviceSet(this);
	m_DeviceSet->hide();
#if defined(_AV_WARE_CODE_OPENSOURCE)
#else
	m_FactorySet = NULL;
#endif
	
	m_Upgrade = NULL;
	m_NetSet = NULL;

	FixDlgUi();
	m_DeviceList.clear();

	m_mouseSearchRightMenu = NULL;
	m_SearchItemModelcolumn = -1;
	m_SearchItemModelrow = -1;


	m_mouseDevicesRightMenu = NULL;
	m_DevicesItemModelcolumn = -1;
	m_DevicesItemModelrow  = -1;
	m_bSearching = false;

	
#if defined(_AV_WARE_CODE_OPENSOURCE)
	ui->BtnFacInfo->hide();
#endif

}

FrmDevice::~FrmDevice()
{
	AvQDebug("clear up\n");
    delete ui;
}

void FrmDevice::DlgDeviceSetShow(int ModifyChannel)
{

	CAvUiComm::FormInCenter(m_DeviceSet);
	m_DeviceSet->SelectModifyChannel(ModifyChannel);
	m_DeviceSet->exec();
}

void FrmDevice::GetSelectForModiyDevies()
{
	std::list <C_DevSearch>::iterator ilist;
	m_ForModifyDeviceList.clear();
	for (int i = 0; i < m_SearchItemModel->rowCount(); i++){
		if (m_SearchItemModel->item(i, 0)->checkState() == Qt::Checked){
			QString SearchWgMacAddr = m_SearchItemModel->item(i, 4)->text();
			for (ilist = m_DeviceList.begin(); ilist != m_DeviceList.end(); ilist++){
				if (SearchWgMacAddr == QString(ilist->NetWorkProfile.Mac)){
					m_ForModifyDeviceList.push_back(*ilist);
				}
			}
		}
	}
}

#define D_SEARCHITEM_HEADSECTION_NO				0
#define D_SEARCHITEM_HEADSECTION_DEVNAME		1
#define D_SEARCHITEM_HEADSECTION_IPADDR			2
#define D_SEARCHITEM_HEADSECTION_GATEWAY		3
#define D_SEARCHITEM_HEADSECTION_MACADDR		4
#define D_SEARCHITEM_HEADSECTION_CHANNELS		5
#define D_SEARCHITEM_HEADSECTION_PORTS			6
#define D_SEARCHITEM_HEADSECTION_STARTUP		7
#define D_SEARCHITEM_HEADSECTION_NR				8

#define D_DEVICES_HEADSECTION_CHNO				0
#define D_DEVICES_HEADSECTION_IPADDR			1
#define D_DEVICES_HEADSECTION_PORT				2
#define D_DEVICES_HEADSECTION_PROTOCOL			3
#define D_DEVICES_HEADSECTION_STAT				4
#define D_DEVICES_HEADSECTION_NR				5

void FrmDevice::FixDlgUi()
{

	m_SearchItemModel = new QStandardItemModel;
	m_SearchItemModel->setColumnCount(D_SEARCHITEM_HEADSECTION_NR);
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_NO,		Qt::Horizontal, tr("Nu"));
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_DEVNAME,	Qt::Horizontal, tr("DevName"));
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_IPADDR, Qt::Horizontal, tr("StrIp"));
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_GATEWAY, Qt::Horizontal, tr("GateWay"));
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_MACADDR, Qt::Horizontal, tr("StrMac"));
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_CHANNELS, Qt::Horizontal, tr("Chns"));
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_PORTS, Qt::Horizontal, tr("PortNu"));
	m_SearchItemModel->setHeaderData(D_SEARCHITEM_HEADSECTION_STARTUP, Qt::Horizontal, tr("Startup"));
	
	ui->TviewSearchDevice->setModel(m_SearchItemModel);
	ui->TviewSearchDevice->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewSearchDevice->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewSearchDevice->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TviewSearchDevice->verticalHeader()->hide();


	m_DevicesItemModel = new QStandardItemModel;
	m_DevicesItemModel->setColumnCount(D_DEVICES_HEADSECTION_NR);
	m_DevicesItemModel->setHeaderData(D_DEVICES_HEADSECTION_CHNO, Qt::Horizontal, tr("Chno"));
	m_DevicesItemModel->setHeaderData(D_DEVICES_HEADSECTION_IPADDR, Qt::Horizontal, tr("IpAddr"));
	m_DevicesItemModel->setHeaderData(D_DEVICES_HEADSECTION_PORT, Qt::Horizontal, tr("Port"));
	m_DevicesItemModel->setHeaderData(D_DEVICES_HEADSECTION_PROTOCOL, Qt::Horizontal, tr("Protocol"));
	m_DevicesItemModel->setHeaderData(D_DEVICES_HEADSECTION_STAT, Qt::Horizontal, tr("Status"));
	ui->TviewChnDevice->setModel(m_DevicesItemModel);
	ui->TviewChnDevice->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewChnDevice->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewChnDevice->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TviewChnDevice->verticalHeader()->hide();

	QObject::connect((const QObject *)ui->TviewSearchDevice->horizontalScrollBar(),
		SIGNAL(rangeChanged(int, int)), this, SLOT(SlotsHorizontalScrollBarRangChanged(int, int)));
	QObject::connect((const QObject *)ui->TviewChnDevice->horizontalScrollBar(),
		SIGNAL(rangeChanged(int, int)), this, SLOT(SlotsHorizontalScrollBarRangChanged(int, int)));

	ui->ProgressSearch->setValue(0);


	av_u32 protolMask = 0;
	CAvGui::NetCaptureProtocolsMask(protolMask);
	ui->CBoxProtoType->clear();

	for (int i = 0; i < sizeof(protolMask); i++){
		if (!(AvMask(i) & protolMask)) continue;
		ui->CBoxProtoType->addItem(AvUiLangsLinkProtocol((LinkProtocol)i));
	}

	

	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}

void FrmDevice::FixViewItem()
{

	QSize viewSize = ui->TviewSearchDevice->viewport()->size();
	int TviewWidth = viewSize.width() + 3;
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_NO, 0.08*TviewWidth);
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_DEVNAME, 0.13*TviewWidth);
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_IPADDR, 0.15*TviewWidth);
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_GATEWAY, 0.15*TviewWidth);
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_MACADDR, 0.15*TviewWidth);
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_CHANNELS, 0.07*TviewWidth);
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_PORTS, 0.10*TviewWidth);
	ui->TviewSearchDevice->setColumnWidth(D_SEARCHITEM_HEADSECTION_STARTUP, 0.17*TviewWidth);


	viewSize = ui->TviewChnDevice->viewport()->size();
	TviewWidth = viewSize.width();
	ui->TviewChnDevice->setColumnWidth(D_DEVICES_HEADSECTION_CHNO, 0.10*TviewWidth);
	ui->TviewChnDevice->setColumnWidth(D_DEVICES_HEADSECTION_IPADDR, 0.30*TviewWidth);
	ui->TviewChnDevice->setColumnWidth(D_DEVICES_HEADSECTION_PORT, 0.25*TviewWidth);
	ui->TviewChnDevice->setColumnWidth(D_DEVICES_HEADSECTION_PROTOCOL, 0.25*TviewWidth);
	ui->TviewChnDevice->setColumnWidth(D_DEVICES_HEADSECTION_STAT, 0.10*TviewWidth);
}
void FrmDevice::resizeEvent(QResizeEvent * event)
{

	FixViewItem();

}
void FrmDevice::showEvent(QShowEvent *e)
{
	FixViewItem();
	DrawItemLinkView();
}

void FrmDevice::DrawItemForLinkView()
{
	std::list<C_DevSearch>::iterator iList;
	int i = m_DevicesItemModel->rowCount();
	for ( iList = m_LinkDeviceList.begin(); iList != m_LinkDeviceList.end(); iList++, i++){
		m_DevicesItemModel->setItem(i, D_DEVICES_HEADSECTION_CHNO, new QStandardItem(QString::number(iList->ManufacturerInfo.FacTime)));
		m_DevicesItemModel->item(i, D_DEVICES_HEADSECTION_CHNO)->setCheckable(true);
		m_DevicesItemModel->setItem(i, D_DEVICES_HEADSECTION_IPADDR, new QStandardItem(QString(iList->NetWorkProfile.Ipv4)));
		m_DevicesItemModel->setItem(i, D_DEVICES_HEADSECTION_PORT, new QStandardItem(QString::number(iList->NetWorkProfile.ServicePort)));
		m_DevicesItemModel->setItem(i, D_DEVICES_HEADSECTION_PROTOCOL, new QStandardItem(QString(tr("Protocol"))));
		QIcon icon(":/image/info.png");
		m_DevicesItemModel->setItem(i, D_DEVICES_HEADSECTION_STAT, new QStandardItem(icon, QString("")));
	}
}

void FrmDevice::DrawItemLinkView()
{
	int TotalChns = CAvGui::NetCaptureGetTotalNm();
//	m_DevicesItemModel->clear();
	m_DevicesItemModel->removeRows(0, m_DevicesItemModel->rowCount());
	for (int i = 0, j = 0; i < TotalChns; i++){
// 		Capture::EAvCaptureStatus stat = CAvGui::CaptureStatus(i);
// 		if (stat != Capture::EAvCapture_STOP){
			Capture *pCapture = CAvGui::CaptureInstance(i);
			CAvNetProto *pNetProto = pCapture->AvNetProtoHandle();
			if (pNetProto == NULL){
				continue;
			}
			C_ProtoFormats &ProtoFormats = pNetProto->ProtoFromats();
			QString Address;
			QString ProtoString;
			QString ServicePort;

			switch (ProtoFormats.ProtoMode)
			{
			case ProtocolOnvif:
				ProtoString.append(AvUiLangsLinkProtocol(LinkProtocol_Onvif));
				break;
			case 	ProtocolRtmp:
				ProtoString.append(AvUiLangsLinkProtocol(LinkProtocol_RTSP));
				break;
			case 	ProtocolRtsp:
				ProtoString.append(AvUiLangsLinkProtocol(LinkProtocol_RTSP));
				break;
			case 	ProtocolMoon:
			{
				Address.append(ProtoFormats.MoonFormats.Url);
				ServicePort = QString::number(ProtoFormats.MoonFormats.Port);
				ProtoString.append(AvUiLangsLinkProtocol(LinkProtocol_Moon));
			}
			break;

			default:
				break;
			}
			
			m_DevicesItemModel->setItem(j, D_DEVICES_HEADSECTION_CHNO, new QStandardItem(QString::number(i)));
			m_DevicesItemModel->item(j, D_DEVICES_HEADSECTION_CHNO)->setCheckable(true);
			m_DevicesItemModel->setItem(j, D_DEVICES_HEADSECTION_IPADDR, new QStandardItem(Address));
			m_DevicesItemModel->setItem(j, D_DEVICES_HEADSECTION_PORT, new QStandardItem(ServicePort));
			m_DevicesItemModel->setItem(j, D_DEVICES_HEADSECTION_PROTOCOL, new QStandardItem(ProtoString));
			QIcon icon(":/image/info.png");
			m_DevicesItemModel->setItem(j, D_DEVICES_HEADSECTION_STAT, new QStandardItem(icon, QString("")));
			//m_DevicesItemModel->setItem(j, D_DEVICES_HEADSECTION_STAT, new QStandardItem::);
			j++;
//		}
	}


}
void FrmDevice::DrawItemForSearchView()
{
	std::list<C_DevSearch>::iterator iList;
	int i = 0;
	
	//del old item;
	//m_SearchItemModel->removeColumns(1, m_SearchItemModel->columnCount() -1);
	m_SearchItemModel->removeRows(0, m_SearchItemModel->rowCount());

	for ( iList = m_DeviceList.begin(); iList != m_DeviceList.end(); iList++, i++){
		m_SearchItemModel->setItem(i, 0, new QStandardItem(QString::number(i + 1)));
		m_SearchItemModel->item(i, 0)->setCheckable(true);
		m_SearchItemModel->setItem(i, 1, new QStandardItem(QString::fromLocal8Bit(iList->DevStatusInfo.DeviceName)));
		m_SearchItemModel->setItem(i, 2, new QStandardItem(iList->NetWorkProfile.Ipv4));
		m_SearchItemModel->setItem(i, 3, new QStandardItem(iList->NetWorkProfile.GateWay));
		m_SearchItemModel->setItem(i, 4, new QStandardItem(iList->NetWorkProfile.Mac));
		m_SearchItemModel->setItem(i, 5, new QStandardItem(QString::number(iList->ManufacturerInfo.ChannelMax)));
		m_SearchItemModel->setItem(i, 6, new QStandardItem(QString::number(iList->NetWorkProfile.ServicePort)));
		int uptime = iList->DevStatusInfo.UpTime;
		int day, hour, min, sec;
		day = uptime / (3600 * 24);
		hour = (uptime % (3600 * 24)) / 3600;
		min = (uptime % 3600) / 60;
		sec = uptime % 60;
		QString StringUptime;
		StringUptime.clear();
		if (day != 0){
			StringUptime = QString::asprintf("%dD", day);
		}
		
		if (day != 0){
			StringUptime += QString::asprintf("-%dH", hour);
		}
		else if (hour != 0){
			StringUptime = QString::asprintf("%dH", hour);
		}

		if (hour != 0 || day != 0){
			StringUptime += QString::asprintf("-%dM", min);
		}
		else if (min != 0){
			StringUptime = QString::asprintf("%dM", min);
		}

		if (min != 0 || day != 0 || hour != 0){
			StringUptime += QString::asprintf("-%dS", sec);
		}
		else{
			StringUptime = QString::asprintf("%dS", sec);
		}
		m_SearchItemModel->setItem(i, 7, new QStandardItem(StringUptime));
	}
}

void FrmDevice::SlotsSearchViewRightMenuSelect()
{
	m_SearchItemModel->item(m_SearchItemModelrow, 0)->setCheckState(Qt::Checked);
}
void FrmDevice::SlotsSearchViewRightMenuSelectAll()
{
	for (int i = 0; i < m_SearchItemModel->rowCount(); i++){
		m_SearchItemModel->item(i, 0)->setCheckState(Qt::Checked);
	}
}
void FrmDevice::SlotsSearchViewRightMenuSelectOther()
{
	for (int i = 0; i < m_SearchItemModel->rowCount(); i++){
		if (m_SearchItemModel->item(i, 0)->checkState() == Qt::Checked){
			m_SearchItemModel->item(i, 0)->setCheckState(Qt::Unchecked);
		}
		else{
			m_SearchItemModel->item(i, 0)->setCheckState(Qt::Checked);
		}
	}
}


void FrmDevice::SlotsSearchViewSortFromLargeToSmall()
{
	AvQDebug("SlotsSortFromLargeToSmall\n");
	switch (m_SearchItemModelcolumn)
	{
	case 2:
		m_DeviceList.sort(CmpareIpAddressFromLargeToSmall());
		break;

	case 3:
		m_DeviceList.sort(CmpareGateAddressFromLargeToSmall());
		break;

	case 7:
		m_DeviceList.sort(CmpareStartUpFromLargeToSmall());
		break;
	default:
		CAvUiComm::ShowMessageBoxError(QString(tr("Not Support")));
		return;
	}
	DrawItemForSearchView();
}
void FrmDevice::SlotsSearchViewSortFromSmall2Large()
{
	AvQDebug("SlotsSortFromSmall2Large\n");
	switch (m_SearchItemModelcolumn)
	{
	case 2:
		m_DeviceList.sort(CmpareIpAddressFromSmallToLarge());
		break;

	case 3:
		m_DeviceList.sort(CmpareGateAddressFromSmallToLarge());
		break;

	case 7:
		m_DeviceList.sort(CmpareStartUpFromSmallToLarge());
		break;
	default:
		CAvUiComm::ShowMessageBoxError(QString(tr("Not Support")));
		return;
	}
	DrawItemForSearchView();
}
void FrmDevice::SlotsHorizontalScrollBarRangChanged(int min, int max)
{
	AvQDebug("SlotsHorizontalScrollBarRangChanged min = %d, max = %d\n", min, max);
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));

}

void FrmDevice::SlotsDevicesViewRightMenuSelect()
{
	m_DevicesItemModel->item(m_DevicesItemModelrow, 0)->setCheckState(Qt::Checked);
}
void FrmDevice::SlotsDevicesViewRightMenuSelectAll()
{
	for (int i = 0; i < m_DevicesItemModel->rowCount(); i++){
		m_DevicesItemModel->item(i, 0)->setCheckState(Qt::Checked);
	}
}
void FrmDevice::SlotsDevicesViewRightMenuSelectOther()
{
	for (int i = 0; i < m_DevicesItemModel->rowCount(); i++){
		if (m_DevicesItemModel->item(i, 0)->checkState() == Qt::Checked){
			m_DevicesItemModel->item(i, 0)->setCheckState(Qt::Unchecked);
		}
		else{
			m_DevicesItemModel->item(i, 0)->setCheckState(Qt::Checked);
		}
	}
}

void FrmDevice::SlotsSearchTimer()
{
	int SearchSecond = 3;
	m_SearchTimerCnt++;

	LinkProtocol PtoType = LinkProtocol_None;
	QString Comcurrtext = ui->CBoxProtoType->currentText();
	if (Comcurrtext == QString("Moon")){
		PtoType = LinkProtocol_Moon;
	}
	else if (Comcurrtext == QString("Onvif")){
		PtoType = LinkProtocol_Onvif;
	}
	else if (Comcurrtext == QString("Rtsp")){
		PtoType = LinkProtocol_RTSP;
	}
	else{
		assert(0);
	}

	if (m_SearchTimerCnt == SearchSecond * 2 + 1){
		ui->ProgressSearch->setValue(0);
		m_bSearching = false;
		return;
	}
	if (m_SearchTimerCnt == SearchSecond * 2){

		ui->ProgressSearch->setValue(100);
		CAvGui::NetCaptureSearchResult(PtoType, m_DeviceList);
		DrawItemForSearchView();
		QTimer::singleShot(1500, this, SLOT(SlotsSearchTimer()));

		return;
	}
	if (m_SearchTimerCnt % 3 == 0){
		CAvGui::NetCaptureSearch(PtoType, av_false);
	}
	ui->ProgressSearch->setValue(m_SearchTimerCnt*(100 / (SearchSecond * 2)));
	QTimer::singleShot(500, this, SLOT(SlotsSearchTimer()));
	
	

}
void FrmDevice::on_BtnClose_clicked()
{
	this->close();
}

void FrmDevice::on_CBoxProtoType_activated(const QString &arg1)
{

}

void FrmDevice::on_BtnSearch_clicked()
{
	if (m_bSearching == true){
		return;
	}
	m_bSearching = true;
	LinkProtocol PtoType = LinkProtocol_None;
	QString Comcurrtext = ui->CBoxProtoType->currentText();
	if (Comcurrtext == QString("Moon")){
		PtoType = LinkProtocol_Moon;
	}
	else if (Comcurrtext == QString("Onvif")){
		PtoType = LinkProtocol_Onvif;
	}
	else if (Comcurrtext == QString("Rtsp")){
		PtoType = LinkProtocol_RTSP;
	}
	else{
		assert(0);
	}

	m_DeviceList.clear();
	CAvGui::NetCaptureSearch(PtoType, av_true);
	QTimer SearchTimer;
	m_SearchTimerCnt = 0;
	QTimer::singleShot(500, this, SLOT(SlotsSearchTimer()));



}

void FrmDevice::on_BtnAddByHand_clicked()
{
	DlgHandAddDevice * HandAddDevice = new DlgHandAddDevice;
	HandAddDevice->exec();
}

void FrmDevice::on_BtnModifNet_clicked()
{
	GetSelectForModiyDevies();
	//DlgNetSet *DlgnetSet = new DlgNetSet;
	if (m_NetSet == NULL)m_NetSet = new DlgNetSet;

	m_NetSet->SetModifyDeviceList(m_ForModifyDeviceList);
	m_NetSet->move(pos());
	m_NetSet->resize(size());
	m_NetSet->exec();
}

void FrmDevice::on_BtnUpgrade_clicked()
{
	GetSelectForModiyDevies();
	//DlgUpgrade *Dlgupgrade = new DlgUpgrade;
	if (m_Upgrade == NULL) m_Upgrade = new DlgUpgrade;

	m_Upgrade->SetModifyDeviceList(m_ForModifyDeviceList);
	m_Upgrade->move(pos());
	m_Upgrade->resize(size());
	m_Upgrade->exec();
}

void FrmDevice::on_BtnFacInfo_clicked()
{
#if defined(_AV_WARE_CODE_OPENSOURCE)
#else
	GetSelectForModiyDevies();
	//DlgFactorySet *Factory = new DlgFactorySet;
	if (NULL == m_FactorySet) m_FactorySet = new DlgFactorySet;

	m_FactorySet->SetModifyDeviceList(m_ForModifyDeviceList);
	m_FactorySet->move(pos());
	m_FactorySet->resize(size());
	m_FactorySet->exec();
#endif


}

void FrmDevice::on_BtnChnDeleteDevice_clicked()
{
	for (int i = 0; i < m_DevicesItemModel->rowCount(); i++){
		if (m_DevicesItemModel->item(i, 0)->checkState() == Qt::Checked){
			int DelChn = m_DevicesItemModel->item(i, 0)->text().toInt();
			emit SignalPreviewStart(DelChn, 0, false);
			CAvGui::NetCaptureDelDevice(DelChn);
		}
	}


	DrawItemLinkView();

	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}

void FrmDevice::on_BtnChnModifDevice_clicked()
{
	m_DeviceSet->move(pos());
	m_DeviceSet->exec();
}



void FrmDevice::on_BtnMax_clicked()
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

void FrmDevice::on_BtnAddDevice_clicked()
{
	GetSelectForModiyDevies();
	m_LinkDeviceList = m_ForModifyDeviceList;
	

	std::list<C_DevSearch>::iterator iList;

	int j = 0;
	C_ProtoFormats ProtoFormats;
	for (iList = m_LinkDeviceList.begin(); iList != m_LinkDeviceList.end(); iList++){
		memset(&ProtoFormats, 0x00, sizeof(C_ProtoFormats));
		ProtoFormats.MoonFormats.Port = iList->NetWorkProfile.ServicePort == 0 ? 5000 : iList->NetWorkProfile.ServicePort;
		sprintf(ProtoFormats.MoonFormats.Url, iList->NetWorkProfile.Ipv4);
		sprintf(ProtoFormats.CheckAliveAddress, iList->NetWorkProfile.Ipv4);
		sprintf(ProtoFormats.UsrName, "admin");
		sprintf(ProtoFormats.Passwd, "admin");
		ProtoFormats.ProtoMode = ProtocolMoon;
		ProtoFormats.IsEnable = av_true;
		int emptyChn = CAvGui::NetCaptureGetEmptyChannel();
		AvQDebug("Get Empty Channel [%d]\n", emptyChn);
		if (emptyChn == -1){
			CAvUiComm::ShowMessageBoxError(QString(tr("Have not Empty Channel!")));
			for (; iList != m_LinkDeviceList.end();){
				iList = m_LinkDeviceList.erase(iList);
			}
			break;
		}
		else{
			iList->ManufacturerInfo.FacTime = emptyChn;
			emit SignalPreviewStart(emptyChn, 0, true);
			CAvGui::NetCaptureAddDevice(emptyChn, ProtoFormats);
		}
	}
	DrawItemForLinkView();
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}


void FrmDevice::on_TviewSearchDevice_customContextMenuRequested(const QPoint &pos)
{
	AvQDebug("this is costom menu\n");
	if (NULL == m_mouseSearchRightMenu){
		m_mouseSearchRightMenu = new QMenu(ui->TviewSearchDevice->horizontalHeader());
		m_mouseSearchRightMenu->addAction(QString(tr("Select")), this, SLOT(SlotsSearchViewRightMenuSelect()));
		m_mouseSearchRightMenu->addSeparator();
		m_mouseSearchRightMenu->addAction(QString(tr("SelectAll")), this, SLOT(SlotsSearchViewRightMenuSelectAll()));
		m_mouseSearchRightMenu->addAction(QString(tr("SelectOther")), this, SLOT(SlotsSearchViewRightMenuSelectOther()));
		m_mouseSearchRightMenu->addSeparator();
		m_mouseSearchRightMenu->addAction(QString(tr("SortLargeT2Small")), this, SLOT(SlotsSearchViewSortFromLargeToSmall()));
		m_mouseSearchRightMenu->addAction(QString(tr("SortSmall2LargeT")), this, SLOT(SlotsSearchViewSortFromSmall2Large()));

	}
	if (m_SearchItemModelrow != -1 && m_SearchItemModel->rowCount() > 0){
		m_mouseSearchRightMenu->exec(QCursor::pos());
	}



}

void FrmDevice::on_TviewSearchDevice_doubleClicked(const QModelIndex &index)
{
	m_SearchItemModelIndex = index;
	m_SearchItemModelrow = index.row();
	m_SearchItemModelcolumn = index.column();

	if (m_SearchItemModel->item(m_SearchItemModelrow, 0)->checkState() == Qt::Checked){
		m_SearchItemModel->item(m_SearchItemModelrow, 0)->setCheckState(Qt::Unchecked);
	}
	else{
		m_SearchItemModel->item(m_SearchItemModelrow, 0)->setCheckState(Qt::Checked);
	}

	//AvQDebug("on_TviewSearchDevice_doubleClicked\n");
}


void FrmDevice::on_TviewSearchDevice_pressed(const QModelIndex &index)
{
	m_SearchItemModelIndex = index;
	m_SearchItemModelrow = index.row();
	m_SearchItemModelcolumn = index.column();
	//AvQDebug("on_TviewSearchDevice_pressed\n");
}



void FrmDevice::on_TviewChnDevice_customContextMenuRequested(const QPoint &pos)
{
	if (NULL == m_mouseDevicesRightMenu){
		m_mouseDevicesRightMenu = new QMenu(ui->TviewChnDevice->horizontalHeader());
		m_mouseDevicesRightMenu->addAction(QString(tr("Select")), this, SLOT(SlotsDevicesViewRightMenuSelect()));
		m_mouseDevicesRightMenu->addSeparator();
		m_mouseDevicesRightMenu->addAction(QString(tr("SelectAll")), this, SLOT(SlotsDevicesViewRightMenuSelectAll()));
		m_mouseDevicesRightMenu->addAction(QString(tr("SelectOther")), this, SLOT(SlotsDevicesViewRightMenuSelectOther()));
	}

	if (m_DevicesItemModelrow != -1 && m_DevicesItemModel->rowCount() > 0){
		m_mouseDevicesRightMenu->exec(QCursor::pos());
	}
	
}


void FrmDevice::on_TviewChnDevice_doubleClicked(const QModelIndex &index)
{
	m_DevicesItemModelIndex = index;
	m_DevicesItemModelcolumn = index.column();
	m_DevicesItemModelrow =  index.row();


	if (m_DevicesItemModel->item(m_DevicesItemModelrow, 0)->checkState() == Qt::Checked){
		m_DevicesItemModel->item(m_DevicesItemModelrow, 0)->setCheckState(Qt::Unchecked);
	}
	else{
		m_DevicesItemModel->item(m_DevicesItemModelrow, 0)->setCheckState(Qt::Checked);
	}
	AvQDebug("on_TviewChnDevice_doubleClicked\n");
}


void FrmDevice::on_TviewChnDevice_pressed(const QModelIndex &index)
{
	m_DevicesItemModelIndex = index;
	m_DevicesItemModelcolumn = index.column();
	m_DevicesItemModelrow = index.row();

    AvQDebug("on_TviewChnDevice_pressed\n");
}
