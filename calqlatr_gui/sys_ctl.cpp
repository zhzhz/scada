#include "sys_ctl.h"
#include "qdebug.h"
#include "QQuickItem"

Sys_ctl::Sys_ctl(QQuickItem *qmlItem, QObject *parent) : QObject(parent)
{
    //dlg = 0;
    //dlg_neterror = 0;
    this->qmlItem = qmlItem;
    thread = new Controller(this, "init");//新建唯一一个新线程，用来采集数据
    //i = j = 0;

    readList << "led" << "key";//读led和key的数值
    i = j = 0;

    write_flag = false;
    read_write_flag = false;
    read_none = true;

}

Sys_ctl::~Sys_ctl()
{
    qDebug() << "Sys_ctl::~Sys_ctl()";
    delete thread;
}

//没事依次发送读指令，有写指令发送写指令
//修改发送为线程发送方式
void Sys_ctl::start(void)
{
    qDebug() << "Sys_ctl::start" << read_write_flag;
    //QMap<int, QMap<QString, QVariant> > leds = configFile->getDevice("led");
    //QMap<int, QMap<QString, QVariant> > keys = configFile->getDevice("key");
    QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map = configFile->device_map;

    //qDebug() << "device_map" << device_map;
    QByteArray data;
    data[0] = 0;

    if (read_write_flag == false)
    {
again:
        if (i < readList.size())
        {
            //qDebug() << "readList.at(i)" << readList.at(i);
            if (device_map.contains(readList.at(i)))
            {
                //qDebug() << "device_map.contains" << readList.at(i);
                //bug ,device_map包括led，在哪？
                read_none = false;

                QMap<int, QMap<QString, QVariant>> leds = device_map[readList.at(i)];
                if (j < leds.size())
                {
                    //qDebug() << "thread->get_data()";
                    //qDebug() << "---------------------1 " << leds[j] << readList.at(i);
                    //qDebug() << "------------------------------回来3";
                    thread->get_data(leds[j], readList.at(i));
                    //j++;
                }
                else
                {
                    //i++;//如果当前读项已经读完，转移到下一个读项
                    //j = 0;
                    //i++;
                }
            }
            else
            {
                //qDebug() << "i++" << i;
                i++;//如果没有读的一项，转移到下一个读项,保证有读项时，立即执行读项
                goto again;
            }
        }
        else
        {
            //找到结尾也没有要执行的指令，如果指令不是空则重新寻找，如果为空则放弃寻找
            if (read_none == false)
            {
                i = j = 0;
                goto again;
            }
            else
            {
                i = j = 0;
            }
        }

    }
    else {
        //有写指令，发送写指令

        write_flag = true;
        qDebug() << "write plc data;----------------------------------";
        data_save = data_saves.at(0);//取出全局最开始的那条指令
        thread->get_data(data_save);
        //dev_driver->write_data(&data_save);

    }
}

//接收返回数据,根据name更新对应的系统参数led,key，并继续采集数据
//如果有错误弹出窗口，关闭
void Sys_ctl::data_come(QByteArray &data, data_exchange data_save)
{
    //qDebug() << "Sys_ctl::data_come";


    QQuickItem* item = qmlItem->findChild<QQuickItem*>("plcError");
    //qDebug() << "-----------------------------------" << data_save.name << item;
    item->setProperty("visible", false);

    //QMap<int, QMap<QString, QVariant> > leds = configFile->getDevice("led");
    //QMap<int, QMap<QString, QVariant> > keys = configFile->getDevice("key");
    QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map = configFile->device_map;

    if (read_write_flag == false)
    {
        QMap<int, QMap<QString, QVariant>> leds = device_map[readList.at(i)];
        if (j < leds.size() - 1)
        {

            j++;
            //qDebug() << "-----------------------------j" << j;
        }
        else
        {
            //i++;//如果当前读项已经读完，转移到下一个读项
            j = 0;
            if (i < readList.size() - 1)
            {

                i++;
                //qDebug() << "-----------------------------i" << i;
            }
            else
            {
                i = 0;
                //qDebug() << "-----------------------------i==0";
            }
        }


    }

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

    //qDebug() << "------------------------------回来1";

    //看看数据是不是led数据，根据led数据设置gui显示
    if (data_save.read_write == 0)
    {
        QQuickItem* item = qmlItem->findChild<QQuickItem*>(data_save.name);
        //qDebug() << "-----------------------------------" << data_save.name << item;
        item->setProperty("text", data.at(0));

    }
    else {
        QQuickItem* item = qmlItem->findChild<QQuickItem*>(data_save.name);
        //qDebug() << "-----------------------------------" << data_save.name << item;
        item->setProperty("fontSize", 20);
    }

    //qDebug() << "------------------------------回来2";
    start();//继续采集数据
}

//com没有从设备处取得数据，而是从超时处返回，此时data为空
//1.显示通讯出错对话框
//2.发送失败的通讯指令，直到通讯成功才发下一条指令
void Sys_ctl::data_come_error(QByteArray &data, data_exchange data_save)
{
//qDebug() << "超时data_come_error";
//    if (dlg == 0)
//        dlg = new error_dialog();
    //显示通讯出错对话框，调用qml函数实现

    QQuickItem* item = qmlItem->findChild<QQuickItem*>("plcError");
    //qDebug() << "-----------------------------------" << data_save.name << item;
    item->setProperty("visible", true);

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
    QMap<QString, QVariant> gui_info = configFile->get_gui_info_byname(button_name);
    //bug在这个地方调用了qmap的[]方法，所以生成了一个包含led的空数组


    write_data(gui_info, "key", data);

}

//把写数据缓存起来，在start中判断发送
//修改这个函数，使支持多个写指令，否则后一条写指令会覆盖前一条指令
void Sys_ctl::write_data(QMap<QString, QVariant>data, QString data_type,QByteArray data_write)
{
    data_exchange data_save;
    read_write_flag = true;//判断读和写的总开关

    data_save.read_write = 1;
    data_save.write_data = data_write;

    data_save.name = data["name"].toString();
    data_save.device = data["device"].toString();
    data_save.dev_id = data["dev_id"].toInt();
    data_save.variable = data["variable"].toInt();

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

//从连接错误中恢复，注意要删除一些写指令
void Sys_ctl::connect_resume()
{
    i = j = 0;
    write_flag = false;
    read_write_flag = false;
    read_none = true;
    data_saves.clear();

    start();
}

void Sys_ctl::com_error()
{
    QQuickItem* item = qmlItem->findChild<QQuickItem*>("comError");
    //qDebug() << "-----------------------------------" << data_save.name << item;
    item->setProperty("visible", true);
}

void Sys_ctl::com_error_ok()
{
    QQuickItem* item = qmlItem->findChild<QQuickItem*>("comError");
    //qDebug() << "-----------------------------------" << data_save.name << item;
    item->setProperty("visible", false);
}


