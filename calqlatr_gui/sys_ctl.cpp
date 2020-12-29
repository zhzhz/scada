#include "sys_ctl.h"
#include "qdebug.h"

Sys_ctl::Sys_ctl(Dev_driver *dev_driver, QObject *parent) : QObject(parent)
{
    //dlg = 0;
    //dlg_neterror = 0;
    m_timer = 0;
    i = j = 0;

    networkerror_f = false;
    host_closed_f = false;

    write_flag = false;

    this->dev_driver = dev_driver;
    connect(dev_driver, SIGNAL(data_rev(QByteArray &)), this, SLOT(data_come(QByteArray &)));
    connect(dev_driver, SIGNAL(data_rev_error(QByteArray &)), this, SLOT(data_come_error(QByteArray &)));

    //连接了下面这两项，才可以使用slot
    connect(dev_driver, SIGNAL(host_closed_signal(QTcpSocket *)), this, SLOT(host_closed(QTcpSocket *)));
    connect(dev_driver, SIGNAL(networkerror_signal(QTcpSocket *)), this, SLOT(networkerror(QTcpSocket *)));

    //在ip=服务器ip上连接失败会触发ConnectionRefusedError
    //在别的ip的机子上不会触发任何error，此时需要手动触发error处理函数
    if (dev_driver->connect_net() == false)
    {
        if (networkerror_f == false)
        {
            host_closed(0);
        }
    }

    read_write_flag = false;
    read_none = false;

}

//没事依次发送读指令，有写指令发送写指令
void Sys_ctl::start(void)
{
    QMap<int, void *> leds = configFile->getDevice("led");
    QMap<int, void *> keys = configFile->getDevice("key");

    QByteArray data;
    data[0] = 0;

    if (read_write_flag == false)
    {
        if (leds.count() == 0 && keys.count() == 0)
        {
            read_none = 1;
        }
        if (i < leds.count())
            dev_driver->write_read_data(leds[i], "led");
        else if (j < keys.count())
        {
            dev_driver->write_read_data(keys[j], "key");//led为空时，发送这条指令。
        }



    }
    else {
        //有写指令，发送写指令

        write_flag = true;
        qDebug() << "write plc data;----------------------------------";
        data_save = data_saves.at(0);//取出全局最开始的那条指令
        //data_saves.remove(0);//从全局去除最开始的那条指令
        dev_driver->write_data(&data_save);
    }
}

//接收返回数据,根据name更新对应的系统参数led,key，并继续采集数据
//如果有错误弹出窗口，关闭
void Sys_ctl::data_come(QByteArray &data)
{

    if (write_flag == true)//如果data_saves为空，则恢复
    {
        data_saves.remove(0);//从全局去除最开始的那条指令

        if (data_saves.size() == 0)
        {
            //没有要发送的写指令了
            read_write_flag = false;
        }
        else
        {
            //还有要发送的写指令，read_write_flag保持写状态
            read_write_flag = true;
        }

        write_flag = false;
    }

//    if (dlg)
//    {
//        delete dlg;
//        dlg = 0;
//    }
    QMap<int, void *> leds = configFile->getDevice("led");
    QMap<int, void *> keys = configFile->getDevice("key");

    if (read_write_flag == false)
    {
        if (i < leds.count())
        {
            i++;
        }
        else if (j < keys.count() - 1)
        {
            j++;
        }
        else
        {
            i = j = 0;
        }
    }

    //qDebug() << "Sys_ctl::data_come" << dev_driver->data_save.name["name"] << data;
    //qDebug() << "Sys_ctl::data_come";
    //看看数据是不是led数据，根据led数据设置gui显示
    if (dev_driver->data_save.read_write == 0)
    {
        //gui_info *info = gui->get_gui_info_byname(dev_driver->data_save.name);

//        if (info->type == "led")
//        {
//            QRadioButton *r = (QRadioButton *)info->ptr;
//            r->setText(QString::number(data.at(0)));
//        }
//        else if(info->type == "key")
//        {
//            QPushButton *r = (QPushButton *)info->ptr;
//            r->setText(QString::number(data.at(0)));
//        }
    }
    else {
//        gui_info *info = gui->get_gui_info_byname(dev_driver->data_save.name);
//        if (info->type == "led")
//        {
//            //QRadioButton *r = (QRadioButton *)info->ptr;
//            //r->setText(QString::number(data.at(0)));
//        }
//        else if(info->type == "key")
//        {
//            if (data.at(0) == 1)//测试返回值
//            {
//                QPushButton *r = (QPushButton *)info->ptr;
//                QFont font;
//                font.setFamily("微软雅黑");
//                r->setFont(font);
//            }
//        }
    }


    start();//继续采集数据
}

