#ifndef _ICONCOMM_H_
#define _ICONCOMM_H_

#include <QObject>
#include <QFont>
#include <QFontDatabase>
#include <QMutex>
#include <QLabel>
#include <QPushButton>
#include <QApplication>

class IconComm : public QObject
{
private:
    explicit IconComm(QObject *parent = 0);
    QFont iconFont;
    static IconComm* _instance;

public:
    static IconComm* Instance()
    {
        static QMutex mutex;
        if (!_instance) {
            QMutexLocker locker(&mutex);
            if (!_instance) {
                _instance = new IconComm;
            }
        }
        return _instance;
    }

    void SetIcon(QLabel* lab, QChar c, int size = 10);
    void SetIcon(QPushButton* btn, QChar c, int size = 10);

};

#endif
