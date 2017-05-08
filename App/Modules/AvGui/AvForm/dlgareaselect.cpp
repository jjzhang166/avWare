#include "dlgareaselect.h"
#include "ui_dlgareaselect.h"
#include <QPainter>
#include "AvUiComm/AvUiComm.h"
#include "AvUiComm/IconComm.h"


DlgAreaSelect::DlgAreaSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAreaSelect)
{

	ui->setupUi(this);
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool );
	//this->setAttribute(Qt::WA_DeleteOnClose);
	IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
	IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);

	m_Capture = NULL;

	m_CurrMousePlace.PressX = 0;
	m_CurrMousePlace.PressY = 0;
	m_CurrMousePlace.ReleaseX = 0;
	m_CurrMousePlace.ReleaseY = 0;

	m_CurrMouseSelect.PressX = 0;
	m_CurrMouseSelect.PressY = 0;
	m_CurrMouseSelect.ReleaseX = 0;
	m_CurrMouseSelect.ReleaseY = 0;

	m_MaxArea = 4;
	mp_ResultZoneList = NULL;

	m_AreaType = GriddingSelect;
	for (int i = 0; i < ConfMotionDetectionLine; i++){
		for (int j = 0; j < ConfMotionDetectionRows; j++){
			m_MotionDetectionResult[i][j].bFillIn = false;
			m_MotionDetectionResult[i][j].PressIndex = 0;
		}
	}
	m_MotionPressIndex = 0;
	m_LabShowImagePoint = ui->LabFrmAreaShowImage->pos();
	m_LabShowImageSize = ui->LabFrmAreaShowImage->size();

}

DlgAreaSelect::~DlgAreaSelect()
{
    delete ui;
}


void DlgAreaSelect::FixUi()
{

}
void DlgAreaSelect::FillInCaps()
{

}

void CoordinateTransformation(MousePoint &Zone, QSize Image, QSize Widget)
{
	float tempCoefficient;

	//pic pos
	tempCoefficient = (float)Image.width() / (float)Widget.width();
	Zone.PressX *= tempCoefficient;
	Zone.ReleaseX *= tempCoefficient;

	tempCoefficient = (float)Image.height() / (float)Widget.height();
	Zone.ReleaseY *= tempCoefficient;
	Zone.PressY *= tempCoefficient;


	//AV_WARE_COVER_RECT_WIDTH pos
	tempCoefficient = (float)AV_WARE_COVER_RECT_WIDTH / (float)Image.width();
	Zone.PressX *= tempCoefficient;
	Zone.ReleaseX *= tempCoefficient;

	tempCoefficient = (float)AV_WARE_COVER_RECT_HEITH / (float)Image.height();
	Zone.PressY *= tempCoefficient;
	Zone.ReleaseY *= tempCoefficient;
}

void DlgAreaSelect::FillInProfile()
{
	C_AdvancedSystemProfile AdvancedSystemProfile;
	C_CoverProfile &CoverProfile = AdvancedSystemProfile.CoverProfile;
	AdvancedSystemProfile._msg = __MsgVideoCoverProfile;

	av_bool ret = m_Capture->AdvancedSystemGetProfile(AdvancedSystemProfile);
	if (ret != av_true){
		av_error("AdvancedSystemGetProfile  __MsgVideoCoverProfile error \n");
		return;
	}

	int pic_width = m_BgPixmap.width();
	int pic_heigh = m_BgPixmap.height();
	QSize Widget = ui->LabFrmAreaShowImage->size();
	if (pic_width <= 0 || pic_heigh <= 0) return ;


	MousePoint Zone;
	for (int i = 0; i < ConfMaxCover; i++){

		if (!(CoverProfile.CoverbEnMask & AvMask(i))){
			continue;
		}
		av_msg("CoverProfile [%d %d, %d %d]\n", CoverProfile.CoverZone[i].Sx, CoverProfile.CoverZone[i].Sy,
			CoverProfile.CoverZone[i].Width, CoverProfile.CoverZone[i].Heigh);
		Zone.PressX = CoverProfile.CoverZone[i].Sx;
		Zone.PressY = CoverProfile.CoverZone[i].Sy;
		Zone.ReleaseX = CoverProfile.CoverZone[i].Width;
		Zone.ReleaseY = CoverProfile.CoverZone[i].Heigh;

		float tempFloat = (float)pic_width / AV_WARE_COVER_RECT_WIDTH;
		Zone.PressX *= tempFloat;
		Zone.ReleaseX *= tempFloat;
		tempFloat = (float)pic_heigh / AV_WARE_COVER_RECT_HEITH;
		Zone.PressY *= tempFloat;
		Zone.ReleaseY *= tempFloat;

		tempFloat = (float)Widget.width() / pic_width;
		Zone.PressX *= tempFloat;
		Zone.ReleaseX *= tempFloat;
		tempFloat = (float)Widget.height() / pic_heigh;
		Zone.PressY *= tempFloat;
		Zone.ReleaseY *= tempFloat;

		Zone.ReleaseX += Zone.PressX;
		Zone.ReleaseY += Zone.PressY;
		m_MousezoneList.push_back(Zone);

	}

	update();

}
void DlgAreaSelect::SetStyle(AreaType t)
{
	m_AreaType = t;
}

