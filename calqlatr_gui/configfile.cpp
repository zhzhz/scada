#include "configfile.h"
#include "qdebug.h"

ConfigFile::ConfigFile(QObject *parent) : QObject(parent)
{

}

QMap<int, QMap<QString, QVariant> > ConfigFile::getDevice(QString str)
{
    QMap<int, QMap<QString, QVariant> > p;
    if (device_map.contains(str))
        return device_map[str];
    else
        return p;
}

void ConfigFile::setConfig(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map)
{
    this->device_map = device_map;
}

QMap<QString, QVariant> ConfigFile::get_gui_info_byname(QString name)
{
    QMap<QString, QVariant> p;

    qDebug() << "button_name " << name;
    //qDebug() << "" << name;
    QMap<QString, QMap<int, QMap<QString, QVariant>>>::iterator iter = device_map.begin();
    while (iter != device_map.end())
    {
        //qDebug() << "Iterator " << iter.key() << ":" << iter.value(); // 迭代器

        QMap<int, QMap<QString, QVariant> > p = iter.value();
        for (int j = 0; j < p.count(); j++)
        {
            if (p[j]["name"].toString() == name)
            {
                qDebug() << "找到" << name;
                return p[j];
            }
        }
        iter++;
    }
    //QMap<int, QMap<QString, QVariant> > p = getDevice();



    return p;
}
