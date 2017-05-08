#ifndef _AVUICOMM_H_
#define _AVUICOMM_H_
#include "Apis/AvWareCplusplus.h"
#include <QtCore>
#include <QtGui>
#include <QDesktopWidget>
#include <QCursor>
#include <QApplication>
#include "frmmessagebox.h"
#include <QTextCodec>  
#include "frmnotificationbox.h"


#define AvQDebug av_msg
#define AvQError av_error
#define AvQWaring av_warning
class CAvUiComm: public QObject
{

public:

    //设置为开机启动
    static void AutoRunWithSystem(bool IsAutoRun, QString AppName, QString AppPath)
    {
        QSettings *reg = new QSettings(
            "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
            QSettings::NativeFormat);

        if (IsAutoRun) {
            reg->setValue(AppName, AppPath);
        } else {
            reg->setValue(AppName, "");
        }
    }
	//设置字体
	static void SetFont()
	{
		QFont font;
		font.setFamily("simhei");
		font.setBold(false);
		qApp->setFont(font);
	}
    //设置编码为UTF8
	static void SetUTF8Code()
    {
#if (QT_VERSION <= QT_VERSION_CHECK(5,8,0))
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
//         QTextCodec::setCodecForCStrings(codec);
//         QTextCodec::setCodecForTr(codec);
#endif
    }

    //设置皮肤样式
    static void SetStyle(const QString &styleName)
    {
		//
       QFile file(QString(":/style/%1.css").arg(styleName));
       bool ret = file.open(QFile::ReadOnly);

       QString qss = QLatin1String(file.readAll());
       qApp->setStyleSheet(qss);
       qApp->setPalette(QPalette(QColor("#F0F0F0")));
	   //qApp->setPalette(QPalette(QColor("#000000")));
    }

    //加载中文字符
    static void SetChinese()
    {
        QTranslator *translator = new QTranslator(qApp);
		//bool ret = translator->load("zh_CN.qm", "./");
		bool ret = translator->load(":/languages/zh_CN.qm");
		if (ret != true){
			AvQDebug("load zh_CN.qm Error\n");
		}
        ret = qApp->installTranslator(translator);
		if (ret != true){
			AvQDebug("installTranslator zh_CN.qm Error\n");
		}
    }

    //判断是否是IP地址
    static bool IsIP(QString IP)
    {
        QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
        return RegExp.exactMatch(IP);
    }

    //显示信息框,仅确定按钮
    static void ShowMessageBoxInfo(QString info)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(info, 0);
        msg->exec();
    }

    //显示错误框,仅确定按钮
    static void ShowMessageBoxError(QString info)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(info, 2);
        msg->exec();
    }

    //显示询问框,确定和取消按钮
    static int ShowMessageBoxQuesion(QString info)
    {
        frmMessageBox *msg = new frmMessageBox;
        msg->SetMessage(info, 1);
        return msg->exec();
    }

	static int ShowNotificationBox(QString Message, QString Title=QString(tr("Notification")), int TimeOut = 3000)
	{
		static FrmNotificationBox *_msg = NULL; 
		if (NULL == _msg){
			_msg = new FrmNotificationBox;
		}
		_msg->SetShowInfo(Title, Message, TimeOut);
		_msg->show();
		return 0;
	}
    //延时
    static void Sleep(int sec)
    {
        QTime dieTime = QTime::currentTime().addMSecs(sec);
        while ( QTime::currentTime() < dieTime ) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    //窗体居中显示
    static void FormInCenter(QWidget *frm)
    {
        int frmX = frm->width();
        int frmY = frm->height();
        QDesktopWidget w;
		QRect deskRect = GetWindowsOnScreen();
		int deskWidth = deskRect.width();
		int deskHeight = deskRect.height();
		int frmx = deskWidth / 2 - frmX / 2 + deskRect.x();
		int frmy = deskHeight / 2 - frmY / 2 + deskRect.y();

        QPoint movePoint(frmx, frmy);
        frm->move(movePoint);
    }

	static void FormInLeftDwon(QWidget *frm)
	{
		int frmX = frm->width();
		int frmY = frm->height();
		QDesktopWidget w;
		QRect deskRect = GetWindowsOnScreen();
		int deskWidth = deskRect.width();
		int deskHeight = deskRect.height();

		int frmx = deskWidth - frmX  + deskRect.x();
		int frmy = deskHeight - frmY + deskRect.y();
		QPoint movePoint(frmx, frmy);
		frm->move(movePoint);
	}

	static std::list<QRect> GetScreenInfo()
	{
		std::list<QRect> ret;
		QDesktopWidget *desktopWidget = QApplication::desktop();
		QRect deskRect;
		QRect screenRect;
		int screenTotal = desktopWidget->screenCount();
		for (int i = 0; i < screenTotal; i++){
			deskRect = desktopWidget->availableGeometry(i);
			screenRect = desktopWidget->screenGeometry(i);
			ret.push_back(screenRect);
		}
		return ret;
	}

	static QRect GetWindowsOnScreen()
	{
		QPoint curRect = QCursor::pos();
		QDesktopWidget *desktopWidget = QApplication::desktop();

		int curMonitor = desktopWidget->screenNumber(curRect);
		QRect screenRect;
		QRect deskRect;
		screenRect = desktopWidget->screenGeometry(curMonitor);
		deskRect = desktopWidget->availableGeometry(curMonitor);
		return deskRect;
	}

	static QString GBK2UTF8(const QString &inStr)
	{
		QTextCodec *gbk = QTextCodec::codecForName("GB18030");
		QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

		QString g2u = gbk->toUnicode(gbk->fromUnicode(inStr));              // gbk  convert utf8  
		return g2u;
	}

	static QString UTF82GBK(const QString &inStr)
	{
		//QTextCodec *gbk = QTextCodec::codecForName("GB18030");
		QTextCodec *gbk = QTextCodec::codecForName("GB2312");
		QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

		QString utf2gbk = gbk->toUnicode(inStr.toLocal8Bit());
		return utf2gbk;
	}

	static std::string gbk2utf8(const QString &inStr)
	{
		return GBK2UTF8(inStr).toStdString();
	}

	static QString utf82gbk(const std::string &inStr)
	{
		QString str = QString::fromStdString(inStr);

		return UTF82GBK(str);
	}

};



#endif // MYHELPER_H
