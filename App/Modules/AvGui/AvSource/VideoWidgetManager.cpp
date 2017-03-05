#include "AvGui/AvGui.h"
#include "AvUiComm/AvUiComm.h"
#include "VideoWidgetManager.h"
#include <QPainter>
#include <QEvent>


#define GRID_LINE_WIDTH     1
//#define GRID_LINE_COLOR     QColor(255, 255, 128)
#define GRID_LINE_COLOR     QColor(192, 192, 192)


VideoWidgetManager::VideoWidgetManager(QWidget *parent)
: QWidget(parent)
{
	m_WidgetLayoutNum = 0;
    initVideoWidgetManager();
    this->setMouseTracking(true);

	m_WgetVideoInfo = new WgetVideoInfo(this);
	m_WgetVideoInfo->setMouseTracking(true);
	m_WgetVideoInfo->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
	m_WgetVideoInfo->setAttribute(Qt::WA_TranslucentBackground, true);
	m_WgetVideoInfo->hide();
	m_bOpenVideoInfoWindows = false;
	
	connect(m_WgetVideoInfo, SIGNAL(SignalsUiButtonMessage(WGET_VIDEOINFO_UIMSG)), this, SLOT(SlotVideoInfoOnUiMsg(WGET_VIDEOINFO_UIMSG)));


	startTimer(1000);

}


VideoWidgetManager::~VideoWidgetManager()
{
	
}

void VideoWidgetManager::initVideoWidgetManager()
{    
	setLayoutMode(LAYOUT_MODE_FOUR);
	//setLayoutMode(LAYOUT_MODE_SIZTYFOUR);
}

void VideoWidgetManager::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);

	painter.setClipRegion(event->rect());
    
    drawGrid(painter, m_WidgetLayout.rows, m_WidgetLayout.cols);
}

void VideoWidgetManager::resizeEvent(QResizeEvent * event)
{
    resizeVideoWidgets(event->size().width(), event->size().height());
	VideoWidget * pSelectedWidget = GetSelectedWidget();
	if (pSelectedWidget != NULL){
		AvQDebug("resizeevent\n");
		QPoint tmpPos = mapToGlobal(pSelectedWidget->pos());
		m_WgetVideoInfo->resize(pSelectedWidget->size());
		m_WgetVideoInfo->move(tmpPos);
		m_WgetVideoInfo->raise();
	}
}
void	VideoWidgetManager::timerEvent(QTimerEvent *event)
{
	VideoWidget * pSelectedWidget = GetSelectedWidget();
	if (pSelectedWidget != NULL){
		QString ImageSize, EnCodeType, FrameRate, BitRate;
		pSelectedWidget->PreviewGetStatistics(ImageSize, EnCodeType, FrameRate, BitRate);
		m_WgetVideoInfo->SetOSDVideoInfo(BitRate, ImageSize, FrameRate, EnCodeType);
	}
}
void	VideoWidgetManager::moveEvent(QMoveEvent *event)
{
	
	VideoWidget * pSelectedWidget = GetSelectedWidget();
	if (NULL != pSelectedWidget){
		QPoint tmpPos = mapToGlobal(pSelectedWidget->pos());
		m_WgetVideoInfo->resize(pSelectedWidget->size());
		m_WgetVideoInfo->update();
		m_WgetVideoInfo->move(tmpPos);
		m_WgetVideoInfo->raise();
// 		m_WgetVideoInfo->SetOSDVideoInfo(QString("256 Kbps"), 
// 			QString("1920*1080"), QString("25 FPS"), QString("H264"));
	}
}
void VideoWidgetManager::resizeVideoWidgets(int w, int h)
{
    QRect rect;
    for (int i = 0; i < m_WidgetLayout.zones.size(); ++i)
    {
        WidgetZone & widgetZone = m_WidgetLayout.zones[i];
        
        rect = getZoneRect(w, h, widgetZone.zone);
		
		widgetZone.widget->setGeometry(rect);
    }
}

