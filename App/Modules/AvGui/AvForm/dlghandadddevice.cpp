#include "dlghandadddevice.h"
#include "ui_dlghandadddevice.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvGui/AvGui.h"
#include "AvSource/AvQStringsLanguage.h"

DlgHandAddDevice::DlgHandAddDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgHandAddDevice)
{
    ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
	this->setAttribute(Qt::WA_DeleteOnClose);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnMenu, QChar(0xf0c9), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	FixUi();

}

DlgHandAddDevice::~DlgHandAddDevice()
{
    delete ui;
}

void DlgHandAddDevice::FixUi()
{
	unsigned int protolMask;
	CAvGui::NetCaptureProtocolsMask(protolMask);
	ui->CBoxProtocol->clear();

	for (int i = 0; i < sizeof(protolMask); i++){
		if (!(AvMask(i) & protolMask)) continue;
		ui->CBoxProtocol->addItem(AvUiLangsLinkProtocol((LinkProtocol)i));
	}

	ui->CBoxTcpUdp->clear();
	ui->CBoxTcpUdp->addItem(QString("TCP"));
	ui->CBoxTcpUdp->addItem(QString("UDP"));
	
	ui->LiEditAddrIp->settext("192.168.1.90");
	ui->LiEditPort->setText(QString::number(5000));
	ui->LiEditUsrName->setText("admin");
	ui->LiEditPasswd->setText("admin");

}
void DlgHandAddDevice::on_BtnClose_clicked()
{
	close();
}

void DlgHandAddDevice::on_BtnSubmit_clicked()
{
	QString currentIndex = ui->CBoxProtocol->currentText();

	if (currentIndex == AvUiLangsLinkProtocol(LinkProtocol_Moon)){

	}
	else if (currentIndex == AvUiLangsLinkProtocol(LinkProtocol_RTMP)){

	}
	else if (currentIndex == AvUiLangsLinkProtocol(LinkProtocol_Onvif)){

	}
	else if (currentIndex == AvUiLangsLinkProtocol(LinkProtocol_RTSP)){

	}
	else if (currentIndex == AvUiLangsLinkProtocol(LinkProtocol_GBT28181)){

	}
}

void DlgHandAddDevice::on_CBoxProtocol_currentIndexChanged(const QString &arg1)
{
	if (arg1 == AvUiLangsLinkProtocol(LinkProtocol_Moon)){
		ui->LabConnectUrl->hide();
		ui->LiEditConnectUrl->hide();
		ui->LabTcpUdp->hide();
		ui->CBoxTcpUdp->hide();
	}
	else if (arg1 == AvUiLangsLinkProtocol(LinkProtocol_RTMP)){
		
	}
	else if (arg1 == AvUiLangsLinkProtocol(LinkProtocol_Onvif)){
		ui->LabConnectUrl->show();
		ui->LiEditConnectUrl->show();
	}
	else if (arg1 == AvUiLangsLinkProtocol(LinkProtocol_RTSP)){
		ui->LabTcpUdp->show();
		ui->CBoxTcpUdp->show();
	}
	else if (arg1 == AvUiLangsLinkProtocol(LinkProtocol_GBT28181)){

	}
}

void DlgHandAddDevice::on_BtnCannel_clicked()
{
    close();
}
