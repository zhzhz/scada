#include "tcp508neth.h"
#include "qdebug.h"


//Tcp508neth::Tcp508neth()
//{
//}
//将设备列表转换为优化后的结构
//read_list == led,key
//升序排列
//bool compare(data_exchange p1, data_exchange p2) {
//    return p1.name_variable < ;
//}

QVector<data_exchange> gen_read_vec(QMap<QString, QMap<int, QMap<QString, QVariant>>> device_map, QStringList read_list)
{
    QVector<data_range> read_data;
    QVector<data_exchange> data;

    for (int i = 0; i < read_list.count(); i++)
    {
        if (device_map.contains(read_list.at(i)))//包含led和key
        {
            QMap<int, QMap<QString, QVariant>> leds = device_map[read_list.at(i)];

            //遍历leds（包括led和key），识别其中可以读组合的个体，如果可以组合则组合，如果不可组合则生成新的组合体
            for (int j = 0; j < leds.count(); j++)
            {
                if (leds[j]["device"].toString() == "tcp508neth")
                {
                    data_range d;
                    //取出当前组的元素的variable，判断当前组的范围
                    int variable = leds[j]["variable"].toInt();

                    if (variable >= 1 && variable <=10000)//DO
                    {
                        d.range_min = 1;d.range_max = 10000;
                    }
                    else if (variable >= 10001 && variable <=20000)//DI
                    {
                        d.range_min = 10001;d.range_max = 20000;
                    }
                    else if (variable >= 40001 && variable <=50000)//AO
                    {
                        d.range_min = 40001;d.range_max = 50000;
                    }

                    d.data.read_write = 0;
                    d.data.dev_id = leds[j]["dev_id"].toInt();
                    d.data.device = leds[j]["device"].toString();
                    d.data.name_variable[leds[j]["name"].toString()] = leds[j]["variable"].toInt() - d.range_min;
                    d.data.name_variable_old[leds[j]["name"].toString()] = leds[j]["variable"].toInt();
                    //遍历vector，寻找可以合并的，如果不能合并，则创建新的vector对象
                    bool merge = false;

                    for (int i = 0; i < read_data.count(); i++)
                    {
                        if (read_data.at(i).data.dev_id == d.data.dev_id && read_data.at(i).data.device == d.data.device &&
                                read_data.at(i).range_min == d.range_min && read_data.at(i).range_max == d.range_max)
                        {
                            //可以合并,合并
                            read_data[i].data.name_variable[leds[j]["name"].toString()] = leds[j]["variable"].toInt() - d.range_min;
                            read_data[i].data.name_variable_old[leds[j]["name"].toString()] = leds[j]["variable"].toInt();
                            merge = true;
                            break;
                        }
                    }

                    if (merge == false)
                    {
                        //没有找到可以合并的，插入vector
                        //qDebug() << "cant merge" << d.data.name_variable;
                        read_data << d;
                    }
                }
            }
        }
    }



    //从data_range中提取data_exchange
    for (int i = 0; i < read_data.count(); i++)
    {
        data << read_data[i].data;
    }

    //给read_data name_variable减去基准
    //qDebug() << "data.count()" << data.count();
    for (int i = 0; i < data.count(); i++)
    {
        QMap<QString, int> name_var = data.at(i).name_variable;
        QVector<int> sort;
        QMap<QString, int>::iterator iter = name_var.begin();
        //qDebug() << "start0" << name_var.count();
        //qDebug() << "start1";
        while(iter != name_var.end())
        {
            sort << iter.value();
            //qDebug() << "-------------";
            iter++;
        }
        qSort(sort.begin(), sort.end());

        //遍历元素，减去最小值
        iter = name_var.begin();
        while(iter != name_var.end())
        {
//                iter.value() = iter.value() - sort.first();
              name_var[iter.key()] = iter.value() - sort.first();
              iter++;
        }
        data_exchange f = data.at(i);
        f.name_variable = name_var;
        data.replace(i, f);
    }

    return data;
}

//根据参数，生成tcp508neth指令
//首先关注01 05
QByteArray gen_code(uchar addr, uchar control_byte, ushort start, ushort count, QByteArray byte_send)
{
    /*1.读取开关量输入状态 DI（读一区）
    发送：01 02 0000 0008 crc
    返回：01 02 01 02 crc
    */
    QByteArray header;
    header.resize(6);
    header[0] = 0;
    header[1] = 0;
    header[2] = 0;
    header[3] = 0;
    header[4] = 0;

    if (control_byte == 2)
    {
        QByteArray data;
        data.resize(6);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);

        header[5] = data.size();
        return header + data;
    }

    /*
    2.设置单路继电器输出状态 DO
    发送：01 05 0000 ff00 crc
    返回：01 05 0000 ff00 crc
    */
    if (control_byte == 5)
    {
        QByteArray data;
        data.resize(6);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);

        header[5] = data.size();
        return header + data;
    }

    /*
    3. 设置多路继电器输出状态 DO
    发送：01 0f 0000 0008 01 03 crc
    返回：01 0f 0000 0008 crc
    */
    if (control_byte == 0xf)
    {
        QByteArray data;
        int byte_count = byte_send.count();
        data.resize(7+byte_count);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);
        data[6] = byte_count;
        int i = 0;
        for (; i < byte_count; i++)
            data[7+i] = byte_send.at(i);
        //data[6] = 0x88;
        //data[7] = 0x88;

        header[5] = data.size();
        return header + data;
    }

    /*
    4. 读取继电器输出状态 DO（读0区）
    发送：01 01 0000 0008 crc
    返回：01 01 01 03 crc
    */
    if (control_byte == 1)
    {
        QByteArray data;
        data.resize(6);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);

        header[5] = data.size();
        return header + data;
    }

    //读AO
    //发送 01 03 00 00 00 04
    //返回 01 01 08 00 00 00 00 00 00 00 00
    if (control_byte == 3)
    {
        QByteArray data;
        data.resize(6);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);

        header[5] = data.size();
        return header + data;
    }

    //写AO
    //发送 01 10 00 00 00 01 02 xx xx crc crc
    if (control_byte == 0x10)
    {
        QByteArray data;
        int byte_count = byte_send.count();
        data.resize(7+byte_count);
        data[0] = addr;
        data[1] = control_byte;
        data[2] = (start & 0xff00) >> 8;
        data[3] = (start & 0xff);
        data[4] = (count & 0xff00) >> 8;
        data[5] = (count & 0xff);
        data[6] = byte_count;
        int i = 0;
        for (; i < byte_count; i++)
            data[7+i] = byte_send.at(i);
        //data[6] = 0x88;
        //data[7] = 0x88;

        header[5] = data.size();
        qDebug() << header + data;
        return header + data;
    }

}

