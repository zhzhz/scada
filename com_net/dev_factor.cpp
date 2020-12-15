#include "dev_factor.h"
#include "qt_tcp_client.h"
#include "qt_serial_client.h"

dev_factor::dev_factor()
{

}

modbus_dev::modbus_dev()
{

}

Client * modbus_dev::modbus()
{
    Qt_tcp_client *client = new Qt_tcp_client;
    client->set_param(QString("192.168.2.101"), 9999);
    client->connect_line();
    return client;
}



Client * Mitsubishi_dev::Mitsubishi()
{
    Qt_tcp_client *client = new Qt_tcp_client;
    client->set_param(QString("192.168.2.101"), 8888);
    client->connect_line();
    return client;
}

Client * tcp508neth_dev::tcp508neth()
{
    Qt_tcp_client *client = new Qt_tcp_client;
    client->set_param(QString("192.168.2.75"), 502);
    client->connect_line();
    return client;
}

tcp508neth_dev::~tcp508neth_dev()
{
    qDebug() << "tcp508neth_dev::~tcp508neth_dev()";
}



Client * dev_factor::product(QString name)
{
    if (name == "modbus")
    {
        return modbus_dev::modbus();
    }
    else if(name == "Mitsubishi")
    {
        return Mitsubishi_dev::Mitsubishi();
    }
    else if(name == "tcp508neth")
    {
        return tcp508neth_dev::tcp508neth();
    }
    else if(name == "tcp508nserial")
    {
        return new Qt_serial_client();
    }
}
