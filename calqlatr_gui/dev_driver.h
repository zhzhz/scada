#ifndef DEV_DRIVER_H
#define DEV_DRIVER_H

#include <QObject>
#include "qmap.h"
#include "qvector.h"
#include "QLibrary"
#include "client.h"
#include "qt_tcp_client.h"
//#include "error_dialog.h"
#include "qtimer.h"

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
    QString device;
    int dev_id;
    QMap<QString, int> name_variable;
    QMap<QString, int> name_variable_old;
    QByteArray write_data;


    friend QDataStream& operator>>(QDataStream&, data_exchange&);
    friend QDataStream& operator<<(QDataStream&, data_exchange&);
}data_exchange;
Q_DECLARE_METATYPE(data_exchange)



class Dev_driver : public QObject
{
    Q_OBJECT
public:
    explicit Dev_driver(QObject *parent = nullptr);
    ~Dev_driver();

    bool connect_net(void);
    //void setClient(Client *client);

    QMap<QString, dev_info> devinfo;//设备总信息

    void write_write_data(void *data, QString data_type, QByteArray data_write);
    void write_read_data(QMap<QString, QVariant> data, QString data_type);
    void write_data(void *);
    void write_data(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map, QStringList readList);

    void get_Device(QString dev_name);

    data_exchange data_save;
    bool wait_for_first_connect;
    Client *client;

    QString dev_name;
    //void test();
    bool connect_ok;
signals:
    //void data_rev(QByteArray &, int id);
    void data_rev(QByteArray &, data_exchange);
    void data_rev_error(QByteArray &, data_exchange);
    void host_closed_signal(QTcpSocket *);
    void networkerror_signal(QTcpSocket *);
    void read_map(QVector<data_exchange>);

public slots:
    //void TimerTimeout(void);
private:
    QMap<int, void*> dev_table;

    QTimer *m_timer;

    typedef QByteArray  (*gen_code)(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    typedef QByteArray (*input_data_exchange)(data_exchange *);
    typedef QByteArray (*output_filter)(QByteArray data);
    data_exchange *data_ex;


    gen_code fun;
private slots:
    void handle_data(QByteArray &data);
    void host_closed(QTcpSocket *tcp);
    void networkerror(QTcpSocket *tcp);
};

#endif // DEV_DRIVER_H
