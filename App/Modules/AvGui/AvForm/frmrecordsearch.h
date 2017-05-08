#ifndef FRMRECORDSEARCH_H
#define FRMRECORDSEARCH_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class FrmRecordSearch;
}

class FrmRecordSearch : public QWidget
{
    Q_OBJECT

public:
    explicit FrmRecordSearch(QWidget *parent = 0);
    ~FrmRecordSearch();
public:
	void FixUiDlg();

private slots:
    void on_BtnClose_clicked();

private:
    Ui::FrmRecordSearch *ui;
	QStandardItemModel *		m_ChannelItemModel;
};

#endif // FRMRECORDSEARCH_H
