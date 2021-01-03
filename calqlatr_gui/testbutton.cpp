#include "testbutton.h"
#include "qdebug.h"
#include "qjsvalue.h"
#include "qjsvalueiterator.h"


TestButton::TestButton(QObject *parent) : QObject(parent)
{
}

void TestButton::buttonClicked(QString str)
{
    qDebug() << "TestButton按钮按下" << str;
    emit buttonclicked(str);
}

//测试json能不能这样解析
//打印显示TestButton::testVar QVariant(QJSValue, )
void TestButton::walkObj(QJSValue obj)
{
    QJSValueIterator it(obj);
    while (it.hasNext()) {
        it.next();
        qDebug() << it.name();// << ": " << it.value();
        if (it.value().isObject())
        {
            walkObj(it.value());
        }
        else
        {
            qDebug() << ":" << it.value().toString();
        }
    }
}
void TestButton::testVar(QVariant json)
{
    qDebug() << "TestButton::testVar" << json;
    if (json.canConvert<QJSValue>())
    {
        qDebug() << "可以转换为QJSValue";
        QJSValue obj = json.value<QJSValue>();
        //walkObj(obj);
//        QJSValueIterator it(config);
//        while (it.hasNext()) {
//            it.next();
//            qDebug() << it.name();// << ": " << it.value();
//            if (it.value().isObject())
//            {
//                QJSValueIterator
//            }
//        }


//        while (obj.isObject()) {
//            QJSValueIterator it(obj);
//            while (it.hasNext()) {
//                it.next();
//                qDebug() << it.name();
//            }
//            obj = obj.prototype();
//        }
    }
}

//实现js环境中解析对象结构，给c++设置系统配置
void TestButton::setConfigure(QString name,int index, QVariant config)
{
    static QMap<int, QMap<QString, QVariant>> map;
    QMap<QString, QVariant> p;

    //sendType *st;
    //QMap<QString, QMap<int, void *>> device_map;

    QJSValue obj = config.value<QJSValue>();

    QJSValueIterator it(obj);
    while (it.hasNext()) {
        it.next();

        if (it.value().isNumber())
        {
            p[it.name()] = it.value().toInt();
        }
        else if (it.value().isString())
        {
            p[it.name()] = it.value().toString();
        }
    }

    if (name != name_init)
    {
        map.clear();
        name_init = name;
    }

    map.insert(index, p);
    device_map[name]  = map;
    qDebug() << device_map;
}
