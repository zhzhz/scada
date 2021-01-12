#include "dev_driver.h"
#include "configfile.h"
#include "qdebug.h"
#include "dev_factor.h"
#include "qdatastream.h"
//QDataStream& operator>>(QDataStream& src, QVector<data_exchange>& data)
//{
//    /*
//     错误:C2679: 二进制“>>”: 没有找到接受“quint32”类型的右操作数的运算
//    解决方法：因为没有添加头文件<QDataStream>
//    */

//    src >> data.read_write >> data.device >> data.dev_id >> data.name_variable >> data.write_data;

//    return src;
//}

//QDataStream& operator<<(QDataStream& out, const QVector<data_exchange>& data)
//{
//    //out << data.m_nType << data.m_strName;
//    //out << data.ID;
//    for (int i = 0; i < data.count(); i++)
//    {
//        out << data.at(i).read_write << data.at(i).device << data.at(i).dev_id  << data.at(i).name_variable << data.at(i).write_data;
//    }

//    return out;
//}


Dev_driver::Dev_driver(QObject *parent) : QObject(parent)
{
}

//在这释放init时申请的资源
Dev_driver::~Dev_driver()
{
    qDebug() << "Dev_driver::~Dev_driver()1";
    QMap<QString, dev_info>::Iterator it=devinfo.begin();
    while(it!=devinfo.end())
    {
        //qDebug()<<it.key()<<"\t"<<it.value();
//        if (it.key() == dev_name)
//        {
//            qDebug() << "Dev_driver name exit";
//            return;
//        }
//        it++;
        delete it.value().dev;
        delete it.value().client;
        it++;
    }
    qDebug() << "Dev_driver::~Dev_driver()2";

}

//从内存结构得到设备信息，并初始化驱动
void Dev_driver::get_Device(QString dev_name)
{
//    this->dev_table = dev_table;
////得到driver，初始化驱动.dll
//    for (int i = 0; i < dev_table.count(); i++) {
//        device *dev = (device *)dev_table[i];
//        //qDebug() << dev->name["name"];
//        init_dev(dev->name);
//    }
    this->dev_name = dev_name;
    init_dev(dev_name);
}

//void Dev_driver::test()
//{
//    qDebug()<<QStringLiteral("test") << fun(1,1,0,8,NULL);
//}

//如果没有初始化过，初始化。如果被删除了，也初始化
void Dev_driver::init_dev(QString dev_name)
{
    dev_info devinfo1 = {0, 0};

    QMap<QString, dev_info>::Iterator it=devinfo.begin();
    while(it!=devinfo.end())
    {
        //qDebug()<<it.key()<<"\t"<<it.value();
        if (it.key() == dev_name)
        {
            //qDebug() << "1.Dev_driver init_dev exit";
            return;
        }
        it++;
    }
    qDebug() << "Dev_driver::init_dev" << dev_name;

    QLibrary *test_dll = new QLibrary(dev_name + ".dll");
    devinfo1.dev = test_dll;
    if(test_dll->load()) {//判断是否加载成功
        qDebug() << "dll load ok" << dev_name;
    }
    else {
        qDebug()<< dev_name << QStringLiteral("dll load error");//dll文件加载失败
    }

    //加载驱动对应的数据通讯器
    //多个设备用工厂模式
    devinfo1.client = dev_factor::product(dev_name);
    connect(devinfo1.client, SIGNAL(data_come(QByteArray &)), this, SLOT(handle_dev_data(QByteArray &)));

    devinfo.insert(dev_name, devinfo1);
    //qDebug() << devinfo.count();
}


//void Dev_driver::write_write_data(void *data, QString data_type,QByteArray data_write)
//{
//    data_exchange write_data_;
//    write_data_.read_write = 1;
//    write_data_.write_data = data_write;
//    if (data_type == "led")
//    {
//        led *led_data = (led *)data;
//        write_data_.name = led_data->name;
//        write_data_.device = led_data->device;
//        write_data_.dev_id = led_data->dev_id;
//        write_data_.variable = led_data->variable;
//    }
//    else if (data_type == "key") {
//        key *key_data = (key *)data;
//        write_data_.name = key_data->name;
//        write_data_.device = key_data->device;
//        write_data_.dev_id = key_data->dev_id;
//        write_data_.variable = key_data->variable;
//    }
//    write_data(&write_data_);
//}

