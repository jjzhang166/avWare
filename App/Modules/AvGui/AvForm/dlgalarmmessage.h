#ifndef DLGALARMMESSAGE_H
#define DLGALARMMESSAGE_H

#include <QDialog>
#include <QStandardItemModel>
#include "Apis/AvWareStruct.h"
namespace Ui {
class DlgAlarmMessage;
}

class DlgAlarmMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DlgAlarmMessage(QWidget *parent = 0);
    ~DlgAlarmMessage();
public:
	void 	FixDlgUi();
	void	AddAlmItem(C_AlmMsg &Msg);

private:
	void	ResizeUi();
protected:
	void showEvent(QShowEvent *e);
	void resizeEvent(QResizeEvent *e);
private slots:
    void on_BtnAlarmMessageDeleteAll_clicked();

    void on_BtnClose_clicked();

private:
    Ui::DlgAlarmMessage *ui;
	QStandardItemModel *		m_MessageItemModel;
};

#endif // DLGALARMMESSAGE_H
