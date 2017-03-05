#ifndef DLGAREASELECT_H
#define DLGAREASELECT_H

#include <QDialog>

#include "AvCapture/AvManCapture.h"
namespace Ui {
class DlgAreaSelect;
}
typedef struct{
	int  PressX, PressY;
	int  ReleaseX, ReleaseY;
}MousePoint;

typedef struct{
	unsigned int PressIndex;
	bool		 bFillIn;
}MotionDetectionNode;

class DlgAreaSelect : public QDialog
{
    Q_OBJECT
public:
	enum AreaType{
		AreaSelect,
		GriddingSelect,
	};
public:
    explicit DlgAreaSelect(QWidget *parent = 0);
    ~DlgAreaSelect();
public:
	void SetStyle(AreaType t);
	void SetChannel(int Channel);
	void UpdateBgImage();

	void FixUi();
	void FillInCaps();
	void FillInProfile();



protected:
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	std::list <MousePoint> m_MousezoneList;
	MousePoint m_CurrMousePlace;
	MousePoint m_CurrMouseSelect;
	int m_MaxArea;
	std::list<MousePoint> *mp_ResultZoneList;


private slots:
    void on_BtnFrmAreaDel_clicked();

    void on_BtnFrmAreaCancel_clicked();


    void on_BtnFrmAreaSubmit_clicked();

    void on_BtnClose_clicked();

public:
	void StatisticsMotionDetection(MousePoint &currentMousePoint);
	void DrawMotionDetection(QPainter &Painter);
	void MotionDetectionUiPos2Arrary(unsigned int Result[]/*ConfMotionDetectionLine*/);
	void MotionDetectionArrary2UiPos(unsigned int Result[]/*ConfMotionDetectionLine*/);
private:
	MotionDetectionNode m_MotionDetectionResult[ConfMotionDetectionLine][ConfMotionDetectionRows];
	unsigned int m_MotionPressIndex;
private:
	QPixmap			m_BgPixmap;
	Capture		*	m_Capture;

	QPoint			m_LabShowImagePoint;
	QSize			m_LabShowImageSize;
	QSize			m_ImageSize;
	
	AreaType		m_AreaType;


private:
    Ui::DlgAreaSelect *ui;
};

#endif // DLGAREASELECT_H
