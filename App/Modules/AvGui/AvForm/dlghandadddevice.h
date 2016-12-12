#ifndef DLGHANDADDDEVICE_H
#define DLGHANDADDDEVICE_H

#include <QDialog>

namespace Ui {
class DlgHandAddDevice;
}

class DlgHandAddDevice : public QDialog
{
    Q_OBJECT

public:
    explicit DlgHandAddDevice(QWidget *parent = 0);
    ~DlgHandAddDevice();

private slots:
    void on_BtnClose_clicked();

private:
    Ui::DlgHandAddDevice *ui;
};

#endif // DLGHANDADDDEVICE_H