void VideoWidgetManager::drawGrid(QPainter & painter, int rows, int cols)
{
    QRect rect;
    QRect focusRect(0, 0, -1, -1);

	painter.setPen(QPen(GRID_LINE_COLOR, GRID_LINE_WIDTH, Qt::SolidLine));
    
    for (int i = 0; i < m_WidgetLayout.zones.size(); ++i)
    {
        WidgetZone & widgetZone = m_WidgetLayout.zones[i];
        
        rect = getZoneRect(width(), height(), widgetZone.zone);
        rect = rect.adjusted(-GRID_LINE_WIDTH, -GRID_LINE_WIDTH, GRID_LINE_WIDTH, GRID_LINE_WIDTH);

        painter.drawRect(rect);

        if (widgetZone.widget->m_bSelected)
        {
            focusRect = rect;
        }
    }

    if (focusRect.isValid())
    {
		AvQDebug("draw select \n");
        painter.setPen(QPen(Qt::blue, GRID_LINE_WIDTH+1));
        painter.drawRect(focusRect);
	}
	else{
		m_WgetVideoInfo->hide();
		m_bOpenVideoInfoWindows = false;
	}
}

void VideoWidgetManager::setLayoutMode(int layoutMode)
{
	CAvPreview::SetSpiltScreen(layoutMode);
	m_WidgetLayoutNum = layoutMode;
    LayoutZoneList zones;
    switch (layoutMode)
    {
    case LAYOUT_MODE_ONE:
        addZone(zones, 0, 0, 0, 0);
        
        setLayout(1, 1, zones);
        break;

    case LAYOUT_MODE_FOUR:
        addZone(zones, 0, 0, 0, 0);
        addZone(zones, 1, 1, 0, 0);
        addZone(zones, 0, 0, 1, 1);
        addZone(zones, 1, 1, 1, 1);

        setLayout(2, 2, zones);
        break;

    case LAYOUT_MODE_SIX:
        addZone(zones, 0, 1, 0, 1);
        addZone(zones, 2, 2, 0, 0);
        addZone(zones, 2, 2, 1, 1);
        addZone(zones, 0, 0, 2, 2);
        addZone(zones, 1, 1, 2, 2);
        addZone(zones, 2, 2, 2, 2);
        
        setLayout(3, 3, zones);
        break;

    case LAYOUT_MODE_EIGHT:
        addZone(zones, 0, 2, 0, 2);
        addZone(zones, 3, 3, 0, 0);
        addZone(zones, 3, 3, 1, 1);
        addZone(zones, 3, 3, 2, 2);
        addZone(zones, 0, 0, 3, 3);
        addZone(zones, 1, 1, 3, 3);
        addZone(zones, 2, 2, 3, 3);
        addZone(zones, 3, 3, 3, 3);
        
        setLayout(4, 4, zones);
        break;    

    case LAYOUT_MODE_NINE:
        addZone(zones, 0, 0, 0, 0);
        addZone(zones, 1, 1, 0, 0);
        addZone(zones, 2, 2, 0, 0);
        addZone(zones, 0, 0, 1, 1);
        addZone(zones, 1, 1, 1, 1);
        addZone(zones, 2, 2, 1, 1);
        addZone(zones, 0, 0, 2, 2);
        addZone(zones, 1, 1, 2, 2);
        addZone(zones, 2, 2, 2, 2);
        
        setLayout(3, 3, zones);
        break; 

    case LAYOUT_MODE_SIXTEEN:
        addZone(zones, 0, 0, 0, 0);
        addZone(zones, 1, 1, 0, 0);
        addZone(zones, 2, 2, 0, 0);
        addZone(zones, 3, 3, 0, 0);
        addZone(zones, 0, 0, 1, 1);
        addZone(zones, 1, 1, 1, 1);
        addZone(zones, 2, 2, 1, 1);
        addZone(zones, 3, 3, 1, 1);
        addZone(zones, 0, 0, 2, 2);
        addZone(zones, 1, 1, 2, 2);
        addZone(zones, 2, 2, 2, 2);
        addZone(zones, 3, 3, 2, 2);
        addZone(zones, 0, 0, 3, 3);
        addZone(zones, 1, 1, 3, 3);
        addZone(zones, 2, 2, 3, 3);
        addZone(zones, 3, 3, 3, 3);
        
        setLayout(4, 4, zones);
        break;

    case LAYOUT_MODE_TWENTYFIVE:
        addZone(zones, 0, 0, 0, 0);
        addZone(zones, 1, 1, 0, 0);
        addZone(zones, 2, 2, 0, 0);
        addZone(zones, 3, 3, 0, 0);
        addZone(zones, 4, 4, 0, 0);

        addZone(zones, 0, 0, 1, 1);
        addZone(zones, 1, 1, 1, 1);
        addZone(zones, 2, 2, 1, 1);
        addZone(zones, 3, 3, 1, 1);
        addZone(zones, 4, 4, 1, 1);

        addZone(zones, 0, 0, 2, 2);
        addZone(zones, 1, 1, 2, 2);
        addZone(zones, 2, 2, 2, 2);
        addZone(zones, 3, 3, 2, 2);
        addZone(zones, 4, 4, 2, 2);

        addZone(zones, 0, 0, 3, 3);
        addZone(zones, 1, 1, 3, 3);
        addZone(zones, 2, 2, 3, 3);
        addZone(zones, 3, 3, 3, 3);
        addZone(zones, 4, 4, 3, 3);

        addZone(zones, 0, 0, 4, 4);
        addZone(zones, 1, 1, 4, 4);
        addZone(zones, 2, 2, 4, 4);
        addZone(zones, 3, 3, 4, 4);
        addZone(zones, 4, 4, 4, 4);
        setLayout(5, 5, zones);
        break;

    case LAYOUT_MODE_THIRTYSIZ:
        addZone(zones, 0, 0, 0, 0);
        addZone(zones, 1, 1, 0, 0);
        addZone(zones, 2, 2, 0, 0);
        addZone(zones, 3, 3, 0, 0);
        addZone(zones, 4, 4, 0, 0);
        addZone(zones, 5, 5, 0, 0);

        addZone(zones, 0, 0, 1, 1);
        addZone(zones, 1, 1, 1, 1);
        addZone(zones, 2, 2, 1, 1);
        addZone(zones, 3, 3, 1, 1);
        addZone(zones, 4, 4, 1, 1);
        addZone(zones, 5, 5, 1, 1);

        addZone(zones, 0, 0, 2, 2);
        addZone(zones, 1, 1, 2, 2);
        addZone(zones, 2, 2, 2, 2);
        addZone(zones, 3, 3, 2, 2);
        addZone(zones, 4, 4, 2, 2);
        addZone(zones, 5, 5, 2, 2);

        addZone(zones, 0, 0, 3, 3);
        addZone(zones, 1, 1, 3, 3);
        addZone(zones, 2, 2, 3, 3);
        addZone(zones, 3, 3, 3, 3);
        addZone(zones, 4, 4, 3, 3);
        addZone(zones, 5, 5, 3, 3);

        addZone(zones, 0, 0, 4, 4);
        addZone(zones, 1, 1, 4, 4);
        addZone(zones, 2, 2, 4, 4);
        addZone(zones, 3, 3, 4, 4);
        addZone(zones, 4, 4, 4, 4);
        addZone(zones, 5, 5, 4, 4);

        addZone(zones, 0, 0, 5, 5);
        addZone(zones, 1, 1, 5, 5);
        addZone(zones, 2, 2, 5, 5);
        addZone(zones, 3, 3, 5, 5);
        addZone(zones, 4, 4, 5, 5);
        addZone(zones, 5, 5, 5, 5);

        setLayout(6, 6, zones);
        break;
    case LAYOUT_MODE_SIZTYFOUR:
        addZone(zones, 0, 0, 0, 0);
        addZone(zones, 1, 1, 0, 0);
        addZone(zones, 2, 2, 0, 0);
        addZone(zones, 3, 3, 0, 0);
        addZone(zones, 4, 4, 0, 0);
        addZone(zones, 5, 5, 0, 0);
        addZone(zones, 6, 6, 0, 0);
        addZone(zones, 7, 7, 0, 0);

        addZone(zones, 0, 0, 1, 1);
        addZone(zones, 1, 1, 1, 1);
        addZone(zones, 2, 2, 1, 1);
        addZone(zones, 3, 3, 1, 1);
        addZone(zones, 4, 4, 1, 1);
        addZone(zones, 5, 5, 1, 1);
        addZone(zones, 6, 6, 1, 1);
        addZone(zones, 7, 7, 1, 1);

        addZone(zones, 0, 0, 2, 2);
        addZone(zones, 1, 1, 2, 2);
        addZone(zones, 2, 2, 2, 2);
        addZone(zones, 3, 3, 2, 2);
        addZone(zones, 4, 4, 2, 2);
        addZone(zones, 5, 5, 2, 2);
        addZone(zones, 6, 6, 2, 2);
        addZone(zones, 7, 7, 2, 2);

        addZone(zones, 0, 0, 3, 3);
        addZone(zones, 1, 1, 3, 3);
        addZone(zones, 2, 2, 3, 3);
        addZone(zones, 3, 3, 3, 3);
        addZone(zones, 4, 4, 3, 3);
        addZone(zones, 5, 5, 3, 3);
        addZone(zones, 6, 6, 3, 3);
        addZone(zones, 7, 7, 3, 3);

        addZone(zones, 0, 0, 4, 4);
        addZone(zones, 1, 1, 4, 4);
        addZone(zones, 2, 2, 4, 4);
        addZone(zones, 3, 3, 4, 4);
        addZone(zones, 4, 4, 4, 4);
        addZone(zones, 5, 5, 4, 4);
        addZone(zones, 6, 6, 4, 4);
        addZone(zones, 7, 7, 4, 4);

        addZone(zones, 0, 0, 5, 5);
        addZone(zones, 1, 1, 5, 5);
        addZone(zones, 2, 2, 5, 5);
        addZone(zones, 3, 3, 5, 5);
        addZone(zones, 4, 4, 5, 5);
        addZone(zones, 5, 5, 5, 5);
        addZone(zones, 6, 6, 5, 5);
        addZone(zones, 7, 7, 5, 5);

        addZone(zones, 0, 0, 6, 6);
        addZone(zones, 1, 1, 6, 6);
        addZone(zones, 2, 2, 6, 6);
        addZone(zones, 3, 3, 6, 6);
        addZone(zones, 4, 4, 6, 6);
        addZone(zones, 5, 5, 6, 6);
        addZone(zones, 6, 6, 6, 6);
        addZone(zones, 7, 7, 6, 6);

        addZone(zones, 0, 0, 7, 7);
        addZone(zones, 1, 1, 7, 7);
        addZone(zones, 2, 2, 7, 7);
        addZone(zones, 3, 3, 7, 7);
        addZone(zones, 4, 4, 7, 7);
        addZone(zones, 5, 5, 7, 7);
        addZone(zones, 6, 6, 7, 7);
        addZone(zones, 7, 7, 7, 7);

        setLayout(8, 8, zones);
        break;
    }
	
}

