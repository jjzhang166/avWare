#include "dlgoverlay.h"
#include "ui_dlgoverlay.h"
#include "AvCapture/AvManCapture.h"
#include "AvUiComm/IconComm.h"
#include <QPainter>
#include "AvUiComm/AvUiComm.h"
#include "AvSource/AvQStringsLanguage.h"

#define __FONTS_WIDHT_HEIGH_RATIO 0.6875

DlgOverLay::DlgOverLay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgOverLay)
{
    ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
	this->setAttribute(Qt::WA_DeleteOnClose);
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	{
		m_LabChannel = NULL;
		m_LabDateTime = NULL;
		m_PressIndex = -1;
		m_FontSize = 16;

		m_eFontSize = FontSize__16;
		m_DateTimeStyle = OverLayDateTimeStyle_H_M_S_Y_M_D;
		m_DateTimeSplit = OverLayDateTimeSplit_Split_Symbol;

		m_bEnableTitle = false;
		m_bEnableDateTime = false;
	}

	{	
		m_LabChannel = new QLabel(this);
		m_LabChannel->raise();

		m_LabDateTime = new QLabel(this);
		m_LabDateTime->raise();

	}

	raise();
	QTimer::singleShot(100, this, SLOT(SlotsInitTimer()));



}

void DlgOverLay::SlotsInitTimer()
{
	FixUi();
	
	FillInCaps();
	
	FillInProfile();
	
	UpdateBgImage();


}

void DlgOverLay::FixUi()
{

	SetFontSize(m_FontSize);
	QColor Color = QColor("red");
	SetFontColor(Color);
	QString title = QString("IPC");
	SetTitleString(&title);

	CAvPacket *SnapPacket = m_Capture->Snapshot();
	m_ImageSize.setWidth(SnapPacket->ImageWidth());
	m_ImageSize.setHeight(SnapPacket->ImageHeigh());

}

static void formationTransCoordinate(C_RECT &RECT, QSize Image, QSize Widget)
{
	float tempCoefficient;
	tempCoefficient = (float)Image.width() / (float)AV_WARE_COVER_RECT_WIDTH;
	RECT.Sx *= tempCoefficient;
	RECT.Width *= tempCoefficient;
	tempCoefficient = (float)Image.height() / (float)AV_WARE_COVER_RECT_HEITH;
	RECT.Sy *= tempCoefficient;
	RECT.Heigh *= tempCoefficient;

	tempCoefficient = (float)Widget.width() / (float)Image.width() ;
	RECT.Sx *= tempCoefficient;
	RECT.Width *= tempCoefficient;
	tempCoefficient = (float)Widget.height() / (float)Image.height();
	RECT.Heigh *= tempCoefficient;
	RECT.Sy *= tempCoefficient;
}

static void CoordinateTransformation(C_RECT &RECT, QSize Image, QSize Widget)
{
	float tempCoefficient;

	//pic pos
	tempCoefficient = (float)Image.width() / (float)Widget.width();
	RECT.Sx *= tempCoefficient;
	RECT.Width *= tempCoefficient;

	tempCoefficient = (float)Image.height() / (float)Widget.height();
	RECT.Heigh *= tempCoefficient;
	RECT.Sy *= tempCoefficient;


	//AV_WARE_COVER_RECT_WIDTH pos
	tempCoefficient = (float)AV_WARE_COVER_RECT_WIDTH / (float)Image.width();
	RECT.Sx *= tempCoefficient;
	RECT.Width *= tempCoefficient;

	tempCoefficient = (float)AV_WARE_COVER_RECT_HEITH / (float)Image.height();
	RECT.Sy *= tempCoefficient;
	RECT.Heigh *= tempCoefficient;
}


