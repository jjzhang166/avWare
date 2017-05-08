#ifndef WGETVIDEOINFO_H
#define WGETVIDEOINFO_H

#include <QWidget>
#include <QResizeEvent>

namespace Ui {
class WgetVideoInfo;
}
typedef enum {
	UIMSG_START_VIDEO,
	UIMSG_STOP_VIDEO,
	UIMSG_START_LISTEN,
	UIMSG_STOP_LISTEN,
	UIMSG_START_RECORD,
	UIMSG_STOP_RECORD,
	UIMSG_START_SPEAKLISTEN,
	UIMSG_STOP_SPEAKLISTEN,
	UIMSG_REQUEST_SNAP,
}WGET_VIDEOINFO_UIMSG;


class WgetVideoInfo : public QWidget
{
    Q_OBJECT

public:
    explicit WgetVideoInfo(QWidget *parent = 0);
    ~WgetVideoInfo();
	
	void ResetOSDVideoInfo();
	void SetOSDVideoInfo(QString kbps, QString ImageSize, QString FrameRate, QString EncodeType);
	void ResizeIconSize(int size);

public:
	QString GetPaletteTriggerStly();
	QString GetPaletteCommonStly();


private slots:
	void mousePressEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);


private:
	void contextMenuEvent(QContextMenuEvent *event);
	void 	resizeEvent(QResizeEvent * event);
	void	timerEvent(QTimerEvent *event);
	void	moveEvent(QMoveEvent *event);
signals:
	void SignalsMousePress(QMouseEvent *event);
	void SignalsMouseDoubleClickEvent(QMouseEvent *event);
	void SignalsContextMenuEvent(QContextMenuEvent *event);
	void SignalsUiButtonMessage(WGET_VIDEOINFO_UIMSG UiMsg);

private slots:
    void on_BtnRecord_clicked();

    void on_BtnSnap_clicked();

    void on_BtnSpeakListen_clicked();

    void on_BtnListen_clicked();

    void on_BtnEnableVideo_clicked();

private:
    Ui::WgetVideoInfo	*ui;
	QPalette			*	m_Palette;
	QPalette			*	m_PaletteTrigger;

	bool				m_bBtnRecordClicked;
	bool				m_bLabRecordStatus;
	bool				m_bBtnSnapClicked;
	
	bool				m_bBtnSpeakListenCliecked;
	bool				m_bLabSpeakStatus;
	bool				m_bLabListenStatus;

	bool				m_bBtnListenClicked;
	bool				m_bBtnEnableVideoClicked;
	bool				m_bLabAlarmtatus;
	int					m_IconSize;
};

#endif // WGETVIDEOINFO_H
