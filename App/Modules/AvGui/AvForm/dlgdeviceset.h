#ifndef DLGDEVICESET_H
#define DLGDEVICESET_H

#include <QDialog>
#include "AvUiComm/tablemodel.h"
#include "Apis/AvWareStruct.h"
#include "AvCapture/AvCapture.h"

namespace Ui {
class DlgDeviceSet;
}

class DlgDeviceSet : public QDialog
{
    Q_OBJECT

public:
    explicit DlgDeviceSet(QWidget *parent = 0);
    ~DlgDeviceSet();
	Capture * GetChannelHandle();

	void FixDlgUi();
	void FixDlgUiCapture();
	void FixDlgUiEncode();
	void FixDlgUiAudio();
	void FixDlgUiVersion();
	void FixDlgUiRecord();
	void FixDlgUiNetWork();
	void FixDlgUiAlarm();
	void FixDlgUiUserManager();
	void FixDlgUiLog();
	void FixDlgPtz();

	void FillInCapture();
	void FillInEncode();
	void FillInAudio();
	void FillInVersion();
	void FillInRecord();
	void FillInNetWork();
	void FillInAlarm();
	void FillInUserManager();
	void FillInLog();
	void FillInPtz();


	void SubmitCapture();
	void SubmitEncode();
	void SubmitAudio();
	void SubmitVersion();
	void SubmitRecord();
	void SubmitNetWork();
	void SubmitAlarm();
	void SubmitUserManager();
	void SubmitLog();
	void SubmitPtz();


public:
	void FillInNetWorkNetSet();
	void FillInNetWorkFtp();
	void FillInNetWorkEmail();
	void FillInNetWorkUpnp();
	void FillInNetWorkRtsp();
	void FillInNetWorkDDNS();
	void FillInNetWorkP2P();
	void FillInNetWorkRTMP();
	void FillInNetWorkNTP();


	void SubmitNetWorkNetSet();
	void SubmitNetWorkFtp();
	void SubmitNetWorkEmail();
	void SubmitNetWorkUpnp();
	void SubmitNetWorkRtsp();
	void SubmitNetWorkDDNS();
	void SubmitNetWorkP2P();
	void SubmitNetWorkRTMP();
	void SubmitNetWorkNTP();


public:
	void ShowErrorConnectMesageBox();
	void ShowErrorGetArgs();
	void ShowSucceedSetArgs();
	void ShowErrorSetArgs();

private:
	void resizeEvent(QResizeEvent * event);
	void showEvent(QShowEvent *event);
private slots:
    void on_BtnClose_clicked();

    void on_TBoxNetWork_objectNameChanged(const QString &objectName);

    void on_TBoxNetWork_currentChanged(int index);

    void on_BtnNetWorkRTMPAdd_clicked();

    void on_BtnNetWorkRTMPDel_clicked();

private slots:
	void SlotsTviewRtmpLinkInfoHorizontalScrollBarRangChanged(int min, int max);
        void on_GBoxNetWork_RTMP_objectNameChanged(const QString &objectName);

        void on_StWidgetNetWork_currentChanged(int arg1);

        void on_StWidgetNetWork_objectNameChanged(const QString &objectName);

        void on_TabWidgetDeviceSet_currentChanged(int index);

        void on_CBoxIrCut_currentIndexChanged(const QString &arg1);

        void on_CBoxAnitFlicker_currentIndexChanged(const QString &arg1);

        void on_CBoxExposureMode_currentIndexChanged(const QString &arg1);

        void on_CBoxWhitBalance_currentIndexChanged(const QString &arg1);

        void on_BtnSubmit_clicked();

        void on_BtnUpdate_clicked();

        void on_CBoxSettingChannel_currentIndexChanged(int index);


        void on_CBoxMImageSize_currentIndexChanged(const QString &arg1);

        void on_CBoxMBitRateCtrl_currentIndexChanged(const QString &arg1);

        void on_CBoxSBitRateCtrl_currentIndexChanged(const QString &arg1);

        void on_CBoxMBitRateQlevel_currentIndexChanged(const QString &arg1);

        void on_CBoxSBitRateQlevel_currentIndexChanged(const QString &arg1);



        void on_RBtnAiMicIn_clicked(bool checked);

        void on_RBtnAiLineIn_clicked(bool checked);

        void on_CBoxAiEncodeType_currentIndexChanged(const QString &arg1);

        void on_BtnVideoCover_clicked();

        void on_BtnOverlay_clicked();

        void on_BtnAlmTimeSelect_clicked();

        void on_BtnAlmAreaSelect_clicked();

        void on_CBoxNetWorkFtpEnable_clicked(bool checked);

        void on_CBoxNetWorkEmailEnable_clicked(bool checked);

       // void on_CBoxNetWorkUpnpEnable_clicked(bool checked);

        void on_CBoxNetWorkRtspEnable_clicked(bool checked);

        void on_CBoxNetWorkDdnsEnable_clicked(bool checked);

        void on_CBoxNetWorkP2PEnable_clicked(bool checked);

        void on_CBoxNetWorkP2PUseLinkID_clicked(bool checked);

        void on_CBoxNetWorkTimeEnable_clicked(bool checked);

        void on_CBoxNetWorkUpnpWeb_clicked(bool checked);

        void on_CBoxNetWorkUpnpProtoPort_clicked(bool checked);

        void on_CBoxNetWorkUpnpRtsp_clicked(bool checked);

        void on_CBoxAlmEnable_clicked(bool checked);

private:
	TableModel			* m_TviewRtmpListTableModel;
private:
    Ui::DlgDeviceSet *ui;
	bool	m_UiInitedOver;
	C_AudioCaps		m_CurrentAudioCaps;
	C_AudioProfile	m_CurrentAudioProfile;

	C_TimeArea  m_AlarmTimeArea[AvMaxWeeks][AvMaxTimeArea];
	av_u32		m_AlarmMotionArea[ConfMotionDetectionLine];

};

#endif // DLGDEVICESET_H
