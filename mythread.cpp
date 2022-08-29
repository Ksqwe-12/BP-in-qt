#include "mythread.h"
#include<QThread>
#include<QDebug>
mythread::mythread(QObject *parent) : QObject(parent)
{

}

void mythread::myTimeout()
{
    while (flag)
    {
        QThread::msleep(1200);
        emit mySignal();
        qDebug()<<"子："<<QThread::currentThread();
    }
}

void mythread::setFlag(bool a)
{
    flag=a;
}