//从modbus返回数据中返回有效字节
QByteArray output_filter(QByteArray data)
{
    /*2.读取开关量输入状态 DI（读一区）
    发送：01 02 0000 0008 crc
    返回：01 02 01 02 crc*/

    /*
    1. 读取继电器输出状态 DO（读0区）
    发送：01 01 0000 0008 crc
    返回：01 01 01 03 crc
    */

    data.remove(0, 6);//去掉头部

    if (data.at(1) == 2 || data.at(1) == 1)
    {
        QByteArray return_data;
        return_data.resize(data.at(2) + 4);//4代表头部的比特数据
        return_data[0]=1;return_data[1]=0;return_data[2]=0;return_data[3]=0;
        for (int i = 0; i < data.at(2); i++) {
            return_data[i + 4] = data.at(3+i);
        }

        return return_data;
    }

    /*
    2.设置单路继电器输出状态 DO
    发送：01 05 0000 ff00 crc
    返回：01 05 0000 ff00 crc
    */
    if(data.at(1) == 5 || data.at(1) == 0xf)
    {
        QByteArray return_data;
        return_data[0] = 1;//假设成功
        return return_data;
    }

    //读AO
    if (data.at(1) == 0x3)
    {
        QByteArray return_data;
        return_data.resize(data.at(2) + 4);//4代表头部的比特数据
        return_data[0]=16;return_data[1]=0;return_data[2]=0;return_data[3]=0;
        for (int i = 0; i < data.at(2); i++) {
            return_data[i + 4] = data.at(3+i);
        }

        //翻转高低位为小端模式
        unsigned char p;
        for (int i = 0; i < data.at(2)/2; i++) {
            //return_data[i + 4] = data.at(3+i);
            p = return_data.at(4 + i*2+1);
            return_data[4 + i*2+1] = return_data.at(4 + i*2);
            return_data[4 + i*2] = p;
        }

        return return_data;
    }

    //多写AO
    if (data.at(1) == 0x10)
    {
        QByteArray return_data;
        return_data[0] = 1;//假设成功
        return return_data;
    }

}

void test_dll(void)
{

}

//根据data格式内容。返回modbus驱动指令
//int read_write; ==0，读 ==1写
QByteArray input_data_exchange(data_exchange *data_ex)
{
    //根据data_ex数据（用户组态）生成modbus指令返回给系统。
    int addr = data_ex->dev_id;
    //int variable = data_ex->variable;
    QByteArray data = data_ex->write_data;
    int count;
    int control_byte;
    int start;
    int read_write = data_ex->read_write;


    //遍历name_variable里的variable变量，生成rtu访问code
    QMap<QString, int> name_variable = data_ex->name_variable_old;
    QMap<QString, int>::iterator iter = name_variable.begin();
    //name_variable里不一定是按照大小排序的，先排序
    QVector<int> i;

    if (iter != name_variable.end())
    {
        while(iter != name_variable.end())
        {
            i << iter.value();
            iter++;
        }
        qSort(i.begin(), i.end());
        //count = i.last() - i.first() + 1;
    }
    else
    {
        i << name_variable.first();
    }

    int variable = i.first();
    if (read_write == 0)//如果是读
    {

        ////////////////////////////////////////////////////////
        //计算control_byte 和 start
        if (variable >= 10001 && variable <=20000)//读DI
        {
            control_byte = 0x02;
            start = variable - 10001;//从哪个变量读取
        }
        else if (variable >= 1 && variable <= 10000)//读DO
        {
            control_byte = 0x01;//读取key 读DO
            start = variable - 1;//从哪个变量读取
        }
        else if (variable >= 40001 && variable <= 50000)//读AO
        {
            control_byte = 0x03;
            start = variable - 40001;
        }

        //count = name_variable.last() - name_variable.first() + 1;
        //map不是sort好的，需要手动寻找最大和最小值
        count = i.last() - i.first() + 1;
    }
    else
    {
        //写(目前只支持单写DO,多写AO)
        if (variable >= 1 && variable <= 10000)
        {
            control_byte = 0x05;//写key 写DO
            start = variable - 1;//从哪开始写
            count = 0xff00;//固定写法
        }
        else if (variable >= 40001 && variable <= 50000)
        {
            control_byte = 0x10;//写key 写AO
            start = variable - 40001;//从哪开始写
            qDebug() << "----------------------------写AO" << start << " " << data.count();
            count = data.count()/2;
        }
    }

    return gen_code(addr, control_byte, start, count, data);
}

