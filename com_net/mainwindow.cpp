#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qtimer.h"
//#include "modbus.h"
//#include "modbus_global.h"
#include "QLibrary"
#include "custom_data.h"
#include <QMetaType>




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<data_exchange>();
    qRegisterMetaType<QByteArray>("QByteArray&");
    qRegisterMetaType<QMap<int, void *>>();

    ui->setupUi(this);

    //读取json配置文件，得到系统配置信息
    configFile.read_config_file("../tests/a.txt");

    connect(&qt_tcp, SIGNAL(data_come(QTcpSocket *)), this, SLOT(handle_data(QTcpSocket *)));//收到gui发来的消息
    connect(&qt_tcp, SIGNAL(host_closed(QTcpSocket *)), this, SLOT(host_closed(QTcpSocket *)));//收到gui网络断开的消息



    //发送modbus命令，接收modbus数据，并打印出来，后续把这项工作放到dll中实现
    //modbus();
    m_timer = new QTimer;
    m_timer->setSingleShot(false);
      //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(2000);
      //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(ImageTimerTimeout()));

}

void MainWindow::ImageTimerTimeout()
{

}

//void MainWindow::test1()
//{
//    //依次发送要读取的leds,而不是modbus协议的细节，让modbus.dll去实现发送细节
//    QMap<int, void *> leds = configFile.getDevice("led");
//    QMap<int, void *> keys = configFile.getDevice("key");
//    static int i = 0;
//    QByteArray data;
//    data[0] = 0;

//    if ((i++) % 2 == 0)
//    {
//        qDebug() << "dev_driver leds";
//        dev_driver.write_read_data(leds[2], "led");
//    }
//    else
//    {
//        qDebug() << "dev_driver keys";
//        //dev_driver.write_read_data(keys[2], "key");
//        dev_driver.write_write_data(keys[2],"key",data);
//    }
//}

//读取gui net客户端发来的数据,调用driver函数，发送数据给plc，接收plc的数据
//多个gui eth对象数据来了，都是这个函数处理

void MainWindow::handle_data(QTcpSocket *guiSocket)
{
    QByteArray data = guiSocket->peek(10000);
    //qDebug() << "读取发来的数据，反序列化，调用函数";
    QDataStream in(&data, QIODevice::ReadOnly);//从网络中读取的data中读到数据

    int id;
    Custom_data custom_data_rev;
    in >> id >> custom_data_rev;//将数据留出到custom_data_rev，反序列化

//    qDebug() << "custom" << guiSocket << id << custom_data_rev.l.read_write << custom_data_rev.l.name << custom_data_rev.l.device <<
//                custom_data_rev.l.dev_id << custom_data_rev.l.variable << custom_data_rev.l.write_data;
    //只有没挂起的数据才能读取出来。

    static int i = 0;
    if (id == 0)//0则调用如下函数
    {
       // dev_driver.write_data(&custom_data_rev.l);
        //判断gui数据是否挂起，如果不挂起，则发送采集线程采集数据，并置位当前设备busy标志位，如果挂起则挂起。
        //挂起的标准是，当前设备标志位busy = 1，则挂起。
        //为每个gui eth创建唯一的采集线程。

        //qDebug() << "主线程id：" << QThread::currentThreadId();
        if (tcp2thread[guiSocket] == 0)
        {
            //创建gui对应的新线程--采集线程
            Controller *p = new Controller(configFile.getDevice("device"), this);
            tcp2thread[guiSocket] = p;
            //p->worker->dev_driver.get_Device(configFile.getDevice("device"));//采集线程程序初始化//这一句会有错误，因为跨线程了
            //收到数据后处理
            connect(p, SIGNAL(data_come(QString , QTcpSocket *, QByteArray )),
                    this, SLOT(data_handle(QString , QTcpSocket *, QByteArray )));
            qDebug() << "新线程------------------------------" << tcp2thread.count() << guiSocket;
        }

        //判断当前数据是执行还是挂起
        QString dev_name = custom_data_rev.l.device;
        if (dev_suspend[dev_name] == 0)//初始化挂起相关
        {
            suspand *suspand_ = new suspand;
            suspand_->is_suspend = false;
            suspand_->dev_name = dev_name;
            dev_suspend[dev_name] = suspand_;
        }
        else
        {
            dev_suspend[dev_name]->dev_name = dev_name;
        }

        //如果is_suspend==0没有挂起，则执行；如果is_suspend==1，则挂起
        if (dev_suspend[dev_name]->is_suspend == false)
        {
            //当前设备忙
            //qDebug() << "在这设置is_suspend为true";
            dev_suspend[dev_name]->is_suspend = true;
            dev_suspend[dev_name]->busy_client = guiSocket;

            //执行当前gui请求
            //调用Controller的get_data函数来执行采集任务
            //清空缓存区数据，防止对下次造成干扰。
            guiSocket->readAll();
            tcp2thread[guiSocket]->get_data(custom_data_rev.l, guiSocket);
        }
        else
        {
            //挂起当前gui请求
            dev_suspend[dev_name]->tcp_clients << guiSocket;



            //qDebug() << "挂起" << dev_name << guiSocket;
        }


        //qDebug() << "tcp2thread[serverSocket]" << tcp2thread[serverSocket];
    }

}


