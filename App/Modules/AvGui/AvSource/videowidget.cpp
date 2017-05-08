#include "videowidget.h"
#include "AvUiComm/AvUiComm.h"
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QIcon>
#include "Layout.h"
#include "AvForm/dlgptzwindows.h"
#include "AvSource/AvGuiStatusMachine.h"
#include "AvGuiApplication.h"


VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette;
    palette.setColor(this->backgroundRole(), Qt::black);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    m_bSelected = false;
    this->setMouseTracking(true);
	m_Channel = -1;
	m_TotalWindows = 0;
	m_Preview = new CAvPreview;
	m_bPreview = false;
}

VideoWidget::~VideoWidget()
{
	if (m_bPreview == true){
		m_Preview->Stop();
	}
	m_bPreview = false;
	delete m_Preview;
}

void VideoWidget::VideoWidgetInit(int ToTalWins, int ScreenID, int SysChannel)
{
	m_ScreenID = ScreenID;
	m_TotalWindows = ToTalWins;
	m_Channel = SysChannel;
#if defined(WIN32)
	m_Preview->Set(-1, m_ScreenID);
#else
	m_Preview->Set(winId(), m_ScreenID);
#endif
}
void VideoWidget::VideoWidgetDeInit()
{
	m_Preview->Stop();
	m_bPreview = false;
}

void VideoWidget::PreviewSetStatistics(bool bOpen)
{
	if (NULL != m_Preview){
		m_Preview->SetStatistics(bOpen == true ? av_true : av_false);
	}
}
void VideoWidget::PreviewGetStatistics(QString &ImageSize, QString &Comp, QString &FrameRate, QString &BitRate)
{
	if (NULL != m_Preview){
		std::string stdImageSize, stdComp, stdFrameRate, stdBitRate;
		av_bool ret = m_Preview->GetStatistics(stdImageSize, stdComp, stdFrameRate, stdBitRate);
		ImageSize.append(stdImageSize.c_str());
		Comp.append(stdComp.c_str());
		FrameRate.append(stdFrameRate.c_str());
		BitRate.append(stdBitRate.c_str());
	}
	else{

	}
}
void VideoWidget::SelectWindowsLive()
{
	m_Preview->StartAudio(av_false);
}
void VideoWidget::SlotsMenuTriggered(QAction *Action)
{
	Action->text();
	printf("triggered %s\n", Action->text().toStdString().c_str());
	QString text = Action->text();

	if (text == tr("IpcSet")){
		QAvEvent AvEvent(QAvEvent::QAvEvent_ShowDeviceSet);
		AvEvent.FillInUsrData((char *)&m_Channel, sizeof(m_Channel), 0);
		g_AvGuiApp.PostQAvEvent(AvEvent);
	}
	else if (text == tr("RecordSet")){
		QAvEvent AvEvent(QAvEvent::QAvEvent_ShowRecordSet);
		g_AvGuiApp.PostQAvEvent(AvEvent);
	}
	else if (text == tr("PlayBack")){
		QAvEvent AvEvent(QAvEvent::QAvEvent_ShowRecordPlay);
		g_AvGuiApp.PostQAvEvent(AvEvent);
	}
	else if (text == tr("Ptz")){
		if (m_Channel < 0 || m_Channel >= SYS_CHN_NUM){
			CAvUiComm::ShowMessageBoxError(tr("Channel Can't Linked !"));
		}
		else{
			AvQDebug("DlgPtzWindows Channel = %d\n", m_Channel);
			DlgPtzWindows *PtzWindows = new DlgPtzWindows;
			PtzWindows->SetModiyChannel(m_Channel);
			PtzWindows->exec();
		}

	}
	else if (text == tr("MediaInfo")){
		QAvEvent AvEvent(QAvEvent::QAvEvent_MediaInfo);
		g_AvGuiApp.PostQAvEvent(AvEvent);
	}
	else if (text == tr("MainMenu")){
		QAvEvent AvEvent(QAvEvent::QAvEvent_ShowMainMenum);
		g_AvGuiApp.PostQAvEvent(AvEvent);
	}
	else{//分屏信息
		QString String;
		String = tr("PIC ");
		String += QString::number(SYS_CHN_NUM);
		if (text == String){
			av_warning("Splite %d all channel\n", SYS_CHN_NUM);
			emit SignalsSpiltScreen(SYS_CHN_NUM, 1, SYS_CHN_NUM);
		}
		else{
			char splitinfo[128];
			int Start_Channel;
			int End_Channel;
			int spliteNum;

			sprintf(splitinfo, "%s", Action->text().toStdString().c_str());

			int ret = sscanf(splitinfo, "%d-%d", &Start_Channel, &End_Channel);
			if (ret == 2){
				AvQDebug("splite get Start[%d] End[%d] SpliteNum [%d]\n", Start_Channel, End_Channel, End_Channel- Start_Channel + 1);
				emit SignalsSpiltScreen(End_Channel - Start_Channel + 1, Start_Channel, End_Channel);
			}
			else{
				AvQError("splite get error message [%s]\n", splitinfo);
			}
		}
	}
}

