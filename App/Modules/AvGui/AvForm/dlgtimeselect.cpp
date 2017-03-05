#include "dlgtimeselect.h"
#include "ui_dlgtimeselect.h"

#include <QToolTip>
#include <QPainter>
#include <QMouseEvent>
#include "AvUiComm/IconComm.h"
#include "AvUiComm/AvUiComm.h"
#define AreaHigh 18
#define Max_Rang_NUM 3

#define CTIMEAREA_START_X 20
#define CTIMEAREA_START_Y 50
#define CTIMEAREA_LENGTH  570

DlgTimeSelect::DlgTimeSelect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgTimeSelect)
{
    ui->setupUi(this);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(CTIMEAREA_LENGTH+100, 300 + 70);
	this->setWindowModality(Qt::ApplicationModal);
	setWindowFlags(Qt::Tool | Qt::X11BypassWindowManagerHint);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    //this->setAttribute(Qt::WA_DeleteOnClose);
    IconComm::Instance()->SetIcon(ui->BtnClose, QChar(0xf00d), 10);
    IconComm::Instance()->SetIcon(ui->LabIco, QChar(0xf015), 12);


    int time_start_x ;
    int time_start_y = 35;
    time_start_x = 14;
    ui->Time_00->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_02->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_04->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_06->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_08->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_10->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_12->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_14->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_16->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_18->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_20->move(time_start_x, time_start_y);
    time_start_x += 48;
    ui->Time_22->move(time_start_x, time_start_y);
    time_start_x += 48;

    int week_start_y = 55;
    int week_start_y_step = 40;
    int week_start_x = 620;

    ui->Week1->move(week_start_x, week_start_y);
    ui->Week1checkBox->move(week_start_x - 18, week_start_y);
    week_start_y+= week_start_y_step;
    ui->Week2->move(week_start_x, week_start_y);
    ui->Week2checkBox->move(week_start_x - 18, week_start_y);
    week_start_y+= week_start_y_step;
    ui->Week3->move(week_start_x, week_start_y);
    ui->Week3checkBox->move(week_start_x - 18, week_start_y);
    week_start_y+= week_start_y_step;
    ui->Week4->move(week_start_x, week_start_y);
    ui->Week4checkBox->move(week_start_x - 18, week_start_y);
    week_start_y+= week_start_y_step;
    ui->Week5->move(week_start_x, week_start_y);
    ui->Week5checkBox->move(week_start_x - 18, week_start_y);
    week_start_y+= week_start_y_step;
    ui->Week6->move(week_start_x, week_start_y);
    ui->Week6checkBox->move(week_start_x - 18, week_start_y);
    week_start_y+= week_start_y_step;
    ui->Week7->move(week_start_x, week_start_y);
    ui->Week7checkBox->move(week_start_x - 18, week_start_y);
    week_start_y+= week_start_y_step;


        int start_x = CTIMEAREA_START_X;
        int start_y = CTIMEAREA_START_Y;
    for (int i = 0; i < 7; i++){
        m_WeekRect[i].Start_x = start_x;
        m_WeekRect[i].Start_y = start_y;
                m_WeekRect[i].End_x = start_x + CTIMEAREA_LENGTH;
        m_WeekRect[i].End_y = start_y + 30;
        start_y += 40;
    }

    ClearMouseZone();
}

DlgTimeSelect::~DlgTimeSelect()
{
    delete ui;
}

void DlgTimeSelect::on_Week4checkBox_clicked(bool checked)
{
    int index = 3;
    if(checked == true){
        m_FillInList[index].clear();
        CTimeAreaZone Zone;
        Zone.Start_x = m_WeekRect[index].Start_x;
        Zone.End_x = m_WeekRect[index].End_x;
        Zone.Start_y = m_WeekRect[index].End_y - AreaHigh;
        Zone.End_y   = m_WeekRect[index].End_y;
        m_FillInList[index].push_back(Zone);
    }else{
        m_FillInList[index].clear();
    }
    update();
}

