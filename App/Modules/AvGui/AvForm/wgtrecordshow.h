#ifndef WGTRECORDSHOW_H
#define WGTRECORDSHOW_H

#include <QWidget>
#include <QLabel>

namespace Ui {
class WgtRecordShow;
}

class WgtRecordShow : public QWidget
{
    Q_OBJECT

public:
    explicit WgtRecordShow(QWidget *parent = 0);
    ~WgtRecordShow();
public:
	void paintEvent(QPaintEvent *event);


private:
	void DrawTimeShatf(QPainter &Painter);
	void DrawRectangle(QPainter &Painter);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void enterEvent(QEvent * event);
protected:
	bool event(QEvent *e);
private:
	QLabel *m_NumberLabel[12];
private:
    Ui::WgtRecordShow *ui;
};

#endif // WGTRECORDSHOW_H
