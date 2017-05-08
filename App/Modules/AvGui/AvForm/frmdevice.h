#ifndef FRMDEVICE_H
#define FRMDEVICE_H

#include <QDialog>
#include <QStandardItemModel>
#include <QMenu>
#include "AvForm/dlgdeviceset.h"
#include "AvGuiSystem.h"
#include "AvQStandardItem.h"
#include "AvForm/dlgnetset.h"
#include "AvForm/dlgupgrade.h"
#if defined(_AV_WARE_CODE_OPENSOURCE)
#else
#include "AvForm/dlgfactoryset.h"
#endif

namespace Ui {
class FrmDevice;
}

class FrmDevice : public QDialog
{
    Q_OBJECT

public:
    explicit FrmDevice(QWidget *parent = 0);
    ~FrmDevice();

public:
	void DlgDeviceSetShow(int ModifyChannel);
private:
	void FixViewItem();
	void resizeEvent(QResizeEvent * event);
	void showEvent(QShowEvent *e);
	void DrawItemForSearchView();
	void DrawItemForLinkView();
	void DrawItemLinkView();



	void GetSelectForModiyDevies();
	void FixDlgUi();
signals:
	void SignalPreviewStart(int Channel, int Slave, bool bOpen);

private slots:
	void SlotsSearchTimer();

	void SlotsSearchViewRightMenuSelect();
	void SlotsSearchViewRightMenuSelectAll();
	void SlotsSearchViewRightMenuSelectOther();
	void SlotsSearchViewSortFromLargeToSmall();
	void SlotsSearchViewSortFromSmall2Large();
	void SlotsHorizontalScrollBarRangChanged(int min, int max);

	void SlotsDevicesViewRightMenuSelect();
	void SlotsDevicesViewRightMenuSelectAll();
	void SlotsDevicesViewRightMenuSelectOther();

private slots:
    void on_BtnClose_clicked();

    void on_CBoxProtoType_activated(const QString &arg1);

    void on_BtnSearch_clicked();

    void on_BtnAddByHand_clicked();

    void on_BtnModifNet_clicked();

    void on_BtnUpgrade_clicked();

    void on_BtnFacInfo_clicked();

    void on_BtnChnDeleteDevice_clicked();

    void on_BtnChnModifDevice_clicked();

    void on_BtnMax_clicked();

    void on_BtnAddDevice_clicked();

	
	void on_TviewSearchDevice_customContextMenuRequested(const QPoint &pos);

	void on_TviewSearchDevice_doubleClicked(const QModelIndex &index);

	void on_TviewSearchDevice_pressed(const QModelIndex &index);

	void on_TviewChnDevice_customContextMenuRequested(const QPoint &pos);

	void on_TviewChnDevice_doubleClicked(const QModelIndex &index);

	void on_TviewChnDevice_pressed(const QModelIndex &index);

private:
	DlgDeviceSet *	m_DeviceSet;
#if defined(_AV_WARE_CODE_OPENSOURCE)
#else
	DlgFactorySet * m_FactorySet;
#endif
	DlgUpgrade	* m_Upgrade;
	DlgNetSet	* m_NetSet;

	std::list<C_DevSearch> m_DeviceList;
	std::list<C_DevSearch> m_ForModifyDeviceList;
	std::list<C_DevSearch> m_LinkDeviceList;
private:
    Ui::FrmDevice *ui;
	QMenu		*		m_mouseSearchRightMenu;
	QModelIndex			m_SearchItemModelIndex;
	int					m_SearchItemModelcolumn;
	int					m_SearchItemModelrow;

	QMenu		*		m_mouseDevicesRightMenu;
	QModelIndex			m_DevicesItemModelIndex;
	int					m_DevicesItemModelcolumn;
	int					m_DevicesItemModelrow;

private:
	QStandardItemModel *		m_SearchItemModel;
	QStandardItemModel *		m_DevicesItemModel;
    bool						m_WidowsMax;
	QRect						m_localRect;
	int							m_SearchTimerCnt;
	bool						m_bSearching;

};

#endif // FRMDEVICE_H
