#include "qt_tcp_client.h"

Qt_tcp_client::Qt_tcp_client(QObject *parent):Client(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(socket_Read_Data()));

    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
                            this, SLOT(msg_error(QAbstractSocket::SocketError)));
}

void Qt_tcp_client::set_param(QString IP, int port)
{
    this->IP = IP;
    this->port = port;
}

Qt_tcp_client::~Qt_tcp_client()
{
    qDebug() << "Qt_tcp_client::~Qt_tcp_client()";
    delete socket;
}

bool Qt_tcp_client::connect_line(void)
{
    socket->connectToHost(IP, port);
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Connection failed!" << port;
        return false;
    }
    else
    {
        qDebug() << "Connect successfully!" << port;
        return true;
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

void Qt_tcp_client::msg_error(QAbstractSocket::SocketError error)
{
    //qDebug() << "Qt_tcp_client::msg_error" << "网络断开";
    QTcpSocket *sender_client = qobject_cast<QTcpSocket  *>(sender());
    qDebug() << "Qt_tcp_client::msg_error" << error;
    switch(error)
    {
        case QAbstractSocket::RemoteHostClosedError://客户端断开
        {
            emit host_closed(sender_client);

            break;
        }

    case QAbstractSocket::NetworkError://网络断开
            //emit host_closed(sender_client);
            emit networkerror(sender_client);
        break;

    case QAbstractSocket::ConnectionRefusedError://有网络没server
            emit networkerror(sender_client);
        break;
        default:
        {
            //error = -1;
            //QMessageBox::information(this, "show", "error");
        //QMessageBox::information(NULL, "Title", "Content",
        //                         QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            break;
        }
    }
}
