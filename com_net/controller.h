#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QObject>
#include<QThread>
#include<QDebug>
#include "worker.h"

// controller用于启动线程和处理线程执行结果
class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:
    Controller(QString device_name, QObject *parent= nullptr);
    ~Controller();

    //Worker *worker;

    void get_data(data_exchange data, QTcpSocket *tcp);
    void get_data(QString dev_name,QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map, QStringList readList, QTcpSocket *tcp);

public slots:
    void handleResults(QString dev_name, QTcpSocket *tcp, QByteArray data);
    void read_vec(QString dev_name, QVector<data_exchange> data, QTcpSocket *tcp);
    // 处理线程执行的结果
//    void handleResults(const int rslt)
//    {
//        qDebug()<<"receive the resultReady signal---------------------------------";
//        qDebug()<<"     current thread ID:"<<QThread::currentThreadId()<<'\n';
//        qDebug()<<"     the last result is:"<<rslt;
//    }
signals:
    // 发送信号触发线程
    void operate(data_exchange, QTcpSocket *);
    void operate(QString ,QMap<QString, QMap<int, QMap<QString, QVariant>>> , QStringList , QTcpSocket *);

    void init(QString);

    //发送给主处理程序处理
    void data_come(QString , QTcpSocket *, QByteArray);
    void read_vec_signal(QString dev_name, QVector<data_exchange> data, QTcpSocket *tcp);

};

#endif // CONTROLLER_H
