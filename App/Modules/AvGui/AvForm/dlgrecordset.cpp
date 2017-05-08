#include "dlgrecordset.h"
#include "ui_dlgrecordset.h"
#include "AvForm/dlgtimeselect.h"
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"
#include "AvConfigs/AvConfigRecord.h"
DlgRecordSet::DlgRecordSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRecordSet)
{
    ui->setupUi(this);

	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);

	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	//this->setAttribute(Qt::WA_DeleteOnClose);

	memset(m_RecordTm, 0x00, sizeof(m_RecordTm));
	for (int i = 0; i < AvMaxWeeks; i++){
		for (int j = 0; j < AvMaxTimeArea; j++){
			m_RecordTm[i][j].WeekIndex = i;
		}
	}
	FixDlgUi();

	FillInUiOptions();
	FillInUiValue();

}

DlgRecordSet::~DlgRecordSet()
{
    delete ui;
}


void DlgRecordSet::FixDlgUi()
{
	for (int i = 0; i < SYS_CHN_NUM; i++){
		ui->CBoxRecordSetChannel->addItem(QString::number(i));
	}
	ui->CBoxRecordSetChannel->addItem(QString(tr("AllChannel")));

	ui->CBoxRecordSetRecordPerRecord->addItem(QString::number(0));
	ui->CBoxRecordSetRecordPerRecord->addItem(QString::number(3));
	ui->CBoxRecordSetRecordPerRecord->addItem(QString::number(5));


	ui->CBoxRecordSetRecordOverlayStrategy->addItem(QString(tr("OverlayDisk")));
	ui->CBoxRecordSetRecordOverlayStrategy->addItem(QString(tr("OverlayWeek")));
	ui->CBoxRecordSetRecordOverlayStrategy->addItem(QString(tr("OverlayMonth")));
}
void DlgRecordSet::FillInUiOptions()
{

}
void DlgRecordSet::FillInUiValue()
{
	int index = ui->CBoxRecordSetChannel->currentIndex();
	if (index == SYS_CHN_NUM) {
		ui->CBoxRecordSetRecordTypeAlarm->setChecked(false);
		ui->CBoxRecordSetRecordTypeHandle->setChecked(false);
		ui->CBoxRecordSetRecordTypeTimer->setChecked(false);
		ui->CBoxRecordSetRecordStreamMain->setChecked(false);
		ui->CBoxRecordSetRecordStreamSub1->setChecked(false);
		ui->BtnCRecordSetRecordTypeTimeSelect->hide();
		return;
	}

	CAvConfigRecord AvConfigRecord;
	AvConfigRecord.Update();
	ConfigRecordProfile &ConfRecordProfile = AvConfigRecord.GetConfig(index);
	if (ConfRecordProfile.RecordTypeMask & AvMask(RecordFileType_ALARM)){
		ui->CBoxRecordSetRecordTypeAlarm->setChecked(true);
	}
	else{
		ui->CBoxRecordSetRecordTypeAlarm->setChecked(false);
	}

	if (ConfRecordProfile.RecordTypeMask & AvMask(RecordFileType_REALTIME)){
		ui->CBoxRecordSetRecordTypeHandle->setChecked(true);
	}
	else{
		ui->CBoxRecordSetRecordTypeHandle->setChecked(false);
	}
	if (ConfRecordProfile.RecordTypeMask & AvMask(RecordFileType_TIMER)){
		ui->CBoxRecordSetRecordTypeTimer->setChecked(true);
	}
	else{
		ui->CBoxRecordSetRecordTypeTimer->setChecked(false);
	}

	if (ConfRecordProfile.RecordCHLMask & AvMask(CHL_MAIN_T)){
		ui->CBoxRecordSetRecordStreamMain->setChecked(true);
	}
	else{
		ui->CBoxRecordSetRecordStreamMain->setChecked(false);
	}
	if (ConfRecordProfile.RecordCHLMask & AvMask(CHL_SUB1_T)){
		ui->CBoxRecordSetRecordStreamSub1->setChecked(true);
	}
	else{
		ui->CBoxRecordSetRecordStreamSub1->setChecked(false);
	}
	

	memcpy(m_RecordTm, ConfRecordProfile.RecordTm, sizeof(m_RecordTm));

	switch (ConfRecordProfile.RecordPerSecond)
	{
	case 0:
		ui->CBoxRecordSetRecordPerRecord->setCurrentIndex(0);
		break;
	case 3:
		ui->CBoxRecordSetRecordPerRecord->setCurrentIndex(1);
		break;
	case 5:
		ui->CBoxRecordSetRecordPerRecord->setCurrentIndex(2);
		break;
	default:
		break;
	}




}

