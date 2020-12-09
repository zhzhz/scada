#include "dev_factor.h"
#include "qt_tcp_client.h"

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
}