void VideoWidget::SlotsContextMenuEvent(QContextMenuEvent *event)
{
	contextMenuEvent(event);
}

void  VideoWidget::DrawMenuSplit(QMenu *MenuSplit, int SplitNum)
{
	QString String;
	String = tr("PIC ");
	//av_warning("Add DrawMenuSplit %d\n", SplitNum);
	String += QString::number(SplitNum);
	if (SplitNum == SYS_CHN_NUM){
		MenuSplit->addAction(String);
	}
	else{
		QMenu *ChannelQMenu = MenuSplit->addMenu(String);
		DrawMenuSplitChannel(ChannelQMenu, SplitNum);
	}
}
void  VideoWidget::DrawMenuSplitChannel(QMenu *MenuSplit, int SplitNum)
{
	int iteam = 0;
	iteam = SYS_CHN_NUM / SplitNum;
	if (0 != SYS_CHN_NUM%SplitNum){
		iteam += 1;
	}
	QString String;
	int Start_Channel;
	int End_Channel;
	for (int i = 0; i < iteam; i++){
		String.clear();
		Start_Channel = i*SplitNum + 1;
		End_Channel = i*SplitNum + SplitNum;
		if (End_Channel > SYS_CHN_NUM){
			End_Channel = SYS_CHN_NUM;
			Start_Channel = End_Channel - SplitNum + 1;
		}
		String = String.sprintf("%d-%d", Start_Channel, End_Channel);
		MenuSplit->addAction(String);
	}
}

void VideoWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *MenuMain = new QMenu(this);
//	QIcon  MenuIcon;
	QObject::connect(MenuMain, SIGNAL(triggered(QAction *)), this, SLOT(SlotsMenuTriggered(QAction *)));
	
	QMenu *MenuSplit = MenuMain->addMenu(tr("SplitScreen"));
	{
		switch (SYS_CHN_NUM)
		{
		case 128:
		{
			DrawMenuSplit(MenuSplit, 128);
		}
		case 64:
		{
			DrawMenuSplit(MenuSplit, 64);
		}
		case 36:
		{
			DrawMenuSplit(MenuSplit, 36);
		}
		case 25:
		{
			DrawMenuSplit(MenuSplit, 25);
		}
		case 16:
		{
			DrawMenuSplit(MenuSplit, 16);
		}
		case 9:
		{
			DrawMenuSplit(MenuSplit, 9);
		}
		case 8:
		{
			DrawMenuSplit(MenuSplit, 8);
		}
		case 6:
		{
			DrawMenuSplit(MenuSplit, 6);
		}
		case 4:
		{
			DrawMenuSplit(MenuSplit, 4);
		}
		
		}
	}
	MenuMain->addAction(tr("Ptz"));
	MenuMain->addAction(tr("IpcSet"));
	MenuMain->addAction(tr("RecordSet"));
	MenuMain->addAction(tr("PlayBack"));
	//MenuMain->addAction(tr("MediaInfo"));
	MenuMain->addAction(tr("MainMenu"));
	MenuMain->move(cursor().pos());
	MenuMain->show();
}


