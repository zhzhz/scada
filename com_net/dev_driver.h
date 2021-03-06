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
    QString name;
    QString device;
    int dev_id;
    int variable;
    QByteArray write_data;
}data_exchange;
Q_DECLARE_METATYPE(data_exchange)


class Dev_driver : public QObject
{
    Q_OBJECT
public:
    explicit Dev_driver(QObject *parent = nullptr);

    QByteArray data_;

    ~Dev_driver();

    void get_Device(QString dev_name);

    //void setClient(Client *client);

    QMap<QString, dev_info> devinfo;
    QString dev_name;//设备总信息

    void write_write_data(void *data, QString data_type, QByteArray data_write);
    void write_read_data(void *data, QString data_type);
    void write_data(void *);

    data_exchange data_save;
    //void test();
signals:
    void data_rev(QByteArray &, int id);

public slots:
private:

    void init_dev(QString dev_name);

    typedef int (*get_expect_len)(void);
    typedef QByteArray  (*gen_code)(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    typedef QByteArray (*input_data_exchange)(data_exchange *);
    typedef QByteArray (*output_filter)(QByteArray data);
    data_exchange *data_ex;


    gen_code fun;
private slots:
    void handle_dev_data(QByteArray &data);
};

#endif // DEV_DRIVER_H
