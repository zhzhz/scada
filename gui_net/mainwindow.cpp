#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qtimer.h"
//#include "modbus.h"
//#include "modbus_global.h"
#include "QLibrary"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //读取json配置文件，得到系统配置信息
    configFile.read_config_file("../tests/a.txt");

    //用device结构初始化驱动结构
    //dev_driver.get_Device(configFile.getDevice("device"));
    //QMap<int, void *> device = configFile.getDevice("device");

    gui = new Gui(&configFile, this);

    sys_ctl = new Sys_ctl(&dev_driver, this);
    sys_ctl->setConfigureFile(&configFile);
    sys_ctl->setGui(gui);



    this->setCentralWidget(gui);

    sys_ctl->start();//开始采集数据



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


MainWindow::~MainWindow()
{
    delete ui;
}
