#include "custom_data.h"

Custom_data::Custom_data(QObject *parent) : QObject(parent)
{
/*
    int read_write;
    QMap<QString, QString> name;
    QMap<QString, QString> device;
    QMap<QString, int> dev_id;
    QMap<QString, int> variable;
    QByteArray write_data;*/
}

QDataStream& operator>>(QDataStream& src, Custom_data& data)
{
    /*
     错误:C2679: 二进制“>>”: 没有找到接受“quint32”类型的右操作数的运算
    解决方法：因为没有添加头文件<QDataStream>
    */
    //src >> data.ID;
    src >> data.l.read_write >> data.l.name >> data.l.device >> data.l.dev_id >> data.l.variable >> data.l.write_data;
    //src >> data.m_strName;
    return src;
}

QDataStream& operator<<(QDataStream& out, Custom_data& data)
{
    //out << data.m_nType << data.m_strName;
    //out << data.ID;
    out << data.l.read_write << data.l.name << data.l.device << data.l.dev_id  << data.l.variable << data.l.write_data;
    return out;
}
