#include "AvUiComm/StartupWindow.h"
#include "AvGui/AvGui.h"
#include "AvUiComm/AvUiComm.h"




QCStartupWindows::QCStartupWindows(QPixmap &pixmap) :QSplashScreen(pixmap)
{

	m_ProgressBar = new QProgressBar(this);
	m_ProgressBar->setGeometry(0, pixmap.height() - 5, pixmap.width(), 15);
	m_ProgressBar->setRange(0, 100);
	m_ProgressBar->setValue(0);
	connect(m_ProgressBar, SIGNAL(valueChanged(int)), this, SLOT(SlotOnProgressChanged(int))); //值改变时，立刻repaint  
	
	QFont font;
	font.setPointSize(48);
	setFont(font); 

	m_ProgressBarValue = 0;
}
QCStartupWindows::~QCStartupWindows()
{
	delete m_ProgressBar;
	m_ProgressBar = NULL;


}
void QCStartupWindows::SlotOnProgressChanged(int value)
{
	AvQDebug("ProgressChanged %d\n", value);
}

void QCStartupWindows::setProgressBarValue(int value)
{
	m_ProgressBarValue = value;
	m_ProgressBar->setValue(value);
}

