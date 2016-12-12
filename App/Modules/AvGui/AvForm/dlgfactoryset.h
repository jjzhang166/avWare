#ifndef DLGFACTORYSET_H
#define DLGFACTORYSET_H

#include <QDialog>
#include "AvGuiSystem.h"
#include "AvUiComm/tablemodel.h"
#include "AvUiComm/progressbardelegate.h"
#include <QResizeEvent>
#include <QStandardItemModel>
namespace Ui {
class DlgFactorySet;
}

class DlgFactorySet : public QDialog
{
    Q_OBJECT

public:
    explicit DlgFactorySet(QWidget *parent = 0);
    ~DlgFactorySet();
	void SetModifyDeviceList(std::list<C_DevSearch> &ModifyList);
private:
	void FixDlgUi();
	void FillInUiOptions();
	void FillInUiValue();
	void resizeEvent(QResizeEvent * event);
private slots:
    void on_BtnClose_clicked();
	void SlotsHorizontalScrollBarRangChanged(int min, int max);
        void on_BtnSubmit_clicked();

        void on_BtnCancel_clicked();

private:
    Ui::DlgFactorySet		* ui;
	QStandardItemModel		* m_TviewDeviceTableModel;
	ProgressBarDelegate		* m_ProgressBar;
	std::list<C_DevSearch>	* m_ModifyList;
};

#endif // DLGFACTORYSET_H
