#ifndef FRMNOTIFICATIONBOX_H
#define FRMNOTIFICATIONBOX_H

#include <QWidget>

namespace Ui {
class FrmNotificationBox;
}

class FrmNotificationBox : public QWidget
{
    Q_OBJECT

public:
    explicit FrmNotificationBox(QWidget *parent = 0);
    ~FrmNotificationBox();
public:
	void SetShowInfo(QString &Title, QString &Message, int ShowTimeOut = 3000);
protected:
	void showEvent(QShowEvent *event);
	
private slots:
    void on_BtnClose_clicked();
	void SlotsInitTimer();
private:
    Ui::FrmNotificationBox *ui;
	
private:
	int		m_ShowTimeOut;
};

#endif // FRMNOTIFICATIONBOX_H