void DlgTimeSelect::on_Week2checkBox_clicked(bool checked)
{
    int index = 1;
    if(checked == true){
        m_FillInList[index].clear();
        CTimeAreaZone Zone;
        Zone.Start_x = m_WeekRect[index].Start_x;
        Zone.End_x = m_WeekRect[index].End_x;
        Zone.Start_y = m_WeekRect[index].End_y - AreaHigh;
        Zone.End_y   = m_WeekRect[index].End_y;
        m_FillInList[index].push_back(Zone);
    }else{
        m_FillInList[index].clear();
    }
    update();
}

void DlgTimeSelect::on_Week6checkBox_clicked(bool checked)
{
    int index = 5;
    if(checked == true){
        m_FillInList[index].clear();
        CTimeAreaZone Zone;
        Zone.Start_x = m_WeekRect[index].Start_x;
        Zone.End_x = m_WeekRect[index].End_x;
        Zone.Start_y = m_WeekRect[index].End_y - AreaHigh;
        Zone.End_y   = m_WeekRect[index].End_y;
        m_FillInList[index].push_back(Zone);
    }else{
        m_FillInList[index].clear();
    }
    update();
}

void DlgTimeSelect::on_Week1checkBox_clicked(bool checked)
{
    int index = 0;
    if(checked == true){
        m_FillInList[index].clear();
        CTimeAreaZone Zone;
        Zone.Start_x = m_WeekRect[index].Start_x;
        Zone.End_x = m_WeekRect[index].End_x;
        Zone.Start_y = m_WeekRect[index].End_y - AreaHigh;
        Zone.End_y   = m_WeekRect[index].End_y;
        m_FillInList[index].push_back(Zone);
    }else{
        m_FillInList[index].clear();
    }
    update();
}

void DlgTimeSelect::on_Week3checkBox_clicked(bool checked)
{
    int index = 2;
    if(checked == true){
        m_FillInList[index].clear();
        CTimeAreaZone Zone;
        Zone.Start_x = m_WeekRect[index].Start_x;
        Zone.End_x = m_WeekRect[index].End_x;
        Zone.Start_y = m_WeekRect[index].End_y - AreaHigh;
        Zone.End_y   = m_WeekRect[index].End_y;
        m_FillInList[index].push_back(Zone);
    }else{
        m_FillInList[index].clear();
    }
    update();
}

void DlgTimeSelect::on_Week5checkBox_clicked(bool checked)
{
    int index = 4;
    if(checked == true){
        m_FillInList[index].clear();
        CTimeAreaZone Zone;
        Zone.Start_x = m_WeekRect[index].Start_x;
        Zone.End_x = m_WeekRect[index].End_x;
        Zone.Start_y = m_WeekRect[index].End_y - AreaHigh;
        Zone.End_y   = m_WeekRect[index].End_y;
        m_FillInList[index].push_back(Zone);
    }else{
        m_FillInList[index].clear();
    }
    update();
}

void DlgTimeSelect::on_Week7checkBox_clicked(bool checked)
{
    int index = 6;
    if(checked == true){
        m_FillInList[index].clear();
        CTimeAreaZone Zone;
        Zone.Start_x = m_WeekRect[index].Start_x;
        Zone.End_x = m_WeekRect[index].End_x;
        Zone.Start_y = m_WeekRect[index].End_y - AreaHigh;
        Zone.End_y   = m_WeekRect[index].End_y;
        m_FillInList[index].push_back(Zone);
    }else{
        m_FillInList[index].clear();
    }
    update();
}

