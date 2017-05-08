#include "dlgupgrade.h"
#include "ui_dlgupgrade.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvSource/AvQStringsLanguage.h"
#include "AvGui/AvGui.h"
#include "Apis/AvEnum.h"
#include "Apis/AvEnuminline.h"
#include "Apis/LibCommon.h"
#include "Common/gtkutf8.h"



#define AV_PACKET_VER  1
#define AV_PACKET_SYNC 0XBF78AAFF

#include "AvUiComm/AvUiConfigIni.h"
#define  UPGRADE_SECTION "UpgradeSection"
#define  UPGRADE_KEY_FIRMWARE_PATH "UpgradeKeyFirmwarePath"


typedef struct {
	unsigned int Crc32;
	unsigned int FileFormat;
	unsigned int Packetage;
	unsigned int offset;
	unsigned int length;
}PacketagePlace;


typedef enum {
	FF_NULL,
	FF_JFFS2,
	FF_Cramfs,
	FF_Squashfs,
	FF_BIN,
}FT_Mode;

typedef enum{
	PacketAge_App,
	PacketAge_Filesys,
	PacketAge_Kernel,
}PacketAge_e;

typedef enum {
	Cu_Gp = 0,
}Custom;

typedef struct {
	unsigned int 	sync;
	unsigned int 	version;
	unsigned int 	PackageVer;
	unsigned int 	Package;

	unsigned int 	Chip;
	unsigned int 	Custom;
	unsigned int 	mktime;
	unsigned int 	PacketLen;
	PacketagePlace	PacketInfo[4];
}PacketHead;



DlgUpgrade::DlgUpgrade(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgUpgrade)
{
    ui->setupUi(this);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
	//this->setAttribute(Qt::WA_DeleteOnClose);


	m_WindowTitle = windowTitle();
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnMenu, QChar(0xf0c9), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);
	
	m_Timer = NULL;
	m_FirmwareStandardItem = new QStandardItemModel;
	FixDlgUpgradeUi();
}

