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
#include "writestrategy.h"

class QQuickItem;

class Sys_ctl : public QObject
{
    Q_OBJECT
public:
    explicit Sys_ctl(QQuickItem *qmlItem, QObject *parent = nullptr);
    ~Sys_ctl();

    QQuickItem *qmlItem;
    int get_read_data(QByteArray data, int bits, int index);
    void setConfigureFile(ConfigFile *configFile);
    //void setGui(Gui *gui);

    void write_read_data(QMap<QString, QVariant> data, int read_write, QByteArray data_write, int index);

signals:
public slots:
    void button_clicked(QString);
    void start(void);

private slots:
    void data_come(QByteArray &data, data_exchange);
    void data_come_error(QByteArray &data, data_exchange);
    void read_map(QVector<data_exchange>);
    void connect_resume();

    void com_error();
    void com_error_ok();
private:
    //Dev_driver *dev_driver;
    ConfigFile *configFile;
    //Gui *gui;

    bool is_write_instruct;
    bool writeAfterRead;
    data_exchange data_save;
    QVector <data_exchange> data_saves;
    QVector <WriteStrategyBase*> WriteStrategys;
    bool read_none;

    //error_dialog *dlg;
    //error_dialog *dlg_neterror;
    int i, j;
    int state;
    int count;
    QStringList readList;

    bool write_flag;
    //QTimer *m_timer;



    Controller *thread;

    QVector<data_exchange> read_data;


    int device_count;
    //bool is_write_instruct;
    WriteStrategy writeStrategy;
    WriteStrategyBase* genWriteStrategy;
};

#endif // SYS_CTL_H