void DlgTimeSelect::on_CTimeAreaDeleteAll_clicked()
{
    for (int i = 0; i < 7; i++){
        m_FillInList[i].clear();
    }

    ui->Week1checkBox->setChecked(false);
    ui->Week2checkBox->setChecked(false);
    ui->Week3checkBox->setChecked(false);
    ui->Week4checkBox->setChecked(false);
    ui->Week5checkBox->setChecked(false);
    ui->Week6checkBox->setChecked(false);
    ui->Week7checkBox->setChecked(false);
    update();
}

void DlgTimeSelect::on_CTimeAreaDeleate_clicked()
{
    for (int i = 0; i < 7; i++){
        std::list <CTimeAreaZone>::iterator iList;
        for (iList = m_FillInList[i].begin(); iList != m_FillInList[i].end(); iList++){
            CTimeAreaZone AreaZone = *iList;
            if (AreaZone.isSelect == true){
                iList = m_FillInList[i].erase(iList);
                update();
                return;
            }
        }
    }
}

void DlgTimeSelect::on_CTimeAreaSubmit_clicked()
{

}

void DlgTimeSelect::on_CTimeAreaCancel_clicked()
{
    close();
}



void DlgTimeSelect::DrawFillInRect(QPainter &Painter)
{
	QBrush Brush;
	for (int i = 0; i < 7; i++){
		std::list <CTimeAreaZone>::iterator iList;
		for (iList = m_FillInList[i].begin(); iList != m_FillInList[i].end(); iList++){
			CTimeAreaZone AreaZone = *iList;
			if (AreaZone.isSelect == true){
				Brush.setColor(Qt::red);
				Brush.setStyle(Qt::Dense4Pattern);
				Painter.setBrush(Brush);
				//*iList = AreaZone;
				// 				QPoint pos(AreaZone.End_x, AreaZone.End_y);
				// 				QToolTip::showText(QCursor::pos(), QString("12:00-12:00"), this);
			}
			else{
				Brush.setColor(Qt::green);
				Brush.setStyle(Qt::Dense4Pattern);
				Painter.setBrush(Brush);
				//*iList = AreaZone;
			}
			Painter.drawRect(AreaZone.Start_x, AreaZone.Start_y, AreaZone.End_x - AreaZone.Start_x, AreaHigh);
		}
	}
}
void DlgTimeSelect::DrawWeekRect(QPainter &Painter)
{
	Painter.setBrush(Qt::NoBrush);
	Painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));
	for (int i = 0; i < 7; i++){
		Painter.drawRect(m_WeekRect[i].Start_x, m_WeekRect[i].Start_y, CTIMEAREA_LENGTH, 30);
		for (int j = 1; j < 24; j++){
			Painter.drawLine(m_WeekRect[i].Start_x + j * 24, m_WeekRect[i].Start_y, m_WeekRect[i].Start_x + j * 24,
				j % 2 == 0 ? m_WeekRect[i].Start_y + 10 : m_WeekRect[i].Start_y + 5);
		}
	}
}

void DlgTimeSelect::ClearMouseZone()
{

	m_DrawCurrPlace.Start_x = 0;
	m_DrawCurrPlace.Start_y = 0;
	m_DrawCurrPlace.End_x = 0;
	m_DrawCurrPlace.End_y = 0;
	PressEventValid = false;
	PressEventIndex = -1;
}

void DlgTimeSelect::ClearSelectEvent()
{
	std::list <CTimeAreaZone>::iterator iList;
	for (int i = 0; i < 7; i++){
		for (iList = m_FillInList[i].begin(); iList != m_FillInList[i].end(); iList++){
			CTimeAreaZone AreaZone = *iList;
			if (AreaZone.isSelect == true){
				AreaZone.isSelect = false;
				*iList = AreaZone;
			}
		}
	}
}
void DlgTimeSelect::paintEvent(QPaintEvent *)
{
    QPainter Painter(this);
    if (m_DrawCurrPlace.Start_x != 0 && m_DrawCurrPlace.Start_y != 0 && m_DrawCurrPlace.End_x != 0 && m_DrawCurrPlace.End_y != 0)
    {
        QBrush Brush;
        Brush.setColor(Qt::blue);
        Brush.setStyle(Qt::Dense4Pattern);
        Painter.setBrush(Brush);
        Painter.drawRect(m_DrawCurrPlace.Start_x, m_DrawCurrPlace.Start_y, m_DrawCurrPlace.End_x - m_DrawCurrPlace.Start_x, AreaHigh);
    }


    DrawFillInRect(Painter);
    DrawWeekRect(Painter);
}

