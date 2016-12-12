#include "dlgfactoryset.h"
#include "ui_dlgfactoryset.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvUiComm/AvUiConfigIni.h"
#include <QEvent>
#include "AvSource/AvQStringsLanguage.h"
#include <QString>
#include <QStyledItemDelegate>
#include "AvGui/AvGui.h"
#include "AvUiComm/AvUiConfigIni.h"



#define FACTORY_INI_SECTION "FACTORY"
#define FACTORY_INIT_KEY_DEVMODEL "DevModel"
#define FACTORY_INIT_KEY_DevName "DevName"
#define FACTORY_INIT_KEY_FacName "FacName"

#define FACTORY_INIT_KEY_BHasAudio "BHasAudio"
#define FACTORY_INIT_KEY_BHasIO "BHasIO"
#define FACTORY_INIT_KEY_BHasModifyMac "BHasModifyMac"
#define FACTORY_INIT_KEY_BHasPtz "BHasPtz"
#define FACTORY_INIT_KEY_BHasSD "BHasSD"
#define FACTORY_INIT_KEY_BHasWifi "BHasWifi"
#define FACTORY_INIT_KEY_Chip "Chip"
#define FACTORY_INIT_KEY_Channels "Channels"
#define FACTORY_INIT_KEY_Logo "Logo"
#define FACTORY_INIT_KEY_Sensor "Sensor"




DlgFactorySet::DlgFactorySet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgFactorySet)
{
    ui->setupUi(this);
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnMenu, QChar(0xf0c9), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_DeleteOnClose);

	FixDlgUi();

	FillInUiOptions();
	FillInUiValue();
	
}

DlgFactorySet::~DlgFactorySet()
{
    delete ui;
}

void DlgFactorySet::SetModifyDeviceList(std::list<C_DevSearch> &ModifyList)
{
	m_ModifyList = &ModifyList;

	int width_temp = 0;
	std::list <C_DevSearch>::iterator ilist;
	C_DevSearch DevSearchInfo;
	int i = 0;
	for (ilist = m_ModifyList->begin(); ilist != m_ModifyList->end(); ilist++){
		DevSearchInfo = *ilist;
		m_TviewDeviceTableModel->setItem(i, 0, new QStandardItem(QString::number(i)));
		m_TviewDeviceTableModel->setItem(i, 1, new QStandardItem(QString(ilist->DevStatusInfo.devname)));
		m_TviewDeviceTableModel->setItem(i, 2, new QStandardItem(QString(ilist->NetWorkProfile.Ipv4)));
		QIcon icon(":/image/info.png");
		m_TviewDeviceTableModel->setItem(i, 3, new QStandardItem(icon, QString("Wait")));
		i++;
	}

	if (m_ModifyList->size() == 1){
		DevSearchInfo = *m_ModifyList->begin();

	}
	else{

	}
}

void DlgFactorySet::FixDlgUi()
{
	m_TviewDeviceTableModel = new QStandardItemModel;
	m_TviewDeviceTableModel->setColumnCount(4);
	m_TviewDeviceTableModel->setHeaderData(0, Qt::Horizontal, "Nu");
	m_TviewDeviceTableModel->setHeaderData(1, Qt::Horizontal, "DevName");
	m_TviewDeviceTableModel->setHeaderData(2, Qt::Horizontal, "StrIp");
	m_TviewDeviceTableModel->setHeaderData(3, Qt::Horizontal, "status");


	ui->TviewDeviceList->setModel(m_TviewDeviceTableModel);
	ui->TviewDeviceList->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->TviewDeviceList->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->TviewDeviceList->setContextMenuPolicy(Qt::CustomContextMenu);
	ui->TviewDeviceList->verticalHeader()->hide();
	QObject::connect((const QObject *)ui->TviewDeviceList->horizontalScrollBar(), SIGNAL(rangeChanged(int, int)), this, SLOT(SlotsHorizontalScrollBarRangChanged(int, int)));
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));

	
}

