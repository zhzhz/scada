#include "dev_driver.h"
#include "configfile.h"
#include "qdebug.h"


Dev_driver::Dev_driver(QObject *parent) : QObject(parent)
{
}

//从内存结构得到设备信息，并初始化驱动
void Dev_driver::get_Device(QMap<int, void*> dev_table)
{
    this->dev_table = dev_table;
//得到driver，初始化驱动.dll
    for (int i = 0; i < dev_table.count(); i++) {
        device *dev = (device *)dev_table[i];
        //qDebug() << dev->name["name"];
        init_dev(dev->name["name"]);
    }
}

//void Dev_driver::test()
//{
//    qDebug()<<QStringLiteral("test") << fun(1,1,0,8,NULL);
//}

//如果init过，退出，如果没init过，初始化
void Dev_driver::init_dev(QString dev_name)
{
    dev_info devinfo1 = {0};
//    for(int i = 0; i < devinfo.count(); i++)
//    {
//        if (devinfo.at(i).name == dev_name)
//        {
//            //初始化过，直接退出
////            qDebug() <<  "exit";
//            return;
//        }
//    }
    QMap<QString, dev_info>::Iterator it=devinfo.begin();
    while(it!=devinfo.end())
    {
        //qDebug()<<it.key()<<"\t"<<it.value();
        if (it.key() == dev_name)
        {
            qDebug() << "Dev_driver name exit";
            return;
        }
        it++;
    }

    //没初始化过，初始化
    //devinfo1.name = dev_name;

     //qDebug() << "devinfo.insert";


    //QLibrary test_dll("modbus.dll");//加载dll
    QLibrary *test_dll = new QLibrary(dev_name + ".dll");
    devinfo1.dev = test_dll;
    if(test_dll->load()) {//判断是否加载成功
        qDebug() << "dll load ok" << dev_name;
//        gen_code fun1 = (gen_code)test_dll.resolve("gen_code");//获取dll的函数
//        fun = fun1;
//        if (fun1) {//判断是否获取到此函数

//           // client->write_data(code);
//            qDebug()<<QStringLiteral("ok") << fun1(1,1,0,8,NULL);
//        }
//        else {
//            //函数解析失败
//            qDebug()<<QStringLiteral("dll function load error");
//        }


    }
    else {
        qDebug()<< dev_name << QStringLiteral("dll load error");//dll文件加载失败
    }

    //加载驱动对应的数据通讯器
    //modbus为网络通讯
    //三菱也为网络通讯,假设端口
    if (dev_name == "modbus")
    {
        //qDebug() << "modbus";
        Qt_tcp_client *client = new Qt_tcp_client(this);
        client->set_param(QString("192.168.2.101"), 9999);
        client->connect_line();
        devinfo1.client = client;
        connect(client, SIGNAL(data_come(QByteArray &)), this, SLOT(handle_data(QByteArray &)));
    }
    else if (dev_name == "Mitsubishi") {
        Qt_tcp_client *client = new Qt_tcp_client(this);
        client->set_param(QString("192.168.2.101"), 8888);
        client->connect_line();
        devinfo1.client = client;
        connect(client, SIGNAL(data_come(QByteArray &)), this, SLOT(handle_data(QByteArray &)));
    }

    devinfo.insert(dev_name, devinfo1);
    //qDebug() << devinfo.count();
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

    //根据驱动名得到驱动
    QString dev_name = data_ex->device;
    dev_info dev_inf = devinfo[dev_name];
    //qDebug() << dev_name << dev_inf.dev;

    //调用dll驱动
    input_data_exchange data_exchange = (input_data_exchange)dev_inf.dev->resolve("input_data_exchange");
    if (data_exchange) {
       // qDebug() << "data_exchange ok";
        //qDebug() << "variable:" << data_ex->variable["variable"];
        QByteArray data = data_exchange(data_ex);
        //发送数据,得到对应的client
        //qDebug() << "plc_data" << data;
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

//收到数据打印出来
void Dev_driver::handle_data(QByteArray &data)
{
    //qDebug() << "Qt_tcp_client::socket_Read_Data 3";
    //qDebug() << "Dev_driver:" << data;
    //根据驱动名得到驱动
    QString dev_name = data_save.device;//modbus
    dev_info dev_inf = devinfo[dev_name];
    output_filter output_fil = (output_filter)dev_inf.dev->resolve("output_filter");
    //qDebug() << output_fil(data);
    //告诉上级，数据接收ok了
    QByteArray data_fil = output_fil(data);
    emit data_rev(data_fil);
}
