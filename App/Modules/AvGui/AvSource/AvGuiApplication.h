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
#include "AvGui/AvGui.h"
#include "AvUiComm/StartupWindow.h"

class CAvGuiApp:public QObject
{
	Q_OBJECT
public:
	av_bool Initialize();
	av_void exec();
	FrmMainWindows *GetMainWindows();
public:
	SINGLETON_DECLARE(CAvGuiApp);
	CAvGuiApp();
private:
	~CAvGuiApp();
public:
	bool	StartingWindow();
	bool	StartingMessage(QString &String, int progress);
	bool	StartingMessage(LoadingMessage m, int progress);
	bool	StartingWindowClose();
private:
	//QSplashScreen *m_SplashScreen;
	QCStartupWindows *m_SplashScreen;
public:
	av_bool PostQAvEvent(QAvEvent &event);
	bool	ConnectFileToProgram();
private:
	FrmMainWindows *m_MainWindows;
	QApplication	m_Application;
};


#define g_AvGuiApp (*CAvGuiApp::instance())










#endif


