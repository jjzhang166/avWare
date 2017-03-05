#ifndef DLGPTZWINDOWS_H
#define DLGPTZWINDOWS_H

#include <QDialog>
#include "AvCapture/AvManCapture.h"
#include "Apis/AvWareStruct.h"
#include "formptzadvanced.h"
namespace Ui {
class DlgPtzWindows;
}

class DlgPtzWindows : public QDialog
{
    Q_OBJECT

public:
    explicit DlgPtzWindows(QWidget *parent = 0);
    ~DlgPtzWindows();

	void FixUi();
	void PtzStop();
	void PtzStart(PtzCommand Command, C_PtzCmd *Cmd = NULL);
public:
	void mouseMoveEvent(QMouseEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

public:
	void movePtzAdvanced();
private:
	bool	m_mousePressed;
	QPoint  m_mousePoint;
	bool	m_WidowsMax;

private slots:
    void on_BtnClose_clicked();

    void on_BtnPtzLU_pressed();

    void on_BtnPtzLU_released();

    void on_BtnPtzUp_pressed();

    void on_BtnPtzUp_released();

    void on_BtnPtzRU_pressed();

    void on_BtnPtzRU_released();

    void on_BtnPtzLeft_pressed();

    void on_BtnPtzLeft_released();

    void on_BtnPtzAuto_pressed();

    void on_BtnPtzAuto_released();

    void on_BtnPtzRight_pressed();

    void on_BtnPtzRight_released();

    void on_BtnPtzLD_pressed();

    void on_BtnPtzLD_released();

    void on_BtnPtzDown_pressed();

    void on_BtnPtzDown_released();

    void on_BtnPtzRD_pressed();

    void on_BtnPtzRD_released();

    void on_CBoxChannels_currentIndexChanged(const QString &arg1);

    void on_HSliderBrightness_valueChanged(int value);

    void on_HSliderContrast_valueChanged(int value);

    void on_HSliderHue_valueChanged(int value);

    void on_HSliderSaturation_valueChanged(int value);

    void on_BtnImageDefault_clicked();


    void on_HSliderPtzSpeed_valueChanged(int value);

    void on_BtnAutoTour_clicked();

    void on_BtnPreset_clicked();

    void on_BtnAutoScan_clicked();

    void on_BtnAuxOn_clicked();

    void on_BtnAutoPan_clicked();

    void on_BtnPattem_clicked();

    void on_BtnPtzConfig_clicked();

    void on_BtnAuxOff_clicked();

    void on_BtnPtzZoomAdd_pressed();

    void on_BtnPtzZoomAdd_released();

    void on_BtnPtzIrisAdd_pressed();

    void on_BtnPtzIrisAdd_released();

    void on_BtnPtzFocusAdd_pressed();

    void on_BtnPtzFocusAdd_released();

    void on_BtnPtzZoomReduce_pressed();

    void on_BtnPtzZoomReduce_released();

    void on_BtnPtzIrisReduce_pressed();

    void on_BtnPtzIrisReduce_released();

    void on_BtnPtzFocusReduce_pressed();

    void on_BtnPtzFocusReduce_released();

private:
    Ui::DlgPtzWindows *ui;
	int				m_Channel;
	Capture			*m_Capture;
	int				m_PtzSpeed;
	int				m_PtzValue;
	C_ImageProfile	m_ImageProfile;
	bool			m_bAutoTour;
	bool			m_bAutoScan;
	bool			m_bAutoPan;

	FormPtzAdvanced *m_FormPtzAdvanced;
	bool			m_bFromPtzAdvancedShow;
};

#endif // DLGPTZWINDOWS_H
