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

    //delete_tcp_later_flag = false;
    //delete_tcp = 0;

    ui->setupUi(this);

    //读取json配置文件，得到系统配置信息
    configFile.read_config_file("../tests/a.txt");

    connect(&qt_tcp, SIGNAL(data_come(QTcpSocket *)), this, SLOT(handle_gui(QTcpSocket *)));//收到gui发来的消息
    connect(&qt_tcp, SIGNAL(host_closed(QTcpSocket *)), this, SLOT(host_closed(QTcpSocket *)));//收到gui网络断开的消息

    //CSingleton* p1 = CSingleton::GetInstance();
    //p1->get_dev_info("modbus");

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
    //qDebug() << "定时器事件";
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

void MainWindow::handle_gui(QTcpSocket *guiSocket)
{
    qDebug() << "handle_gui1" << guiSocket << dev2thread.size();
    QByteArray data = guiSocket->peek(10000);
    //qDebug() << "读取发来的数据，反序列化，调用函数";
    QDataStream in(&data, QIODevice::ReadOnly);//从网络中读取的data中读到数据

    int id;
    Custom_data custom_data_rev;
    in >> id >> custom_data_rev;//将数据留出到custom_data_rev，反序列化

//    qDebug() << "custom" << guiSocket << id << custom_data_rev.l.read_write << custom_data_rev.l.name << custom_data_rev.l.device <<
//                custom_data_rev.l.dev_id << custom_data_rev.l.variable << custom_data_rev.l.write_data;
    //只有没挂起的数据才能读取出来。


    if (id == 0)//0则调用如下函数
    {
        QString dev_name = custom_data_rev.l.device;
        //qDebug() << "dev_name" << dev_name;
        //判断gui数据是否挂起，如果不挂起，则发送采集线程采集数据，并置位当前设备busy标志位，如果挂起则挂起。
        //挂起的标准是，当前设备标志位busy = 1，则挂起。
        //gui都在主线程，采集线程多少根据采集设备来确定，一个设备，对应唯一一个采集线程
        if (dev2thread[dev_name] == 0)
        {
            //Controller *p = new Controller(configFile.getDevice("device"));
            Controller *p = new Controller(dev_name);
            dev2thread[dev_name] = p;

            connect(p, SIGNAL(data_come(QString , QTcpSocket *, QByteArray )),
                              this, SLOT(data_handle(QString , QTcpSocket *, QByteArray )));
        }

        //判断当前数据是执行还是挂起

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
            dev2thread[dev_name]->get_data(custom_data_rev.l, guiSocket);
        }
        else
        {
            //挂起当前gui请求
            dev_suspend[dev_name]->tcp_clients << guiSocket;
        }

    }
    qDebug() << "handle_gui2" << guiSocket;

}


void MainWindow::data_handle(QString dev_name, QTcpSocket *tcp, QByteArray data)
{
qDebug() << "MainWindow::data_handle1" << dev_name << tcp << data;

//    if (tcp_delete_later.contains(tcp) == true)
//    {
//        qDebug() << "tcp延迟删除，不调用write函数";
//        tcp->disconnect();
//        tcp->disconnectFromHost();
//        tcp->deleteLater();
//    }
//    else
//    {
//        qDebug() << "tcp调用write函数";
//        tcp->write(data);//返回数据给gui
//    }

    if (tcp_delete_later.contains(tcp))
    {
        Q_ASSERT(tcp_delete_later[tcp] == true);
        if (tcp_delete_later[tcp] == true)
        {
            qDebug() << "tcp延迟删除，不调用write函数";
            //tcp->write(data);tcp连接已经关闭，不需要调用write函数
            tcp->disconnect();
            tcp->disconnectFromHost();
            tcp->deleteLater();
        }
        tcp_delete_later.remove(tcp);
        qDebug() << "tcp_delete_later" << tcp_delete_later.size();

    }
    else
    {
         tcp->write(data);//tcp连接没有关闭，正常发送数据
    }


    dev_suspend[dev_name]->busy_client = 0;
    dev_suspend[dev_name]->dev_name = "";

    //如果当前设备有挂起的则解挂运行，如果没有挂起的设置is_suspend==false
    if (dev_suspend[dev_name]->tcp_clients.size() != 0)
    {

        dev_suspend[dev_name]->is_suspend = false;//一定要记住解挂flag，不然刚解挂的又会被挂回去
        //取出最前面运行（最先挂入的gui）
        //qDebug() << "解挂" << dev_suspend[dev_name]->tcp_clients.size() << dev_name << dev_suspend[dev_name]->tcp_clients.at(0);
        //qDebug() << "解挂" << tcp;
        handle_gui(dev_suspend[dev_name]->tcp_clients.at(0));
        dev_suspend[dev_name]->tcp_clients.remove(0);
    }
    else
    {
        dev_suspend[dev_name]->is_suspend = false;
    }
    //qDebug() << "MainWindow::data_handle2" << dev_name << tcp << data;
}

//处理gui网络断开的资源回收问题
//注意当前gui连接关闭后还有可能原来被挂起的解挂运行造成程序错误
void MainWindow::host_closed(QTcpSocket *tcp)
{
    qDebug() << "gui网络断开1" << tcp;
     Q_ASSERT(tcp != nullptr);
    tcp_delete_later[tcp] = false;

    //tcp->deleteLater();


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
                qDebug() << tcp << "解除挂载1";
                itr.value()->tcp_clients.remove(itr.value()->tcp_clients.indexOf(tcp));
                if (itr.value()->tcp_clients.count() == 0)
                {
                    //不能直接置为false！！！1,如果删除的gui正好挂在还没返回的busy运行的gui之后
                    //当旧的gui请求还没返回,新的gui请求会被接收，从而造成两次运行被设计为只能单独运行的代码，导致出错
                    //itr.value()->is_suspend = false;
                    qDebug() << tcp << "解除挂载2";

                }
            }
        }
        if (tcp == itr.value()->busy_client)
        {
            qDebug() << "当前tcp正在等待返回，会调用write函数";
            //delete_tcp << tcp;
            //delete_tcp_later_flag  true;
            tcp_delete_later[tcp] = true;
        }

      }

    if (tcp_delete_later[tcp] == false)//没有地方会用到tcp了，直接删除
    {
        qDebug() << "没有地方会用到tcp了，直接删除";
        tcp->disconnect();
        tcp->disconnectFromHost();
        tcp->deleteLater();
        tcp_delete_later.remove(tcp);
        qDebug() << "tcp_delete_later" << tcp_delete_later.size();
    }

    //这地方不能delete，因为如果tcp正在返回时，会调用MainWindow::data_handle中的write函数，导致出错
    //所以这地方设置一个删除标志位，write时如果有这个删除标志位则不执行write函数，而是执行deleteLater函数
    //如果当前不在执行这个tcp，则这个tcp不会被执行。可以直接删除


        //tcp->disconnect();
        //tcp->disconnectFromHost();
        //tcp->deleteLater();

    //qDebug() << "gui网络断开2" << tcp;
}



MainWindow::~MainWindow()
{
    delete ui;
}
