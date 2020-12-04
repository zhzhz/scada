#ifndef MODBUS_SERVER_H
#define MODBUS_SERVER_H
#include "QObject"
#include "qt_tcp.h"


class Modbus_server : public QObject
{
    Q_OBJECT
public:
    explicit Modbus_server(QObject *parent = nullptr);

signals:
private:
    Qt_tcp qt_tcp;

private slots:
    void handle_data(QTcpSocket *);
};

#endif // MODBUS_SERVER_H
