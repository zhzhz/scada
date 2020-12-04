#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qtimer.h"
//#include "modbus.h"
//#include "modbus_global.h"
#include "QLibrary"
#include "custom_data.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //读取json配置文件，得到系统配置信息
    configFile.read_config_file("../tests/a.txt");

    //用device结构初始化驱动结构
    dev_driver.get_Device(configFile.getDevice("device"));
    //QMap<int, void *> device = configFile.getDevice("device");

    connect(&qt_tcp, SIGNAL(data_come(QTcpSocket *)), this, SLOT(handle_data(QTcpSocket *)));//收到gui发来的消息

    connect(&dev_driver, SIGNAL(data_rev(QByteArray &)), this, SLOT(data_come(QByteArray &)));//收到plc驱动发来的消息

//    sys_ctl = new Sys_ctl(&dev_driver, this);
//    sys_ctl->setConfigureFile(&configFile);




    //this->setCentralWidget(gui);

    //sys_ctl->start();//开始采集数据



    //connect(client, SIGNAL(data_come(QByteArray &)), this, SLOT(handle_data(QByteArray &)));

    /*
    QByteArray data;
    data.resize(2);
    data[0] = 1;
    data[1] = 2;
    client->write_data(data);
    */
    //发送modbus命令，接收modbus数据，并打印出来，后续把这项工作放到dll中实现
    //modbus();
    m_timer = new QTimer;
    m_timer->setSingleShot(false);
      //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(2000);
      //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ImageTimerTimeout()));

    /*
    Modbus modbus;
    int a = 3, b = 4;
    qDebug() << modbus.add(a,b);
    */

}

void MainWindow::ImageTimerTimeout()
{
    //modbus();
    //调用dll函数实现功能
    //test();//调用dll的发送函数，本质没有变，还是com知道modbus协议，只不过是封装了一些细节
    //test1();
}

void MainWindow::test1()
{
    //依次发送要读取的leds,而不是modbus协议的细节，让modbus.dll去实现发送细节
    QMap<int, void *> leds = configFile.getDevice("led");
    QMap<int, void *> keys = configFile.getDevice("key");
    static int i = 0;
    QByteArray data;
    data[0] = 0;

    if ((i++) % 2 == 0)
    {
        qDebug() << "dev_driver leds";
        dev_driver.write_read_data(leds[2], "led");
    }
    else
    {
        qDebug() << "dev_driver keys";
        //dev_driver.write_read_data(keys[2], "key");
        dev_driver.write_write_data(keys[2],"key",data);
    }

     /*
    for (int i = 0; i < leds.count(); i++)
    {
        dev_driver.write_data(leds[i]);
    }

    QMap<int, void *> keys = configFile.getDevice("key");
    for (int i = 0; i < keys.count(); i++)
    {
        dev_driver.write_data(keys[i]);
    }
    */
}
//读取com net客户端发来的数据,调用driver函数，发送数据给plc，接收plc的数据
void MainWindow::handle_data(QTcpSocket *serverSocket)
{
    QByteArray data = serverSocket->readAll();
    //qDebug() << "读取发来的数据，反序列化，调用函数";
    QDataStream in(&data, QIODevice::ReadOnly);//从网络中读取的data中读到数据

    int id;
    Custom_data custom_data_rev;
    in >> id >> custom_data_rev;//将数据留出到custom_data_rev，反序列化
    //if (custom_data_rev.l.read_write == 1)
    qDebug() << "custom" << id << custom_data_rev.l.read_write << custom_data_rev.l.name["name"] << custom_data_rev.l.device["device"] <<
                custom_data_rev.l.dev_id["dev_id"] << custom_data_rev.l.variable["variable"] << custom_data_rev.l.write_data;
    if (id == 0)//0则调用如下函数
    {
        //dev_driver.write_read_data(&custom_data_rev.l, "led");
        dev_driver.write_data(&custom_data_rev.l);
    }

}

//plc驱动返回的数据，把数据返回给gui net
void MainWindow::data_come(QByteArray &data)
{
    int id = 1;//call data_come函数

    QByteArray data_send;
    QDataStream * stream = new QDataStream(&data_send, QIODevice::WriteOnly);
    (*stream) << id << data;//将custom_data序列化写入QByteArray
    qt_tcp.serverSocket->write(data_send);
    delete stream;
}


MainWindow::~MainWindow()
{
    delete ui;
}
