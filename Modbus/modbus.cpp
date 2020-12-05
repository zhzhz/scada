#include "modbus.h"
#include "qbytearray.h"
#include "qdebug.h"

/*
Modbus modbus;

Modbus::Modbus()
{
}

int Modbus::add(int a,int b)
{
    return a+b;
}
*/

//int  min_after(int a, int b)
//{
//    //int d = modbus.add(a,b);
//    return c + d;
//}
//根据参数，生成modbus指令
QByteArray gen_code(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send)
{
    /*1.读取开关量输入状态 DI（读一区）
    发送：01 02 0000 0008 crc
    返回：01 02 01 02 crc
    */
    if (control_byte == 2)
    {
        QByteArray data;
        data.resize(8);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);
        data[6] = 0x88;
        data[7] = 0x88;

        return data;
    }

    /*
    2.设置单路继电器输出状态 DO
    发送：01 05 0000 ff00 crc
    返回：01 05 0000 ff00 crc
    */
    if (control_byte == 5)
    {
        QByteArray data;
        data.resize(8);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);
        data[6] = 0x88;
        data[7] = 0x88;

        return data;
    }

    /*
    3. 设置多路继电器输出状态 DO
    发送：01 0f 0000 0008 01 03 crc
    返回：01 0f 0000 0008 crc
    */
    if (control_byte == 0xf)
    {
        QByteArray data;
        int byte_count = byte_send.count();
        data.resize(9+byte_count);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);
        data[6] = byte_count;
        int i = 0;
        for (; i < byte_count; i++)
            data[7+i] = byte_send.at(i);
        //data[6] = 0x88;
        //data[7] = 0x88;
        data[7+i] = 0x88;
        data[8+i] = 0x88;

        return data;
    }

    /*
    4. 读取继电器输出状态 DO（读0区）
    发送：01 01 0000 0008 crc
    返回：01 01 01 03 crc
    */
    if (control_byte == 1)
    {
        QByteArray data;
        data.resize(8);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);
        data[6] = 0x88;
        data[7] = 0x88;

        return data;
    }



}

//从modbus返回数据中返回有效字节
QByteArray output_filter(QByteArray data)
{
    /*1.读取开关量输入状态 DI（读一区）
    发送：01 02 0000 0008 crc
    返回：01 02 01 02 crc*/

    /*
    4. 读取继电器输出状态 DO（读0区）
    发送：01 01 0000 0008 crc
    返回：01 01 01 03 crc
    */
    if (data.at(1) == 2 || data.at(1) == 1)
    {
        QByteArray return_data;
        return_data.resize(data.at(2));
        for (int i = 0; i < data.at(2); i++) {
            return_data[i] = data.at(3+i);
        }

        return return_data;
    }

    /*
    2.设置单路继电器输出状态 DO
    发送：01 05 0000 ff00 crc
    返回：01 05 0000 ff00 crc
    */
    if(data.at(1) == 5 || data.at(1) == 0xf)
    {
        QByteArray return_data;
        return_data[0] = 1;//假设成功
        return return_data;
    }

}

void test_dll(void)
{

}

//根据data格式内容。返回modbus驱动指令
//int read_write; ==0，读 ==1写
QByteArray input_data_exchange(data_exchange *data_ex)
{
//    qDebug() << data_ex->name["name"];
//    QByteArray data;
//    data.resize(2);
//    data[0] = 1;
//    data[1] = 2;
//    return data;
    //根据data_ex数据（用户组态）生成modbus指令返回给系统。
    int addr = data_ex->dev_id;
    int variable = data_ex->variable;
    QByteArray data = data_ex->write_data;
    int count = 1;
    int control_byte = 0;
    int start = 0;

    //根据绑定的数字来决定指令
    if (variable >= 10000 && variable <=20000)
    {//读取led
        control_byte = 0x02;//这个区暂时只有读指令
        start = variable - 10000 - 1;
    }
    else if (variable >= 0 && variable <= 10000) {
        if (data_ex->read_write == 0)
        {
            control_byte = 0x01;//读取key
        }
        else {
            control_byte = 0x05;//写key
            count = 0xff00;

        }

        start = variable - 1;
    }

    return gen_code(addr, control_byte, start, count, NULL);
    //gen_code(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send)
    //gen_code
}
