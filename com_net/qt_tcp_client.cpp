#include "qt_tcp_client.h"

Qt_tcp_client::Qt_tcp_client(QObject *parent):Client(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));
}

Qt_tcp_client::~Qt_tcp_client()
{
qDebug() << "Qt_tcp_client::~Qt_tcp_client()";
}

void Qt_tcp_client::set_param(QString IP, int port)
{
    this->IP = IP;
    this->port = port;
}

void Qt_tcp_client::connect_line(void)
{
    qDebug() << "Qt_tcp_client::connect_line" <<IP <<port;
    socket->connectToHost(IP, port);
    if(!socket->waitForConnected(30000))
    {
        qDebug() << "Connection failed!" << port;
        return;
    }
    else
    {
        qDebug() << "Connect successfully!" << port;
    }
}


void Qt_tcp_client::socket_Read_Data(void)
{
    QByteArray buffer;
    //读取缓冲区数据
    buffer = socket->readAll();
    //qDebug() << buffer;
    //qDebug() << "Qt_tcp_client::socket_Read_Data 1";
    emit data_come(buffer);
    //qDebug() << "Qt_tcp_client::socket_Read_Data 2";
}

void Qt_tcp_client::write_data(QByteArray &data)
{
//发送数据给server
    socket->write(data);
}
