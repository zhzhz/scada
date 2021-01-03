#ifndef TESTBUTTON_H
#define TESTBUTTON_H

#include <QObject>
#include "qvariant.h"
#include "QJSValue"

//typedef struct
//{
//    QString name;
//    QString device;
//    int dev_id;
//    int variable;
//}sendType;

class TestButton : public QObject
{
    Q_OBJECT
public:
    explicit TestButton(QObject *parent = nullptr);
    Q_INVOKABLE void buttonClicked(QString);
    Q_INVOKABLE void testVar(QVariant);
    Q_INVOKABLE void setConfigure(QString name, int index, QVariant);
    void walkObj(QJSValue obj);

    QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map;
signals:
    void buttonclicked(QString);

private:
    //QMap<QString, QMap<int, void *>> device_map;

    QString name_init;
};

#endif // TESTBUTTON_H
