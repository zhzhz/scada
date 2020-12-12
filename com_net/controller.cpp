#include "controller.h"

Controller::Controller(QMap<int, void *> device, QObject *parent) : QObject(parent)
{
    Worker *worker = new Worker;
    //worker->dev_driver.get_Device(device);
    //调用moveToThread将该任务交给workThread
    worker->moveToThread(&workerThread);
    //operate信号发射后启动线程工作
    //connect(this, SIGNAL(operate(const int)), worker, SLOT(doWork(int)));
    //该线程结束时销毁
    //connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    //线程结束后发送信号，对结果进行处理
    //connect(worker, SIGNAL(resultReady(int)), this, SLOT(handleResults(int)));

    //发射信号，开始执行
//    qDebug()<<"emit the signal to execute!---------------------------------";
//    qDebug()<<"     current thread ID:"<<QThread::currentThreadId()<<'\n';
    //emit operate(0);

    /////////////////////////////////////////////////
    //数据发送给采集线程
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, SIGNAL(operate(data_exchange, QTcpSocket *)), worker, SLOT(doWork(data_exchange, QTcpSocket *)));

    //采集线程返回数据
    connect(worker, SIGNAL(resultReady(QString , QTcpSocket *, QByteArray )), this,
            SLOT(handleResults(QString , QTcpSocket *, QByteArray )));


    //启动线程
    workerThread.start();


    //初始化worker线程
    connect(this, SIGNAL(init(QMap<int, void *>)), worker, SLOT(init(QMap<int, void *>)));

    emit init(device);



}
//析构函数中调用quit()函数结束线程
Controller::~Controller()
{
    //qDebug() << "Controller::~Controller()1";
    workerThread.quit();
    //qDebug() << "Controller::~Controller()2";
    workerThread.wait();
    //qDebug() << "Controller::~Controller()3";
    //delete  worker;

}

//发送采集数据
void Controller::get_data(data_exchange data, QTcpSocket *tcp)
{
    //qDebug() << "2.gui发送读请求给Controller::get_data";
    emit operate(data, tcp);
}

//接收设备数据并返回给主程序处理
void Controller::handleResults(QString dev_name, QTcpSocket *tcp, QByteArray data)
{
    emit data_come(dev_name, tcp, data);
    //qDebug() << "Controller::handleResults:" << dev_name << tcp << data;
}
