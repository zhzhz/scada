#ifndef TCP508NETH_H
#define TCP508NETH_H

#include "tcp508neth_global.h"
#include "qmap.h"
//class TCP508NETH_EXPORT Tcp508neth
//{
//public:
//    Tcp508neth();
//};

typedef struct data_exchange
{
    int read_write;
    QString name;
    QString device;
    int dev_id;
    int variable;
    QByteArray write_data;
}data_exchange;

extern "C"
{
    QByteArray TCP508NETH_EXPORT gen_code(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    QByteArray  TCP508NETH_EXPORT output_filter(QByteArray data);
    QByteArray TCP508NETH_EXPORT input_data_exchange(data_exchange *data_ex);//int  MODBUSSHARED_EXPORT min_after(int a, int b);
}

#endif // TCP508NETH_H
