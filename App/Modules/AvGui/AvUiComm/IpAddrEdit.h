#ifndef MYIPADDREDIT_H
#define MYIPADDREDIT_H

#include <QFrame>

class QLineEdit;
class QLabel;
class IpPartLineEdit;

class IpAddrEdit : public QFrame
{
    Q_OBJECT
public:
    IpAddrEdit(QWidget* pParent = 0);
    ~IpAddrEdit();

    void settext(const QString &text);
    QString text();
    void setStyleSheet(const QString &styleSheet);

signals:
    void textchanged(const QString& text);
    void textedited(const QString &text);

private slots:
    void paintEvent(QPaintEvent *);
    void textchangedslot(const QString& text);
    void texteditedslot(const QString &text);

private:
    IpPartLineEdit *ip_part1;
    IpPartLineEdit *ip_part2;
    IpPartLineEdit *ip_part3;
    IpPartLineEdit *ip_part4;

    QLabel *labeldot1;
    QLabel *labeldot2;
    QLabel *labeldot3;
};

#endif // MYIPADDREDIT_H
