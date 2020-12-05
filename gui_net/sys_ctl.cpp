#include "sys_ctl.h"
#include "qdebug.h"

Sys_ctl::Sys_ctl(Dev_driver *dev_driver, QObject *parent) : QObject(parent)
{
    this->dev_driver = dev_driver;
    connect(dev_driver, SIGNAL(data_rev(QByteArray &)), this, SLOT(data_come(QByteArray &)));
    data_save_bool = false;
    read_none = false;

}

//没事依次发送读指令，有写指令发送写指令
void Sys_ctl::start(void)
{
    QMap<int, void *> leds = configFile->getDevice("led");
    QMap<int, void *> keys = configFile->getDevice("key");
    static int i = 0,j = 0;
    QByteArray data;
    data[0] = 0;

    if (data_save_bool == false)
    {
//        if ((i++) % 2 == 0)
//        {
//            qDebug() << "dev_driver leds";
//            dev_driver->write_read_data(leds[3], "led");
//        }
//        else
//        {
//            qDebug() << "dev_driver keys";
//            //dev_driver->write_write_data(keys[2],"key",data);
//            dev_driver->write_read_data(keys[3], "key");
//        }
        //依次发送读led数据,然后依次发送读key数据
        //如果发送指令的为空，则置位read_none = 1;
        if (leds.count() == 0 && keys.count() == 0)
        {
            read_none = 1;
        }
        if (i < leds.count())
            dev_driver->write_read_data(leds[i++], "led");
        else {
            if (j < keys.count())
            {
                dev_driver->write_read_data(keys[j++], "key");//led为空时，发送这条指令。
            }
            else
            {
                //qDebug() << j << "==" << keys.count();
                //qDebug() << i << "==" << keys.count();
                i = 0;
                j = 0;
                if (i < leds.count())
                    dev_driver->write_read_data(leds[i++], "led");//led为空，发送上面两条指令后发送这条指令，出错，因为led为空
                else if (j < keys.count())
                {
                    dev_driver->write_read_data(keys[j++], "key");
                }
            }
        }


    }
    else {
        //有写指令，发送写指令
        data_save_bool = false;
        qDebug() << "write plc data;----------------------------------";
        dev_driver->write_data(&data_save);
    }
}

//接收返回数据,根据name更新对应的系统参数led,key，并继续采集数据
void Sys_ctl::data_come(QByteArray &data)
{
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
        //处理写指令返回数据，根据name找到指针，设置button指针内容=0x1
//        qDebug()  << "Sys_ctl::data_come" << dev_driver->data_save.name["name"] << data;
//        QMap<int, void *> keys = gui->getDevice("key");
//        for (int i = 0; i < keys.count(); i++)
//        {
//             gui_info *info = (gui_info *)keys[i];
//             //qDebug() << info->name << dev_driver->data_save.name["name"];
//             if (info->name == dev_driver->data_save.name["name"])
//             {

//                 if (info->type == "key")//如果类型是key，设置key显示出来
//                 {
//                    qDebug() << "type == led" << info->name;
//                    QPushButton *r = (QPushButton *)info->ptr;
//                    QFont font;
//                    font.setFamily("微软雅黑");
//                    r->setFont(font);
//                    break;
//                 }
//             }
//        }
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
