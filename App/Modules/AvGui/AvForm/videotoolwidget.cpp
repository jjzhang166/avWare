#include "videotoolwidget.h"
#include "ui_videotoolwidget.h"

VideoToolWidget::VideoToolWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoToolWidget)
{
    ui->setupUi(this);
}

VideoToolWidget::~VideoToolWidget()
{
    delete ui;
}
