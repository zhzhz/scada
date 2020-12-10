// work定义了线程要执行的工作
#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include<QDebug>
#include<QThread>
#include "dev_driver.h"

class Worker:public QObject
{
    Q_OBJECT
public:
    Worker(QObject* parent = nullptr);
    ~Worker();

    Dev_driver dev_driver;
public slots:
     // doWork定义了线程要执行的操作
    void doWork(data_exchange l, QTcpSocket *tcp);


// 线程完成工作时发送的信号
signals:
    void resultReady(QString , QTcpSocket *, QByteArray );

private slots:

    void data_come(QByteArray &data);

    void init(QMap<int, void *> device);

private:
    QTcpSocket *tcp_save;
    QString device_save;

};

#endif // WORKER_H
