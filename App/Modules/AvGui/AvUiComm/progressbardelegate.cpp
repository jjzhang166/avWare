#include "progressbardelegate.h"
#include <qdebug.h>
#include <QPainter>
#include <QApplication>

ProgressBarDelegate::ProgressBarDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    m_indexProgressBar = 0;
}


void ProgressBarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool percentum = true;
    int  value = 0;
    QString Text;



//	drawBackground(painter, option, index);
    if(index.column() == m_indexProgressBar) {
        QString args = index.model()->data(index).toString();
        //qDebug() << "ProgressBar Recv Message "<< args;
        QStringList StringList = args.split(':');
        QList<QString>::Iterator iList;
        for (iList = StringList.begin(); iList != StringList.end(); iList++){
            if (*iList == QString("no-percentum")){
                percentum = false;
            }else if (*iList == QString("text")){
                iList++;
                Text = *iList;
            }else if (*iList == QString("value")){
                iList++;
                value = iList->toInt();
            }
        }
        QStyleOptionProgressBarV2 progressBarOption;
        progressBarOption.state = QStyle::State_Enabled;
        progressBarOption.direction = QApplication::layoutDirection ();
        progressBarOption.fontMetrics = QApplication::fontMetrics ();
        progressBarOption.rect = option.rect.adjusted(4, 4, -4, -4);
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment =Qt::AlignCenter;// Qt::AlignRight;
        progressBarOption.textVisible = true;
        progressBarOption.progress = value;
        if (Text.length() != 0){
            progressBarOption.text = Text;
        }
        if (percentum == true){
            if (progressBarOption.text.length() != 0){
                progressBarOption.text += QString("  ");
            }
            progressBarOption.text += QString("%1%").arg(progressBarOption.progress);
        }
        painter->save();

        if (option.state & QStyle::State_Selected) {
            painter->fillRect(option.rect, option.palette.highlight());
			painter->setBrush(option.palette.highlightedText());
        }
// 		painter->setBrush(Qt::red);
		painter->setPen(Qt::black);
// 		painter->setBackgroundMode(Qt::BGMode::TransparentMode);
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
		
        painter->restore();
    } else {
        return QItemDelegate::paint (painter, option, index);
    }
}

void ProgressBarDelegate::SetProgressBarIndex(int index)
{
    m_indexProgressBar = index;
}