void VideoWidget::setGeometry(const QRect &Rect)
{
	
	QWidget::setGeometry(Rect);

}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
	AvQDebug("mousePressEvent\n");
	if (!m_bSelected){
		emit widgetSelecting(this);
	}
		

}

void VideoWidget::MaxMinWindowsPc(bool bMax)
{
	if (bMax == true){

		this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
		this->showMaximized();
		QRect cur = CAvUiComm::GetWindowsOnScreen();
		this->setGeometry(cur);
		emit SignalsMaxWindows(this, true);
		PreviewStart(CHL_MAIN_T, true);
	}
	else{
		this->setWindowFlags(Qt::SubWindow);
		this->showNormal();
		emit videoWidgetResize();
		emit SignalsMaxWindows(this, false);
		int Slave = CHL_SUB1_T;
		if (m_TotalWindows <= 4 || (m_TotalWindows < 9 && m_Channel == 0)){
			Slave = CHL_MAIN_T;
		}
		else {
			Slave = CHL_SUB1_T;
		}
		PreviewStart(Slave, true);
	}
}
void VideoWidget::MaxMinWindowsEmbedded(bool bMax)
{
	CAvGuiStatus::instance()->SetMaximizedWindows(bMax);
	m_bSelected = false;
	if (bMax == true){
		
		emit SignalsSpiltScreen(1, m_Channel + 1, m_Channel + 1);
	}
	else{
		int lastSplitNum, lastStartChannel, lastEndChannel;
		lastSplitNum = CAvGuiStatus::instance()->SplitGetLastNum();
		CAvGuiStatus::instance()->SplitGetLastStartChannel(lastStartChannel, lastEndChannel);
		emit SignalsSpiltScreen(lastSplitNum, lastStartChannel, lastEndChannel);
	}
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *)
{
#if defined (_AV_WARE_RENDER_BY_RECT)
		if (true == CAvGuiStatus::instance()->IsMaximizedWindows()){
			AvQDebug("Min Windows\n");
			MaxMinWindowsEmbedded(false);
		}
		else{
			AvQDebug("Max windows\n");
			MaxMinWindowsEmbedded(true);
		}
#else
		if (this->isMaximized())
		{
			MaxMinWindowsPc(false);
		}
		else
		{
			MaxMinWindowsPc(true);
		}
#endif
}
bool VideoWidget::PreviewGetCHLSlave(int &Channel, int &Slave)
{
	m_Preview->GetChannleSlave(Channel, Slave);
	return m_bPreview;
}
void VideoWidget::PreviewStart(int Slave, bool bOpen)
{
	AvQDebug("PreviewStart Chn[%d][%d] [%s]\n", m_Channel, Slave, bOpen == true?"open":"close");

	if (bOpen == true){
		int RunChannel, RunSlave;
		m_Preview->GetChannleSlave(RunChannel, RunSlave);
		if (m_Channel == RunChannel && RunSlave == Slave){
			AvQDebug("RunChannel Slave [%d][%d] so return \n", RunChannel, RunSlave);
			return;
		}
	}
	if (m_bPreview == true){
		m_Preview->Stop();
		m_bPreview = false;
	}
	if (bOpen == true){
/*		m_Channel = Channel;*/
		m_Preview->Set(winId(), m_ScreenID);
		m_Preview->Start(m_Channel, Slave);
		m_bPreview = true;
	}

}
void VideoWidget::SyncWidgetSize(QRect &Rect)
{
	QPoint point = pos();
	if (NULL == g_AvGuiApp.GetMainWindows()){
		return;
	}
	QPoint gPoint = g_AvGuiApp.GetMainWindows()->GetVideoAreaToGlobal();


	C_RECT cRect;
	cRect.Sx = point.x() + gPoint.x();
	cRect.Sy = point.y() + gPoint.y();

	cRect.Width = Rect.size().width();
	cRect.Heigh = Rect.size().height();
	m_Preview->RenderResize(cRect);
}

void VideoWidget::resizeEvent(QResizeEvent *e)
{
	QRect qRect;
	qRect.setSize(e->size());
	SyncWidgetSize(qRect);
}