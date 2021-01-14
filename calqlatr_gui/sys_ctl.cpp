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

    //readList << "led" << "key";//读led和key的数值
    readList << "led";
    i = j = 0;

    write_flag = false;
    read_write_flag = false;
    read_none = true;
    state = 0;
    count = 0;
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
    QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map = configFile->device_map;

    QByteArray data;
    data[0] = 0;

    if (state == 0)//状态0，无条件读取read_data
    {
        //1.发送device_map和readList给com，com调用tcp508neth的gen_read_vec返回read_vec
        thread->get_data(device_map, readList);
        device_count = device_map["device"].count();
        state++;
    }
    else if (state == 1)
    {
        if (read_write_flag == false)//发送读指令
        {
            thread->get_data(read_data.at(i));
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

}

//接收返回数据,根据name更新对应的系统参数led,key，并继续采集数据
//如果有错误弹出窗口，关闭
void Sys_ctl::data_come(QByteArray &data, data_exchange data_save)
{
    qDebug() << "Sys_ctl::data_come";


    QQuickItem* item = qmlItem->findChild<QQuickItem*>("plcError");
    //qDebug() << "-----------------------------------" << data_save.name << item;
    item->setProperty("visible", false);

    //QMap<int, QMap<QString, QVariant> > leds = configFile->getDevice("led");
    //QMap<int, QMap<QString, QVariant> > keys = configFile->getDevice("key");
    QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map = configFile->device_map;

    if (read_write_flag == false)
    {
//        QMap<int, QMap<QString, QVariant>> leds = device_map[readList.at(i)];
//        if (j < leds.size() - 1)
//        {

//            j++;
//            //qDebug() << "-----------------------------j" << j;
//        }
//        else
//        {
//            //i++;//如果当前读项已经读完，转移到下一个读项
//            j = 0;
//            if (i < readList.size() - 1)
//            {

//                i++;
//                //qDebug() << "-----------------------------i" << i;
//            }
//            else
//            {
//                i = 0;
//                //qDebug() << "-----------------------------i==0";
//            }
//        }
        if (i < read_data.count() - 1)
        {
            i++;
        }
        else
        {
            i = 0;
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
        int *countp = (int *)data.data();
        int bits = *countp;
        data.remove(0, 4);

        //循环读出name_variable数据
        QMap<QString, int>::iterator iter = data_save.name_variable.begin();
        while (iter != data_save.name_variable.end())
        {
            //qDebug() << "Iterator " << iter.key() << ":" << iter.value(); // 迭代器
            QQuickItem* item = qmlItem->findChild<QQuickItem*>(iter.key());
            int i = iter.value();
            item->setProperty("text", get_read_data(data, bits, i));//如果bit是1，如此操作
            iter++;
        }

        //qDebug() << "-----------------------------------" << data_save.name << item;

        //qDebug() << "返回数据了："<< data_save.name_variable;
    }
    else {
//        QQuickItem* item = qmlItem->findChild<QQuickItem*>(data_save.name);
//        //qDebug() << "-----------------------------------" << data_save.name << item;
//        item->setProperty("fontSize", 20);
    }

    //qDebug() << "------------------------------回来2";
    start();//继续采集数据
}

//返回bits=1 = 16的数据
int Sys_ctl::get_read_data(QByteArray data, int bits, int index)
{
    if (bits == 1)
    {
        qDebug() << data << "----------" << bits << "----------"<< index;
        int i = bits << (index % 8);
        return (i & data.at(index/8)) >> (index % 8);
    }
    else if (bits == 16)
    {
        unsigned short *p = (unsigned short *)data.data();
        return p[index];
    }
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

//从com的驱动中读取map
void Sys_ctl::read_map(QVector<data_exchange> data)
{
    qDebug() << "Sys_ctl::read_map ";
    for (int i = 0; i < data.size(); i++)
    {
        qDebug() << "返回的数据data[i].name_variable " << i << " " << data[i].name_variable;
    }

    count++;
    this->read_data += data;

    if (data.count() != 0)
    {
        read_none = false;//不加这一句，写会主动调用start()，造成错误
    }

    if (count == device_count)
    {
       start();//开始采集读数据
    }

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

    //data_save.name = data["name"].toString();
    data_save.device = data["device"].toString();
    data_save.dev_id = data["dev_id"].toInt();
    //data_save.variable = data["variable"].toInt();
    data_save.name_variable[data["name"].toString()] = data["variable"].toInt();
    //让name_variable和name_variable_old一样就可以了
    data_save.name_variable_old = data_save.name_variable;

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
    state = 0;
    count = 0;

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


