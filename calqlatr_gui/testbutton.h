#ifndef TESTBUTTON_H
#define TESTBUTTON_H

#include <QObject>
#include "qvariant.h"
#include "QJSValue"

class TestButton : public QObject
{
    Q_OBJECT
public:
    explicit TestButton(QObject *parent = nullptr);
    Q_INVOKABLE void buttonClicked(QString);
    Q_INVOKABLE void testVar(QVariant);
    void walkObj(QJSValue obj);
signals:
    void buttonclicked(QString);
};

#endif // TESTBUTTON_H
