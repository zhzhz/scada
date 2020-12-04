#include "qt_tcp.h"

Qt_tcp::Qt_tcp(QObject *parent) : QObject(parent)
{
    //设置窗口标题
    //setWindowTitle("server");
    //创建QTcpServer对象
    server=new QTcpServer(this);
    //服务端开始监听是否有客户端连接
    server->listen(QHostAddress::Any,8888);
    //连接信号与槽，若有连接过来则接收连接
    connect(server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

//接收客户端连接
void Qt_tcp::acceptConnection()
{
    //根据客户端连接获取服务端socket对象
    qDebug() << "Qt_tcp8888::acceptConnection()";
    serverSocket=server->nextPendingConnection();
    connect(serverSocket,SIGNAL(readyRead()),this,SLOT(replyToClient()));
}
//回复客户端
void Qt_tcp::replyToClient()
{
    //接收客户端信息
    //QString msg=QString(serverSocket->readAll());
    //构造回复字符串
    //char* replyMsg="server:I am server,I got your message.";
    //将回复字符串发送给客户端
    //serverSocket->write(replyMsg);
    //qDebug() << (int)serverSocket->readAll().count();
    //根据modbus关键字，返回相应响应
    emit data_come(serverSocket);

/*
    //下面的工作应该emit给modbus类，然后在modbus类中处理
    QByteArray data = serverSocket->readAll();
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
    */
}

