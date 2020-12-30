#include "p_root.h"
#include "qdebug.h"
#include "testbutton.h"
#include "qquickitem.h"


P_ROOT::P_ROOT(TestButton *tb, QQuickItem *qmlItem, QObject *parent) : QObject(parent)
{
    //gui已经由qml接管了，这里只需实现通讯部分
    //现在只考虑按钮
    this->tb = tb;
    connect(tb, SIGNAL(buttonclicked(QString)), this, SLOT(buttonClicked(QString)));

    qRegisterMetaType<data_exchange>();
    qRegisterMetaType<QByteArray>("QByteArray&");

    configFile.read_config_file("../tests/a.txt");
    sys_ctl = new Sys_ctl(qmlItem, this);
    connect(this, SIGNAL(buttonClickedSignal(QString)), sys_ctl, SLOT(button_clicked(QString)));
    sys_ctl->setConfigureFile(&configFile);

    //sys_ctl->start();
}

P_ROOT::~P_ROOT()
{
    delete sys_ctl;
}

void P_ROOT::buttonClicked(QString id)
{
    qDebug() << "鼠标按下" << id;
    //这个函数只是传递鼠标按下事件的
    emit buttonClickedSignal(id);
}