void DlgAreaSelect::SetChannel(int Channel)
{
	m_Capture = g_AvManCapture.GetAvCaptureInstance(Channel);
	UpdateBgImage();
	FillInCaps();
	FillInProfile();
}
void DlgAreaSelect::UpdateBgImage()
{
	CAvPacket *SnapPacket = m_Capture->Snapshot();
	if (SnapPacket == NULL){
		AvQDebug("Snapshot error\n");
		return;
	}
	m_ImageSize.setWidth(SnapPacket->ImageWidth());
	m_ImageSize.setHeight(SnapPacket->ImageHeigh());

	if (SnapPacket != NULL){
		bool bret = m_BgPixmap.loadFromData(SnapPacket->GetRawBuffer(), SnapPacket->GetRawLength());
		if (bret == false){
			av_error("m_BgPixmap.loadFromData error GetRawLength [%d]\n", SnapPacket->GetRawLength());
		}
		SnapPacket->Release();
	}
	else{
		av_error("Get Snap Error ret == NULL\n");
	}
}

void DlgAreaSelect::paintEvent(QPaintEvent *pe)
{
	QPixmap PixMap = m_BgPixmap.scaled(ui->LabFrmAreaShowImage->width(), ui->LabFrmAreaShowImage->height());
	QPainter Painter(&PixMap);
	if (m_AreaType == AreaSelect){
		std::list<MousePoint>::iterator ilist;
		MousePoint TempZone;
		for (ilist = m_MousezoneList.begin(); ilist != m_MousezoneList.end(); ilist++){
			TempZone = *ilist;
			Painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
			if (TempZone.PressX == m_CurrMouseSelect.PressX && TempZone.ReleaseX == m_CurrMouseSelect.ReleaseX
				&& TempZone.PressY == m_CurrMouseSelect.PressY && TempZone.ReleaseY == m_CurrMouseSelect.ReleaseY){
				Painter.setPen(QPen(Qt::red, 2, Qt::DashLine));
			}
			Painter.drawRect(TempZone.PressX, TempZone.PressY, TempZone.ReleaseX - TempZone.PressX, TempZone.ReleaseY - TempZone.PressY);
		}
		Painter.setPen(QPen(Qt::green, 2, Qt::DashLine));
		Painter.drawRect(m_CurrMousePlace.PressX, m_CurrMousePlace.PressY, m_CurrMousePlace.ReleaseX - m_CurrMousePlace.PressX, m_CurrMousePlace.ReleaseY - m_CurrMousePlace.PressY);
		Painter.end();
	}
	else{
		Painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
		QPoint DrawDevicePoint = ui->LabFrmAreaShowImage->pos();
		QSize  DrawDeviceSize = ui->LabFrmAreaShowImage->size();
		

		for (int i = 1; i < ConfMotionDetectionLine; i++){
			Painter.drawLine(0, i*DrawDeviceSize.height() / ConfMotionDetectionLine,
				DrawDeviceSize.width(), i*DrawDeviceSize.height() / ConfMotionDetectionLine);
		}
		for (int i = 1; i < ConfMotionDetectionRows; i++){
			Painter.drawLine(i*DrawDeviceSize.width() / ConfMotionDetectionRows, 0,
				i*DrawDeviceSize.width() / ConfMotionDetectionRows, DrawDeviceSize.height());
		}
		
		{//draw map
			StatisticsMotionDetection(m_CurrMousePlace);
			Painter.setBrush(QBrush(Qt::yellow, Qt::DiagCrossPattern));
			//Painter.setBrush(QBrush(Qt::red, Qt::Dense4Pattern));
			DrawMotionDetection(Painter);
		}
		Painter.end();
	}


	QPainter painter(this);
	painter.drawPixmap(ui->LabFrmAreaShowImage->pos(), PixMap);

}

