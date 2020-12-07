#include "qt_tcp.h"

Qt_tcp::Qt_tcp(QObject *parent) : QObject(parent)
{
    //设置窗口标题
    //setWindowTitle("server");
    //创建QTcpServer对象
    server=new QTcpServer(this);
    //服务端开始监听是否有客户端连接
    server->listen(QHostAddress::Any,9999);
    //连接信号与槽，若有连接过来则接收连接
    connect(server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}


//接收客户端连接
void Qt_tcp::acceptConnection()
{
    qDebug() << "Qt_tcp::acceptConnection()" << 9999;
    clientSockets << server->nextPendingConnection();
    connect(clientSockets.last(),SIGNAL(readyRead()),this,SLOT(replyToClient()));
}

//回复客户端
void Qt_tcp::replyToClient()
{
    QTcpSocket *sender_client = qobject_cast<QTcpSocket  *>(sender());
    emit data_come(sender_client);
}