void DlgFactorySet::FillInUiOptions()
{
	ui->CBoxLogoType->setStyleSheet("color:black");
	ui->CBoxChipModel->setStyleSheet("color:black");
	ui->CBoxMaxChannel->setStyleSheet("color:black");
	ui->CBoxSensorModel->setStyleSheet("color:black");


	for (int i = AvChip_H18EV100; i < AvChip_A66; i++){
		ui->CBoxChipModel->addItem(AvUiLangsAvChip((AvChip)i));
	}
	for (int i = AvSensor_NONE +1; i < AvSensor_LAST; i++){
		ui->CBoxSensorModel->addItem(AvUiLangsAvSensor((AvSensor)i));
	}
	{
		QStringList DropList;
		DropList	<< QString::number(1) 
					<< QString::number(4)
					<< QString::number(8)
					<< QString::number(16)
					<< QString::number(24)
					<< QString::number(32)
					<< QString::number(64);

		ui->CBoxMaxChannel->addItems(DropList);
	}

	{
		
		for (int i = 0; i < 5; i++){
			ui->CBoxLogoType->addItem(QString::number(i));
		}
	}
}



void DlgFactorySet::FillInUiValue()
{
	QDateTime DateTime;
	DateTime = QDateTime::currentDateTime();
	ui->LiEditFacTime->setText(DateTime.toString("yyyy-MM-dd hh:mm:ss"));
	ui->LiEditFacTime->setDisabled(true);

	QVariant Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_DEVMODEL));
	ui->LiEditDevModel->setText(Var.toString());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_DevName));
	ui->LiEditDevName->setText(Var.toString());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_FacName));
	ui->LiEditFacName->setText(Var.toString());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasAudio));
	ui->CheckBHasAudio->setChecked(Var.toBool());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasIO));
	ui->CheckBHasIO->setChecked(Var.toBool());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasModifyMac));
	ui->CheckBHasModifyMac->setChecked(Var.toBool());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasPtz));
	ui->CheckBHasPtz->setChecked(Var.toBool());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasSD));
	ui->CheckBHasSD->setChecked(Var.toBool());
	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasWifi));
	ui->CheckBHasWifi->setChecked(Var.toBool());

	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Sensor));
	ui->CBoxSensorModel->setCurrentText(Var.toString());

	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Channels));
	ui->CBoxMaxChannel->setCurrentText(Var.toString());

	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Logo));
	ui->CBoxLogoType->setCurrentText(Var.toString());

	Var = CAvUiConfigIni::Instance()->InigetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Chip));
	ui->CBoxChipModel->setCurrentText(Var.toString());



}

