#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QMainWindow>

#include "configfile.h"
#include "dev_driver.h"
#include "sys_ctl.h"
#include "gui.h"



class MainClient : public QObject
{
    Q_OBJECT

public:
    explicit MainClient(QObject *parent = nullptr);
    ~MainClient();

private:
    //Ui::MainWindow *ui;
    //Qt_tcp_client *qt_tcp_client;

    ConfigFile configFile;
    Dev_driver dev_driver;
    Sys_ctl *sys_ctl;
    Gui *gui;

    QTimer *m_timer;

    void test1();

    typedef QByteArray (*myfun)(int, int);//定义函数格式
    typedef int (*myfun1)(int, int);
    typedef QByteArray  (*gen_code)(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    typedef QByteArray  (*output_filter)(QByteArray data);


private slots:
    void ImageTimerTimeout();

};

#endif // MAINWINDOW_H
