
#include "umslider.h"



UMSlider::UMSlider(QWidget *parent) : QSlider(parent)
{
}


UMSlider::UMSlider(Qt::Orientation orientation, QWidget * parent) : QSlider(orientation, parent)
{
}


void UMSlider::mousePressEvent(QMouseEvent *event) 
{
    blockSignals(true);
    QSlider::mousePressEvent(event);
    blockSignals(false);
    
    double pos = event->x() / (double)width();
    setSliderPosition(pos * (maximum() - minimum()) + minimum());
}



