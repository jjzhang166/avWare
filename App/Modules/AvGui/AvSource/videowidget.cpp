#include "videowidget.h"
#include "AvUiComm/AvUiComm.h"
#include <QDebug>
#include <QMouseEvent>
#include <QMenu>
#include <QIcon>
#include "Layout.h"
#include "AvForm/dlgptzwindows.h"

VideoWidget::VideoWidget(QWidget *parent) :
    QWidget(parent)
{
    QPalette palette;
    palette.setColor(this->backgroundRole(), Qt::black);
	//palette.setColor(this->backgroundRole(), Qt::white);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    m_bSelected = false;
    this->setMouseTracking(true);
	m_VideoInfoWindows = NULL;
	m_Channel = 0;
	m_TotalWindows = 0;
// 	m_VideoInfoWindows = new WgetVideoInfo(parent);
// 	QObject::connect(m_VideoInfoWindows, SIGNAL(SignalsMousePress(QMouseEvent *)), this, SLOT(mousePressEvent(QMouseEvent *)));
// 	QObject::connect(m_VideoInfoWindows, SIGNAL(SignalsMouseDoubleClickEvent(QMouseEvent *)), this, SLOT(mouseDoubleClickEvent(QMouseEvent *)));
// 	QObject::connect(m_VideoInfoWindows, SIGNAL(SignalsContextMenuEvent(QContextMenuEvent*)), this, SLOT(SlotsContextMenuEvent(QContextMenuEvent *)));
// 	
	m_Preview = new CAvPreview;
	m_bPreview = false;
}

VideoWidget::~VideoWidget()
{
	if (NULL != m_VideoInfoWindows) {
		delete m_VideoInfoWindows;
	}
}
void VideoWidget::SetTotalWindows(int totalWins)
{
	m_TotalWindows = totalWins;
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
void VideoWidget::SlotsMenuTriggered(QAction *Action)
{
	Action->text();
	printf("triggered %s\n", Action->text().toStdString().c_str());
	QString text = Action->text();


	if (text == QString("1")){
		emit SignalsSpiltScreen(LAYOUT_MODE_ONE);
	}
	else if (text == QString("4")){
		emit SignalsSpiltScreen(LAYOUT_MODE_FOUR);
	}
	else if (text == QString("6")){
		emit SignalsSpiltScreen(LAYOUT_MODE_SIX);
	}
	else if (text == QString("8")){
		emit SignalsSpiltScreen(LAYOUT_MODE_EIGHT);
	}
	else if (text == QString("9")){
		emit SignalsSpiltScreen(LAYOUT_MODE_NINE);
	}
	else if (text == QString("16")){
		emit SignalsSpiltScreen(LAYOUT_MODE_SIXTEEN);
	}
	else if (text == QString("25")){
		emit SignalsSpiltScreen(LAYOUT_MODE_TWENTYFIVE);
	}
	else if (text == QString("36")){
		emit SignalsSpiltScreen(LAYOUT_MODE_THIRTYSIZ);
	}
	else if (text == QString("64")){
		emit SignalsSpiltScreen(LAYOUT_MODE_SIZTYFOUR);
	}
	else if (text == QString("Ptz")){
		DlgPtzWindows *PtzWindows = new DlgPtzWindows;
		PtzWindows->exec();
	}
	if (NULL != m_VideoInfoWindows){
		m_VideoInfoWindows->raise();
	}

}

void VideoWidget::SlotsContextMenuEvent(QContextMenuEvent *event)
{
	contextMenuEvent(event);
}
void VideoWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu *Menu = new QMenu(this);
	QIcon  MenuIcon;

	QObject::connect(Menu, SIGNAL(triggered(QAction *)), this, SLOT(SlotsMenuTriggered(QAction *)));
	QMenu *Menu2 = Menu->addMenu("VideoWidgets");
	QMenu *Menu3 = Menu2->addMenu("4Widgets");
	QAction *Action;
	Menu3->addAction(new QAction(" 1-4", this));
	Menu3->setFixedHeight(100);
	Menu3->addAction(new QAction(" 5-8", this));
	QMenu *Menu4 = Menu2->addMenu("8Widgets");
	Menu4->addAction(new QAction(" 1-8", this));
	Menu4->addAction(new QAction(" 9-16", this));

	MenuIcon.addFile(":/image/checkbox_checked.png");
	Menu->addAction(MenuIcon, "   new");
	QMenu *MenuSplit = Menu->addMenu("SliptScreen");
	MenuSplit->addAction(new QAction("1", this));
	MenuSplit->addAction(new QAction("4", this));
	MenuSplit->addAction(new QAction("6", this));
	MenuSplit->addAction(new QAction("8", this));
	MenuSplit->addAction(new QAction("9", this));
	MenuSplit->addAction(new QAction("16", this));
	MenuSplit->addAction(new QAction("25", this));
	MenuSplit->addAction(new QAction("36", this));
	MenuSplit->addAction(new QAction("64", this));
	Menu->addAction(new QAction("   stop", this));
	Menu->addAction(new QAction("   save", this));
	Menu->addAction(new QAction("Ptz", this));



	Menu->move(cursor().pos());
	Menu->setMinimumWidth(300);
	Menu->show();
}

