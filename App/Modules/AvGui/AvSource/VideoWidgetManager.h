﻿#ifndef _VIDEO_WIDGET_MANAGER_H_
#define _VIDEO_WIDGET_MANAGER_H_


#include <QWidget>
#include <qevent.h>
#include <QPainter>
#include "videowidget.h"
#include "Layout.h"
#include "wgetvideoinfo.h"
#include <QTimer>
#include "AvForm/dlgsnapshot.h"



typedef struct
{
    LayoutZone			zone;
    VideoWidget		*	widget;
	WgetVideoInfo	*	widgetVideoInfo;
} WidgetZone;

typedef QList<WidgetZone> WidgetZoneList;

typedef struct 
{
    int rows;
    int cols;
    WidgetZoneList zones;
} WidgetLayout;


typedef QList<VideoWidget *> VideoWidgetList;


class VideoWidgetManager : public QWidget
{
    Q_OBJECT

public:
    VideoWidgetManager(QWidget *parent = 0);
    ~VideoWidgetManager();
	
    void    setLayoutMode(int layoutMode);
    void    setLayout(int rows, int cols, LayoutZoneList & zones, bool syncFlag = false);
	void	setVideoInfoWindows();
    VideoWidget * GetSelectedWidget();
    VideoWidget * GetIdleWidget();
	int		GetSplitCount();
	

private:
	bool	m_bOpenVideoInfoWindows;
	
protected:
    void    paintEvent(QPaintEvent * event);
    void 	resizeEvent(QResizeEvent * event);
	void	moveEvent(QMoveEvent *event);
	void	timerEvent(QTimerEvent *event);
	
private:
    void    drawGrid(QPainter & painter, int rows, int cols);
    void    addZone(LayoutZoneList & zones, int l, int r, int t, int b);
    QRect   getGridRect(int w, int h, int left, int right, int top, int bottom);
    QRect   getZoneRect(int w, int h, LayoutZone & zone);
    void    setVideoWidgetRect(WidgetZone & widgetZone);
    void    resizeVideoWidgets(int w, int h);
	void    initVideoWidget(VideoWidget * widget, WidgetZone & widgetZone,int ScreenID, int SysChannel);

public slots:
	void	SlotOnPreviewStart(int Channel, int Slave, bool bOpen);
	void	SlotOnShowInfoWidget(QWidget *pWidget, bool bShow = true);
	void	SlotVideoWidgetReSize();
	void	SlotWidgetSelecting(QWidget *pWidget);
	void	SlotSpiltScreen(int Screens, int StartChanel, int EndChannel);
	void    SlotMaxWindows(QWidget *pWidget, bool bMax);
	void	SlotVideoInfoOnUiMsg(WGET_VIDEOINFO_UIMSG eUiMsg);
	void    SlotinitVideoWidgetStreamTimeOut();
signals:
    void    SignalsWidgetSelecting(QWidget *);

private:
    WidgetLayout		m_WidgetLayout;
    VideoWidgetList		m_VideoWidgets;
	WgetVideoInfo	*	m_WgetVideoInfo;
	int					m_WidgetLayoutNum;
	QTimer				m_TimerInitStream;
	DlgSnapshot		*	m_Dlgsnapshot;
};



#endif 


