#include "configfile.h"
#include "qdebug.h"

ConfigFile::ConfigFile(QObject *parent) : QObject(parent)
{

}

QMap<int, QMap<QString, QVariant> > ConfigFile::getDevice(QString str)
{
    return device_map[str];
}

void ConfigFile::setConfig(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map)
{
    this->device_map = device_map;
}

QMap<QString, QVariant> ConfigFile::get_gui_info_byname(QString name)
{
    QMap<int, QMap<QString, QVariant> > leds = getDevice("led");
    //根据name找到
    for (int i = 0; i < leds.count(); i++)
    {
        if (leds[i]["name"].toString() == name)
            return leds[i];
    }

    QMap<int, QMap<QString, QVariant> > keys = getDevice("key");

    for (int i = 0; i < keys.count(); i++)
    {
        if (keys[i]["name"].toString() == name)
            return keys[i];
    }
}