void DlgTimeSelect::mousePressEvent(QMouseEvent *event)
{
    int mouse_x = event->x();
    int mouse_y = event->y();
    int DrawIndex = 0;
    ClearMouseZone();
    //不在规定的区域内开始，即不是画图，也不是选中,直接不理会
    {
        bool posbool = false;
        for (int i =0; i< 7; i++){
            if (m_WeekRect[i].Start_x <= mouse_x && m_WeekRect[i].End_x > mouse_x
                    && m_WeekRect[i].Start_y < mouse_y && m_WeekRect[i].End_y > mouse_y){
                posbool = true;
                PressEventIndex = DrawIndex = i;
            }
        }
        if (posbool == false) {
            PressEventValid = false;
            return;
        }
    }

    //在已经画完的区域内开始画，认为是选中事件
    {
        std::list <CTimeAreaZone>::iterator iList;
        for (iList = m_FillInList[DrawIndex].begin(); iList != m_FillInList[DrawIndex].end(); iList++){
            CTimeAreaZone AreaZone = *iList;
            if (mouse_x <= AreaZone.End_x && mouse_x >= AreaZone.Start_x){
                ClearSelectEvent();
                AreaZone.isSelect = true;
                *iList = AreaZone;
                PressEventValid = false;
                update();
                return;
            }
        }
    }
    //正确的开始画图
    {
        m_DrawCurrPlace.Start_x = mouse_x;
        m_DrawCurrPlace.Start_y = m_WeekRect[PressEventIndex].End_y - AreaHigh;
        PressEventValid = true;
    }

    update();
}

void DlgTimeSelect::mouseReleaseEvent(QMouseEvent *event)
{
    int mouse_x = event->x();
    int mouse_y = event->y();
    //起始位置本身就是无效的，
    if (PressEventValid == false) {
         ClearMouseZone();
        return;
    }
    //反方向画图，直接不理会。
    {
        if (mouse_x < m_DrawCurrPlace.Start_x){
            ClearMouseZone();
            update();
            return;
        }
    }

    //误操作事件。双击。
    {
         if (abs(m_DrawCurrPlace.Start_x - mouse_x) <= 5){
            return;
        }
    }
    //添加到list 里面
    {
                if (m_FillInList[PressEventIndex].size() >= Max_Rang_NUM){
                        CAvUiComm::ShowMessageBoxInfo(tr("Full Rang"));

                }
                else{
                        CTimeAreaZone AreaZone;
                        AreaZone.Start_x = m_DrawCurrPlace.Start_x;
                        AreaZone.Start_y = m_DrawCurrPlace.Start_y;
                        AreaZone.End_x = m_DrawCurrPlace.End_x;
                        AreaZone.End_y = m_DrawCurrPlace.End_y;
                        AreaZone.isSelect = false;
                        m_FillInList[PressEventIndex].push_back(AreaZone);
                }
    }
    ClearMouseZone();
    ClearSelectEvent();
    update();




}

