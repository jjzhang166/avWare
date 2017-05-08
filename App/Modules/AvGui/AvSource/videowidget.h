#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QWidget>
#include <QAction>

#include "AvForm/wgetvideoinfo.h"
#include "AvPreview/AvPreview.h"


namespace Ui {
class VideoWidget;
}

class VideoWidget : public QWidget
{
    Q_OBJECT

public:
	explicit VideoWidget(QWidget *parent = 0);
	~VideoWidget();

public:
	void VideoWidgetInit(int ToTalWins, int ScreenID, int SysChannel);
	void VideoWidgetDeInit();

	void PreviewSetStatistics(bool bOpen = true);
	void PreviewGetStatistics(QString &ImageSize, QString &Comp, QString &FrameRate, QString &BitRate);
	void SelectWindowsLive();
	void SyncWidgetSize(QRect &Rect);
public:
	void PreviewStart(int Slave, bool bOpen);
	bool PreviewGetCHLSlave(int &Channel, int &Slave);
	void resizeEvent(QResizeEvent *e);
	void setGeometry(const QRect &);

private:
	void DrawMenuSplit(QMenu *MenuSplit, int SplitNum);
	void DrawMenuSplitChannel(QMenu *MenuSplit, int SplitNum);




private:
	void contextMenuEvent(QContextMenuEvent *event);
	void MaxMinWindowsPc(bool bMax);
	void MaxMinWindowsEmbedded(bool bMax);
private slots:
	void SlotsMenuTriggered(QAction *Action);
	void SlotsContextMenuEvent(QContextMenuEvent *event);
signals:
    void widgetSelecting(QWidget *);
    void videoWidgetResize();
	void WidgetChange(QRect &Rect);
	void SignalsSpiltScreen(int split, int StartChannel, int EndChannel);
	void SignalsMaxWindows(QWidget *, bool);

private slots:
    void mousePressEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

public:
    QString		m_filePath;
    bool		m_bSelected;

	CAvPreview	*	m_Preview;
	bool		m_bPreview;
	int			m_Channel;
	int			m_ScreenID;
	int			m_TotalWindows;

};

#endif // VIDEOWIDGET_H
