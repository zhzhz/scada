#ifndef SYS_CTL_H
#define SYS_CTL_H

#include <QObject>
#include "dev_driver.h"
#include "configfile.h"
#include "gui.h"
#include "qradiobutton.h"
#include "qpushbutton.h"
#include "error_dialog.h"
#include "QTimer"

class Sys_ctl : public QObject
{
    Q_OBJECT
public:
    explicit Sys_ctl(Dev_driver *dev_driver,QObject *parent = nullptr);
    void start(void);
    void setConfigureFile(ConfigFile *configFile);
    void setGui(Gui *gui);

    void write_data(void *data, QString data_type,QByteArray data_write);
signals:

private slots:
    void data_come(QByteArray &data);
    void data_come_error(QByteArray &data);

    void host_closed(QTcpSocket *tcp);
    void networkerror(QTcpSocket *);
    void button_clicked(void);
    void TimerTimeout(void);
private:
    Dev_driver *dev_driver;
    ConfigFile *configFile;
    Gui *gui;

    bool data_save_bool;
    data_exchange data_save;
    bool read_none;

    error_dialog *dlg;
    error_dialog *dlg_neterror;
    int i, j;

    bool write_flag;
    QTimer *m_timer;

    bool host_closed_f;
    bool networkerror_f;
};

#endif // SYS_CTL_H