void DlgOverLay::FillInCaps()
{
	m_bInited = false;
	C_AdvancedSystemCaps AdvancedSystemCaps;
	C_OverLayCaps &OverlayCaps = AdvancedSystemCaps.OverLayCaps;

	AdvancedSystemCaps._msg = __MsgOverLayCaps;
	av_bool bret = m_Capture->AdvancedSystemGetCaps(AdvancedSystemCaps);


	if (!(AvMask(OverLayType_Time) & OverlayCaps.OverLayTypeMask)){
		ui->CBoxOverLayDateTimeEnable->setChecked(false);
		ui->CBoxOverLayDateTimeEnable->setCheckable(false);
		m_bEnableDateTime = false;
	}
	else{
		ui->CBoxOverLayDateTimeEnable->setCheckable(true);
	}
	
	if (!(AvMask(OverLayType_Title) & OverlayCaps.OverLayTypeMask)){
		ui->CBoxOverLayChannelEnable->setChecked(false);
		ui->CBoxOverLayChannelEnable->setCheckable(false);
		m_bEnableTitle = false;
	}
	else{
		ui->CBoxOverLayChannelEnable->setCheckable(true);
	}

	if (av_true == OverlayCaps.bSupportBg){
		ui->BtnOverLayBgColor->show();
	}
	else{
		ui->BtnOverLayBgColor->hide();
	}

	if (av_true == OverlayCaps.bSupportFg){
		ui->BtnOverLayFgColor->show();
	}
	else{
		ui->BtnOverLayFgColor->hide();
	}

	ui->CBoxOverLayDateTimeSpilt->clear();
	for (int i = OverLayDateTimeSplit_None; i < OverLayDateTimeSplit_Last; i++){
		if (!(OverlayCaps.OverLayDateTimeSplitMask & AvMask(i))){
			continue;
		}
		ui->CBoxOverLayDateTimeSpilt->addItem(AvUiLangsOverLayDateTimeSplit((OverLayDateTimeSplit)i));
	}
	ui->CBoxOverLayDateTimeStyle->clear();
	for (int i = OverLayDateTimeStyle_None; i < OverLayDateTimeStyle_Last; i++){
		if (!(OverlayCaps.OverLayDateTimeStyleMask & AvMask(i))){
			continue;
		}
		ui->CBoxOverLayDateTimeStyle->addItem(AvUiLangsOverLayDateTimeStyle((OverLayDateTimeStyle)i));
	}

	ui->CBoxOverLayFontSize->clear();
	for (int i = FontSize_None; i < FontSize_Last; i++){
		if (!(OverlayCaps.FontSizeMask & AvMask(i))){
			continue;
		}
		ui->CBoxOverLayFontSize->addItem(AvUiLangsFontSize((FontSize)i));
	}

/*	OverlayCaps.MaxTextExt = 0;*/
	switch (OverlayCaps.MaxTextExt)
	{
	case 0:
		//close 1
		ui->LabOverLayChannelExtText1->hide();
		ui->LiEditOverLayChannelExtText1->hide();
	case 1:
		//close 2
		ui->LabOverLayChannelExtText2->hide();
		ui->LiEditOverLayChannelExtText2->hide();
	case 2:
		//close 3
		ui->LabOverLayChannelExtText3->hide();
		ui->LiEditOverLayChannelExtText3->hide();
		break;

	case 3:
		ui->LabOverLayChannelExtText1->show();
		ui->LiEditOverLayChannelExtText1->show();
		ui->LabOverLayChannelExtText2->show();
		ui->LiEditOverLayChannelExtText2->show();
		ui->LabOverLayChannelExtText3->show();
		ui->LiEditOverLayChannelExtText3->show();
		break;
	default:
		break;
	}

}
void DlgOverLay::FillInProfile()
{
	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_OverLayProfile &OverLayProfile = AdvancedSystemProfile.OverLayProfile;


	QPoint LabImageShowPoint = ui->LabOverLayImageShow->pos();

	AdvancedSystemProfile._msg = __MsgOverLayProfile;
	AdvancedSystemProfile.OverLayProfile.Index = OverLayType_Time;
	av_bool ret = m_Capture->AdvancedSystemGetProfile(AdvancedSystemProfile);

	if (ret != av_true){
		av_error("AdvancedSystemGetProfile __MsgOverLayProfile error\n");
		return;
	}
	
	{
		///time
		if (OverLayProfile.bOpen == av_true){
			ui->CBoxOverLayDateTimeEnable->setChecked(true);
			m_bEnableDateTime = true;
		}else{
			ui->CBoxOverLayDateTimeEnable->setChecked(false);
			m_bEnableDateTime = false;
		}

		ui->CBoxOverLayDateTimeSpilt->setCurrentText(AvUiLangsOverLayDateTimeSplit(OverLayProfile.Split));
		ui->CBoxOverLayDateTimeStyle->setCurrentText(AvUiLangsOverLayDateTimeStyle(OverLayProfile.Style));
		m_DateTimeStyle = OverLayProfile.Style;
		m_DateTimeSplit = OverLayProfile.Split;


		{
			formationTransCoordinate(OverLayProfile.Rect, m_ImageSize, ui->LabOverLayImageShow->size());
			m_LabDateTimePoint.setX(OverLayProfile.Rect.Sx + LabImageShowPoint.x());
			m_LabDateTimePoint.setY(OverLayProfile.Rect.Sy + LabImageShowPoint.y());
		}


		
		{
			ui->CBoxOverLayFontSize->setCurrentText(AvUiLangsFontSize(OverLayProfile.Fontsize));
			m_eFontSize = OverLayProfile.Fontsize;
		}

		{
			printf("bOpen[%d] BgRgba[%x] FgRgba[%x] Fontsize[%d] index [%d] Rect[%d,%d,%d,%d] split[%d] style[%d], Text[%s, %s, %s, %s]\n",
				OverLayProfile.bOpen, OverLayProfile.BgRgba, OverLayProfile.FgRgba, OverLayProfile.Fontsize,
				OverLayProfile.Index, OverLayProfile.Rect.Sx, OverLayProfile.Rect.Sy, OverLayProfile.Rect.Width, OverLayProfile.Rect.Heigh,
				OverLayProfile.Split, OverLayProfile.Style, OverLayProfile.TextChannel, OverLayProfile.TextExt1, OverLayProfile.TextExt2,
				OverLayProfile.TextExt3);
		}


		m_FgColor = OverLayProfile.FgRgba;
		m_BgColor = OverLayProfile.BgRgba;
		switch (m_eFontSize)
		{
		case FontSize__16:
			m_FontSize = 16;
			break;
		case FontSize__24:
			m_FontSize = 24;
			break;
		case FontSize__32:
			m_FontSize = 32;
			break;
		case FontSize__48:
			m_FontSize = 48;
			break;
		case FontSize__64:
			m_FontSize = 64;
			break;
		case FontSize__128:
			m_FontSize = 128;
			break;
		default:
			break;
		}
		SetFontColor(m_FgColor);
		SetFontSize(m_FontSize);

	}
	AdvancedSystemProfile.OverLayProfile.Index = OverLayType_Title;
	ret = m_Capture->AdvancedSystemGetProfile(AdvancedSystemProfile);

	if (ret != av_true){
		av_error("AdvancedSystemGetProfile __MsgOverLayProfile error\n");
		return;
	}
	formationTransCoordinate(OverLayProfile.Rect, m_ImageSize, ui->LabOverLayImageShow->size());
	m_LabChannelPoint.setX(OverLayProfile.Rect.Sx + LabImageShowPoint.x());
	m_LabChannelPoint.setY(OverLayProfile.Rect.Sy + LabImageShowPoint.y());
	{
		if (OverLayProfile.bOpen == av_true){
			ui->CBoxOverLayChannelEnable->setChecked(true);
			m_bEnableTitle = true;
		}
		else{
			ui->CBoxOverLayChannelEnable->setChecked(false);
			m_bEnableTitle = false;
		}

		ui->LiEditOverLayChannelExtText1->setText(QString(OverLayProfile.TextExt1));
		ui->LiEditOverLayChannelExtText2->setText(QString(OverLayProfile.TextExt2));
		ui->LiEditOverLayChannelExtText3->setText(QString(OverLayProfile.TextExt3));

		{
			printf("bOpen[%d] BgRgba[%x] FgRgba[%x] Fontsize[%d] index [%d] Rect[%d,%d,%d,%d] split[%d] style[%d], Text[%s, %s, %s, %s]\n",
				OverLayProfile.bOpen, OverLayProfile.BgRgba, OverLayProfile.FgRgba, OverLayProfile.Fontsize,
				OverLayProfile.Index, OverLayProfile.Rect.Sx, OverLayProfile.Rect.Sy, OverLayProfile.Rect.Width, OverLayProfile.Rect.Heigh,
				OverLayProfile.Split, OverLayProfile.Style, OverLayProfile.TextChannel, OverLayProfile.TextExt1, OverLayProfile.TextExt2,
				OverLayProfile.TextExt3);
		}
	}
	m_bInited = true;
}

