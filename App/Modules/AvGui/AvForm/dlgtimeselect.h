#ifndef DLGTIMESELECT_H
#define DLGTIMESELECT_H

#include <QDialog>
#include "Apis/AvWareStruct.h"
namespace Ui {
class DlgTimeSelect;
}

typedef struct
{
    int Start_x;
    int Start_y;

    int End_x;
    int End_y;
    bool isSelect;
}CTimeAreaMouseZone, CTimeAreaZone;

class DlgTimeSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DlgTimeSelect(QWidget *parent = 0);
    ~DlgTimeSelect();

public:

        void CTimeAreaArrary2UiPos(C_TimeArea  m_AlarmTimeArea[AvMaxWeeks][AvMaxTimeArea]);
        void CTimeAreaUi2ArraryPos(C_TimeArea  m_AlarmTimeArea[AvMaxWeeks][AvMaxTimeArea]);

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);


private slots:
    void on_Week4checkBox_clicked(bool checked);

    void on_Week2checkBox_clicked(bool checked);

    void on_Week6checkBox_clicked(bool checked);

    void on_Week1checkBox_clicked(bool checked);

    void on_Week3checkBox_clicked(bool checked);

    void on_Week5checkBox_clicked(bool checked);

    void on_Week7checkBox_clicked(bool checked);

    void on_CTimeAreaDeleteAll_clicked();

    void on_CTimeAreaDeleate_clicked();

    void on_CTimeAreaSubmit_clicked();

    void on_CTimeAreaCancel_clicked();

    void on_BtnClose_clicked();

private:
    Ui::DlgTimeSelect *ui;

private:
    CTimeAreaMouseZone m_DrawCurrPlace;

    std::list <CTimeAreaZone> m_FillInList[7];
    bool PressEventValid;
    int  PressEventIndex;

private:
    void DrawFillInRect(QPainter &Painter);
    void DrawWeekRect(QPainter &Painter);
    void ClearMouseZone();
    void ClearSelectEvent();
    CTimeAreaZone m_WeekRect[7];

};

#endif // DLGTIMESELECT_H
