#ifndef DEV_DRIVER_H
#define DEV_DRIVER_H

#include <QObject>
#include "qmap.h"
#include "qvector.h"
#include "QLibrary"
#include "client.h"
#include "qt_tcp_client.h"

typedef struct
{
    //QString name;
    //bool init_done;
    QLibrary *dev;
    Client *client;
}dev_info;

typedef struct data_exchange
{
    int read_write;
    QMap<QString, QString> name;
    //QMap<QString, QString> device;
    QString device;
    QMap<QString, int> dev_id;
    QMap<QString, int> variable;
    QByteArray write_data;
}data_exchange;



class Dev_driver : public QObject
{
    Q_OBJECT
public:
    explicit Dev_driver(QObject *parent = nullptr);

    void get_Device(QMap<int, void*> dev_table);

    //void setClient(Client *client);

    QMap<QString, dev_info> devinfo;//设备总信息

    void write_write_data(void *data, QString data_type, QByteArray data_write);
    void write_read_data(void *data, QString data_type);
    void write_data(void *);

    data_exchange data_save;
    //void test();
signals:
    void data_rev(QByteArray &);

public slots:
private:
    QMap<int, void*> dev_table;
    void init_dev(QString dev_name);

    typedef QByteArray  (*gen_code)(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    typedef QByteArray (*input_data_exchange)(data_exchange *);
    typedef QByteArray (*output_filter)(QByteArray data);
    data_exchange *data_ex;


    gen_code fun;
private slots:
    void handle_data(QByteArray &data);
};

#endif // DEV_DRIVER_H
