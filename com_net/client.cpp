#include "client.h"
#include "qdebug.h"

Client::Client(QObject *parent):QObject(parent)
{

}

//释放占有的网络资源
Client::~Client()
{
qDebug() << "Client::~Client()";
}
