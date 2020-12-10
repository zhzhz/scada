#ifndef DEV_FACTOR_H
#define DEV_FACTOR_H
#include "client.h"


class dev_factor
{
public:
    dev_factor();
    static Client * product(QString name);
};

//工厂生产的具体类
class modbus_dev :Client
{
public:
    modbus_dev();
    static Client * modbus();
};

class Mitsubishi_dev :Client
{
public:
    Mitsubishi_dev();
    static Client * Mitsubishi();
};

class tcp508neth_dev :Client
{
public:
    tcp508neth_dev();
    static Client * tcp508neth();
    ~tcp508neth_dev();
};




#endif // DEV_FACTOR_H