DlgUpgrade::~DlgUpgrade()
{
    delete ui;
}
void DlgUpgrade::SetModifyDeviceList(std::list<C_DevSearch> &ModifyList)
{
	m_ModifyList = &ModifyList;

	int width_temp = 0;
	std::list <C_DevSearch>::iterator ilist;
	C_DevSearch DevSearchInfo;
	int i = 0;

	QVector<QStringList> data;
	QString status;
	status += QString("no-percentum:value:0:text:");
	status += QString(tr("WaitForModify"));
	for (ilist = m_ModifyList->begin(); ilist != m_ModifyList->end(); ilist++){
		DevSearchInfo = *ilist;
		ilist->DevStatusInfo.UpTime = ProgressStatus_UpgradeStart << 16 | 0x00;
		data.append(QStringList() << QString::number(i + 1) << QString::fromLocal8Bit(DevSearchInfo.DevStatusInfo.DeviceName)
			<< QString(DevSearchInfo.NetWorkProfile.Ipv4) << status);
		i++;
	}
	m_TviewDeviceTableModel->setData(data);
	ui->TviewDeviceList->verticalHeader()->hide();
	emit m_TviewDeviceTableModel->layoutChanged();
}
void DlgUpgrade::FixDlgUpgradeUi()
{
	m_TviewDeviceTableModel = new TableModel;
	ui->TviewDeviceList->setModel(m_TviewDeviceTableModel);
	m_ProgressBar = new ProgressBarDelegate;
	m_ProgressBar->SetProgressBarIndex(3);
	ui->TviewDeviceList->setItemDelegate(m_ProgressBar);

	emit m_TviewDeviceTableModel->layoutChanged();


	QStringList headers;
	headers.push_back(QString(tr("No")));
	headers.push_back(QString(tr("DevName")));
	headers.push_back(QString(tr("IpAddr")));
	headers.push_back(QString(tr("Progress")));
	m_TviewDeviceTableModel->setHorizontalHeader(headers);

	ui->TviewDeviceList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewDeviceList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewDeviceList->setContextMenuPolicy(Qt::CustomContextMenu);
	QObject::connect((const QObject *)ui->TviewDeviceList->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(SlotsHorizontalScrollBarRangChanged(int, int)));
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
}
void DlgUpgrade::resizeEvent(QResizeEvent * event)
{
	AvQDebug("resizeEvent DlgUpgrade\n");
	int i = 0;
	//m_FirmwareStandardItem->clear();
	m_FirmwareStandardItem->setColumnCount(1);

	ui->TviewPacket->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewPacket->setStyleSheet("selection-background-color:skyblue");
	ui->TviewPacket->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	m_FirmwareStandardItem->setHeaderData(0, Qt::Horizontal, QString("PacketName"));
	ui->TviewPacket->setModel(m_FirmwareStandardItem);
	ui->TviewPacket->setColumnWidth(0, 250 + 15);

	ui->TviewPacket->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewPacket->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewPacket->setContextMenuPolicy(Qt::CustomContextMenu);
	//ui->TviewPacket->verticalHeader()->hide();->clear();
	m_FirmwareStandardItem->setColumnCount(1);

	ui->TviewPacket->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewPacket->setStyleSheet("selection-background-color:skyblue");
	ui->TviewPacket->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");
	m_FirmwareStandardItem->setHeaderData(0, Qt::Horizontal, QString(tr("PacketName")));
	ui->TviewPacket->setModel(m_FirmwareStandardItem);
	ui->TviewPacket->setColumnWidth(0, ui->TviewPacket->width());

	ui->TviewPacket->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewPacket->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewPacket->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TviewPacket->verticalHeader()->hide();

	QRect Rect = this->rect();
	ui->TviewDeviceList->setFixedWidth(Rect.width() / 3 * 2);
	QSize viewSize = ui->TviewDeviceList->viewport()->size();
	ui->TviewDeviceList->setColumnWidth(0, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(1, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(2, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(3, 0.40*viewSize.width());

	AvQDebug("resizeEvent\n");
}


void DlgUpgrade::showEvent(QShowEvent *e)
{
	QRect Rect = this->rect();
	ui->TviewDeviceList->setFixedWidth(Rect.width() / 3 * 2);
	QSize viewSize = ui->TviewDeviceList->viewport()->size();
	ui->TviewDeviceList->setColumnWidth(0, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(1, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(2, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(3, 0.40*viewSize.width());
}
void DlgUpgrade::on_BtnClose_clicked()
{

}

void DlgUpgrade::on_BtnClose_pressed()
{
	emit SignalsDlgClose();
}

void DlgUpgrade::on_BtnClose_released()
{
	std::list<C_FirmwarePacketInfo>::iterator pIlist;
	for (pIlist = m_FirmwareList.begin(); pIlist != m_FirmwareList.end();){
		AvQDebug("Free [%p] Packet [%s] memmory\n",pIlist->base, pIlist->FirmwarePath);
		free(pIlist->base);
		pIlist = m_FirmwareList.erase(pIlist);
	}
	int Count = m_FirmwareStandardItem->rowCount();
	for (int i = 0; i < Count; i++){
		//因为每删除完一个，后面的数据已经前移了
		m_FirmwareStandardItem->removeRow(0);
	}
	this->close();
}
void DlgUpgrade::SlotsHorizontalScrollBarRangChanged(int min, int max)
{
	AvQDebug("SlotsHorizontalScrollBarRangChanged min = %d, max = %d\n", min, max);
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));

}
void DlgUpgrade::on_BtnDelPacket_clicked()
{
	if (m_FirmwareStandardItem->rowCount() <= 0 || false == m_FirmwareModelIndex.isValid()){
		return;
	}
	QString indexString = m_FirmwareStandardItem->item(m_FirmwareModelIndex.row(), 0)->text();
	std::list<C_FirmwarePacketInfo>::iterator pIlist;
	for (pIlist = m_FirmwareList.begin(); pIlist != m_FirmwareList.end();){
		if (indexString == QString(pIlist->FirmwareNames)){
			AvQDebug("Find Packet [%s] row = %d clear it\n", pIlist->FirmwareNames, m_FirmwareModelIndex.row());
			free(pIlist->base);
			pIlist = m_FirmwareList.erase(pIlist);
			break;
		}
		pIlist++;
	}
	m_FirmwareStandardItem->removeRow(m_FirmwareModelIndex.row());
	m_FirmwareModelIndex = QModelIndex();
}




void DlgUpgrade::on_BtnAddPacket_clicked()
{
	char localBuffer[1024];
	char *buffer = localBuffer;
	unsigned int len = sizeof(localBuffer);

	QVariant Var = CAvUiConfigIni::Instance()->InigetValue(QString(UPGRADE_SECTION), QString(UPGRADE_KEY_FIRMWARE_PATH));
	QString FirmwarePath = Var.toString();

	QStringList FirwarePathList = QFileDialog::getOpenFileNames(this, tr("Select Firmware"), FirmwarePath, tr("Firmware Files (*.img)"));
	QList<QString>::Iterator iList;
	if (FirwarePathList.size() == 0) {
		av_error("Have No Select Packet\n");
		return;
	}
	C_FirmwarePacketInfo FirmwarePacketInfo;
	for (iList = FirwarePathList.begin(); iList != FirwarePathList.end(); iList++){
		if (iList->size() >= 128){
			QString FullPach;
			FullPach = *iList;
			FullPach += QObject::tr("Path too deep");
			CAvUiComm::ShowMessageBoxError(FullPach);
			continue;
		}

		
		utf8_to_gbk(iList->toLocal8Bit().data(), iList->toLocal8Bit().size(), &buffer, &len);
		sprintf(FirmwarePacketInfo.FirmwarePath, "%s", buffer);
		char *filename = strrchr(FirmwarePacketInfo.FirmwarePath, '/') == NULL ? FirmwarePacketInfo.FirmwarePath : strrchr(FirmwarePacketInfo.FirmwarePath, '/') + 1;
		sprintf(FirmwarePacketInfo.FirmwareNames, filename);
		std::list <C_FirmwarePacketInfo>::iterator iList;
		
		for (iList = m_FirmwareList.begin(); iList != m_FirmwareList.end(); iList++){
			if (0 == strcasecmp(FirmwarePacketInfo.FirmwareNames, iList->FirmwareNames)){
				QString ErrorMsg;
				ErrorMsg += QObject::tr("Packet");
				ErrorMsg += QString("\n");
				ErrorMsg += QString(FirmwarePacketInfo.FirmwareNames);
				ErrorMsg += QString("\n");
				ErrorMsg += QObject::tr("Has alread load");
				CAvUiComm::ShowMessageBoxError(ErrorMsg);
				continue;
			}
		}
		{//check out frimware
			C_FirmWareCheckoutHand FimWareHead;
			FimWareHead.length = 0;
			if (0 != avFirmWareCheckout(&FimWareHead, FirmwarePacketInfo.FirmwarePath)){
				QString ErrorMsg;
				ErrorMsg += QObject::tr("Packet");
				ErrorMsg += QString("\n");
				ErrorMsg += QString(FirmwarePacketInfo.FirmwareNames);
				ErrorMsg += QString("\n");
				ErrorMsg += QObject::tr("Frimware Checkout Error");
				CAvUiComm::ShowMessageBoxError(ErrorMsg);
				continue;
			}
		}
		
		{//read file
			FILE *fpSrc = NULL;
			fpSrc = fopen(FirmwarePacketInfo.FirmwarePath, "rb");
			if (NULL == fpSrc){
				continue;
			}
#ifdef WIN32
			long fileSize = _filelength(_fileno(fpSrc));
#else
			long fileSize = 0;
			{
				struct stat filestat;
				stat(FirmwarePacketInfo.FirmwarePath, &filestat);
				fileSize = filestat.st_size;
			}
#endif

			FirmwarePacketInfo.base = (char *)malloc(sizeof(char)*fileSize);
			qDebug("read file mallocaddr [%p][%s] size[%d]", FirmwarePacketInfo.base, FirmwarePacketInfo.FirmwareNames, fileSize);
			int ret = 0, i = 0;
			ret = fread(FirmwarePacketInfo.base, 1, fileSize, fpSrc);
			if (ret != fileSize){
				fclose(fpSrc);
				AvQDebug("Error read file %s, file len[%d], read len[%d]\n", fileSize, ret);
				free(FirmwarePacketInfo.base);

				return;
			}
			FirmwarePacketInfo.len = fileSize;
			FirmwarePacketInfo.FirmwareInfo.ChipMask = 0;
			FirmwarePacketInfo.FirmwareInfo.SensorMask = 0;
			fclose(fpSrc);
		}
		PacketHead *pPacketH = (PacketHead *)FirmwarePacketInfo.base;
		if (pPacketH->sync == AV_PACKET_SYNC){
			FirmwarePacketInfo.FirmwareInfo.ChipMask = pPacketH->Chip;
			FirmwarePacketInfo.FirmwareInfo.CustomMask = pPacketH->Custom;
			FirmwarePacketInfo.FirmwareInfo.Version = pPacketH->PackageVer;
			sprintf(FirmwarePacketInfo.FirmwareInfo.Descriptor, "");
			sprintf(FirmwarePacketInfo.FirmwareInfo.FilesystemVerion, "0.0.1");
			sprintf(FirmwarePacketInfo.FirmwareInfo.KernelVersion, "K 0.0.1");
			sprintf(FirmwarePacketInfo.FirmwareInfo.ProtoVersion, "0.0.0");
			
			struct tm *t = NULL;
			time_t t_t = pPacketH->mktime;
			t = localtime(&t_t);
			sprintf(FirmwarePacketInfo.FirmwareInfo.BuildTime, "%d-%d-%d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
			//unsigned int crcValue = avWarecrc32((const unsigned char *)&(FirmwarePacketInfo.base[pPacketH->PacketInfo[0].offset]), pPacketH->PacketInfo[0].length);

			m_FirmwareList.push_back(FirmwarePacketInfo);
		}

	}

	{
		char *dirpath = NULL;

		if (NULL != (dirpath = strrchr(FirmwarePacketInfo.FirmwarePath, '/'))){
			*dirpath = '\0';
			dirpath = FirmwarePacketInfo.FirmwarePath;
		}
		else{
			dirpath = FirmwarePacketInfo.FirmwarePath;
			sprintf(dirpath, ".");
		}


		len = sizeof(localBuffer);
		gbk_to_utf8(dirpath, strlen(dirpath), &buffer, &len);
		CAvUiConfigIni::Instance()->InisetValue(QString(UPGRADE_SECTION), QString(UPGRADE_KEY_FIRMWARE_PATH), QString(buffer));
	}
	


	std::list <C_FirmwarePacketInfo>::iterator  iFirmList;



	int i = 0;
	for (iFirmList = m_FirmwareList.begin(); iFirmList != m_FirmwareList.end(); iFirmList++){
		m_FirmwareStandardItem->setItem(i, new QStandardItem(iFirmList->FirmwareNames));
		ui->TviewPacket->setRowHeight(i++, 20);
	}
}

void DlgUpgrade::on_BtnSubmit_clicked()
{
	if (NULL == m_Timer){
		m_Timer = new QTimer;
		QObject::connect(m_Timer, SIGNAL(timeout()), this, SLOT(SlotsTimeout()));
	}

	std::list<C_DevSearch>::iterator iList;
	C_FirmWareUpgrade FirmWareUpgrade;
	bool ArgsIsVaild = false;
	m_FirmWareUpgradeDevices.clear();
	for (iList = m_ModifyList->begin(); iList != m_ModifyList->end(); iList++){
		ArgsIsVaild = false;
		sprintf(FirmWareUpgrade.DeviceMac, iList->NetWorkProfile.Mac);
		sprintf(FirmWareUpgrade.DeviceName, iList->DevStatusInfo.DeviceName);
		sprintf(FirmWareUpgrade.IpAddr, iList->NetWorkProfile.Ipv4);
		sprintf(FirmWareUpgrade.Usrname, "admin");
		sprintf(FirmWareUpgrade.Passwd, "admin");
		FirmWareUpgrade.Port = iList->NetWorkProfile.ServicePort;
		
		{
			std::list<C_FirmwarePacketInfo>::iterator pIlist;
			for (pIlist = m_FirmwareList.begin(); pIlist != m_FirmwareList.end(); pIlist++){
				if (iList->ManufacturerInfo.FacChip == 0xff ||
					pIlist->FirmwareInfo.ChipMask & AvMask(iList->ManufacturerInfo.FacChip)){
					sprintf(FirmWareUpgrade.FirmwareName, pIlist->FirmwareNames);
					FirmWareUpgrade.FirmwareInfo = pIlist->FirmwareInfo;
					FirmWareUpgrade.FirmwareAddr = (unsigned char *)pIlist->base;
					FirmWareUpgrade.FirmwareSize = pIlist->len;
					FirmWareUpgrade.FirmwareSent = 0;
					FirmWareUpgrade._value = 0;
					FirmWareUpgrade._status = ProgressStatus_UpgradeStart;

					ArgsIsVaild = true;
					break;
				}
			}
		}
		if (ArgsIsVaild == true){
			m_FirmWareUpgradeDevices.push_back(FirmWareUpgrade);
		}
		else{
			FirmWareUpgrade._value = 0;
			FirmWareUpgrade._status = ProgressStatus_HaveNoRightResource;
			m_FirmWareUpgradeDevices.push_back(FirmWareUpgrade);
			AvQDebug("Can't Find Right Packet FacChip[%x]\n", iList->ManufacturerInfo.FacChip);
		}
		
	}
	
	CAvGui::NetCaptureProtoUpgrade(&m_FirmWareUpgradeDevices);
	
	m_TimerCnt = 0;
	m_Timer->start(200);

	ui->BtnSubmit->setEnabled(false);
	ui->BtnClose->setEnabled(false);
	ui->BtnCannel->setEnabled(false);


}
void DlgUpgrade::SlotsTimeout()
{

	m_TimerCnt++;
	bool isAllOver = true;
	std::list <C_FirmWareUpgrade>::iterator iList;
	int j = 0;
	QString ShowMsg;
	QVector<QStringList> data;
	CAvGui::NetCaptureProtoUpgradeProgress(&m_FirmWareUpgradeDevices);
	for (iList = m_FirmWareUpgradeDevices.begin(); iList != m_FirmWareUpgradeDevices.end(); iList++, j++){
		ShowMsg.clear();
		ShowMsg += QString("value:");
		ShowMsg += QString::number(iList->_value);
		AvQDebug("recv value = %d\n", iList->_value);
		ShowMsg += QString(":text:");
		if (iList->_status <= ProgressStatus_None || iList->_status >= ProgressStatus_Last){

			av_error("Staus is error \n");
			iList->_status = ProgressStatus_LinkFailed;
		}
		ShowMsg += AvUiLangsProgressStatus(iList->_status);
		if (iList->_status != ProgressStatus_UpgradeOver
			&& iList->_status != ProgressStatus_Rebootting
			&& iList->_status != ProgressStatus_ModifyOver
			&& iList->_status != ProgressStatus_HaveNoDiskResource
			&& iList->_status != ProgressStatus_HaveNoRightResource
			&& iList->_status != ProgressStatus_LinkFailed){
			isAllOver = false;
		}

		data.append(QStringList() << QString::number(j + 1) << QString::fromLocal8Bit(iList->DeviceName)
			<< QString(iList->IpAddr) << ShowMsg);
	}
	m_TviewDeviceTableModel->setData(data);
	emit m_TviewDeviceTableModel->layoutChanged();

	if (isAllOver == true || m_TimerCnt * 200 >= 3 * 60 * 1000){
		if (m_TimerCnt * 200 >= 3 * 60 * 1000){
			av_error("Ui upgrade time out\n");
		}
		ui->BtnSubmit->setEnabled(true);
		ui->BtnClose->setEnabled(true);
		ui->BtnCannel->setEnabled(true);
		m_Timer->stop();
		m_TimerCnt = 0;
		CAvGui::NetCaptureProtoUpgradeOver();
		setWindowTitle(m_WindowTitle);
	}
	else{
		QString newTitle = m_WindowTitle + QString("   ");
		newTitle += QString::number(3 * 60 * 1000 - m_TimerCnt * 200);
		setWindowTitle(newTitle);
		AvQDebug("set WidowsTitle %s", newTitle.toStdString().c_str());
	}
}

void DlgUpgrade::on_BtnCannel_clicked()
{

}

void DlgUpgrade::on_TviewPacket_clicked(const QModelIndex &index)
{
	QString indexString = m_FirmwareStandardItem->item(index.row(), 0)->text();
	m_FirmwareModelIndex = index;
	qDebug() << indexString;

	std::list <C_FirmwarePacketInfo>::iterator  iFirmList;
	C_FirmwarePacketInfo PacketInfo;
	for (iFirmList = m_FirmwareList.begin(); iFirmList != m_FirmwareList.end(); iFirmList++){
		PacketInfo = *iFirmList;
		if (indexString == PacketInfo.FirmwareNames){
			break;
		}
	}
	ui->LabPacketMsgSoftVer->setText(QString().sprintf("%d.%d.%d.%d",
		PacketInfo.FirmwareInfo.Version >> 24 & 0xff, 
		PacketInfo.FirmwareInfo.Version >> 16 & 0xff, 
		PacketInfo.FirmwareInfo.Version >> 8   & 0xff,
		PacketInfo.FirmwareInfo.Version & 0xff));

	QString ChipMask;
	for (int i = 0; i < AvChip_LAST; i++){
		if (PacketInfo.FirmwareInfo.ChipMask & AvMask(i)){
			av_msg("packet get %d \n", i);
			ChipMask += QString(EnumNameAvChip((AvChip)i));
			ChipMask += QString(" ");
		}
	}
	av_msg("packet chipmask = %x\n", PacketInfo.FirmwareInfo.ChipMask);
	ui->LabPacketMsgHardPlation->setText(ChipMask);
	ui->LabPacketMsgBuildTime->setText(PacketInfo.FirmwareInfo.BuildTime);
	ui->LabPacketMsgCustom->setText("NoLogo");
}