DlgOverLay::~DlgOverLay()
{
    delete ui;
}


void DlgOverLay::SetChannel(int Channel)
{
	m_Capture = g_AvManCapture.GetAvCaptureInstance(Channel);
}
void DlgOverLay::UpdateBgImage()
{
	CAvPacket *SnapPacket = m_Capture->Snapshot();
	m_ImageSize.setWidth(SnapPacket->ImageWidth());
	m_ImageSize.setHeight(SnapPacket->ImageHeigh());

	if (SnapPacket != NULL){
		m_BgPixmap.loadFromData(SnapPacket->GetRawBuffer(), SnapPacket->GetRawLength());
		SnapPacket->Release();
	}



	QPixmap PixMap = m_BgPixmap.scaled(ui->LabOverLayImageShow->width(), ui->LabOverLayImageShow->height());
	ui->LabOverLayImageShow->setPixmap(PixMap);

	
	{
		if (m_bEnableTitle == true){
			m_LabChannel->show();
			MoveLab(0, m_LabChannelPoint);
		}
		else{
			m_LabChannel->hide();
		}

		if (m_bEnableDateTime == true){
			m_LabDateTime->show();
			QDateTime Dt;
			Dt = QDateTime::currentDateTime();
			QString TimeStr;
			TimeStr = TimeStr.sprintf("%d-%02d-%02d %02d:%02d:%02d", Dt.date().year(),
				Dt.date().month(), Dt.date().day(), Dt.time().hour(), Dt.time().minute(), Dt.time().second());
			m_LabDateTime->setText(TimeStr);
			m_LabDateTime->setFixedSize(m_FontSize * 19 * __FONTS_WIDHT_HEIGH_RATIO, m_FontSize);//0.6875 ��ʸ���������߱�
			MoveLab(1, m_LabDateTimePoint);
		}
		else{
			m_LabDateTime->hide();
		}

	}

}

