#ifndef _FLOATINGBUTTON_H_
#define _FLOATINGBUTTON_H_
#include <QWidget>
#include <QBitmap>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
class FloatingButton : public QWidget
{
	Q_OBJECT
public:
	FloatingButton(QWidget *parent = 0);
Q_SIGNALS:
	void SignalsButtonClick();

protected:
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);

private:
	QPoint	m_CurrentPos;
	QPixmap m_Pixmap;
	bool	m_bWidgetMoved;
};


#endif
