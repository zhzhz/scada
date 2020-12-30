#ifndef P_ROOT_H
#define P_ROOT_H

#include <QObject>
#include "sys_ctl.h"
#include "configfile.h"
class TestButton;
class QQuickItem;

class P_ROOT : public QObject
{
    Q_OBJECT
public:
    explicit P_ROOT(TestButton *tb, QQuickItem *qmlItem,QObject *parent = nullptr);
    ~P_ROOT();

Q_INVOKABLE void buttonClicked(QString);

signals:
   void buttonClickedSignal(QString);

public slots:

private:
    //Gui *gui;
    ConfigFile configFile;
    //Dev_driver dev_driver;
    Sys_ctl *sys_ctl;
    TestButton *tb;

};

#endif // P_ROOT_H
