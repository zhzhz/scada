#ifndef QT_SERIAL_CLIENT_H
#define QT_SERIAL_CLIENT_H

#include "client.h"
#include <QSerialPort>        //提供访问串口的功能
#include <QSerialPortInfo>    //提供系统中存在的串口的信息

class Qt_serial_client : public Client
{
    Q_OBJECT
public:
    Qt_serial_client();
    ~Qt_serial_client();
    void write_data(QByteArray &data);

private:
    QSerialPort *serial;

private slots:
    void serialPort_readyRead();
};

#endif // QT_SERIAL_CLIENT_H
