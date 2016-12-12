#ifndef VIDEOTOOLWIDGET_H
#define VIDEOTOOLWIDGET_H

#include <QWidget>

namespace Ui {
class VideoToolWidget;
}

class VideoToolWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VideoToolWidget(QWidget *parent = 0);
    ~VideoToolWidget();

private:
    Ui::VideoToolWidget *ui;
};

#endif // VIDEOTOOLWIDGET_H