void DlgTimeSelect::mouseMoveEvent(QMouseEvent *event)
{
    int mouse_x = event->x();
    int mouse_y = event->y();
    //起始位置本身就是无效的，
    if (PressEventValid == false) return;
    //反方向画图，直接不理会。
    {
        if (mouse_x < m_DrawCurrPlace.Start_x){
            return;
        }
    }

    //正常托图
    {
        //是否有托到后面已经画上的图上。
        {
            std::list <CTimeAreaZone>::iterator iList;
            for (iList = m_FillInList[PressEventIndex].begin(); iList != m_FillInList[PressEventIndex].end(); iList++){
                CTimeAreaZone AreaZone = *iList;
                if (m_DrawCurrPlace.Start_x < AreaZone.Start_x && mouse_x >= AreaZone.Start_x){
                    m_DrawCurrPlace.End_x = AreaZone.Start_x;
                    m_DrawCurrPlace.End_y = AreaZone.End_y;
                    update();
                    return;
                }
            }

            m_DrawCurrPlace.End_x = mouse_x >= m_WeekRect[PressEventIndex].End_x ? m_WeekRect[PressEventIndex].End_x:mouse_x;
            m_DrawCurrPlace.End_y = m_WeekRect[PressEventIndex].End_y;
        }
    }
    update();
}



void DlgTimeSelect::CTimeAreaArrary2UiPos(C_TimeArea  m_AlarmTimeArea[AvMaxWeeks][AvMaxTimeArea])
{
        float RatePerMin = (float)CTIMEAREA_LENGTH / (float)(24 * 60);
		
		

        for (int i = 0; i < AvMaxWeeks; i++){
                for (int j = 0; j < AvMaxTimeArea; j++){
                        if (m_AlarmTimeArea[i][j].StopSec - m_AlarmTimeArea[i][j].StartSec > 5 * 60){
                                CTimeAreaZone Zone;
                                Zone.Start_x = m_WeekRect[i].Start_x + (m_AlarmTimeArea[i][j].StartSec / 60)*RatePerMin;
                                Zone.End_x = Zone.Start_x + (float)((m_AlarmTimeArea[i][j].StopSec - m_AlarmTimeArea[i][j].StartSec)/ 60)*RatePerMin;;
								Zone.Start_y = m_WeekRect[i].End_y - AreaHigh;
								Zone.End_y = m_WeekRect[i].End_y;
								//MultipleMax(Zone.End_x, 5);
								//MultipleMin(Zone.Start_x, 5);

								Zone.isSelect = false;
                                m_FillInList[i].push_back(Zone);
                        }

                }
        }

		update();

}


void DlgTimeSelect::CTimeAreaUi2ArraryPos(C_TimeArea  m_AlarmTimeArea[AvMaxWeeks][AvMaxTimeArea])
{
        std::list <CTimeAreaZone>::iterator iList;
		float RatePerMin = (float)(24 * 60)/(float)CTIMEAREA_LENGTH ;
		memset(m_AlarmTimeArea, 0x00, sizeof(C_TimeArea)*AvMaxWeeks*AvMaxTimeArea);
		for (int i = 0; i < AvMaxWeeks; i++){
			for (int j = 0; j < AvMaxTimeArea; j++){
				m_AlarmTimeArea[i][j].WeekIndex = i;
			}
		}
		for (int i = 0, j = 0; i < AvMaxWeeks; i++){

                for (iList = m_FillInList[i].begin(), j = 0; iList != m_FillInList[i].end(); iList++){
                        CTimeAreaZone AreaZone = *iList;
						


						m_AlarmTimeArea[i][j].StartSec = (AreaZone.Start_x - m_WeekRect[i].Start_x)*RatePerMin * 60;
						m_AlarmTimeArea[i][j].StopSec = (AreaZone.End_x - m_WeekRect[i].Start_x)*RatePerMin * 60;
						if (m_AlarmTimeArea[i][j].StopSec >= 24 * 3600){
							m_AlarmTimeArea[i][j].StopSec = 24 * 3600 - 1;
						}
						//MultipleMax(AreaZone.End_x, 5 * 60);
						//MultipleMin(m_AlarmTimeArea[i][j].StartSec, 5*60);
						j++;
                }
        }
}



void DlgTimeSelect::on_BtnClose_clicked()
{
    close();
}