void DlgOverLay::SetFontSize(int Size)
{
	QFont ft;
	ft.setPointSize(Size);
	m_LabChannel->setFont(ft);
	m_LabDateTime->setFont(ft);
	m_FontSize = Size;
	UpdateBgImage();
}
void DlgOverLay::SetFontColor(QColor &Color)
{
	m_FgColor = Color;
	QPalette Pa;
	Pa.setColor(QPalette::WindowText, Color);
	m_LabChannel->setPalette(Pa);
	m_LabDateTime->setPalette(Pa);
	UpdateBgImage();
}
void DlgOverLay::SetTitleString(QString *strChanel, QString *StrExt1, QString *StrExt2, QString *StrExt3)
{
	QString TitleString = *strChanel;
	int totalExt = 0;
	int MaxWidth = strChanel->size();
	if (StrExt1 != NULL){
		TitleString += "\n";
		TitleString += *StrExt1;
		totalExt++;
		if (MaxWidth < StrExt1->size()){
			MaxWidth = StrExt1->size();
		}
	}

	if (StrExt2 != NULL){
		TitleString += "\n";
		TitleString += *StrExt2;
		totalExt++;
		if (MaxWidth < StrExt2->size()){
			MaxWidth = StrExt2->size();
		}
	}

	if (StrExt3 != NULL){
		TitleString += "\n";
		TitleString += *StrExt3;
		totalExt++;
		if (MaxWidth < StrExt3->size()){
			MaxWidth = StrExt3->size();
		}
	}
	
	m_LabChannel->setFixedSize(QSize(MaxWidth*m_FontSize*__FONTS_WIDHT_HEIGH_RATIO, (totalExt + 1)*m_FontSize + totalExt * 8));
	m_LabChannel->setText(TitleString);
}
bool DlgOverLay::bIsPointInWidget(QPoint &Point, QWidget *Widget)
{
	QPoint WidgetPoint = Widget->pos();
	QSize  WidgetSize = Widget->size();

	if (Point.x() > WidgetPoint.x() && Point.x() < WidgetPoint.x() + WidgetSize.width()
		&& Point.y() > WidgetPoint.y() && Point.y() < WidgetPoint.y() + WidgetSize.height()){
		return true;
	}
	else{
		return false;
	}
}

