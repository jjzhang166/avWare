#include "IpPartLineEdit.h"
#include <QIntValidator>
#include <QKeyEvent>
#include <QDebug>

IpPartLineEdit::IpPartLineEdit(QWidget *parent/* = 0*/)
: QLineEdit(parent)
{
    next_tab = NULL;

    this->setMaxLength(3);
    this->setFrame(false);
    this->setAlignment(Qt::AlignCenter);

    QValidator *validator = new QIntValidator(0, 255, this);
    this->setValidator(validator);

    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(text_edited(const QString&)));
}

IpPartLineEdit::~IpPartLineEdit(void)
{
}

void IpPartLineEdit::focusInEvent(QFocusEvent *e)
{
    this->selectAll();
    QLineEdit::focusInEvent(e);
}

void IpPartLineEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Period)
    {
        if (next_tab)
        {
            next_tab->setFocus();
            next_tab->selectAll();
        }
    }
    else if (event->key() == Qt::Key_Backspace)
    {
        qDebug() << this->text();
        if (last_tab && this->text() == "")
        {
            last_tab->setFocus();
            last_tab->selectAll();
        }
    }
    QLineEdit::keyPressEvent(event);
}

void IpPartLineEdit::text_edited(const QString& text)
{
    QIntValidator v(0, 255, this);
    QString ipaddr = text;
    int pos = 0;
    QValidator::State state = v.validate(ipaddr, pos);
    if (state == QValidator::Acceptable)
    {
        if (ipaddr.size() > 1)
        {
            if (ipaddr.size() == 2)
            {
                int ipnum = ipaddr.toInt();

                if (ipnum > 25)
                {
                    if (next_tab)
                    {
                        next_tab->setFocus();
                        next_tab->selectAll();
                    }
                }
            }
            else
            {
                if (next_tab)
                {
                    next_tab->setFocus();
                    next_tab->selectAll();
                }
            }
        }
    }
}

