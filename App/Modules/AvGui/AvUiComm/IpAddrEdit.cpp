#include "IpAddrEdit.h"
#include <QRegExpValidator>
#include <QLabel>
#include "IpPartLineEdit.h"
#include <QDebug>

IpAddrEdit::IpAddrEdit(QWidget* pParent /* = 0 */)
: QFrame(pParent)
{
    ip_part1 = new IpPartLineEdit(this);
    ip_part2 = new IpPartLineEdit(this);
    ip_part3 = new IpPartLineEdit(this);
    ip_part4 = new IpPartLineEdit(this);

    labeldot1 = new QLabel(this);
    labeldot2 = new QLabel(this);
    labeldot3 = new QLabel(this);

    ip_part1->setGeometry(QRect(0, 0, 30, 20));
    ip_part2->setGeometry(QRect(30, 0, 30, 20));
    ip_part3->setGeometry(QRect(60, 0, 30, 20));
    ip_part4->setGeometry(QRect(90, 0, 30, 20));

    labeldot1->setText(" .");
    labeldot1->setGeometry(QRect(27, 1, 6, 18));
    labeldot1->setAlignment(Qt::AlignCenter);

    labeldot2->setText(" .");
    labeldot2->setGeometry(QRect(57, 1, 6, 18));
    labeldot2->setAlignment(Qt::AlignCenter);

    labeldot3->setText(" .");
    labeldot3->setGeometry(QRect(87, 1, 6, 18));
    labeldot3->setAlignment(Qt::AlignCenter);

    QWidget::setTabOrder(ip_part1, ip_part2);
    QWidget::setTabOrder(ip_part2, ip_part3);
    QWidget::setTabOrder(ip_part3, ip_part4);
    ip_part1->set_edit_list(NULL, ip_part2);
    ip_part2->set_edit_list(ip_part1, ip_part3);
    ip_part3->set_edit_list(ip_part2, ip_part4);
    ip_part4->set_edit_list(ip_part3, NULL);

    this->setFixedHeight(20);

    connect(ip_part1, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));
    connect(ip_part2, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));
    connect(ip_part3, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));
    connect(ip_part4, SIGNAL(textChanged(const QString&)), this, SLOT(textchangedslot(const QString&)));

    connect(ip_part1, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));
    connect(ip_part2, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));
    connect(ip_part3, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));
    connect(ip_part4, SIGNAL(textEdited (const QString&)), this, SLOT(texteditedslot(const QString&)));
}

IpAddrEdit::~IpAddrEdit()
{

}

void IpAddrEdit::textchangedslot(const QString& /*text*/)
{
    QString ippart1, ippart2, ippart3, ippart4;
    ippart1 = ip_part1->text();
    ippart2 = ip_part2->text();
    ippart3 = ip_part3->text();
    ippart4 = ip_part4->text();

    QString ipaddr = QString("%1.%2.%3.%4")
                     .arg(ippart1)
                     .arg(ippart2)
                     .arg(ippart3)
                     .arg(ippart4);

    emit textchanged(ipaddr);
}

void IpAddrEdit::texteditedslot(const QString &/*text*/)
{
    QString ippart1, ippart2, ippart3, ippart4;
    ippart1 = ip_part1->text();
    ippart2 = ip_part2->text();
    ippart3 = ip_part3->text();
    ippart4 = ip_part4->text();

    QString ipaddr = QString("%1.%2.%3.%4")
        .arg(ippart1)
        .arg(ippart2)
        .arg(ippart3)
        .arg(ippart4);

    emit textedited(ipaddr);
}

void IpAddrEdit::settext(const QString &text)
{
    QString ippart1, ippart2, ippart3, ippart4;
    QString qstring_validate = text;

    // IP地址验证
    QRegExp regexp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator regexp_validator(regexp, this);
    int nPos = 0;
    QValidator::State state = regexp_validator.validate(qstring_validate, nPos);
    // IP合法
    if (state == QValidator::Acceptable)
    {
        QStringList ippartlist = text.split(".");

        int strcount = ippartlist.size();
        int index = 0;
        if (index < strcount)
        {
            ippart1 = ippartlist.at(index);
        }
        if (++index < strcount)
        {
            ippart2 = ippartlist.at(index);
        }
        if (++index < strcount)
        {
            ippart3 = ippartlist.at(index);
        }
        if (++index < strcount)
        {
            ippart4 = ippartlist.at(index);
        }
    }

    ip_part1->setText(ippart1);
    ip_part2->setText(ippart2);
    ip_part3->setText(ippart3);
    ip_part4->setText(ippart4);
}

QString IpAddrEdit::text()
{
    QString ippart1, ippart2, ippart3, ippart4;
    ippart1 = ip_part1->text();
    ippart2 = ip_part2->text();
    ippart3 = ip_part3->text();
    ippart4 = ip_part4->text();

    return QString("%1.%2.%3.%4")
        .arg(ippart1)
        .arg(ippart2)
        .arg(ippart3)
        .arg(ippart4);
}

void IpAddrEdit::setStyleSheet(const QString &styleSheet)
{
    ip_part1->setStyleSheet(styleSheet);
    ip_part2->setStyleSheet(styleSheet);
    ip_part3->setStyleSheet(styleSheet);
    ip_part4->setStyleSheet(styleSheet);
}

void IpAddrEdit::paintEvent(QPaintEvent *)
{
    int tmp=this->width()/4;
    int tmp2=this->width()%4;
    ip_part1->setGeometry(QRect( 0, 0, tmp, 20));
    labeldot1->setGeometry(QRect(tmp-3, 1, 6, 18));
    ip_part2->setGeometry(QRect(tmp, 0, tmp, 20));
    labeldot2->setGeometry(QRect(tmp*2-3, 1, 6, 18));
    ip_part3->setGeometry(QRect(tmp*2, 0, tmp, 20));
    labeldot3->setGeometry(QRect(tmp*3-3, 1, 6, 18));
    ip_part4->setGeometry(QRect(tmp*3, 0, tmp+tmp2, 20));

}
