#include "dlgabout.h"
#include "ui_dlgabout.h"
#include "AvUiComm/IconComm.h"
#include <QTextEdit>
#include <QDesktopServices>
#include "config.h"
DlgAbout::DlgAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAbout)
{
    ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
	this->setAttribute(Qt::WA_DeleteOnClose);
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);
	IconComm::Instance()->SetIcon(ui->BtnIE, QChar(0xf153), 10);
	ui->aboutTextEdit->setReadOnly(true);
	setWindowOpacity(0.6);

	{
		QString text;
		text += QObject::tr("Version");
		text += QString::asprintf(": %d.%d.%d.%d %s %s\n", _AV_WARE_VERSION_MAJOR,
			_AV_WARE_VERSION_MINOR, _AV_WARE_VERSION_PATCH, _AV_WARE_VERSION_INWARD,
			_AV_WARE_VERSION_OEM, _AV_WARE_VERSION_RUNTIME);
		text += QString::asprintf("\n");


		text += QObject::tr("Build Time");
		text += QString::asprintf(": %s %s\n", __DATE__, __TIME__);
		text += QString::asprintf("\n");

		text += QObject::tr("technical support\n");
		text += QString::asprintf("\tzhousheng@graceport.cn\n");
		text += QString::asprintf("\tknight@graceport.cn\n");
		text += QString::asprintf("\n");

		text += QString::asprintf("www.graceport.cn");
		
		ui->aboutTextEdit->setText(text);

	
	}


}

DlgAbout::~DlgAbout()
{
    delete ui;
}

void DlgAbout::on_BtnIE_clicked()
{
	QDesktopServices::openUrl(QUrl("http://www.graceport.cn/", QUrl::TolerantMode));
	close();
}

void DlgAbout::on_BtnClose_clicked()
{
	close();
}
