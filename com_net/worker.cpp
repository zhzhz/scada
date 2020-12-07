#include "worker.h"
#include "qdatastream.h"

Worker::Worker(QObject* parent)
{
    connect(&dev_driver, SIGNAL(data_rev(QByteArray &)), this, SLOT(data_come(QByteArray &)));//收到plc驱动发来的消息
}

//发送查询指令，接收返回数据
void Worker::doWork(data_exchange l, QTcpSocket *tcp)
{
    //qDebug() << "custom" << l.read_write << l.name << l.device <<
     //               l.dev_id << l.variable << l.write_data << tcp;

    //qDebug() << "worker线程id：" << QThread::currentThreadId();
    dev_driver.write_data(&l);//调用dev_driver发送数据给设备。
    tcp_save = tcp;
}

//接收到设备返回的数据，返回数据给主线程
void Worker::data_come(QByteArray &data)
{
    int id = 1;//call data_come函数

    QByteArray data_send;
    QDataStream * stream = new QDataStream(&data_send, QIODevice::WriteOnly);
    (*stream) << id << data;//将custom_data序列化写入QByteArray
    //qt_tcp.clientSockets->write(data_send);
    emit resultReady(dev_driver.data_save.device, tcp_save, data_send);//返回数据及相关变量
    delete stream;
}

void Worker::init(QMap<int, void *> device)
{
    dev_driver.get_Device(device);
}
