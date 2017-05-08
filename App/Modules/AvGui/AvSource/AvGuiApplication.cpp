#include "AvGuiApplication.h"
#include "AvUiComm/StartupWindow.h"

SINGLETON_IMPLEMENT(CAvGuiApp);
static int    AppArgc = 1;
static char *AppArgs = __FILE__;
CAvGuiApp::CAvGuiApp() :m_Application(AppArgc, &AppArgs)
{
	m_MainWindows = NULL;
	m_SplashScreen = NULL;
}

CAvGuiApp::~CAvGuiApp()
{


}

av_bool CAvGuiApp::Initialize()
{
	Q_INIT_RESOURCE(avWare);
	CAvUiComm::SetUTF8Code();
	CAvUiComm::SetFont();
	CAvUiComm::SetStyle("black");//黑色风格
	//CAvUiComm::SetStyle("blue");//蓝色风格
	//CAvUiComm::SetStyle("gray");//灰色风格
	//CAvUiComm::SetStyle("navy");//天蓝色风格
	CAvUiComm::SetChinese();
	

	StartingWindow();
	return av_true;
}






bool	CAvGuiApp::StartingWindow()
{
	QPixmap pixmap(":/image/splash2.png");
#if defined(_AV_WARE_RENDER_BY_RECT)
	QRect Rect = CAvUiComm::GetWindowsOnScreen();
	pixmap = pixmap.scaled(Rect.width(), Rect.height());
	AvQDebug("pixmap [%d %d]\n", pixmap.width(), pixmap.height());
#endif
	m_SplashScreen = new QCStartupWindows(pixmap);
	QFont font;
	font.setFamily("simhei");
	font.setBold(true);
	font.setPixelSize(32);

	m_SplashScreen->setFont(font);

	CAvUiComm::FormInCenter(m_SplashScreen);
	m_SplashScreen->show();
	

	qApp->processEvents();

	return true;
}
bool	CAvGuiApp::StartingMessage(QString &String, int progress)
{
	Qt::Alignment topRight = Qt::AlignLeft | Qt::AlignBottom;
	m_SplashScreen->showMessage(String, topRight, Qt::yellow);//显示信息  
	m_SplashScreen->setProgressBarValue(progress);
	return true;
}
bool	CAvGuiApp::StartingMessage(LoadingMessage m, int progress)
{
	QString msg;
	switch (m)
	{

	case LoadingConfig:
		msg = tr("LoadingConfig");
		break;
	case LoadingLog:
		msg = tr("LoadingLog");
		break;
	case LoadingCapture:
		msg = tr("LoadingCapture");
		break;
	case LoadingAlarmService:
		msg = tr("LoadingAlarmService");
		break;
	case LoadingNetService:
		msg = tr("LoadingNetService");
		break;
	case LoadingIntelligentService:
		msg = tr("LoadingIntelligentService");
		break;
	case LoadingUiResource:
		msg = tr("LoadingUiResource");
		break;
	case LoadingInitDevice:
		msg = tr("LoadingInitDevice");
		break;
	case LoadingInitOver:
		msg = tr("LoadingInitOver");
		break;

	default:
		break;
	}

	StartingMessage(msg, progress);

	return true;
}
bool	CAvGuiApp::StartingWindowClose()
{
	
	StartingMessage(LoadingInitOver, 100);
#if defined(_AV_WARE_RENDER_BY_RECT)
	av_msleep(500);
	StartingMessage(LoadingInitOver, 100);
	av_msleep(1000);
#else
	av_msleep(500);
#endif
	delete m_SplashScreen;
	m_SplashScreen = NULL;
	return true;
}

av_void CAvGuiApp::exec()
{
	m_MainWindows = new FrmMainWindows;
#if defined(WIN32)
	m_MainWindows->show();
	m_MainWindows->SetWidgetWindowsMax();
	//CAvUiComm::FormInCenter(m_MainWindows);
#else
	m_MainWindows->show();
	m_MainWindows->SetWidgetVideoAreaMax();
#endif


#if defined (WIN32) && 0
	FloatingButton	*m_FloatButton = new FloatingButton;
	QObject::connect(m_FloatButton, SIGNAL(SignalsButtonClick()), m_MainWindows, SLOT(on_BtnOpenMenu_clicked()));
	m_FloatButton->show();

#else

#endif

#if defined(_AV_WARE_RENDER_BY_RECT)
	ConnectFileToProgram();
#endif

	StartingWindowClose();
	m_Application.exec();
	Q_CLEANUP_RESOURCE(avWare);

}

FrmMainWindows *CAvGuiApp::GetMainWindows()
{
	return m_MainWindows;
}
av_bool CAvGuiApp::PostQAvEvent(QAvEvent &event)
{
	QApplication::postEvent(m_MainWindows, new QAvEvent(event));
	return av_true;
}

bool CAvGuiApp::ConnectFileToProgram()
{
    QSettings *ukenvFileReg = new QSettings("HKEY_CLASSES_ROOT\\.Avf", QSettings::NativeFormat);
    //判断UKEnv类型是否已在注册表中，并关联了正确的打开方式（程序打开方式），没有则写入
     QString currentValue = ukenvFileReg->value("Default").toString();

     if (currentValue.isEmpty() ||
      currentValue != "OTPFile")
     {
      ukenvFileReg->setValue("Default", "OTPFile");

	  QSettings *ukenvIconReg = new QSettings("HKEY_CLASSES_ROOT\\OTPFile\\DefaultIcon", QSettings::NativeFormat);
      //ukenvIconReg->setValue("Default", QString(qApp->argv()[0]) + ",0");   //把当前运行的程序资源中的第一个图标置入
	  
	  ukenvIconReg->setValue("Default", QString(qApp->arguments().first()) + ",0");   //把当前运行的程序资源中的第一个图标置入
	  delete ukenvIconReg;
	 
      QSettings *ukenvProgramReg = new QSettings("HKEY_CLASSES_ROOT\\OTPFile\\shell\\open\\command", QSettings::NativeFormat);
	  ukenvProgramReg->setValue("Default", "\"" + QString(qApp->arguments().first()) + "\"" + " \"" + "%1" + "\""); //把当前运行的程序路径置入
      delete ukenvProgramReg;
     }
     delete ukenvFileReg;

     return true;
}