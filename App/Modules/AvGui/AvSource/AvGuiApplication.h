#ifndef _AVGUIAPPLICATION_H_
#define _AVGUIAPPLICATION_H_


#include <QObject>
#include "AvUiComm/AvUiComm.h"
#include <QWidget>
#include <QMessageBox>
#include "AvSource/QAvEvent.h"
#include "qglobal.h"
#include "AvForm/frmmainwindows.h"
#include "AvUiComm/FloatingButton.h"
#include <QApplication>
#include <QSplashScreen>
#include <QApplication>
#include "QAvEvent.h"
#include "CAvObject.h"


class CAvGuiApp:public QObject
{
	Q_OBJECT
public:
	av_bool Initialize();
	av_void exec();

public:
	SINGLETON_DECLARE(CAvGuiApp);
	CAvGuiApp();
private:
	~CAvGuiApp();

private:
	av_bool PostQAvEvent(QAvEvent &event);

private:
	FrmMainWindows *m_MainWindows;
	QApplication	m_Application;
};


#define g_AvGuiApp (*CAvGuiApp::instance())










#endif


