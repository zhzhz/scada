// work定义了线程要执行的工作
#ifndef WORKER_H
#define WORKER_H
#include <QObject>
#include<QDebug>
#include<QThread>
#include "dev_driver.h"
#include "qtimer.h"

class Worker:public QObject
{
    Q_OBJECT
public:
    Worker(QObject* parent = nullptr);
    ~Worker();

    Dev_driver dev_driver;
public slots:
     // doWork定义了线程要执行的操作
    void doWork(QMap<QString, QVariant>, QString);
    void doWork(data_exchange);


// 线程完成工作时发送的信号
signals:
    void resultReady(QString , QTcpSocket *, QByteArray );
    void data_come_signal(QByteArray &, data_exchange);
    void data_come_error_signal(QByteArray &, data_exchange);

    void host_closed_signal(QTcpSocket *);
    void networkerror_signal(QTcpSocket *);
    void connect_resume();
    void connect_ok();

private slots:

    void data_come(QByteArray &, data_exchange);
    void data_come_error(QByteArray &, data_exchange);

    void init(QString device_name);

    void host_closed(QTcpSocket *);
    void networkerror(QTcpSocket *);

    //void host_closed(QTcpSocket *tcp);
    //void networkerror(QTcpSocket *);

    void TimerTimeout(void);
private:
    QTcpSocket *tcp_save;
    QString device_save;
    QTimer *m_timer;

    bool host_closed_f;
    bool networkerror_f;

};

#endif // WORKER_H