void DlgAreaSelect::mousePressEvent(QMouseEvent *event)
{
	m_LabShowImagePoint = ui->LabFrmAreaShowImage->pos();
	m_LabShowImageSize = ui->LabFrmAreaShowImage->size();
	m_MotionPressIndex++;
	m_CurrMousePlace.PressX = event->x() - m_LabShowImagePoint.x();
	m_CurrMousePlace.PressY = event->y() - m_LabShowImagePoint.y();
	m_CurrMousePlace.ReleaseX = event->x() - m_LabShowImagePoint.x();
	m_CurrMousePlace.ReleaseY = event->y() - m_LabShowImagePoint.y();

	m_CurrMouseSelect.PressX = event->x() - m_LabShowImagePoint.x();
	m_CurrMouseSelect.PressY = event->y() - m_LabShowImagePoint.y();
	m_CurrMouseSelect.ReleaseX = event->x() - m_LabShowImagePoint.x();
	m_CurrMouseSelect.ReleaseY = event->y() - m_LabShowImagePoint.y();
}
void DlgAreaSelect::mouseReleaseEvent(QMouseEvent *event)
{
	QPoint DrawDevicesPoint = m_LabShowImagePoint;
	std::list<MousePoint>::iterator ilist;
	MousePoint TempZone;
	for (ilist = m_MousezoneList.begin(); ilist != m_MousezoneList.end(); ilist++){
		TempZone = *ilist;
		if (TempZone.PressX <= event->x() - DrawDevicesPoint.x() && TempZone.PressY <= event->y() - DrawDevicesPoint.y()
			&& TempZone.ReleaseX >= event->x() - DrawDevicesPoint.x() && TempZone.ReleaseY >= event->y() - DrawDevicesPoint.y()){
			m_CurrMouseSelect.PressX = TempZone.PressX;
			m_CurrMouseSelect.PressY = TempZone.PressY;
			m_CurrMouseSelect.ReleaseX = TempZone.ReleaseX;
			m_CurrMouseSelect.ReleaseY = TempZone.ReleaseY;


		}
	}

	if (abs(event->x() - DrawDevicesPoint.x() - m_CurrMousePlace.PressX) <= 20 || abs(event->y() - DrawDevicesPoint.y() - m_CurrMousePlace.PressY) <= 20){
		m_CurrMousePlace.ReleaseX = 0;
		m_CurrMousePlace.ReleaseY = 0;
		m_CurrMousePlace.PressX = 0;
		m_CurrMousePlace.PressY = 0;
		update();
		return;
	}
	else if (m_CurrMousePlace.PressX > event->x() - DrawDevicesPoint.x() && m_CurrMousePlace.PressY > event->y() - DrawDevicesPoint.y()){
		m_CurrMousePlace.ReleaseX = m_CurrMousePlace.PressX;
		m_CurrMousePlace.ReleaseY = m_CurrMousePlace.PressY;
		m_CurrMousePlace.PressX = event->x() - DrawDevicesPoint.x();
		m_CurrMousePlace.PressY = event->y() - DrawDevicesPoint.y();

	}
	else if (m_CurrMousePlace.PressX > event->x() - DrawDevicesPoint.x() && m_CurrMousePlace.PressY < event->y() - DrawDevicesPoint.y()){
		m_CurrMousePlace.ReleaseX = m_CurrMousePlace.PressX;
		m_CurrMousePlace.ReleaseY = event->y() - -DrawDevicesPoint.y();
		m_CurrMousePlace.PressX = event->x() - -DrawDevicesPoint.x();
		m_CurrMousePlace.PressY = m_CurrMousePlace.PressY;
	}
	else if (m_CurrMousePlace.PressX < event->x() - DrawDevicesPoint.x() && m_CurrMousePlace.PressY > event->y() - DrawDevicesPoint.y()){
		m_CurrMousePlace.ReleaseX = event->x() - DrawDevicesPoint.x();
		m_CurrMousePlace.ReleaseY = m_CurrMousePlace.PressY;
		m_CurrMousePlace.PressX = m_CurrMousePlace.PressX;
		m_CurrMousePlace.PressY = event->y() - DrawDevicesPoint.y();
	}
	else if (m_CurrMousePlace.PressX < event->x() - DrawDevicesPoint.x() && m_CurrMousePlace.PressY < event->y() - DrawDevicesPoint.y()){
		m_CurrMousePlace.ReleaseX = event->x() - DrawDevicesPoint.x();
		m_CurrMousePlace.ReleaseY = event->y() - DrawDevicesPoint.y();
	}

	if (m_CurrMousePlace.ReleaseX > m_LabShowImageSize.width()){
		m_CurrMousePlace.ReleaseX = m_LabShowImageSize.width();
		m_CurrMouseSelect.ReleaseX = m_LabShowImageSize.width();
	}
	if (m_CurrMousePlace.ReleaseY > m_LabShowImageSize.height()){
		m_CurrMousePlace.ReleaseY = m_LabShowImageSize.height();
		m_CurrMouseSelect.ReleaseY = m_LabShowImageSize.height();
	}


	if (m_CurrMousePlace.PressX < 0){
		m_CurrMousePlace.PressX = 0;
		m_CurrMouseSelect.PressX = 0;
	}
	if (m_CurrMousePlace.PressY < 0){
		m_CurrMousePlace.PressY = 0;
		m_CurrMouseSelect.ReleaseY = 0;
	}

	if (m_MousezoneList.size() < m_MaxArea){
		m_MousezoneList.push_back(m_CurrMousePlace);
	}

	update();
}
void DlgAreaSelect::mouseMoveEvent(QMouseEvent *event)
{
	QPoint DrawDevicesPoint = m_LabShowImagePoint;

	m_CurrMousePlace.ReleaseX = event->x() - DrawDevicesPoint.x();
	m_CurrMousePlace.ReleaseY = event->y() - DrawDevicesPoint.y();

	m_CurrMouseSelect.ReleaseX = event->x() - DrawDevicesPoint.x();
	m_CurrMouseSelect.ReleaseY = event->y() - DrawDevicesPoint.y();
	



	if (m_CurrMousePlace.ReleaseX > ui->LabFrmAreaShowImage->width()){
		m_CurrMousePlace.ReleaseX = ui->LabFrmAreaShowImage->width();
		m_CurrMouseSelect.ReleaseX = ui->LabFrmAreaShowImage->width();
	}
	if (m_CurrMousePlace.ReleaseY > ui->LabFrmAreaShowImage->height()){
		m_CurrMousePlace.ReleaseY = ui->LabFrmAreaShowImage->height();
		m_CurrMouseSelect.ReleaseY = ui->LabFrmAreaShowImage->height();
	}

	if (m_CurrMousePlace.ReleaseX < 0){
		m_CurrMousePlace.ReleaseX = 1;
		m_CurrMouseSelect.ReleaseX = 1;
	}
	if (m_CurrMousePlace.ReleaseY < 0){
		m_CurrMousePlace.ReleaseY = 1;
		m_CurrMouseSelect.ReleaseY = 1;
	}
	static int last_ReleaseX = 0;
	static int last_ReleaseY = 0;
	if (m_AreaType == GriddingSelect && abs(last_ReleaseX - m_CurrMouseSelect.ReleaseX) > 15
		|| abs(last_ReleaseY - m_CurrMousePlace.ReleaseY)>15){
		last_ReleaseX = m_CurrMouseSelect.ReleaseX;
		last_ReleaseY = m_CurrMousePlace.ReleaseY;
		update();
	}
	else if (m_AreaType == AreaSelect){
		update();
	}
	
}

