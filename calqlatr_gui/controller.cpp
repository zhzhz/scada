#include "controller.h"
#include "sys_ctl.h"

Controller::Controller(Sys_ctl *sys_ctl, QString device_name, QObject *parent) : QObject(parent)
{
    Worker *worker = new Worker;

    worker->moveToThread(&workerThread);
    //数据发送给采集线程
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, SIGNAL(operate(void *, QString)), worker, SLOT(doWork(void *, QString)));
    connect(this, SIGNAL(operate(data_exchange)), worker, SLOT(doWork(data_exchange)));


    //采集线程返回数据，分为正确数据和错误数据，分别对应有dev数据和没dev数据
    connect(worker, SIGNAL(data_come_signal(QByteArray &, data_exchange)), sys_ctl, SLOT(data_come(QByteArray &, data_exchange)));
    connect(worker, SIGNAL(data_come_error_signal(QByteArray &, data_exchange)), sys_ctl, SLOT(data_come_error(QByteArray &, data_exchange)));

    connect(worker, SIGNAL(connect_resume()), sys_ctl, SLOT(connect_resume()));



    //启动线程
    workerThread.start();


    //初始化worker线程,开始在新线程中处理网络通信，延迟也不影响gui
    connect(this, SIGNAL(init(QString)), worker, SLOT(init(QString)));

    emit init(device_name);

    //首次网络连接ok，通知sys_ctl可以发送数据了
    connect(worker, SIGNAL(connect_ok()), sys_ctl, SLOT(start()));
}
//析构函数中调用quit()函数结束线程
Controller::~Controller()
{
    qDebug() << "Controller::~Controller()1";
    workerThread.quit();
    //qDebug() << "Controller::~Controller()2";
    workerThread.wait();
    //qDebug() << "Controller::~Controller()3";
    //delete  worker;

}

//发送采集数据
void Controller::get_data(void *data, QString data_type)
{
    qDebug() << "2.gui发送读请求给Controller::get_data";
    emit operate(data, data_type);
}

//发送写数据
void Controller::get_data(data_exchange data)
{
    qDebug() << "2.gui发送读请求给Controller::get_data";
    emit operate(data);
}

//接收设备数据并返回给主程序处理
void Controller::handleResults(QString dev_name, QTcpSocket *tcp, QByteArray data)
{
    emit data_come(dev_name, tcp, data);
    //qDebug() << "Controller::handleResults:" << dev_name << tcp << data;
}
