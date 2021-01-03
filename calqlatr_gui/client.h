#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include "qbytearray.h"


class Client :public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    virtual void write_data(QByteArray &data) = 0;

signals:
    void data_come(QByteArray &data);
};

#endif // CLIENT_H
