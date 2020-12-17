#ifndef QT_TCP_CLIENT_H
#define QT_TCP_CLIENT_H

#include "client.h"
#include "QTcpSocket"

class Qt_tcp_client :public Client
{
    Q_OBJECT
public:
    explicit Qt_tcp_client(QObject *parent = nullptr);
    ~Qt_tcp_client();
    //QByteArray gen_data(void);
    /*
    void write_data(QByteArray &data);
signals:
    void data_come(QTcpSocket *);
    */
    void set_param(QString IP, int port);

    bool connect_line(void);

    void write_data(QByteArray &data);

signals:
    void host_closed(QTcpSocket *);
    void networkerror(QTcpSocket *);
//    void data_come(QByteArray &data);

private slots:
    void socket_Read_Data(void);
    void msg_error(QAbstractSocket::SocketError error);
private:
    QTcpSocket *socket;
    QString IP;
    int port;
};

#endif // QT_TCP_CLIENT_H
