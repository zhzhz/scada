#include "worker.h"
#include "qdatastream.h"

//work处理dev_driver网络连接出错的逻辑
Worker::Worker(QObject* parent)
{
    networkerror_f = false;
    host_closed_f = false;

    //connect(&dev_driver, SIGNAL(data_rev(QByteArray &, int)), this, SLOT(data_come(QByteArray &, int)));//收到plc驱动发来的消息
    connect(&dev_driver, SIGNAL(data_rev(QByteArray &, data_exchange)), this, SLOT(data_come(QByteArray &,data_exchange)));
    connect(&dev_driver, SIGNAL(data_rev_error(QByteArray &, data_exchange)), this, SLOT(data_come_error(QByteArray &, data_exchange)));

    connect(&dev_driver, SIGNAL(read_map(QVector<data_exchange>)), this, SLOT(read_map(QVector<data_exchange>)));


    //连接了下面这两项，才可以使用slot
    connect(&dev_driver, SIGNAL(host_closed_signal(QTcpSocket *)), this, SLOT(host_closed(QTcpSocket *)));
    connect(&dev_driver, SIGNAL(networkerror_signal(QTcpSocket *)), this, SLOT(networkerror(QTcpSocket *)));


}

Worker::~Worker()
{

}

//发送查询指令，接收返回数据
//计时，如果超时没有返回数据，则返回出错，即id = 2;
void Worker::doWork(QMap<QString, QVariant> data, QString type)
{
    //建立设备连接，因为在出错处理data_come_error中删除了连接。
    //如果建立过，则直接返回建立好的连接，避免再次连接。
    //dev_driver.get_Device(dev_driver.dev_name);
    if (dev_driver.connect_ok)
    {
        //dev_driver.write_read_data(data, type);//本来是sysctl直接调用这个函数的
    }


}

void Worker::doWork(data_exchange data_save)
{
    //dev_driver.get_Device(dev_driver.dev_name);
    if (dev_driver.connect_ok)
    {
        dev_driver.write_data(&data_save);
    }
}

void Worker::doWork(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map, QStringList readList)
{
    if (dev_driver.connect_ok)
    {
        //dev_driver.write_data(&data_save);
        dev_driver.write_data(device_map, readList);
    }
}


void Worker::data_come(QByteArray &data, data_exchange data_save)
{
    emit data_come_signal(data, data_save);
}

void Worker::data_come_error(QByteArray &data, data_exchange data_save)
{
    emit data_come_error_signal(data, data_save);
}

void Worker::read_map(QVector<data_exchange> data)
{
    emit read_map_signal(data);
}




void Worker::init(QString device_name)
{
//    qDebug() << "1.初始化设备";
    dev_driver.dev_name = device_name;
    //dev_driver.get_Device(device_name);
    if (dev_driver.connect_net() == false)
    {
        //if (networkerror_f == false)
        //{
            host_closed(0);
        //}
    }
    else
    {
        //通知sys_ctl可以发送数据
        emit connect_ok();
    }
}

//gui与com连接中断
//1.弹出error报警框，关闭tcp连接
//2.不断重连服务器，连接上后复位发送（相当于重连新的tcp连接）
void Worker::host_closed(QTcpSocket *tcp)
{
    host_closed_f = true;
    qDebug() << "Worker::host_closed";
    //dev_driver.client->deleteLater();
    dev_driver.connect_ok = false;
    delete dev_driver.client;
    dev_driver.client = 0;

//    if (dlg_neterror == 0)
//            dlg_neterror = new error_dialog();
    emit com_error();

    m_timer = new QTimer;
    m_timer->setSingleShot(false);
      //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(500);
      //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeout()));
}

void Worker::networkerror(QTcpSocket *tcp)
{
qDebug() << "Worker::networkerror";
    networkerror_f = true;
    if (host_closed_f == false)//说明最开始连接时网络断开
    {
        host_closed(tcp);
    }
}

//每500ms执行一次，如果成功则关闭定时器，防止再进来
void Worker::TimerTimeout(void)
{
    qDebug() << "Worker::TimerTimeout不断重连server";
    if (dev_driver.connect_net() == true)
    {
        //qDebug() << "网络返回true1";
        //delete dlg_neterror;//关闭错误窗口
        //dlg_neterror = 0;
        emit com_error_ok();

        delete m_timer;//关闭定时器


        //通知sys_ctl通信恢复，重新开始采集数据
        emit connect_resume();

    }
    else
    {
        //连接无效,关闭连接
        delete dev_driver.client;
        dev_driver.client = 0;
    }
}
