#include "dlgsnapshot.h"
#include "ui_dlgsnapshot.h"
#include "AvUiComm/IconComm.h"

DlgSnapshot::DlgSnapshot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSnapshot)
{
    ui->setupUi(this);

	IconComm::Instance()->SetIcon(ui->btnMenu_Close, QChar(0xf00d), 10);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

}

DlgSnapshot::~DlgSnapshot()
{
    delete ui;
}
void DlgSnapshot::FillInSnapshot(unsigned char *data, int len)
{

}
void DlgSnapshot::on_btnMenu_Close_clicked()
{
    close();
}
