#include "frmmainmenu.h"
#include "ui_frmmainmenu.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvSource/QAvEvent.h"
#include "AvGuiApplication.h"

FrmMainMenu::FrmMainMenu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmMainMenu)
{

    ui->setupUi(this);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_DeleteOnClose);
	raise();
}

FrmMainMenu::~FrmMainMenu()
{
    delete ui;
}

void FrmMainMenu::on_BtnMainMenuDevices_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_MainMenuDevices_Click);
	g_AvGuiApp.PostQAvEvent(AvEvent);
}

void FrmMainMenu::on_BtnMainMenuSystemSet_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_MainMenuSystemSet_Click);
	g_AvGuiApp.PostQAvEvent(AvEvent);
}

void FrmMainMenu::on_BtnMainMenuAlarm_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_MainMenuAlarm_Click);
	g_AvGuiApp.PostQAvEvent(AvEvent);
}

void FrmMainMenu::on_BtnMainMenuSysTools_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_MainMenuSysTools_Click);
	g_AvGuiApp.PostQAvEvent(AvEvent);
}

void FrmMainMenu::on_BtnMainMenuRecord_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_MainMenuRecord_Click);
	g_AvGuiApp.PostQAvEvent(AvEvent);
}

void FrmMainMenu::on_BtnMainMenuIntelligenceAlgorthm_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_MainMenuIntelligenceAlgorthm_Click);
	g_AvGuiApp.PostQAvEvent(AvEvent);
}

void FrmMainMenu::on_BtnMainMenuSystemInfo_clicked()
{
	QAvEvent AvEvent(QAvEvent::QAvEvent_MainMenuSystemInfo_Click);
	g_AvGuiApp.PostQAvEvent(AvEvent);

}

void FrmMainMenu::on_BtnClose_clicked()
{
    hide();
}
