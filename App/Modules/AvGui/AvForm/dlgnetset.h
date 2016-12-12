#ifndef DLGNETSET_H
#define DLGNETSET_H

#include <QDialog>
#include "AvGuiSystem.h"
#include "AvUiComm/progressbardelegate.h"
#include "AvUiComm/tablemodel.h"
#include <QResizeEvent>
#include <QStandardItemModel>

namespace Ui {
class DlgNetSet;
}

class DlgNetSet : public QDialog
{
    Q_OBJECT

public:
    explicit DlgNetSet(QWidget *parent = 0);
    ~DlgNetSet();
	void SetModifyDeviceList(std::list<C_DevSearch> &ModifyList);


private:
	
	
private:
	void resizeEvent(QResizeEvent * event);
	void FixDlgUi();

Q_SIGNALS:
	void SignalsDlgClose();


private slots:
    void on_BtnClose_clicked();
	void SlotsHorizontalScrollBarRangChanged(int min, int max);
        void on_BtnSave_clicked();

        void on_BtnCannel_clicked();

private:
    Ui::DlgNetSet *ui;
	QStandardItemModel	* m_TviewDeviceTableModel;
	ProgressBarDelegate * m_ProgressBar;
	std::list<C_DevSearch> *m_ModifyList;
};

#endif // DLGNETSET_H
