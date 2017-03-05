#ifndef DLGOVERLAY_H
#define DLGOVERLAY_H

#include <QDialog>
#include "AvCapture/AvCapture.h"
#include <QLabel>
namespace Ui {
class DlgOverLay;
}


class DlgOverLay : public QDialog
{
    Q_OBJECT

public:
    explicit DlgOverLay(QWidget *parent = 0);
    ~DlgOverLay();
	void FixUi();

	void FillInCaps();
	void FillInProfile();


public:
	void SetChannel(int Channel);
	void UpdateBgImage();
	void SetFontSize(int Size);
	void SetFontColor(QColor &Color);
	void SetTitleString(QString *strChanel, 
		QString *strExt1 = NULL,
		QString *StrExt2 = NULL,
		QString *StrExt3 = NULL);


private:
	bool bIsPointInWidget(QPoint &Point, QWidget *Widget);
	void MoveLab(int WhichLab, QPoint &NewPoint);
protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void paintEvent(QPaintEvent *pe);
private slots:
        void on_BtnClose_clicked();

        void on_BtnOverLayUpdateBg_clicked();

        void on_BtnOverLaySubmit_clicked();

        void on_BtnOverLayFgColor_clicked();

        void on_BtnOverLayBgColor_clicked();

        void on_CBoxOverLayChannelEnable_clicked(bool checked);

        void on_CBoxOverLayDateTimeEnable_clicked(bool checked);

		void SlotsInitTimer();

        void on_CBoxOverLayDateTimeSpilt_currentIndexChanged(const QString &arg1);

        void on_CBoxOverLayFontSize_currentIndexChanged(const QString &arg1);

        void on_CBoxOverLayDateTimeStyle_currentIndexChanged(const QString &arg1);


private:
	FontSize	m_eFontSize;
	OverLayDateTimeStyle m_DateTimeStyle;
	OverLayDateTimeSplit m_DateTimeSplit;
	bool				 m_bEnableTitle;
	bool				 m_bEnableDateTime;


	QColor				m_FgColor;
	QColor				m_BgColor;
private:
	QPixmap			m_BgPixmap;
	Capture		*	m_Capture;

    int				m_FontSize;

	QPoint			m_BgPoint;
	QSize			m_BgSize;
	QSize			m_ImageSize;

	QLabel		*	m_LabChannel;
	QLabel		*	m_LabDateTime;

	int				m_PressIndex;//vaild -1, 0 channel, 1 title;
	QPoint			m_SeekPoint;

	bool			m_bInited;

	QPoint			m_LabChannelPoint;
	QPoint			m_LabDateTimePoint;


private:
    Ui::DlgOverLay *ui;
};

#endif // DLGOVERLAY_H
