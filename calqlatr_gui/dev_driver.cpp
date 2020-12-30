#include "dev_driver.h"
#include "configfile.h"
#include "qdebug.h"
#include "custom_data.h"

//如果连接出错则显示报错，并不断重连直到连接上，关闭报错窗口。
Dev_driver::Dev_driver(QObject *parent) : QObject(parent)
{
    client = 0;
    connect_ok = false;
}

Dev_driver::~Dev_driver()
{
    qDebug() << "Dev_driver::~Dev_driver()" << client;
    if (client != 0)
    {
        delete client;
        client = 0;
    }
}

bool Dev_driver::connect_net(void)
{
    Qt_tcp_client *client = new Qt_tcp_client();//连接8686 com端口，用来发送数据
    client->set_param(QString("192.168.2.101"), 8686);
    this->client = client;
    connect(client, SIGNAL(data_come(QByteArray &)), this, SLOT(handle_data(QByteArray &)));//com发数据来了，处理之
    connect(client, SIGNAL(host_closed(QTcpSocket *)), this, SLOT(host_closed(QTcpSocket *)));//网络连接错误
    connect(client, SIGNAL(networkerror(QTcpSocket *)), this, SLOT(networkerror(QTcpSocket *)));
    connect_ok = client->connect_line();
    return connect_ok;
}


void Dev_driver::write_write_data(void *data, QString data_type,QByteArray data_write)
{
    data_exchange write_data_;
    write_data_.read_write = 1;
    write_data_.write_data = data_write;
    if (data_type == "led")
    {
        led *led_data = (led *)data;
        write_data_.name = led_data->name;
        write_data_.device = led_data->device;
        write_data_.dev_id = led_data->dev_id;
        write_data_.variable = led_data->variable;
    }
    else if (data_type == "key") {
        key *key_data = (key *)data;
        write_data_.name = key_data->name;
        write_data_.device = key_data->device;
        write_data_.dev_id = key_data->dev_id;
        write_data_.variable = key_data->variable;
    }
    write_data(&write_data_);
}

//将读入的数据转换为data_exchange通讯协议格式
void Dev_driver::write_read_data(void *data, QString data_type)
{
    data_exchange read_data;
    read_data.read_write = 0;
    //qDebug() << "Dev_driver::write_read_data";
    //led ll;

    if (data_type == "led")
    {
        led *led_data = (led *)data;
        read_data.name = led_data->name;
        read_data.device = led_data->device;
        read_data.dev_id = led_data->dev_id;
        read_data.variable = led_data->variable;
    }
    else if (data_type == "key") {
        key *key_data = (key *)data;
        read_data.name = key_data->name;
        read_data.device = key_data->device;
        read_data.dev_id = key_data->dev_id;
        read_data.variable = key_data->variable;
    }
    write_data(&read_data);
}

//系统要发送的数据
//发送的data要符合data_exchange协议
void Dev_driver::write_data(void *data)
{
    //qDebug() << ((data_exchange *)data);
    //得到通讯的dll，调用dll函数，得到dll发送指令，发送数据
    data_ex = (data_exchange *)data;

    data_save.device = data_ex->device;//保存这个为了收到数据时找到对应的驱动指针
    data_save.name = data_ex->name;//保存这个为了发和收之间的对应
    data_save.read_write = data_ex->read_write;//保存这个为了接收到数据后处理读和写

    //网络发送数据结构，接收方解码后执行函数调用
    int ID = 0;
    Custom_data custom_data;
    //custom_data.ID = 0;//调用0号函数
    custom_data.l.read_write = data_ex->read_write;
    custom_data.l.name = data_ex->name;
    custom_data.l.device = data_ex->device;
    custom_data.l.dev_id = data_ex->dev_id;
    custom_data.l.variable = data_ex->variable;
    custom_data.l.write_data = data_ex->write_data;

    QByteArray data_send;
    QDataStream * stream = new QDataStream(&data_send, QIODevice::WriteOnly);
    (*stream) << ID << custom_data;//将custom_data序列化写入QByteArray

    client->write_data(data_send);//调用网络发送数据
    //qDebug() << "序列化网络写出";
    delete stream;
}

//com发数据来了，收数据，emit信号
//如果收到的id是2，则代表出错
void Dev_driver::handle_data(QByteArray &data)
{

    QDataStream in(&data, QIODevice::ReadOnly);//从网络中读取的data中读到数据

    int id;
    QByteArray data_fil;
    in >> id >> data_fil;//将数据留出到custom_data_rev，反序列化
    if (id == 1)
    {
        emit data_rev(data_fil, data_save);
    }
    else if (id == 2)
    {
        emit data_rev_error(data_fil, data_save);
    }
}


void Dev_driver::host_closed(QTcpSocket *tcp)
{
    qDebug() << "Dev_driver::host_closed";
    emit host_closed_signal(tcp);
}

void Dev_driver::networkerror(QTcpSocket *tcp)
{
    qDebug() << "Dev_driver::networkerror";
    emit networkerror_signal(tcp);
}

//用来建立连接
void Dev_driver::get_Device(QString dev_name)
{
    this->dev_name = dev_name;
    if (client == 0)
    {
        connect_net();
    }
}