void DlgOverLay::mousePressEvent(QMouseEvent *ev)
{
	QPoint mousePoint = ev->pos();
	
	bool ret = bIsPointInWidget(mousePoint, m_LabChannel);
	if (ret == true){
		AvQDebug("In Channel Lab\n");
		m_SeekPoint = mousePoint - m_LabChannel->pos();
		m_PressIndex = 0;
	}

	ret = bIsPointInWidget(mousePoint, m_LabDateTime);
	if (ret == true){
		AvQDebug("In DateTime Lab\n");
		m_SeekPoint = mousePoint - m_LabDateTime->pos();
		m_PressIndex = 1;
	}

	
}

void DlgOverLay::MoveLab(int WhichLab, QPoint &NewPoint)
{
	QPoint LabImageShowPoint = ui->LabOverLayImageShow->pos();
	QSize  LabImageShowSize = ui->LabOverLayImageShow->size();
	if (WhichLab == 0){
		if (NewPoint.x() < LabImageShowPoint.x()){
			NewPoint.setX(LabImageShowPoint.x());
		}
		if (NewPoint.y() < LabImageShowPoint.y()){
			NewPoint.setY(LabImageShowPoint.y());
		}
		if (NewPoint.x() + m_LabChannel->size().width() > LabImageShowPoint.x() + LabImageShowSize.width()){
			NewPoint.setX(LabImageShowPoint.x() + LabImageShowSize.width() - m_LabChannel->size().width());
		}
		if (NewPoint.y() + m_LabChannel->size().height() > LabImageShowPoint.y() + LabImageShowSize.height()){
			NewPoint.setY(LabImageShowPoint.y() + LabImageShowSize.height() - m_LabChannel->size().height());
		}
		m_LabChannelPoint = NewPoint;
		m_LabChannel->move(NewPoint);
	}
	else if (WhichLab == 1){
		if (NewPoint.x() < LabImageShowPoint.x()){
			NewPoint.setX(LabImageShowPoint.x());
		}
		if (NewPoint.y() < LabImageShowPoint.y()){
			NewPoint.setY(LabImageShowPoint.y());
		}
		if (NewPoint.x() + m_LabDateTime->size().width() > LabImageShowPoint.x() + LabImageShowSize.width()){
			NewPoint.setX(LabImageShowPoint.x() + LabImageShowSize.width() - m_LabDateTime->size().width());
		}
		if (NewPoint.y() + m_LabDateTime->size().height() > LabImageShowPoint.y() + LabImageShowSize.height()){
			NewPoint.setY(LabImageShowPoint.y() + LabImageShowSize.height() - m_LabDateTime->size().height());
		}
		m_LabDateTimePoint = NewPoint;
		m_LabDateTime->move(NewPoint);
	}
}
void DlgOverLay::mouseMoveEvent(QMouseEvent *ev)
{
	QPoint NewPoint = ev->pos() - m_SeekPoint;

	if (m_PressIndex == 0){
		MoveLab(m_PressIndex, NewPoint);
	}
	else if (m_PressIndex == 1){
		MoveLab(m_PressIndex, NewPoint);
	}
	else{

	}

}