// void VideoWidget::move(const QPoint &point)
// {
// 	QWidget::move(point);
// 	if (NULL != m_VideoInfoWindows){
// 		m_VideoInfoWindows->move(point);
// 	}
// 
// }

void VideoWidget::setGeometry(const QRect &Rect)
{
	
	QWidget::setGeometry(Rect);
	if (NULL != m_VideoInfoWindows){
		m_VideoInfoWindows->setGeometry(Rect);
		m_VideoInfoWindows->raise();
	}

}
// void VideoWidget::show()
// {
// 	QWidget::show();
// 	if (NULL != m_VideoInfoWindows){
// 		m_VideoInfoWindows->show();
// 		m_VideoInfoWindows->raise();
// 	}
// 
// 	
// }
// void VideoWidget::hide()
// {
// 	QWidget::hide();
// 	if (NULL != m_VideoInfoWindows){
// 		m_VideoInfoWindows->hide();
// 	}
// 
// }

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
	AvQDebug("mousePressEvent\n");
	if (!m_bSelected){
		emit widgetSelecting(this);
	}
		
	if (NULL != m_VideoInfoWindows){
		m_VideoInfoWindows->raise();
	}
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *)
{
    if(this->isMaximized())
    {
        this->setWindowFlags(Qt::SubWindow);
        this->showNormal();
		if (NULL != m_VideoInfoWindows){
			m_VideoInfoWindows->setWindowFlags(Qt::SubWindow);
			m_VideoInfoWindows->showNormal();
		}
        emit videoWidgetResize();
		emit SignalsMaxWindows(this, false);
		//PreviewStart(m_Channel, CHL_SUB1_T, true);
		int Slave = CHL_SUB1_T;
		if (m_TotalWindows <= 4 || (m_TotalWindows < 9 && m_Channel == 0)){
			Slave = CHL_MAIN_T;
		}
		else {
			Slave = CHL_SUB1_T;
		}
		PreviewStart(m_Channel, Slave, true);

    }
    else
    {
        this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        this->showMaximized();
		if (NULL != m_VideoInfoWindows){
			m_VideoInfoWindows->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
			m_VideoInfoWindows->showMaximized();
		}
		QRect cur = CAvUiComm::GetWindowsOnScreen();
		this->setGeometry(cur);
		emit SignalsMaxWindows(this, true);
		PreviewStart(m_Channel, CHL_MAIN_T, true);
    }
}

void VideoWidget::PreviewStart(int Channel, int Slave, bool bOpen)
{
	AvQDebug("PreviewStart Chn[%d][%d] [%s]\n", Channel, Slave, bOpen == true?"open":"close");

	if (bOpen == true){
		int RunChannel, RunSlave;
		m_Preview->GetChannleSlave(RunChannel, RunSlave);
		if (Channel == RunChannel && RunSlave == Slave){
			AvQDebug("RunChannel Slave [%d][%d] so return \n", RunChannel, RunSlave);
			return;
		}
	}
	if (m_bPreview == true){
		m_Preview->Stop();
		m_bPreview = false;
	}
	if (bOpen == true){
		m_Channel = Channel;
		m_Preview->Set(winId(), Channel);
		m_Preview->Start(Channel, Slave);
		m_bPreview = true;
	}

}