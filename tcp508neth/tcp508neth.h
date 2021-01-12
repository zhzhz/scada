#ifndef TCP508NETH_H
#define TCP508NETH_H

#include "tcp508neth_global.h"
#include "qmap.h"
#include "qvariant.h"
#include "qvector.h"

//class TCP508NETH_EXPORT Tcp508neth
//{
//public:
//    Tcp508neth();
//};

typedef struct data_exchange
{
    int read_write;
    QString device;
    int dev_id;
    QMap<QString, int> name_variable;
    QMap<QString, int> name_variable_old;
    QByteArray write_data;
}data_exchange;

typedef struct data_range
{
    data_exchange data;
    int range_min;
    int range_max;
}data_range;

extern "C"
{
    QByteArray TCP508NETH_EXPORT gen_code(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    QByteArray  TCP508NETH_EXPORT output_filter(QByteArray data);
    QByteArray TCP508NETH_EXPORT input_data_exchange(data_exchange *data_ex);//int  MODBUSSHARED_EXPORT min_after(int a, int b);
    QVector<data_exchange> TCP508NETH_EXPORT gen_read_vec(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map, QStringList read_list);
}

#endif // TCP508NETH_H
