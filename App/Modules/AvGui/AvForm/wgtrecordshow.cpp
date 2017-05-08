#include "wgtrecordshow.h"
#include "ui_wgtrecordshow.h"
#include <QPainter>
#include <QLabel>
#include <QToolTip>
#include "AvUiComm/AvUiComm.h"

WgtRecordShow::WgtRecordShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WgtRecordShow)
{
    ui->setupUi(this);
	for (int i = 0; i < 12; i++){
		m_NumberLabel[i] = new QLabel(this);
		m_NumberLabel[i]->setText(QString::number(i*2 + 1));
		m_NumberLabel[i]->show();
	}
}

WgtRecordShow::~WgtRecordShow()
{
    delete ui;
}


void WgtRecordShow::paintEvent(QPaintEvent *event)
{
	QPainter Painter(this);
	DrawTimeShatf(Painter);
	DrawRectangle(Painter);

}
void WgtRecordShow::DrawTimeShatf(QPainter &Painter)
{
	QRect WgtRect = rect();
	Painter.setBrush(Qt::NoBrush);
	Painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
	WgtRect.setWidth(WgtRect.width() - 2);
	WgtRect.setHeight(WgtRect.height() - 6);
	Painter.drawRect(WgtRect);

	int x1, y1, x2, y2;
	float widthRate = WgtRect.width();
	widthRate /= 24;
	y1 = WgtRect.y();
	for (int i = 0; i <= 24; i++){
		x2 = x1 = i*widthRate;
		if (i % 2 == 1){
			y2 = 10;
			m_NumberLabel[i/2]->move(x1 + 3, y1 + 3);
		}
		else{
			y2 = 5;
		}
		Painter.drawLine(x1, y1, x2, y2);
	}
}
void WgtRecordShow::mouseMoveEvent(QMouseEvent *event)
{
	AvQDebug("mouseMoveEvent event\n");
}
void WgtRecordShow::mousePressEvent(QMouseEvent *event)
{
	QToolTip::showText(QCursor::pos(), QString("12:34:08"));
	AvQDebug("mousePressEvent event\n");
}
void WgtRecordShow::mouseReleaseEvent(QMouseEvent *event)
{
	AvQDebug("mouseReleaseEvent event\n");
}
 void WgtRecordShow::enterEvent(QEvent * event)
 {
 	AvQDebug("enter event\n");
	setCursor(Qt::IBeamCursor);
 }
bool WgtRecordShow::event(QEvent *e)
{
	if (e->type() == QEvent::ToolTip) {
		AvQDebug("ToolTip\n");
// 		qDebug() << "tool tip show";
// 		g_toolTip->showMessage(labelIcon->pixmap(),
// 			labelName->text(),
// 			labelInfo->text(),
// 			QCursor::pos());
		QToolTip::showText(QCursor::pos(), QString("12:34:08"));
		//e->ignore();
	}
	else if (e->type() == QEvent::Leave) {
		AvQDebug(" Leave ToolTip\n");
// 		qDebug() << "tool tip leave";
// 		g_toolTip->hide();
		QToolTip::hideText();
		e->ignore();
	}
	return QWidget::event(e);
}
void WgtRecordShow::DrawRectangle(QPainter &Painter)
{
	QBrush Brush;
	QRect WgtRect = rect();

	Brush.setColor(Qt::green);
	Brush.setStyle(Qt::Dense4Pattern);
	Painter.setBrush(Brush);

#define RectangleHeigh 10

	QRect Rectangle = WgtRect;
	Rectangle.setX(WgtRect.x());
	Rectangle.setY(WgtRect.y() + 20);
	Rectangle.setWidth(WgtRect.width() - 2);
	Rectangle.setHeight(10);
	Painter.drawRect(Rectangle);


	Rectangle.setX(WgtRect.x());
	Rectangle.setY(WgtRect.y() + 31);
	Rectangle.setWidth(WgtRect.width() - 2);
	Rectangle.setHeight(10);
	Painter.drawRect(Rectangle);


	Rectangle.setX(WgtRect.x());
	Rectangle.setY(WgtRect.y() + 42);
	Rectangle.setWidth(WgtRect.width() - 2);
	Rectangle.setHeight(10);
	Painter.drawRect(Rectangle);


	Rectangle.setX(WgtRect.x());
	Rectangle.setY(WgtRect.y() + 53);
	Rectangle.setWidth(WgtRect.width() - 2);
	Rectangle.setHeight(10);
	Painter.drawRect(Rectangle);
	
}