void VideoWidgetManager::addZone(LayoutZoneList & zones, int l, int r, int t, int b)
{
    LayoutZone zone;
    
    zone.left = l;
    zone.right = r;
    zone.top = t;
    zone.bottom = b;

    zones.push_back(zone);
}
void	VideoWidgetManager::setVideoInfoWindows()
{
	if (m_bOpenVideoInfoWindows == true){
		m_WgetVideoInfo->hide();
		m_bOpenVideoInfoWindows = false;
	}
	else{
		m_bOpenVideoInfoWindows = true;
		m_WgetVideoInfo->show();
	}
}
void VideoWidgetManager::setLayout(int rows, int cols, LayoutZoneList & zones, bool syncFlag)
{
    m_WidgetLayout.rows = rows;
    m_WidgetLayout.cols = cols;

    int i, j, index = 0;
	WidgetZone widgetZone = {0};
    VideoWidget * widget = 0;
        
    for (i = 0; i < m_WidgetLayout.zones.size(); ++i)
    {
        widgetZone = m_WidgetLayout.zones[i];
            
        if (widgetZone.widget->m_filePath.isEmpty())
        {
            delete widgetZone.widget;
        }
        else
        {
            widgetZone.widget->hide();

            m_VideoWidgets.insert(index++, widgetZone.widget);
        }
    }

    m_WidgetLayout.zones.clear();
    
    for (i = 0; i < zones.size(); ++i)
    {
        widget = 0;
        
        widgetZone.zone.left = zones[i].left;
        widgetZone.zone.right = zones[i].right;
        widgetZone.zone.top = zones[i].top;
        widgetZone.zone.bottom = zones[i].bottom;
        widgetZone.zone.filePath = zones[i].filePath;

        if (!zones[i].filePath.isEmpty())
        {
            for (j = 0; j < m_VideoWidgets.size(); ++j)
            {
                if (m_VideoWidgets[j]->m_filePath == zones[i].filePath)
                {
                    widget = m_VideoWidgets.takeAt(j);
                    break;
                }
            }
        }

        if (!syncFlag && !widget && !m_VideoWidgets.isEmpty())
        {
            widget = m_VideoWidgets.takeAt(0);
        }
        
        initVideoWidget(widget, widgetZone, i);
        
        m_WidgetLayout.zones.push_back(widgetZone);
    }

    if (syncFlag)
    {
        for (i = 0; i < m_VideoWidgets.size(); ++i)
        {
            widget = m_VideoWidgets[i];
            delete widget;
        }

        m_VideoWidgets.clear();
    }
	
    update();
}

