#ifndef FRMMAINMENU_H
#define FRMMAINMENU_H

#include <QWidget>

namespace Ui {
class FrmMainMenu;
}

class FrmMainMenu : public QWidget
{
    Q_OBJECT

public:
    explicit FrmMainMenu(QWidget *parent = 0);
    ~FrmMainMenu();

private slots:
    void on_BtnMainMenuDevices_clicked();

    void on_BtnMainMenuSystemSet_clicked();

    void on_BtnMainMenuAlarm_clicked();

    void on_BtnMainMenuSysTools_clicked();

    void on_BtnMainMenuRecord_clicked();

    void on_BtnMainMenuIntelligenceAlgorthm_clicked();

    void on_BtnMainMenuSystemInfo_clicked();

    void on_BtnClose_clicked();

private:
    Ui::FrmMainMenu *ui;
};

#endif // FRMMAINMENU_H