void DlgAreaSelect::on_BtnFrmAreaDel_clicked()
{
	if (m_AreaType == AreaSelect){
		std::list<MousePoint>::iterator ilist;
		MousePoint TempZone;
		for (ilist = m_MousezoneList.begin(); ilist != m_MousezoneList.end(); ilist++){
			TempZone = *ilist;
			if (TempZone.PressX == m_CurrMouseSelect.PressX && TempZone.ReleaseX == m_CurrMouseSelect.ReleaseX
				&& TempZone.PressY == m_CurrMouseSelect.PressY && TempZone.ReleaseY == m_CurrMouseSelect.ReleaseY){
				m_MousezoneList.erase(ilist);
				break;
			}
		}
	}
	else{
		for (int i = 0; i < ConfMotionDetectionLine; i++){
			for (int j = 0; j < ConfMotionDetectionRows; j++){
				m_MotionDetectionResult[i][j].bFillIn = false;
				m_MotionDetectionResult[i][j].PressIndex = 0;
			}
		}
		m_MotionPressIndex = 0;
	}


	update();
}

void DlgAreaSelect::on_BtnFrmAreaCancel_clicked()
{
	close();
}

void DlgAreaSelect::on_BtnFrmAreaSubmit_clicked()
{

	if (m_AreaType == AreaSelect){

		C_AdvancedSystemProfile AdvancedSystemProfile;

		//memset(&AdvancedSystemProfile, 0x00, sizeof(C_AdvancedSystemProfile));
		std::list<MousePoint>::iterator ilist;
		int i = 0;
		for (ilist = m_MousezoneList.begin(); ilist != m_MousezoneList.end(); ilist++, i++){
			MousePoint TempZone = *ilist;
			CoordinateTransformation(TempZone, m_ImageSize, ui->LabFrmAreaShowImage->size());
			AdvancedSystemProfile.CoverProfile.CoverbEnMask |= AvMask(i);
			AdvancedSystemProfile.CoverProfile.CoverBgRGB = 0xff;
			AdvancedSystemProfile.CoverProfile.CoverZone[i].Sx = TempZone.PressX;
			AdvancedSystemProfile.CoverProfile.CoverZone[i].Sy = TempZone.PressY;
			AdvancedSystemProfile.CoverProfile.CoverZone[i].Width = TempZone.ReleaseX - TempZone.PressX;
			AdvancedSystemProfile.CoverProfile.CoverZone[i].Heigh = TempZone.ReleaseY - TempZone.PressY;
		}

		if (i == 0){
			AdvancedSystemProfile.CoverProfile.CoverbEnMask = av_false;
		}
		AdvancedSystemProfile._msg = __MsgVideoCoverProfile;

		av_bool ret = m_Capture->AdvancedSystemSetProfile(AdvancedSystemProfile);
		if (ret == av_false){

		}
	}
	else{

	}
	UpdateBgImage();
}

