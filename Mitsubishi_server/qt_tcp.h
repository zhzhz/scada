#ifndef QT_TCP_H
#define QT_TCP_H

#include <QObject>
#include "QTcpServer"
#include "qtcpsocket.h"

class Qt_tcp : public QObject
{
    Q_OBJECT
public:
    explicit Qt_tcp(QObject *parent = nullptr);

public slots:
    void acceptConnection();
    void replyToClient();

signals:
    void data_come(QTcpSocket *);

private:
    QTcpServer *server;
    QVector <QTcpSocket *> clientSockets;
};

#endif // QT_TCP_H
