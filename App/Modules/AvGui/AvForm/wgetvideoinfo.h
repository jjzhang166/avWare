#ifndef WGETVIDEOINFO_H
#define WGETVIDEOINFO_H

#include <QWidget>
#include <QResizeEvent>

namespace Ui {
class WgetVideoInfo;
}

class WgetVideoInfo : public QWidget
{
    Q_OBJECT

public:
    explicit WgetVideoInfo(QWidget *parent = 0);
    ~WgetVideoInfo();
	
	void ResetOSDVideoInfo();
	void SetOSDVideoInfo(QString kbps, QString ImageSize, QString FrameRate, QString EncodeType);
	void ResizeIconSize(int size);
private slots:
	void mousePressEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);

private:
	void contextMenuEvent(QContextMenuEvent *event);
	void 	resizeEvent(QResizeEvent * event);
signals:
	void SignalsMousePress(QMouseEvent *event);
	void SignalsMouseDoubleClickEvent(QMouseEvent *event);
	void SignalsContextMenuEvent(QContextMenuEvent *event);

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
	bool				m_bBtnSnapClicked;
	bool				m_bBtnSpeakListenCliecked;
	bool				m_bBtnListenClicked;
	bool				m_bBtnEnableVideoClicked;

	int					m_IconSize;
};

#endif // WGETVIDEOINFO_H