void DlgAreaSelect::on_BtnClose_clicked()
{
    close();
}


void DlgAreaSelect::StatisticsMotionDetection(MousePoint &currentMousePoint)
{
	QSize  DrawDeviceSize = m_LabShowImageSize;
	float heigh = (float)DrawDeviceSize.height() / ConfMotionDetectionLine;
	float Width = (float)DrawDeviceSize.width() / ConfMotionDetectionRows;
	QRect rect;
	rect.setWidth((int)Width);
	rect.setHeight((int)heigh);
	if (currentMousePoint.PressX == 0 && currentMousePoint.PressY == 0 && currentMousePoint.ReleaseX == 0 && currentMousePoint.ReleaseY == 0){
		return;
	}
	for (int i = 0; i < ConfMotionDetectionLine; i++){
		for (int j = 0; j < ConfMotionDetectionRows; j++){
			rect.setX(Width*j);
			rect.setY(heigh*i);
			if (currentMousePoint.PressX < rect.x() + Width && currentMousePoint.PressY < rect.y() + heigh
				&& currentMousePoint.ReleaseX >= rect.x() && currentMousePoint.ReleaseY >= rect.y()){
				if (m_MotionDetectionResult[i][j].PressIndex != m_MotionPressIndex){
					m_MotionDetectionResult[i][j].bFillIn = !m_MotionDetectionResult[i][j].bFillIn;
					m_MotionDetectionResult[i][j].PressIndex = m_MotionPressIndex;
				}
			}

		}
	}
}
void DlgAreaSelect::DrawMotionDetection(QPainter &Painter)
{
	QSize  DrawDeviceSize = m_LabShowImageSize;
	float heigh = (float)DrawDeviceSize.height() / ConfMotionDetectionLine;
	float Width = (float)DrawDeviceSize.width() / ConfMotionDetectionRows;
	QRect rect;
	rect.setWidth((int)Width);
	rect.setHeight((int)heigh);
	for (int i = 0; i < ConfMotionDetectionLine; i++){
		for (int j = 0; j < ConfMotionDetectionRows; j++){
			if (m_MotionDetectionResult[i][j].bFillIn == true){
				rect.setX(Width*j);
				rect.setY(heigh*i);
				rect.setWidth((int)Width);
				rect.setHeight((int)heigh);
				Painter.drawRect(rect);
			}
		}
	}
}

void DlgAreaSelect::MotionDetectionUiPos2Arrary(unsigned int Result[])
{
	memset(Result, 0x00, sizeof(unsigned int)*ConfMotionDetectionLine);
		for (int i = 0; i < ConfMotionDetectionLine; i++) {
		Result[i] = 0;
		for (int j = 0; j < ConfMotionDetectionRows; j++){
			if (m_MotionDetectionResult[i][j].bFillIn == true){
				Result[i] |= (0x01 << (ConfMotionDetectionRows - j - 1));
			}
		}
	}
}
void DlgAreaSelect::MotionDetectionArrary2UiPos(unsigned int Result[])
{
	for (int i = 0; i < ConfMotionDetectionLine; i++){
		for (int j = 0; j < ConfMotionDetectionRows; j++){
			if (Result[i] & (0x01 << (ConfMotionDetectionRows - j - 1))){
				m_MotionDetectionResult[i][j].bFillIn = true;
				m_MotionDetectionResult[i][j].PressIndex = 0;
			}
		}
	}
}