//com没有从设备处取得数据，而是从超时处返回，此时data为空
//1.显示通讯出错对话框
//2.发送失败的通讯指令，直到通讯成功才发下一条指令
void Sys_ctl::data_come_error(QByteArray &data)
{
//qDebug() << "超时data_come_error";
//    if (dlg == 0)
//        dlg = new error_dialog();

    if (write_flag == true)
    {
        write_flag = false;
        //read_write_flag保持不变，如果是读则还未为读，如果为写则还为写
    }

    start();//继续采集数据

}

void Sys_ctl::setConfigureFile(ConfigFile *configFile)
{
    this->configFile = configFile;
}

//void Sys_ctl::setGui(Gui *gui)
//{
//    this->gui = gui;
//    //将gui里面的button的clicked事件绑定到sys_ctl处理函数中
//    QMap<int, void *> keys = gui->getDevice("key");
//    for (int i = 0; i < keys.count(); i++)
//    {
//        gui_info *info = (gui_info *)keys[i];
//        QPushButton *r = (QPushButton *)info->ptr;
//        connect(r, SIGNAL(clicked()), this, SLOT(button_clicked()));
//    }
//}

//目前只有key会到这，因此可以简化
void Sys_ctl::button_clicked(QString button_name)
{
    //QPushButton *pushbutton=qobject_cast<QPushButton *>(sender());
    //找到指针对应的configure参数。
    //先找到指针对应的name，然后对应到configure
    qDebug() << "-----------------------------------------------------------" << button_name;
    QByteArray data;data[0] = 0;

    //gui_info *info = gui->get_gui_info_byptr(pushbutton);//利用info的name部分
    //根据name找到包含name的led项或者key项
    //void *gui_info = configFile->get_gui_info_byname(info->name);
    void *gui_info = configFile->get_gui_info_byname(button_name);


    write_data(gui_info, "key", data);

}

//把写数据缓存起来，在start中判断发送
//修改这个函数，使支持多个写指令，否则后一条写指令会覆盖前一条指令
void Sys_ctl::write_data(void *data, QString data_type,QByteArray data_write)
{
    data_exchange data_save;
    read_write_flag = true;//判断读和写的总开关

    data_save.read_write = 1;
    data_save.write_data = data_write;

    if (data_type == "led")
    {
        led *led_data = (led *)data;
        data_save.name = led_data->name;
        data_save.device = led_data->device;
        data_save.dev_id = led_data->dev_id;
        data_save.variable = led_data->variable;
    }
    else if (data_type == "key") {
        key *key_data = (key *)data;
        data_save.name = key_data->name;
        data_save.device = key_data->device;
        data_save.dev_id = key_data->dev_id;
        data_save.variable = key_data->variable;
    }

    data_saves << data_save;//将当前发送指令保存到全局

    if (read_none == 1)
    {
        start();
    }
}

//void Sys_ctl::write_data(void *data, QString data_type,QByteArray data_write)
//{
//    //this->configFile = configFile;
//    data_save_bool = true;
//    data_save.read_write = 1;
//    data_save.write_data = data_write;

//    //led ll;

//    if (data_type == "led")
//    {
//        led *led_data = (led *)data;
//        data_save.name = led_data->name;
//        data_save.device = led_data->device;
//        data_save.dev_id = led_data->dev_id;
//        data_save.variable = led_data->variable;
//    }
//    else if (data_type == "key") {
//        key *key_data = (key *)data;
//        data_save.name = key_data->name;
//        data_save.device = key_data->device;
//        data_save.dev_id = key_data->dev_id;
//        data_save.variable = key_data->variable;
//    }

//    if (read_none == 1)
//    {
//        start();
//    }
//}

//gui与com连接中断
//1.弹出error报警框，关闭tcp连接
//2.不断重连服务器，连接上后复位发送（相当于重连新的tcp连接）
void Sys_ctl::host_closed(QTcpSocket *tcp)
{
    host_closed_f = true;
    qDebug() << "Sys_ctl::host_closed";
    dev_driver->client->deleteLater();

//    if (dlg_neterror == 0)
//            dlg_neterror = new error_dialog();

    m_timer = new QTimer;
    m_timer->setSingleShot(false);
      //启动或重启定时器, 并设置定时器时间：毫秒
    m_timer->start(500);
      //定时器触发信号槽
    connect(m_timer, SIGNAL(timeout()), this, SLOT(TimerTimeout()));
}

void Sys_ctl::networkerror(QTcpSocket *tcp)
{
qDebug() << "Sys_ctl::networkerror";
    networkerror_f = true;
    if (host_closed_f == false)//说明最开始连接时网络断开
    {
        host_closed(tcp);
    }
}

//每500ms执行一次，如果成功则关闭定时器，防止再进来
void Sys_ctl::TimerTimeout(void)
{
    if (dev_driver->connect_net() == true)
    {
        //qDebug() << "网络返回true1";
        //delete dlg_neterror;//关闭错误窗口
        //dlg_neterror = 0;

        delete m_timer;//关闭定时器

        i = j = 0;
        write_flag = false;
        read_write_flag = false;
        read_none = false;

        start();

    }
    else
    {
        //连接无效,关闭连接
        delete dev_driver->client;
    }
}
