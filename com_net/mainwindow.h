#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "configfile.h"
#include "dev_driver.h"
#include "qt_tcp.h"
#include "controller.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    //QVector <bool> delete_tcp_later_flag;
    //QVector <QTcpSocket *>delete_tcp;
    QMap <QTcpSocket *, bool> tcp_delete_later;

    ConfigFile configFile;


    QTimer *m_timer;

    Qt_tcp qt_tcp;

    QMap<QString, Controller *> dev2thread;

    //挂起相关
    typedef struct
    {
        bool is_suspend;
        QTcpSocket *busy_client;
        QString dev_name;
        QVector <QTcpSocket *> tcp_clients;
    }suspand;

    QMap<QString, suspand *> dev_suspend;

    void test1();

    typedef QByteArray (*myfun)(int, int);//定义函数格式
    typedef int (*myfun1)(int, int);
    typedef QByteArray  (*gen_code)(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    typedef QByteArray  (*output_filter)(QByteArray data);


private slots:
    void ImageTimerTimeout();
private slots:
    void handle_gui(QTcpSocket *);
    void data_handle(QString dev_name, QTcpSocket *tcp, QByteArray data);
    void host_closed(QTcpSocket *);
    void read_vec(QString dev_name, QVector<data_exchange> data, QTcpSocket *tcp);

 private:



};

#endif // MAINWINDOW_H
