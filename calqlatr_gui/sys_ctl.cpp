#include "sys_ctl.h"
#include "qdebug.h"
#include "QQuickItem"
#include "writestrategyfactor.h"

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
    is_write_instruct = false;
    read_none = true;
    state = 0;
    count = 0;
    is_write_instruct = false;
    writeAfterRead = false;
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
    qDebug() << "Sys_ctl::start" << is_write_instruct;
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
        //        if (read_write_flag == false)//发送读指令
        //        {
        //            thread->get_data(read_data.at(i));
        //        }
        //        else {
        //            //有写指令，发送写指令

        //            write_flag = true;
        //            qDebug() << "write plc data;----------------------------------";
        //            data_save = data_saves.at(0);//取出全局最开始的那条指令
        //            thread->get_data(data_save);
        //            //dev_driver->write_data(&data_save);
        //        }

        if (is_write_instruct == true)
        {
            //写指令，可能有多条指令会包含读指令
            //1.单写DO指令（只有写）
            //2.多写AO指令（先读后写）
            //从buf中取出指令，如果是读执行读，如果是写执行写(读写是相同的get_data函数)
            write_flag = true;
            data_save = data_saves.at(0);
            thread->get_data(data_save);
        }
        else
        {
            //空闲的读指令
            thread->get_data(read_data.at(i));
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

    if (is_write_instruct == false)
    {
        if (i < read_data.count() - 1)
        {
            i++;
        }
        else
        {
            i = 0;
        }

    }




    /////////////////////////////////////////////////////////////////////////////

    WriteStrategyFactor factor;
    //qDebug() << is_write_instruct << " " << data_save.name_variable;
    //write_flag代表有没有执行write部分
    //如果不用write_flag而用is_write_instruct，会造成is_write_instruct == 1时调用data_come出错
    if (write_flag == false)
    {

        writeStrategy.genReadStrategy(factor.genReadStrategy(write_flag, this), data, data_save,
                                      0);
    }




    if (write_flag == true)//如果data_saves为空，则恢复
    {
        if (data_save.read_write == 0)
        {
            //先读后写
            qDebug() << "方式1" << data_save.name_variable;
            writeStrategy.genReadStrategy(factor.genReadStrategy(write_flag, this), data, data_save,
                                                      WriteStrategys.at(0));
        }
        else
        {
                //单纯的写
            qDebug() << "方式2" << data_save.name_variable;
                writeStrategy.genReadStrategy(factor.genReadStrategy(write_flag, this), data, data_save,
                                                          WriteStrategys.at(0));
                WriteStrategys.remove(0);

        }



        data_saves.remove(0);//从全局去除最开始的那条指令


        if (data_saves.size() == 0)
        {
            //没有要发送的写指令了

            is_write_instruct = false;
        }
        else
        {
            //还有要发送的写指令，read_write_flag保持写状态
            is_write_instruct = true;
        }

        write_flag = false;
    }


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
//void Sys_ctl::button_clicked(QString button_name)
//{
//    //QPushButton *pushbutton=qobject_cast<QPushButton *>(sender());
//    //找到指针对应的configure参数。
//    //先找到指针对应的name，然后对应到configure
//    qDebug() << "-----------------------------------------------------------" << button_name;
//    QByteArray data;data[0] = 0;

//    //gui_info *info = gui->get_gui_info_byptr(pushbutton);//利用info的name部分
//    //根据name找到包含name的led项或者key项
//    //void *gui_info = configFile->get_gui_info_byname(info->name);
//    QMap<QString, QVariant> gui_info = configFile->get_gui_info_byname(button_name);
//    //bug在这个地方调用了qmap的[]方法，所以生成了一个包含led的空数组


//    write_data(gui_info, "key", data);

//}

void Sys_ctl::button_clicked(QString button_name)
{

//    qDebug() << "-----------------------------------------------------------" << button_name;
//    QByteArray data;data[0] = 0;


//    QMap<QString, QVariant> gui_info = configFile->get_gui_info_byname(button_name);



//    write_data(gui_info, "key", data);


//鼠标按下，现在可能有两种按钮，一种是单写DO，一种是多写AO，其中AO需要先读后写，两条指令都写入buffer中
    QMap<QString, QVariant> gui_info = configFile->get_gui_info_byname(button_name);


    is_write_instruct = true;//下一条指令为写指令
    //根据当前variable变量生成写策略函数
    WriteStrategyFactor factor;
    genWriteStrategy = factor.genWriteStrategy(gui_info["variable"].toInt(), this);
    WriteStrategys << genWriteStrategy;//把写策略存入向量中
    writeStrategy.genWriteStrategy(genWriteStrategy, gui_info);

    if (read_none == 1)
    {
        start();
    }

}

//把写数据缓存起来，在start中判断发送
void Sys_ctl::write_read_data(QMap<QString, QVariant>data, int read_write,QByteArray data_write, int index)
{
    data_exchange data_save;
    //is_write_instruct = true;//判断读和写的总开关

    data_save.read_write = read_write;
    data_save.write_data = data_write;


    data_save.device = data["device"].toString();
    data_save.dev_id = data["dev_id"].toInt();

    data_save.name_variable[data["name"].toString()] = data["variable"].toInt();
    //让name_variable和name_variable_old一样就可以了
    data_save.name_variable_old = data_save.name_variable;

    if (index == 0)
    {
        data_saves << data_save;//将当前发送指令保存到全局最尾端
    }
    else
    {
        data_saves.insert(index, data_save);//将当前发送指令保存到index位置上
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
    is_write_instruct = false;
    read_none = true;
    data_saves.clear();
    state = 0;
    count = 0;
    is_write_instruct = false;
    writeAfterRead = false;

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


