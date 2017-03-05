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
	void FixUi();
private slots:
    void on_BtnClose_clicked();

    void on_BtnSubmit_clicked();

    void on_CBoxProtocol_currentIndexChanged(const QString &arg1);

    void on_BtnCannel_clicked();

private:
    Ui::DlgHandAddDevice *ui;
};

#endif // DLGHANDADDDEVICE_H
