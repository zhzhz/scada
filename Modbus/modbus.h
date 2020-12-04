#ifndef MODBUS_H
#define MODBUS_H

#include "modbus_global.h"
#include "qmap.h"

/*
class MODBUSSHARED_EXPORT Modbus
{

public:
    Modbus();
    int add(int a,int b);
};
*/

//int add(int a, int b);
typedef struct data_exchange
{
    int read_write;
    QMap<QString, QString> name;
    QMap<QString, QString> device;
    QMap<QString, int> dev_id;
    QMap<QString, int> variable;
    QByteArray write_data;
}data_exchange;

extern "C"
{
    QByteArray MODBUSSHARED_EXPORT gen_code(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    QByteArray  MODBUSSHARED_EXPORT output_filter(QByteArray data);
    QByteArray MODBUSSHARED_EXPORT input_data_exchange(data_exchange *data_ex);//int  MODBUSSHARED_EXPORT min_after(int a, int b);
}

#endif // MODBUS_H
