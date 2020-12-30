#ifndef SYS_CTL_H
#define SYS_CTL_H

#include <QObject>
#include "dev_driver.h"
#include "configfile.h"
//#include "gui.h"
//#include "qradiobutton.h"
//#include "qpushbutton.h"
//#include "error_dialog.h"
#include "QTimer"
#include "controller.h"

class Sys_ctl : public QObject
{
    Q_OBJECT
public:
    explicit Sys_ctl(QObject *parent = nullptr);
    ~Sys_ctl();

    void setConfigureFile(ConfigFile *configFile);
    //void setGui(Gui *gui);

    void write_data(void *data, QString data_type,QByteArray data_write);

signals:
public slots:
    void button_clicked(QString);
    void start(void);

private slots:
    void data_come(QByteArray &data, data_exchange);
    void data_come_error(QByteArray &data, data_exchange);
    void connect_resume();


private:
    //Dev_driver *dev_driver;
    ConfigFile *configFile;
    //Gui *gui;

    bool read_write_flag;
    data_exchange data_save;
    QVector <data_exchange> data_saves;
    bool read_none;

    //error_dialog *dlg;
    //error_dialog *dlg_neterror;
    int i, j;

    bool write_flag;
    //QTimer *m_timer;



    Controller *thread;
};

#endif // SYS_CTL_H
