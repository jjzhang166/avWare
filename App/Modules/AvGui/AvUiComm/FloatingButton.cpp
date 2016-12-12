#include "FloatingButton.h"
#include "AvUiComm/AvUiComm.h"
FloatingButton::FloatingButton(QWidget *parent)
	: QWidget(parent, Qt::FramelessWindowHint)
{
	this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_DeleteOnClose);

	m_Pixmap.load(":/image/floatbutton.png");
	resize(m_Pixmap.size());
	setMask(m_Pixmap.mask());
	m_bWidgetMoved = false;

}
void FloatingButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton){
		m_Pixmap.load(":/image/floatbutton.png");
		resize(m_Pixmap.size());
		setMask(m_Pixmap.mask());
		update();
		event->accept();

		if (m_bWidgetMoved == false){
			emit SignalsButtonClick();
		}
	}
	m_bWidgetMoved = false;
	
}
void FloatingButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_Pixmap.load(":/image/floatbutton_press.png");
		resize(m_Pixmap.size());
		setMask(m_Pixmap.mask());
		update();
		m_CurrentPos = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
	else if (event->button() == Qt::RightButton){
		AvQDebug("close floatingbutton\n");
		this->close();
	}
	
}

void FloatingButton::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() && Qt::LeftButton) {
		move(event->globalPos() - m_CurrentPos);
		m_bWidgetMoved = true;
		event->accept();
	}
}

void FloatingButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, m_Pixmap);
}

void FloatingButton::leaveEvent(QEvent *event)
{ 
	setCursor(Qt::ArrowCursor);
}

void FloatingButton::enterEvent(QEvent *event)
{
	setCursor(Qt::PointingHandCursor);
}
