#include "gui.h"
//#include "qradiobutton.h"
//#include "qpushbutton.h"
//#include "QHBoxLayout"
#include "qdebug.h"

//根据解析出来的内存结构，创建界面内存结构，创建界面
Gui::Gui(ConfigFile *configure, QObject *parent) : QObject(parent)
{
    this->configure = configure;
    /*
     * pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(20, 70, 75, 23));
        radioButton = new QRadioButton(centralWidget);
     *
     * QPushButton *pushButton;
        QRadioButton *radioButton;
     *
     * */
    //创建led和按钮，创建内存结构为name+指针
    QMap<int, void *> leds = configure->getDevice("led");
    QMap<int, void *> keys = configure->getDevice("key");

    //界面在qml中生成了，此处不需要生成界面了，但需要
//    QHBoxLayout* pLayout = new QHBoxLayout();



//    for (int i = 0; i < leds.count(); i++)
//    {
//        QRadioButton *radioButton = new QRadioButton(this);
//        gui_info *info = new gui_info();
//        //led.insert(i, radioButton);
//        led *l = (led *)leds[i];
//        info->name = l->name;
//        info->type = "led";
//        info->ptr = radioButton;
//        leds.insert(i, info);
//        pLayout->addWidget(radioButton);
//    }

    for (int i = 0; i < keys.count(); i++)
    {
        //QPushButton *pushButton = new QPushButton(this);
        gui_info *info = new gui_info();

        key *l = (key *)keys[i];
        info->name = l->name;
        info->type = "key";
        info->ptr = pushButton;
        keys.insert(i, info);

        //pLayout->addWidget(pushButton);
    }

    device_map.insert("led", leds);
    //qDebug() << "led" << leds;
    device_map.insert("key", keys);

    //this->setLayout(pLayout);
}

QMap<int, void *> Gui::getDevice(QString str)
{
    return device_map[str];
}

//根据name找到对应的gui信息
//遍历led和key
gui_info *Gui::get_gui_info_byname(QString name)
{
    QMap<int, void *> leds = getDevice("led");
    for (int i = 0; i < leds.count(); i++)
    {
         gui_info *info = (gui_info *)leds[i];
         //qDebug() << info->name << dev_driver->data_save.name["name"];
         if (info->name == name)
         {
            return info;
         }
    }

    QMap<int, void *> keys = getDevice("key");
    for (int i = 0; i < keys.count(); i++)
    {
         gui_info *info = (gui_info *)keys[i];
         //qDebug() << info->name << dev_driver->data_save.name["name"];
         if (info->name == name)
         {
            return info;
         }
    }
    return 0;
}

//根据ptr，返回包含ptr的gui_info，主要是需要里面的name信息
gui_info *Gui::get_gui_info_byptr(void *ptr)
{
    QMap<int, void *> leds = getDevice("led");
    for (int i = 0; i < leds.count(); i++)
    {
         gui_info *info = (gui_info *)leds[i];
         //qDebug() << info->name << dev_driver->data_save.name["name"];
         if (info->ptr == ptr)
         {
            return info;
         }
    }

    QMap<int, void *> keys = getDevice("key");
    for (int i = 0; i < keys.count(); i++)
    {
         gui_info *info = (gui_info *)keys[i];
         //qDebug() << info->name << dev_driver->data_save.name["name"];
         if (info->ptr == ptr)
         {
            return info;
         }
    }
    return 0;
}
