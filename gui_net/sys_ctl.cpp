#include "sys_ctl.h"
#include "qdebug.h"

Sys_ctl::Sys_ctl(Dev_driver *dev_driver, QObject *parent) : QObject(parent)
{
    dlg = 0;
    i = j = 0;

    write_flag = false;

    this->dev_driver = dev_driver;
    connect(dev_driver, SIGNAL(data_rev(QByteArray &)), this, SLOT(data_come(QByteArray &)));
    connect(dev_driver, SIGNAL(data_rev_error(QByteArray &)), this, SLOT(data_come_error(QByteArray &)));

    data_save_bool = false;
    read_none = false;

}

//没事依次发送读指令，有写指令发送写指令
void Sys_ctl::start(void)
{
    QMap<int, void *> leds = configFile->getDevice("led");
    QMap<int, void *> keys = configFile->getDevice("key");

    QByteArray data;
    data[0] = 0;

    if (data_save_bool == false)
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
        //data_save_bool = false;
        write_flag = true;
        qDebug() << "write plc data;----------------------------------";
        dev_driver->write_data(&data_save);
    }
}

//接收返回数据,根据name更新对应的系统参数led,key，并继续采集数据
//如果有错误弹出窗口，关闭
void Sys_ctl::data_come(QByteArray &data)
{
    //data_save_bool = false;
    if (write_flag == true)
    {
        write_flag = false;
        data_save_bool = false;
    }

    if (dlg)
    {
        delete dlg;
        dlg = 0;
    }
    QMap<int, void *> leds = configFile->getDevice("led");
    QMap<int, void *> keys = configFile->getDevice("key");

    if (data_save_bool == false)
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
        gui_info *info = gui->get_gui_info_byname(dev_driver->data_save.name);

        if (info->type == "led")
        {
            QRadioButton *r = (QRadioButton *)info->ptr;
            r->setText(QString::number(data.at(0)));
        }
        else if(info->type == "key")
        {
            QPushButton *r = (QPushButton *)info->ptr;
            r->setText(QString::number(data.at(0)));
        }
    }
    else {
        gui_info *info = gui->get_gui_info_byname(dev_driver->data_save.name);
        if (info->type == "led")
        {
            //QRadioButton *r = (QRadioButton *)info->ptr;
            //r->setText(QString::number(data.at(0)));
        }
        else if(info->type == "key")
        {
            if (data.at(0) == 1)//测试返回值
            {
                QPushButton *r = (QPushButton *)info->ptr;
                QFont font;
                font.setFamily("微软雅黑");
                r->setFont(font);
            }
        }
    }


    start();//继续采集数据
}

//com没有从设备处取得数据，而是从超时处返回，此时data为空
//1.显示通讯出错对话框
//2.发送失败的通讯指令，直到通讯成功才发下一条指令
void Sys_ctl::data_come_error(QByteArray &data)
{
//qDebug() << "超时data_come_error";
    if (dlg == 0)
        dlg = new error_dialog();

    if (write_flag == true)
    {
        write_flag = false;
        //data_save_bool = false;
    }

    start();//继续采集数据

}

void Sys_ctl::setConfigureFile(ConfigFile *configFile)
{
    this->configFile = configFile;
}

void Sys_ctl::setGui(Gui *gui)
{
    this->gui = gui;
    //将gui里面的button的clicked事件绑定到sys_ctl处理函数中
    QMap<int, void *> keys = gui->getDevice("key");
    for (int i = 0; i < keys.count(); i++)
    {
        gui_info *info = (gui_info *)keys[i];
        QPushButton *r = (QPushButton *)info->ptr;
        connect(r, SIGNAL(clicked()), this, SLOT(button_clicked()));
    }
}

//目前只有key会到这，因此可以简化
void Sys_ctl::button_clicked(void)
{
    QPushButton *pushbutton=qobject_cast<QPushButton *>(sender());
    //找到指针对应的configure参数。
    //先找到指针对应的name，然后对应到configure
//    QMap<int, void *> keys = gui->getDevice("key");
//    QMap<int, void *> keys_conf = configFile->getDevice("key");
//    QByteArray data;data[0] = 0;
//    for (int i = 0; i < keys.count(); i++)
//    {
//        gui_info *info = (gui_info *)keys[i];
//        //qDebug() << info->name << dev_driver->data_save.name["name"];
//        if (info->ptr == pushbutton)
//        {
//            //此时的info->name为configue对应的name
//            for (int j = 0; j < keys_conf.count(); j++)
//            {
//                key *conf = (key *)keys_conf[j];
//                if (conf->name["name"] == info->name)
//                {
//                    write_data(keys_conf[j], "key", data);
//                    //qDebug() << "key" << pushbutton;
//                    break;
//                }
//            }
//            break;
//        }
//    }
    QByteArray data;data[0] = 0;

    gui_info *info = gui->get_gui_info_byptr(pushbutton);//利用info的name部分
    //根据name找到包含name的led项或者key项
    void *gui_info = configFile->get_gui_info_byname(info->name);

    if (info->type == "led")
    {
        //
    }
    else if (info->type == "key"){
        write_data(gui_info, "key", data);
    }
}

//把写数据缓存起来，在start中判断发送
void Sys_ctl::write_data(void *data, QString data_type,QByteArray data_write)
{
    //this->configFile = configFile;
    data_save_bool = true;
    data_save.read_write = 1;
    data_save.write_data = data_write;

    //led ll;

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

    if (read_none == 1)
    {
        start();
    }
}
