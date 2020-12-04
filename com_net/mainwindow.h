#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "configfile.h"
#include "dev_driver.h"
#include "qt_tcp.h"

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
    //Qt_tcp_client *qt_tcp_client;

    ConfigFile configFile;
    Dev_driver dev_driver;

    QTimer *m_timer;

    Qt_tcp qt_tcp;

    void test1();

    typedef QByteArray (*myfun)(int, int);//定义函数格式
    typedef int (*myfun1)(int, int);
    typedef QByteArray  (*gen_code)(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send);
    typedef QByteArray  (*output_filter)(QByteArray data);


private slots:
    void ImageTimerTimeout();
private slots:
    void handle_data(QTcpSocket *);
    void data_come(QByteArray &data);
};

#endif // MAINWINDOW_H