void DlgFactorySet::resizeEvent(QResizeEvent * event)
{
	QRect Rect = this->rect();
	ui->TviewDeviceList->setFixedWidth(Rect.width() / 3 * 2);
	QSize viewSize = ui->TviewDeviceList->viewport()->size();
	ui->TviewDeviceList->setColumnWidth(0, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(1, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(2, 0.20*viewSize.width());
	ui->TviewDeviceList->setColumnWidth(3, 0.40*viewSize.width());
	AvQDebug("resizeEvent\n");
}

void DlgFactorySet::on_BtnClose_clicked()
{
	m_ModifyList = NULL;

	QVariant Var;
	Var = ui->LiEditDevModel->text();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_DEVMODEL), Var);

	Var = ui->LiEditDevName->text();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_DevName), Var);
	Var = ui->LiEditFacName->text();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_FacName), Var);
	Var = ui->CheckBHasAudio->isChecked();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasAudio), Var);

	Var = ui->CheckBHasIO->isChecked();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasIO), Var);
	Var = ui->CheckBHasModifyMac->isChecked();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasModifyMac), Var);
	Var = ui->CheckBHasPtz->isChecked();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasPtz), Var);
	Var = ui->CheckBHasSD->isChecked();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasSD), Var);
	Var = ui->CheckBHasWifi->isChecked();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_BHasWifi), Var);

	Var = ui->CBoxSensorModel->currentText();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Sensor), Var);
	Var = ui->CBoxMaxChannel->currentText();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Channels), Var);
	
	Var = ui->CBoxLogoType->currentText();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Logo), Var);
	
	Var = ui->CBoxChipModel->currentText();
	CAvUiConfigIni::Instance()->InisetValue(QString(FACTORY_INI_SECTION), QString(FACTORY_INIT_KEY_Chip), Var);

    this->close();
}
void DlgFactorySet::SlotsHorizontalScrollBarRangChanged(int min, int max)
{
	AvQDebug("SlotsHorizontalScrollBarRangChanged min = %d, max = %d\n", min, max);
	QApplication::sendEvent(this, new QEvent(QEvent::Resize));
	
}
void DlgFactorySet::on_BtnSubmit_clicked()
{
	QString DevName = ui->LiEditDevName->text();
	QString FacName = ui->LiEditFacName->text();
	QString DevModel = ui->LiEditDevModel->text();
	QString StrChip = ui->CBoxChipModel->currentText();
	QString StrSensor = ui->CBoxSensorModel->currentText();
	int ChipIndex = -1;
	int SensorModelIndex = -1;
	for (int i = AvChip_H18EV100; i < AvChip_LAST; i++){
		if (StrChip == AvUiLangsAvChip((AvChip)i)){
			ChipIndex = i;
			break;
		}
	}
	assert(ChipIndex != -1);

	for (int i = AvSensor_NONE; i < AvSensor_LAST; i++){
		if (StrSensor == AvUiLangsAvSensor((AvSensor)i)){
			SensorModelIndex = i;
			break;
		}
	}
	assert(SensorModelIndex != -1);
	int LogTyepIndex = ui->CBoxLogoType->currentIndex();
	int MaxChannel = ui->CBoxMaxChannel->currentText().toInt();;

	bool SupportWifi = ui->CheckBHasWifi->isChecked();
	bool SupportPtz = ui->CheckBHasPtz->isChecked();
	bool SupportAudio = ui->CheckBHasAudio->isChecked();
	bool SupportSd = ui->CheckBHasSD->isChecked();
	bool SupportModifyMac = ui->CheckBHasModifyMac->isChecked();
	bool SupportIo = ui->CheckBHasIO->isChecked();

	std::list <C_DevSearch>::iterator ilist;
	C_ManufacturerInfo Manufactureerinfo;
	

	Manufactureerinfo.ChannelMax = MaxChannel;
	Manufactureerinfo.FacChip = (AvChip)(ChipIndex);
	Manufactureerinfo.FacSenSor = (AvSensor)(SensorModelIndex);


	Manufactureerinfo.FacTime = time(NULL);
	sprintf(Manufactureerinfo.FacManufacturer, "%s", FacName.toStdString().c_str());
	sprintf(Manufactureerinfo.FacProductionModel, DevModel.toStdString().c_str());
	sprintf(Manufactureerinfo.HardWareVersion, "0-0-0");


	C_DevSearch DevSearchInfo;
	int i = 0x22;
	int j = 0;
	for (ilist = m_ModifyList->begin(); ilist != m_ModifyList->end(); ilist++){
		i++;
		DevSearchInfo = *ilist;
		sprintf(Manufactureerinfo.ProtocolUniqueCode, "%s", DevSearchInfo.ManufacturerInfo.ProtocolUniqueCode);
		qDebug("ProtocolUniqueCode[%s]\n", DevSearchInfo.ManufacturerInfo.ProtocolUniqueCode);
		if (true == SupportModifyMac){
			time_t t = time(NULL);
			char buff[12];
			sprintf(buff, "%08X", t);
			sprintf(Manufactureerinfo.FacProductionSerialNo, "00-08:%X:%c%c:%c%c:%c%c:%c%c-998877", i,
				buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7]);
		}
		else{
			sprintf(Manufactureerinfo.FacProductionSerialNo, "%s", DevSearchInfo.ManufacturerInfo.FacProductionSerialNo);
		}

		av_bool ret = CAvGui::NetCaptureSetFactoryInfo(Manufactureerinfo);
		if (ret == av_true){
			QIcon icon(":/image/info.png");
			m_TviewDeviceTableModel->setItem(j, 3, new QStandardItem(icon, QString("succeed")));
		}
		else{
			QIcon icon(":/image/error.png");
			m_TviewDeviceTableModel->setItem(j, 3, new QStandardItem(icon, QString("failed")));
		}

		j++;
	}
}

void DlgFactorySet::on_BtnCancel_clicked()
{
	this->close();
}
