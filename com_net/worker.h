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
    void doWork(data_exchange l, QTcpSocket *tcp);
    void doWork(QString dev_name, QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map,
                        QStringList readList, QTcpSocket *tcp);


// 线程完成工作时发送的信号
signals:
    void resultReady(QString , QTcpSocket *, QByteArray );
    void read_vec(QString, QVector<data_exchange>, QTcpSocket *);

private slots:

    void data_come(QByteArray &data, int id);
    void data_come_error();

    void init(QString device_name);

private:
    QTcpSocket *tcp_save;
    QString device_save;
    QTimer *m_timer;

};

#endif // WORKER_H
