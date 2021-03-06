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

    QVector <QTcpSocket *> clientSockets;

public slots:
    void acceptConnection();
    void replyToClient();
    void msg_error(QAbstractSocket::SocketError);

signals:
    void data_come(QTcpSocket *);
    void host_closed(QTcpSocket *);

private:
    QTcpServer *server;

};

#endif // QT_TCP_H
