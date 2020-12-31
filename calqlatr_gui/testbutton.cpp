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
        walkObj(obj);
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
