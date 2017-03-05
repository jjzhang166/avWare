#include "frmareaselect.h"
#include "ui_frmareaselect.h"

FrmAreaSelect::FrmAreaSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmAreaSelect)
{
    ui->setupUi(this);
}

FrmAreaSelect::~FrmAreaSelect()
{
    delete ui;
}
