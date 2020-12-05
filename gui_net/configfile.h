#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QObject>
#include "cJSON.h"
#include "qmap.h"

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
    void read_config_file(char *config_file);
    QMap<QString, QMap<int, void *>> device_map;//从json得到的数据

    QMap<int, void *> getDevice(QString str);
    void *get_gui_info_byname(QString name);
signals:

public slots:

private:
    void dofile2(char *filename);
    void doit2(char *text);
    void printJson(cJSON * root);
};

#endif // CONFIGFILE_H
