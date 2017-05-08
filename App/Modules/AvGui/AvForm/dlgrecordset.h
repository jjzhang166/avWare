#ifndef DLGRECORDSET_H
#define DLGRECORDSET_H

#include <QDialog>
#include "Apis/AvWareType.h"
#include "Apis/AvWareStruct.h"
namespace Ui {
class DlgRecordSet;
}

class DlgRecordSet : public QDialog
{
    Q_OBJECT

public:
    explicit DlgRecordSet(QWidget *parent = 0);
    ~DlgRecordSet();

public:

	void FixDlgUi();
	void FillInUiOptions();
	void FillInUiValue();

private slots:


    void on_BtnClose_clicked();

    void on_CBoxRecordSetChannel_currentIndexChanged(int index);

    void on_BtnCRecordSetRecordTypeTimeSelect_clicked();

    void on_CBoxRecordSetRecordTypeAlarm_clicked(bool checked);

    void on_CBoxRecordSetRecordTypeHandle_clicked(bool checked);

    void on_CBoxRecordSetRecordTypeTimer_clicked(bool checked);

    void on_BtnSubmit_clicked();

private:
    Ui::DlgRecordSet *ui;
	C_TimeArea		m_RecordTm[AvMaxWeeks][AvMaxTimeArea];
};

#endif // DLGRECORDSET_H
