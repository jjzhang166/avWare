#ifndef FRMAREASELECT_H
#define FRMAREASELECT_H

#include <QWidget>

namespace Ui {
class FrmAreaSelect;
}

class FrmAreaSelect : public QWidget
{
    Q_OBJECT

public:
    explicit FrmAreaSelect(QWidget *parent = 0);
    ~FrmAreaSelect();

private:
    Ui::FrmAreaSelect *ui;
};

#endif // FRMAREASELECT_H
