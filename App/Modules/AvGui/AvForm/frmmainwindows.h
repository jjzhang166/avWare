#ifndef FRMMAINWINDOWS_H
#define FRMMAINWINDOWS_H

#include <QWidget>
#include "QMouseEvent"
#include "QAvEvent.h"
#include "AvUiComm/FloatingButton.h"



namespace Ui {
class FrmMainWindows;
}

class FrmMainWindows : public QWidget
{
    Q_OBJECT
public:
	explicit FrmMainWindows(QWidget *parent = 0);
	~FrmMainWindows();
public:
	bool eventFilter(QObject *obj, QEvent *event);
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
protected:
	void customEvent(QEvent* event);

private slots:
        void on_BtnMenu_clicked();

        void on_BtnMin_clicked();

        void on_BtnMax_clicked();

        void on_BtnClose_clicked();

        void on_BtnOpenMenu_clicked();

        void on_BtnDevices_clicked();

        void on_BtnSysSet_clicked();

        void on_BtnAlarm_clicked();

        void on_BtnAlm_clicked();

        void on_BtnSysInfo_clicked();

        void on_BtnAlgorithm_clicked();

        void on_BtnVideoWindowInfo_clicked();

private:
	QRect	m_localRect;
	bool	m_mousePressed;
	QPoint  m_mousePoint;
	bool	m_WidowsMax;

	bool	m_bOpenedMenu;
private:
    Ui::FrmMainWindows *ui;
	FloatingButton		*		m_FloatButton;
};

#endif // FRMMAINWINDOWS_H