void DlgOverLay::mouseReleaseEvent(QMouseEvent *ev)
{
	m_PressIndex = -1;
}

void DlgOverLay::paintEvent(QPaintEvent *pe)
{

}
void DlgOverLay::on_BtnClose_clicked()
{
    close();
}

void DlgOverLay::on_BtnOverLayUpdateBg_clicked()
{
	UpdateBgImage();
}

void DlgOverLay::on_BtnOverLaySubmit_clicked()
{

	QPoint LabImageShowPoint = ui->LabOverLayImageShow->pos();
	AvQDebug("LabImageShowPoint [%d, %d]\n", LabImageShowPoint.x(), LabImageShowPoint.y());
	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_OverLayProfile &OverLayProfile = AdvancedSystemProfile.OverLayProfile;
	AdvancedSystemProfile._msg = __MsgOverLayProfile;
	memset(&OverLayProfile, 0x00, sizeof(C_OverLayProfile));
	OverLayProfile.Index = OverLayType_Time;
	OverLayProfile.bOpen = m_bEnableDateTime == true ? av_true: av_false;
	OverLayProfile.FgRgba = m_FgColor.alpha();
	OverLayProfile.BgRgba = m_BgColor.alpha();
	OverLayProfile.Fontsize = m_eFontSize;
	OverLayProfile.Split = m_DateTimeSplit;
	OverLayProfile.Style = m_DateTimeStyle;
	{
		OverLayProfile.Rect.Sx = m_LabDateTimePoint.x() - LabImageShowPoint.x();
		OverLayProfile.Rect.Sy = m_LabDateTimePoint.y() - LabImageShowPoint.y();
		OverLayProfile.Rect.Width = m_LabDateTime->width();
		OverLayProfile.Rect.Heigh = m_LabDateTime->height();

		CoordinateTransformation(OverLayProfile.Rect, m_ImageSize, ui->LabOverLayImageShow->size());
	}
	m_Capture->AdvancedSystemSetProfile(AdvancedSystemProfile);


	memset(&OverLayProfile, 0x00, sizeof(C_OverLayProfile));
	OverLayProfile.Index = OverLayType_Title;
	OverLayProfile.bOpen = m_bEnableTitle == true ? av_true : av_false;
	OverLayProfile.FgRgba = m_FgColor.alpha();
	OverLayProfile.BgRgba = m_BgColor.alpha();
	OverLayProfile.Fontsize = FontSize_None;
	OverLayProfile.Split = OverLayDateTimeSplit_None;
	OverLayProfile.Style = OverLayDateTimeStyle_None;
	{
		OverLayProfile.Rect.Sx = m_LabChannelPoint.x() - LabImageShowPoint.x();
		OverLayProfile.Rect.Sy = m_LabChannelPoint.y() - LabImageShowPoint.y();
		OverLayProfile.Rect.Width = m_LabChannel->width();
		OverLayProfile.Rect.Heigh = m_LabChannel->height();
		CoordinateTransformation(OverLayProfile.Rect, m_ImageSize, ui->LabOverLayImageShow->size());
	}

	sprintf(OverLayProfile.TextChannel, "IPC");

	if (ui->LiEditOverLayChannelExtText1->text().size() != 0){
		sprintf(OverLayProfile.TextExt1, ui->LiEditOverLayChannelExtText1->text().toLocal8Bit());
	}
	if (ui->LiEditOverLayChannelExtText2->text().size() != 0){
		sprintf(OverLayProfile.TextExt2, ui->LiEditOverLayChannelExtText2->text().toLocal8Bit());
	}
	if (ui->LiEditOverLayChannelExtText3->text().size() != 0){
		sprintf(OverLayProfile.TextExt3, ui->LiEditOverLayChannelExtText3->text().toLocal8Bit());
	}


	m_Capture->AdvancedSystemSetProfile(AdvancedSystemProfile);

	UpdateBgImage();
}

