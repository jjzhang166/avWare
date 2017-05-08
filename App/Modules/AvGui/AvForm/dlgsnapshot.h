#ifndef DLGSNAPSHOT_H
#define DLGSNAPSHOT_H

#include <QDialog>

namespace Ui {
class DlgSnapshot;
}

class DlgSnapshot : public QDialog
{
    Q_OBJECT

public:
    explicit DlgSnapshot(QWidget *parent = 0);
    ~DlgSnapshot();

public:
	void FillInSnapshot(unsigned char *data, int len);
private slots:
    void on_btnMenu_Close_clicked();

private:
    Ui::DlgSnapshot *ui;
};

#endif // DLGSNAPSHOT_H
