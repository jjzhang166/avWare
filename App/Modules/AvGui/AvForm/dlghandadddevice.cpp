#include "dlghandadddevice.h"
#include "ui_dlghandadddevice.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"

DlgHandAddDevice::DlgHandAddDevice(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgHandAddDevice)
{
    ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_DeleteOnClose);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->BtnMax, QChar(0xf096), 10);
	IconComm::Instance()->SetIcon(ui->BtnMin, QChar(0xf068), 10);
	IconComm::Instance()->SetIcon(ui->BtnMenu, QChar(0xf0c9), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);
}

DlgHandAddDevice::~DlgHandAddDevice()
{
    delete ui;
}

void DlgHandAddDevice::on_BtnClose_clicked()
{
	this->close();
}