#include <QColorDialog>
void DlgOverLay::on_BtnOverLayFgColor_clicked()
{
	QColorDialog Color(this);
	Color.setOption(QColorDialog::ShowAlphaChannel);
	Color.exec();
	QColor c = Color.selectedColor();

	SetFontColor(c);
}

void DlgOverLay::on_BtnOverLayBgColor_clicked()
{
	QColorDialog Color(this);
	Color.setOption(QColorDialog::ShowAlphaChannel);
	Color.exec();
	QColor c = Color.selectedColor();

	m_BgColor = c;
}

void DlgOverLay::on_CBoxOverLayChannelEnable_clicked(bool checked)
{
	m_bEnableTitle = checked;
	UpdateBgImage();
}

void DlgOverLay::on_CBoxOverLayDateTimeEnable_clicked(bool checked)
{
	m_bEnableDateTime = checked;
	UpdateBgImage();
}


void DlgOverLay::on_CBoxOverLayDateTimeSpilt_currentIndexChanged(const QString &arg1)
{
	if (m_bInited == false){
		return;
	}


	int i = 0;
	for (i = OverLayDateTimeSplit_None; i < OverLayDateTimeSplit_Last; i++){
		if (arg1 == AvUiLangsOverLayDateTimeSplit((OverLayDateTimeSplit)i)){
			break;
		}
	}

	switch (i)
	{
	case OverLayDateTimeSplit_Split_Symbol:
		break;
	case OverLayDateTimeSplit_Split_Word:
		break;
	case OverLayDateTimeSplit_Last:
	case OverLayDateTimeSplit_None:
	default:
		return;
		break;
	}

	m_DateTimeSplit = (OverLayDateTimeSplit)i;
	UpdateBgImage();
}

void DlgOverLay::on_CBoxOverLayFontSize_currentIndexChanged(const QString &arg1)
{
	if (m_bInited == false){
		return;
	}
	int i = 0;
	for (i = FontSize_None; i < FontSize_Last; i++){
		if (arg1 == AvUiLangsFontSize((FontSize)i)){
			break;
		}
	}

	int Fonts = 16;;
	switch (i)
	{
	case FontSize__16:
		Fonts = 16;
		break;
	case FontSize__24:
		Fonts = 24;
		break;
	case FontSize__32:
		Fonts = 32;
		break;
	case FontSize__64:
		Fonts = 64;
		break;
	case FontSize__128:
		Fonts = 128;
		break;
	case FontSize_None:
	default:
		return;
		break;
	}
	m_eFontSize = (FontSize)i;
	SetFontSize(Fonts);
}

void DlgOverLay::on_CBoxOverLayDateTimeStyle_currentIndexChanged(const QString &arg1)
{
	if (m_bInited == false){
		return;
	}
	int i = 0;
	for (i = OverLayDateTimeStyle_None; i < OverLayDateTimeStyle_Last; i++){
		if (arg1 == AvUiLangsOverLayDateTimeStyle((OverLayDateTimeStyle)i)){
			break;
		}
	}

	switch (i)
	{

	
	case	OverLayDateTimeStyle_Y_M_D_H_M_S:
		break;
	case	OverLayDateTimeStyle_Y_M_D_M_H_M_S:
		break;
	case	OverLayDateTimeStyle_H_M_S_Y_M_D:
		break;
	case	OverLayDateTimeStyle_Last:
	case OverLayDateTimeStyle_None:
	default:
		return;
		break;
	}

	m_DateTimeStyle = (OverLayDateTimeStyle)i;


	UpdateBgImage();

}
