
#ifndef _UM_SLIDER_H_
#define _UM_SLIDER_H_

#include <QSlider>
#include <QMouseEvent>

class UMSlider : public QSlider 
{
    Q_OBJECT
        
public:
    UMSlider(QWidget *parent = 0);
    UMSlider(Qt::Orientation orientation, QWidget * parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);

};

#endif  // _UM_SLIDER_H_
