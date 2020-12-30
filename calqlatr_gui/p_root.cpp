#include "p_root.h"
#include "qdebug.h"


P_ROOT::P_ROOT(QObject *parent) : QObject(parent)
{
    //gui已经由qml接管了，这里只需实现通讯部分
    //现在只考虑按钮

    configFile.read_config_file("../tests/a.txt");
    sys_ctl = new Sys_ctl(&dev_driver, this);
    connect(this, SIGNAL(buttonClickedSignal(QString)), sys_ctl, SLOT(button_clicked(QString)));
    sys_ctl->setConfigureFile(&configFile);

    sys_ctl->start();
}

void P_ROOT::buttonClicked(QString id)
{
    qDebug() << "鼠标按下" << id;
    //这个函数只是传递鼠标按下事件的
    emit buttonClickedSignal(id);
}