void VideoWidgetManager::initVideoWidget(VideoWidget * widget, WidgetZone & widgetZone, int ScreenID)
{
    if (widget)
    {
        widgetZone.widget = widget;
		widgetZone.widget->SetTotalWindows(m_WidgetLayoutNum);
    }
    else
    {
		widgetZone.widget = new VideoWidget(this);
		widgetZone.widget->BindWidgetScreenID(ScreenID);
        widgetZone.widget->m_filePath = widgetZone.zone.filePath;
        QObject::connect(widgetZone.widget, SIGNAL(videoWidgetResize()), this, SLOT(SlotVideoWidgetReSize()));
		QObject::connect(widgetZone.widget, SIGNAL(widgetSelecting(QWidget*)), this, SLOT(SlotWidgetSelecting(QWidget*)));
		QObject::connect(widgetZone.widget, SIGNAL(SignalsSpiltScreen(int)), this, SLOT(SlotSpiltScreen(int)), Qt::QueuedConnection);
		QObject::connect(widgetZone.widget, SIGNAL(SignalsMaxWindows(QWidget*, bool)), this, SLOT(SlotMaxWindows(QWidget *, bool)));
		widgetZone.widget->SetTotalWindows(m_WidgetLayoutNum);
    }

    setVideoWidgetRect(widgetZone);
	
    widgetZone.widget->show();
	

}

