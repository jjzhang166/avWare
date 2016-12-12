#ifndef MYIPPARTLINEEDIT_H
#define MYIPPARTLINEEDIT_H

#include <QLineEdit>

class QWidget;
class QFocusEvent;
class QKeyEvent;

class IpPartLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    IpPartLineEdit(QWidget *parent = 0);
    ~IpPartLineEdit(void);

    void set_edit_list(QLineEdit *lasttab, QLineEdit *nexttab) { last_tab = lasttab;next_tab = nexttab; }

protected:
    virtual void focusInEvent(QFocusEvent *e);
    virtual    void keyPressEvent(QKeyEvent *event);

private slots:
    void text_edited(const QString& text);

private:
    QLineEdit *last_tab;
    QLineEdit *next_tab;
};

#endif // MYIPPARTLINEEDIT_H
