#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include<QThread>
#include<QDebug>
#include "worker.h"
class Sys_ctl;

// controller用于启动线程和处理线程执行结果
class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller(Sys_ctl *sys_ctl, QString device_name, QObject *parent= nullptr);
    ~Controller();

    //Worker *worker;

    void get_data(QMap<QString, QVariant> data, QString data_type);
    void get_data(data_exchange data);

public slots:
    void handleResults(QString dev_name, QTcpSocket *tcp, QByteArray data);
    // 处理线程执行的结果
//    void handleResults(const int rslt)
//    {
//        qDebug()<<"receive the resultReady signal---------------------------------";
//        qDebug()<<"     current thread ID:"<<QThread::currentThreadId()<<'\n';
//        qDebug()<<"     the last result is:"<<rslt;
//    }
signals:
    // 发送信号触发线程
    //void operate(data_exchange, QTcpSocket *);
    void operate(QMap<QString, QVariant>, QString);
    void operate(data_exchange);

    void init(QString);

    //发送给主处理程序处理
    void data_come(QString , QTcpSocket *, QByteArray);

};

#endif // CONTROLLER_H
