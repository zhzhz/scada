#include "worker.h"
#include "qdatastream.h"

Worker::Worker(QObject* parent)
{
    connect(&dev_driver, SIGNAL(data_rev(QByteArray &, int)), this, SLOT(data_come(QByteArray &, int)));//收到plc驱动发来的消息
}

Worker::~Worker()
{
    //qDebug() << "Worker::~Worker()1";
//    for (int i = 0; i < 1000000; i++)
//        ;
    //qDebug() << "Worker::~Worker()2";

}

//发送查询指令，接收返回数据
//计时，如果超时没有返回数据，则返回出错，即id = 2;
void Worker::doWork(data_exchange l, QTcpSocket *tcp)
{
    //建立设备连接，因为在出错处理data_come_error中删除了连接。
    //如果建立过，则直接返回建立好的连接，避免再次连接。
    dev_driver.get_Device(dev_driver.dev_name);

    device_save = l.device;
    dev_driver.write_data(&l);//调用dev_driver发送数据给设备。
    tcp_save = tcp;


    m_timer = new QTimer;
    qDebug() << "Worker::doWork" << m_timer;
    m_timer->setSingleShot(true);
    //并设置定时器时间：1秒超时
    m_timer->start(1000);
    //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(data_come_error()));
}

//直接得到驱动的信息，不需要采集设备信息
void Worker::doWork(QString dev_name, QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map,
                    QStringList readList, QTcpSocket *tcp)
{
    qDebug() << "Worker::doWork "<< dev_name ;
    QVector<data_exchange> data = dev_driver.get_read_vec(dev_name, device_map, readList);
//    for (int i = 0; i < data.count(); i++)
//    {
//        qDebug()<< "-------------------------------" << data.at(i).name_variable << data.at(i).name_variable_old;
//    }
    emit read_vec(dev_name, data, tcp);
}

//接收到设备返回的数据，返回数据给主线程
//正常返回数据，关闭定时器，防止定时器返回数据
//id == 1 ,正常数据； id==2 ，出错数据
void Worker::data_come(QByteArray &data, int id)
{
    qDebug() << "Worker::data_come1";
    m_timer->disconnect();
    delete m_timer;
    qDebug() << "Worker::data_come2";
    //int id = 1;//call data_come函数
//    if (device_save == "tcp508neth")
//        qDebug() << "3.数据由plc返回";
    QByteArray data_send;
    QDataStream * stream = new QDataStream(&data_send, QIODevice::WriteOnly);
    (*stream) << id << data;//将custom_data序列化写入QByteArray
    //qt_tcp.clientSockets->write(data_send);
    emit resultReady(dev_driver.data_save.device, tcp_save, data_send);//返回数据及相关变量
    delete stream;
    qDebug() << "Worker::data_come3";
}

//出错数据返回在这
//1.关闭连接，防止连接再发数据过来
//2.返回数据id==2;
void Worker::data_come_error()
{
    QByteArray data;
    int id = 2;
    //关闭连接（保证有这个连接才删除）
    qDebug() << "Worker::data_come_error()1" << tcp_save << device_save << sender() << QThread::currentThread();

    dev_driver.data_.clear();

    QMap<QString, dev_info>::Iterator it=dev_driver.devinfo.begin();
    while(it!=dev_driver.devinfo.end())
    {
        if (it.key() == device_save)
        {
            qDebug() << "Worker::data_come_error()2";
            delete it.value().dev;
            delete it.value().client;
            qDebug() << "Worker::data_come_error()3";
            //这个地方执行下一句，程序会报错！！！！！！！！
            //qDebug() << "1.数据超时，删除连接" << (it.value().client);
            break;
            //delete
        }
        it++;
    }
    dev_driver.devinfo.remove(device_save);
    qDebug() << "Worker::data_come_error()4";

    //qDebug() << "2.返回数据超时，返回id==2，数据为空";
    data_come(data, id);
}


void Worker::init(QString device_name)
{
//    qDebug() << "1.初始化设备";
    dev_driver.get_Device(device_name);
}