void VideoWidgetManager::SlotVideoWidgetReSize()
{
    resizeVideoWidgets(this->width(), this->height());
}

void VideoWidgetManager::setVideoWidgetRect(WidgetZone & widgetZone)
{
    QRect rect = getZoneRect(width(), height(), widgetZone.zone);
	widgetZone.widget->setGeometry(rect);
}


QRect VideoWidgetManager::getZoneRect(int w, int h, LayoutZone & zone)
{
    return getGridRect(w, h, zone.left, zone.right, zone.top, zone.bottom);
}

QRect VideoWidgetManager::getGridRect(int w, int h, int left, int right, int top, int bottom)
{
    float gridWidth = w / (float) m_WidgetLayout.cols;
    float gridHeight = h / (float) m_WidgetLayout.rows;
    
    int x1 = left * gridWidth + GRID_LINE_WIDTH;
    int y1 = top * gridHeight + GRID_LINE_WIDTH;

    int x2 = (right + 1) * gridWidth - GRID_LINE_WIDTH;
    int y2 = (bottom + 1) * gridHeight - GRID_LINE_WIDTH;

    return QRect(x1, y1, x2 - x1, y2 - y1);
}

VideoWidget * VideoWidgetManager::GetSelectedWidget()
{
    VideoWidget * pSelectedWidget = NULL;
    
    for (int i = 0; i < m_WidgetLayout.zones.size(); ++i)
    {
        WidgetZone & widgetZone = m_WidgetLayout.zones[i];
        
        if (widgetZone.widget->m_bSelected)
        {
            pSelectedWidget = widgetZone.widget;
            break;
        }
    }

    return pSelectedWidget;
}
int		VideoWidgetManager::GetSplitCount()
{
	return m_WidgetLayoutNum;
}
VideoWidget * VideoWidgetManager::GetIdleWidget()
{
    VideoWidget * pIdleWidget = NULL;
    
    for (int i = 0; i < m_WidgetLayout.zones.size(); ++i)
    {
        WidgetZone & widgetZone = m_WidgetLayout.zones[i];
        
        if (widgetZone.widget->m_filePath.isEmpty())
        {
            pIdleWidget = widgetZone.widget;
            break;
        }
    }

    return pIdleWidget;
}

