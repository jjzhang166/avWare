#ifndef _STARTUP_WINDOWS_H_
#define _STARTUP_WINDOWS_H_






#include <QSplashScreen>  
#include <QPixmap>  
#include <QProgressBar>  
#include <QList>  
#include <QtGlobal>  


class QCStartupWindows : public QSplashScreen
{
	Q_OBJECT

public:
	QCStartupWindows(QPixmap &pixmap);
	~QCStartupWindows();
public:
	void setProgressBarValue(int value);
	//void startLeftProgress();
private slots:
	void SlotOnProgressChanged(int);


private:
	QProgressBar *	m_ProgressBar;
	//QTimer		 *	m_Timer;
	int				m_ProgressBarValue;
};





#endif