void DlgRecordSet::on_BtnClose_clicked()
{
	close();
}

void DlgRecordSet::on_CBoxRecordSetChannel_currentIndexChanged(int index)
{
	FillInUiValue();
}

void DlgRecordSet::on_BtnCRecordSetRecordTypeTimeSelect_clicked()
{
	DlgTimeSelect *TimeSelect = new DlgTimeSelect;
	TimeSelect->CTimeAreaArrary2UiPos(m_RecordTm);
	TimeSelect->exec();
	TimeSelect->CTimeAreaUi2ArraryPos(m_RecordTm);
	delete TimeSelect;
}

void DlgRecordSet::on_CBoxRecordSetRecordTypeAlarm_clicked(bool checked)
{

}

void DlgRecordSet::on_CBoxRecordSetRecordTypeHandle_clicked(bool checked)
{ 
	if (checked == true){
		ui->CBoxRecordSetRecordTypeAlarm->setChecked(false);
		ui->CBoxRecordSetRecordTypeTimer->setChecked(false);
		ui->BtnCRecordSetRecordTypeTimeSelect->hide();

		ui->CBoxRecordSetRecordTypeAlarm->setCheckable(false);
		ui->CBoxRecordSetRecordTypeTimer->setCheckable(false);
	}
	else{
		

		ui->CBoxRecordSetRecordTypeAlarm->setCheckable(true);
		ui->CBoxRecordSetRecordTypeTimer->setCheckable(true);
	}

}

void DlgRecordSet::on_CBoxRecordSetRecordTypeTimer_clicked(bool checked)
{
	if (true == checked){
		ui->BtnCRecordSetRecordTypeTimeSelect->show();
	}
	else{
		ui->BtnCRecordSetRecordTypeTimeSelect->hide();
	}
}

void DlgRecordSet::on_BtnSubmit_clicked()
{

	ConfigRecordProfile RecordProfile;
	memset(&RecordProfile, 0x00, sizeof(ConfigRecordProfile));
	if (ui->CBoxRecordSetRecordTypeAlarm->isChecked() == true){
		RecordProfile.RecordTypeMask |= AvMask(RecordFileType_ALARM);
	}
	if (ui->CBoxRecordSetRecordTypeHandle->isChecked() == true){
		RecordProfile.RecordTypeMask |= AvMask(RecordFileType_REALTIME);
	}
	if (ui->CBoxRecordSetRecordTypeTimer->isChecked() == true){
		RecordProfile.RecordTypeMask |= AvMask(RecordFileType_TIMER);
	}
	
	if (ui->CBoxRecordSetRecordStreamMain->isChecked() == true){
		RecordProfile.RecordCHLMask |= AvMask(CHL_MAIN_T);
	}
	if (ui->CBoxRecordSetRecordStreamSub1->isChecked() == true){
		RecordProfile.RecordCHLMask |= AvMask(CHL_SUB1_T);
	}
	memcpy(RecordProfile.RecordTm, m_RecordTm, sizeof(m_RecordTm));
	bool ok = true;
	RecordProfile.RecordPerSecond = ui->CBoxRecordSetRecordPerRecord->currentData().toInt(&ok);

	int index = ui->CBoxRecordSetChannel->currentIndex();
	CAvConfigRecord AvConfigRecord;
	AvConfigRecord.Update();
	if (index == SYS_CHN_NUM){
		for (int i = 0; i < SYS_CHN_NUM; i++){
			ConfigRecordProfile &ConfRecordProfile = AvConfigRecord.GetConfig(i);
			memcpy(&ConfRecordProfile, &RecordProfile, sizeof(ConfigRecordProfile));
		}
		
		AvConfigRecord.SettingUp();
	}
	else{
		ConfigRecordProfile &ConfRecordProfile = AvConfigRecord.GetConfig(index);
		memcpy(&ConfRecordProfile, &RecordProfile, sizeof(ConfigRecordProfile));
		AvConfigRecord.SettingUp();
	}


	close();
}
