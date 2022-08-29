#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>

class mythread : public QObject
{
    Q_OBJECT
public:
    explicit mythread(QObject *parent = nullptr);

    bool flag=1;
    //线程处理函数
    void myTimeout();
    void setFlag(bool a);

signals:
    void mySignal();

public slots:
};

#endif // MYTHREAD_H