void VideoWidgetManager::SlotOnPreviewStart(int Channel, int Slave, bool bOpen)
{
	av_warning("mark \n");
	if (Channel >= m_WidgetLayout.zones.size()){
		AvQDebug("channel = %d  zones.size = %d\n", Channel, m_WidgetLayout.zones.size());
		return;
	}
	WidgetZone & widgetZone = m_WidgetLayout.zones[Channel];
	widgetZone.widget->m_filePath.append(QString::number(Channel * 10 + Slave));

	if (m_WidgetLayoutNum <= 4 || (m_WidgetLayoutNum < 9 && Channel == 0)){
		Slave = CHL_MAIN_T;
	}
	else {
		Slave = CHL_SUB1_T;
	}
	av_warning("mark \n");
	widgetZone.widget->PreviewStart(Channel, Slave, bOpen);
	av_warning("mark \n");

}
void	VideoWidgetManager::SlotOnShowInfoWidget(QWidget *pWidget, bool bShow)
{
	QPoint tmpPos = pWidget->mapToGlobal(pWidget->mapFromParent(pWidget->pos()));
	m_WgetVideoInfo->resize(pWidget->size());
	m_WgetVideoInfo->update();
	m_WgetVideoInfo->move(tmpPos);
	if (m_bOpenVideoInfoWindows == true){
		m_WgetVideoInfo->show();
	}
	
	m_WgetVideoInfo->raise();
}
void VideoWidgetManager::SlotWidgetSelecting(QWidget * pWidget)
{
	
    VideoWidget * pSelectedWidget = GetSelectedWidget();
	AvQDebug("VideoWidgetManager::SlotWidgetSelecting = %p\n", pSelectedWidget);
	SlotOnShowInfoWidget(pWidget);
	if (pSelectedWidget != pWidget)
    {
        if (pSelectedWidget)
        {
            pSelectedWidget->m_bSelected = false;
        }

        pSelectedWidget = (VideoWidget *) pWidget;
        pSelectedWidget->m_bSelected = true;
        update();
        emit SignalsWidgetSelecting(pWidget);
    }
}

void    VideoWidgetManager::SlotMaxWindows(QWidget *pWidget, bool bMax)
{
	AvQDebug("SlotMaxWindows\n");
	QPoint tmpPos = pWidget->mapToGlobal(pWidget->mapFromParent(pWidget->pos()));
	m_WgetVideoInfo->resize(pWidget->size());
	m_WgetVideoInfo->update();
	m_WgetVideoInfo->move(tmpPos);
	m_WgetVideoInfo->raise();
}
void	VideoWidgetManager::SlotVideoInfoOnUiMsg(WGET_VIDEOINFO_UIMSG eUiMsg)
{
	switch (eUiMsg)
	{
	case UIMSG_START_LISTEN:
		av_msg("UIMSG_START_LISTEN\n");
		break;
	case UIMSG_STOP_LISTEN:
		av_msg("UIMSG_STOP_LISTEN\n");
		break;
	case UIMSG_START_RECORD:
		av_msg("UIMSG_START_RECORD\n");
		break;
	case UIMSG_STOP_RECORD:
		av_msg("UIMSG_STOP_RECORD\n");
		break;
	case UIMSG_START_SPEAKLISTEN:
		av_msg("UIMSG_START_SPEAKLISTEN\n");
		break;
	case UIMSG_STOP_SPEAKLISTEN:
		av_msg("UIMSG_STOP_SPEAKLISTEN\n");
		break;
	case UIMSG_START_VIDEO:
		av_msg("UIMSG_START_VIDEO\n");
		break;
	case UIMSG_STOP_VIDEO:
		av_msg("UIMSG_STOP_VIDEO\n");
		break;
	case UIMSG_REQUEST_SNAP:
		av_msg("UIMSG_REQUEST_SNAP\n");
		break;
	default:
		break;
	}
}

void	VideoWidgetManager::SlotSpiltScreen(int Screens)
{

	setLayoutMode(Screens);
	m_WgetVideoInfo->hide();
	m_bOpenVideoInfoWindows = false;
}