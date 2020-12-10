#include "modbus_server.h"


Modbus_server::Modbus_server(QObject *parent) : QObject(parent)
{
    connect(&qt_tcp, SIGNAL(data_come(QTcpSocket *)), this, SLOT(handle_data(QTcpSocket *)));
}

//读取客户端发来的数据。处理返回modbus指令
void Modbus_server::handle_data(QTcpSocket *serverSocket)
{
    QByteArray data = serverSocket->readAll();
    //qDebug() << data.toHex();
    //int j = 4;(int)
    QByteArray ba;
    switch (data.at(1)) {
    case 2://读DI
        ba.resize(6);
        ba[0] = 0x01;
        ba[1] = 0x02;
        ba[2] = 0x01;//1个字节数据
        ba[3] = 0x02;//00000010,只有一位为高电平
        ba[4] = 0x88;
        ba[5] = 0x88;
        serverSocket->write(ba);
        break;

    case 5://写DO
        ba.resize(8);
        ba[0] = 0x01;
        ba[1] = 0x05;
        ba[2] = 0x00;
        ba[3] = 0x00;
        ba[4] = 0xff;
        ba[5] = 0x00;
        ba[6] = 0x88;
        ba[7] = 0x88;
        serverSocket->write(ba);
        //qDebug() << data.toHex();
        break;

    case 0xf://写多个DO
        ba.resize(8);
        ba[0] = 0x01;
        ba[1] = 0x0f;
        ba[2] = 0x00;
        ba[3] = 0x00;
        ba[4] = 0x00;
        ba[5] = 0x08;
        ba[6] = 0x88;
        ba[7] = 0x88;
        serverSocket->write(ba);
        break;

    case 1://读多个DO
        ba.resize(6);
        ba[0] = 0x01;
        ba[1] = 0x01;
        ba[2] = 0x01;
        ba[3] = 0x03;//返回00000011
        ba[4] = 0x88;
        ba[5] = 0x88;
        serverSocket->write(ba);
        break;
    default:
        break;
    }
}



