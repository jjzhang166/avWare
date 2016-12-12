#include "AvGuiApplication.h"


SINGLETON_IMPLEMENT(CAvGuiApp);
static int    AppArgc = 1;
static char *AppArgs = __FILE__;
CAvGuiApp::CAvGuiApp() :m_Application(AppArgc, &AppArgs)
{
	m_MainWindows = NULL;

}

CAvGuiApp::~CAvGuiApp()
{


}

av_bool CAvGuiApp::Initialize()
{
	Q_INIT_RESOURCE(avWare);
	CAvUiComm::SetUTF8Code();
	CAvUiComm::SetStyle("black");//黑色风格
	//CAvUiComm::SetStyle("blue");//蓝色风格
	//CAvUiComm::SetStyle("gray");//灰色风格
	//CAvUiComm::SetStyle("navy");//天蓝色风格
	CAvUiComm::SetChinese();



	m_MainWindows = new FrmMainWindows;
	m_MainWindows->show();
	CAvUiComm::FormInCenter(m_MainWindows);

	FloatingButton	*m_FloatButton = new FloatingButton;
	// 	QPoint point = this->pos();
	// 	QRect rect = this->rect();
	// 	point.setX(point.x() + rect.width() / 3 * 2);
	// 	point.setY(point.y() + rect.height() / 3 * 2);
	// 	m_FloatButton->move(point);
	QObject::connect(m_FloatButton, SIGNAL(SignalsButtonClick()), m_MainWindows, SLOT(on_BtnOpenMenu_clicked()));

	m_FloatButton->show();

	return av_true;
}

av_void CAvGuiApp::exec()
{


	//QAvEvent event(QAvEvent::QAvEvent_SearchDevice);
	//PostQAvEvent(event);
	m_Application.exec();

	Q_CLEANUP_RESOURCE(avWare);

}


av_bool CAvGuiApp::PostQAvEvent(QAvEvent &event)
{
	QApplication::postEvent(m_MainWindows, new QAvEvent(event));
	return av_true;
}