void MainWindow::data_handle(QString dev_name, QTcpSocket *tcp, QByteArray data)
{
//qDebug() << "MainWindow::data_handle" << dev_name << tcp << data;
    tcp->write(data);//返回数据给gui
    dev_suspend[dev_name]->busy_client = 0;
    dev_suspend[dev_name]->dev_name = "";

    //如果当前设备有挂起的则解挂运行，如果没有挂起的设置is_suspend==false
    if (dev_suspend[dev_name]->tcp_clients.size() != 0)
    {

        dev_suspend[dev_name]->is_suspend = false;//一定要记住解挂flag，不然刚解挂的又会被挂回去
        //取出最前面运行（最先挂入的gui）
        //qDebug() << "解挂" << dev_suspend[dev_name]->tcp_clients.size() << dev_name << dev_suspend[dev_name]->tcp_clients.at(0);
        handle_data(dev_suspend[dev_name]->tcp_clients.at(0));
        dev_suspend[dev_name]->tcp_clients.remove(0);
    }
    else
    {
        dev_suspend[dev_name]->is_suspend = false;
    }
}

//处理gui网络断开的资源回收问题
//注意当前gui连接关闭后还有可能原来被挂起的解挂运行造成程序错误
void MainWindow::host_closed(QTcpSocket *tcp)
{
    //qDebug() << "网络断开" << tcp;
    Controller *p = tcp2thread[tcp];
    delete p;
    //qDebug() << "remove前" << tcp2thread.count();
    tcp2thread.remove(tcp);
    qDebug() << "remove后" << tcp2thread.count() << tcp;

    //1.如果gui tcp有挂起的，清除挂载(不管是被挂载在哪个dev_name下)
    QMapIterator<QString, suspand *> itr(dev_suspend);
    //qDebug() << "最初的1："<<dev_suspend["modbus"]->is_suspend << dev_suspend["modbus"]->tcp_clients <<
    //           dev_suspend["Mitsubishi"]->is_suspend << dev_suspend["Mitsubishi"]->tcp_clients;
    while(itr.hasNext())
    {
        itr.next();
    //qDebug()<<itr.key()<<itr.value();
        QVector<QTcpSocket *> tV = itr.value()->tcp_clients;
        QVector<QTcpSocket *>::Iterator itr_;

        for(itr_=tV.begin();itr_!=tV.end();itr_++){
            //qDebug()<<*itr;
            if (*itr_ == tcp)
            {
                //qDebug() << tcp << "解除挂载1";
                itr.value()->tcp_clients.remove(itr.value()->tcp_clients.indexOf(tcp));
                if (itr.value()->tcp_clients.count() == 0)
                {
                    itr.value()->is_suspend = false;
                    //qDebug() << tcp << "解除挂载2";
                    //看看最初的有没有解挂
                    //qDebug() << "最初的2："<<dev_suspend["modbus"]->is_suspend << dev_suspend["Mitsubishi"]->is_suspend;
                }
            }
        }

        //2.如果清除的gui是当前busy的gui,则恢复suspend标志位==0;
        if (tcp == itr.value()->busy_client)
        {
            //qDebug() << "当前正在运行" << tcp;
            itr.value()->is_suspend = false;
            //因为前面调用Controller *p = tcp2thread[tcp];
            //delete p;
            //使得不会再从Controller返回数据，无法执行data_handle函数
            //也就无法执行挂起的gui，所以需要手动调用data_handle函数
            QByteArray data;
            data.resize(6);
            data[0] = 1;
            data[1] = 2;
            data[2] = 3;
            data[3] = 4;
            data[4] = 5;
            data_handle(itr.value()->dev_name, tcp, data);

        }
    }


//    if (tcp == dev_suspend[dev_name]->busy_client)
//    {

//    }



}



MainWindow::~MainWindow()
{
    delete ui;
}
