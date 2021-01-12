#include "customvectordata.h"

CustomVectorData::CustomVectorData(QObject *parent) : QObject(parent)
{

}

QDataStream& operator>>(QDataStream& src, CustomVectorData& data)
{
    /*
     错误:C2679: 二进制“>>”: 没有找到接受“quint32”类型的右操作数的运算
    解决方法：因为没有添加头文件<QDataStream>
    */
    //QVector<data_exchange> l = data.l;
    int count;
    src >> count;
    //qDebug() << "operator>>" << count;
    data.l.resize(count);
    for (int i = 0; i < count; i++)
    {

        src >> data.l[i].read_write >> data.l[i].device >> data.l[i].dev_id >> data.l[i].name_variable >> data.l[i].name_variable_old >> data.l[i].write_data;
    }
    //qDebug() << "operator>>>>" << count;
    return src;
}

QDataStream& operator<<(QDataStream& out, CustomVectorData& data)
{
    //out << data.m_nType << data.m_strName;
    //out << data.ID;
    out << data.l.count();

    for (int i = 0; i < data.l.count(); i++)
    {
       out << data.l[i].read_write << data.l[i].device << data.l[i].dev_id  << data.l[i].name_variable << data.l[i].name_variable_old << data.l[i].write_data;
    }

    return out;
}
