#ifndef DLGUPGRADE_H
#define DLGUPGRADE_H

#include <QDialog>
#include "AvGuiSystem.h"
#include "AvUiComm/progressbardelegate.h"
#include "AvUiComm/tablemodel.h"
#include <QResizeEvent>
#include <QFileDialog>
#include <QStandardItemModel>
#include "Apis/AvWareStruct.h"

typedef struct {
	C_FirmwareInfo FirmwareInfo;
	char  FirmwareNames[64];
	char  FirmwarePath[128];
	char *base;
	int   len;
}C_FirmwarePacketInfo;

namespace Ui {
class DlgUpgrade;
}

class DlgUpgrade : public QDialog
{
    Q_OBJECT

public:
    explicit DlgUpgrade(QWidget *parent = 0);
    ~DlgUpgrade();
	void SetModifyDeviceList(std::list<C_DevSearch> &ModifyList);
private:
	void FixDlgUpgradeUi();
	void resizeEvent(QResizeEvent * event);
	void showEvent(QShowEvent *e);
Q_SIGNALS:
	void SignalsDlgClose();


private slots:
	void SlotsHorizontalScrollBarRangChanged(int min, int max);
    void on_BtnClose_clicked();

    void on_BtnClose_pressed();

    void on_BtnClose_released();

    void on_BtnDelPacket_clicked();

    void on_BtnAddPacket_clicked();

    void on_BtnSubmit_clicked();

    void on_BtnCannel_clicked();

    void on_TviewPacket_clicked(const QModelIndex &index);

	void SlotsTimeout();
private:
    Ui::DlgUpgrade		*				ui;
	TableModel			*				m_TviewDeviceTableModel;
	ProgressBarDelegate *				m_ProgressBar;
	std::list<C_DevSearch> *			m_ModifyList;

	std::list <C_FirmWareUpgrade>		m_FirmWareUpgradeDevices;
	std::list <C_FirmwarePacketInfo>	m_FirmwareList;
	QModelIndex							m_FirmwareModelIndex;
	QStandardItemModel *				m_FirmwareStandardItem;
	QTimer				*				m_Timer;
	unsigned	int						m_TimerCnt;
	QString								m_WindowTitle;
};

#endif // DLGUPGRADE_H