//将读入的数据转换为data_exchange通讯协议格式
//void Dev_driver::write_read_data(void *data, QString data_type)
//{
//    data_exchange read_data;
//    read_data.read_write = 0;
//    //qDebug() << "Dev_driver::write_read_data";
//    //led ll;

//    if (data_type == "led")
//    {
//        led *led_data = (led *)data;
//        read_data.name = led_data->name;
//        read_data.device = led_data->device;
//        read_data.dev_id = led_data->dev_id;
//        read_data.variable = led_data->variable;
//    }
//    else if (data_type == "key") {
//        key *key_data = (key *)data;
//        read_data.name = key_data->name;
//        read_data.device = key_data->device;
//        read_data.dev_id = key_data->dev_id;
//        read_data.variable = key_data->variable;
//    }
//    write_data(&read_data);
//}

//系统要发送的数据
//发送的data要符合data_exchange协议
void Dev_driver::write_data(void *data)
{
    //qDebug() << ((data_exchange *)data);
    //得到通讯的dll，调用dll函数，得到dll发送指令，发送数据
    data_ex = (data_exchange *)data;

    data_save.device = data_ex->device;//保存这个为了收到数据时找到对应的驱动指针
    //data_save.name = data_ex->name;//保存这个为了发和收之间的对应
    data_save.name_variable = data_ex->name_variable;
    data_save.read_write = data_ex->read_write;//保存这个为了接收到数据后处理读和写

    //根据驱动名得到驱动
    QString dev_name = data_ex->device;
    dev_info dev_inf = devinfo[dev_name];
    //qDebug() << dev_name << dev_inf.dev;

    //调用dll驱动
    input_data_exchange data_exchange = (input_data_exchange)dev_inf.dev->resolve("input_data_exchange");
    if (data_exchange) {
        //qDebug() << "data_exchange ok" << data_save.device;
        //qDebug() << "variable:" << data_ex->variable;
        QByteArray data = data_exchange(data_ex);
        //发送数据,得到对应的client
        //qDebug() << "plc_data" << (int)data[6] << (int)data[7] << data.size();
        dev_inf.client->write_data(data);
    }
    else {
        qDebug() << "data_exchange error";
    }
}

//只支持一个设备out了，改用新方法
//void Dev_driver::setClient(Client *client)
//{
//    this->client = client;
//    connect(client, SIGNAL(data_come(QByteArray &)), this, SLOT(handle_data(QByteArray &)));
//}

//收到设备数据，过滤一下，发送出去
//bug修复，对于串口，需要多次等待调用这个函数，将数据拼凑起来
void Dev_driver::handle_dev_data(QByteArray &data)
{
    //static QByteArray data_;
    QString dev_name = data_save.device;//modbus
    if (dev_name != "tcp508nserial")
    {
        //qDebug() << "Qt_tcp_client::socket_Read_Data 3";
        //qDebug() << "Dev_driver:" << data_save.device << data.size();
        //根据驱动名得到驱动

        dev_info dev_inf = devinfo[dev_name];
        output_filter output_fil = (output_filter)dev_inf.dev->resolve("output_filter");
        //qDebug() << output_fil(data);
        //告诉上级，数据接收ok了
        QByteArray data_fil = output_fil(data);
        emit data_rev(data_fil, 1);
        //qDebug() <<"过滤前"<<dev_name <<data;
        //qDebug() <<"过滤后"<<dev_name <<data_fil;
    }
    else
    {

        dev_info dev_inf = devinfo[dev_name];
        get_expect_len f = (get_expect_len)dev_inf.dev->resolve("get_expect_len");

        data_ = data_ + data;

        if (data_.size() == f())
        {
            output_filter output_fil = (output_filter)dev_inf.dev->resolve("output_filter");
            QByteArray data_fil = output_fil(data_);
            emit data_rev(data_fil, 1);
            data_.clear();
        }
    }

}

QVector<data_exchange> Dev_driver::get_read_vec(QString dev_name, QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map,
                                    QStringList readList)
{
    dev_info dev_inf = devinfo[dev_name];
    gen_read_vec f = (gen_read_vec)dev_inf.dev->resolve("gen_read_vec");
    QVector<data_exchange> d;
    if (f) {
        QVector<data_exchange> data = f(device_map, readList);
        //得到数据，返回回去
        //emit get_vec()
        //dev_inf.client->write_data(data);
        //qDebug()<< "-------------------------------"<< data;

        return data;
    }
    else {
        qDebug() << "gen_read_vec error";
    }
    return d;
}


