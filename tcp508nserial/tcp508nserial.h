#ifndef TCP508NSERIAL_H
#define TCP508NSERIAL_H

#include "tcp508nserial_global.h"
#include "qvariant.h"
#include "qmap.h"

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
    //int expect_len;
    int        TCP508NSERIAL_EXPORT get_expect_len(void);
    QByteArray TCP508NSERIAL_EXPORT gen_code(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    QByteArray TCP508NSERIAL_EXPORT output_filter(QByteArray data);
    QByteArray TCP508NSERIAL_EXPORT input_data_exchange(data_exchange *data_ex);//int  MODBUSSHARED_EXPORT min_after(int a, int b);
    QVector<data_exchange> TCP508NSERIAL_EXPORT gen_read_vec(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map, QStringList read_list);
}

#endif // TCP508NSERIAL_H
