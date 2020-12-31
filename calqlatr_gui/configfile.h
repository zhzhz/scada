#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>
#include "cJSON.h"
#include "qmap.h"
#include "qvariant.h"

typedef struct device
{
    QMap<QString, QString> name;//name:modbus1 name:Mitsubishi
    QMap<QString, int> id;
}device;

typedef struct led
{
    //QMap<QString, QString> name;
    //QMap<QString, QString> device;
    QString name;
    QString device;
    int dev_id;
    int variable;
    //QMap<QString, int> dev_id;
    //QMap<QString, int> variable;
}led, key;



class ConfigFile : public QObject
{
    Q_OBJECT
public:
    explicit ConfigFile(QObject *parent = nullptr);

    //QMap<QString, QMap<int, void *>> device_map;//从json得到的数据
    QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map;

    QMap<int, QMap<QString, QVariant>> getDevice(QString str);
    QMap<QString, QVariant> get_gui_info_byname(QString name);

    void setConfig(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map);
signals:

public slots:

private:
};

#endif // CONFIGFILE_H
