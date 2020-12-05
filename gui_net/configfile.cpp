#include "configfile.h"
#include "qDebug"

ConfigFile::ConfigFile(QObject *parent) : QObject(parent)
{

}





//得到json格式的系统配置信息
void ConfigFile::printJson(cJSON * root)
{

//    for(int i=0; i<cJSON_GetArraySize(root); i++)   //遍历最外层json键值对
//    {
//        cJSON * item = cJSON_GetArrayItem(root, i);
//        if(cJSON_Object == item->type)      //如果对应键的值仍为cJSON_Object就递归调用printJson
//            printJson(item);
//        else                                //值不为json对象就直接打印出键和值
//        {
//            printf("%s->", item->string);
//            printf("%s\n", cJSON_Print(item));//遍历生成的树结构，打印
//        }
//    }

    //遍历cjson结构，转化为内存结构，现在用的是手写结构
    QMap<int, void *> map_device;
    QMap<int, void *> map_led;
    QMap<int, void *> map_key;

    for(int i=0; i<cJSON_GetArraySize(root); i++)//3个
    {
        //device led keys
        cJSON * item = cJSON_GetArrayItem(root, i);//device
        for (int j = 0; j < cJSON_GetArraySize(item); j++)//遍历2个obj，8个obj,8个obj
        {
            cJSON * item_obj = cJSON_GetArrayItem(item, j);//得到obj

            device *dev1 = new device();
            led *led1= new led();
            key *key1= new key();
            for (int k = 0; k < cJSON_GetArraySize(item_obj); k++)//得到obj下面的真正数据的个数，每次一个标准key value键值对
            {
                cJSON * item_data = cJSON_GetArrayItem(item_obj, k);//得到真正的数据

                //qDebug() << item_data->string << item_data->valuestring;
                if (QString(item->string) == "device")
                {
                    //给device结构赋值
                    if (QString(item_data->string) == "name")
                    {
                        dev1->name.insert("name", item_data->valuestring);
                        //qDebug() << "led1->name.insert" << item_data->valuestring;
                        //qDebug() << led1 << "led1 insert name" <<  item_data->valuestring;
                    }
                    else if (QString(item_data->string) == "id") {
                        dev1->id.insert("id", item_data->valueint);
                    }

                }
                else if(QString(item->string) == "led")
                {
                    //qDebug() << item_data->string << item_data->valuestring <<item_data->valueint;

                    if (QString(item_data->string) == "name")
                    {
                        led1->name.insert("name", item_data->valuestring);
                        //qDebug() << "led1->name.insert" << item_data->valuestring;
                        //qDebug() << led1 << "led1 insert name" <<  item_data->valuestring;
                    }
                    else if (QString(item_data->string) == "device") {
                        led1->device = item_data->valuestring;
                    }
                    else if (QString(item_data->string) == "dev_id"){
                        led1->dev_id.insert("dev_id", item_data->valueint);
                    }
                    else if (QString(item_data->string) == "variable") {
                        led1->variable.insert("variable", item_data->valueint);
                    }
                }
                else if (QString(item->string) == "key") {
                    if (QString(item_data->string) == "name")
                    {
                        key1->name.insert("name", item_data->valuestring);
                        //qDebug() << "led1->name.insert" << item_data->valuestring;
                        //qDebug() << led1 << "led1 insert name" <<  item_data->valuestring;
                    }
                    else if (QString(item_data->string) == "device") {
                        key1->device = item_data->valuestring;
                    }
                    else if (QString(item_data->string) == "dev_id"){
                        key1->dev_id.insert("dev_id", item_data->valueint);
                    }
                    else if (QString(item_data->string) == "variable") {
                        key1->variable.insert("variable", item_data->valueint);
                    }
                }

            }

            if(QString(item->string) == "device")
            {
                map_device.insert(j, dev1);
            }
            else {
                delete dev1;
            }

            if(QString(item->string) == "led")
            {
                map_led.insert(j, led1);
            }
            else {
                delete led1;
            }

            if(QString(item->string) == "key")
            {
                map_key.insert(j, key1);
            }
            else {
                delete key1;
            }
        }

            //qDebug() << item->string << j;
    }        //qDebug() << item;
    device_map.insert("device", map_device);//这句只能执行一句
    device_map.insert("led", map_led);
    device_map.insert("key", map_key);
    //device_map.insert("key", map_key);
    //qDebug() << ((led *)map_led[2])->name["name"];

/*
    //处理device
    QMap<int, void *> map_device;
    device *dev1 = new device();
    dev1->name.insert("name", "modbus1");

    device *dev2 = new device();
    dev2->name.insert("name", "modbus2");

    map_device.insert(0, dev1);
    map_device.insert(1,dev2);

    //处理led
//////////////////////////////////////////////////////////////////////////////
    QMap<int, void *> map_led;
    int count = 8;//假设8个leds
    for (int i = 0; i < count; i++) {
        led *led1= new led();
        led1->name.insert("name", QString("led") + QString::number(i+1));
        led1->device.insert("device", "modbus1");
        led1->variable.insert("variable", 10001 + i);
        map_led.insert(i, led1);
        //qDebug() << led1->name["name"];
        //
    }

    //处理key
//////////////////////////////////////////////////////////////////////////////
    QMap<int, void *> map_key;
    for (int i = 0; i < count; i++) {
        key *key1 = new key();
        key1->name.insert("name", QString("key") + QString::number(i+1));
        key1->device.insert("device", "modbus1");
        key1->variable.insert("variable", 1 + i);
        map_key.insert(i, key1);
    }


    device_map.insert("device", map_device);
    device_map.insert("led", map_led);
    device_map.insert("key", map_key);

    */

//    QMap<QString, QMap<int, void *>>::iterator iter = device_map.begin();
//    while (iter != device_map.end())
//    {
//        qDebug() << "Iterator " << iter.key() << ":" << iter.value(); // 迭代器
//        //遍历device
//        if (iter.key() == "device")
//        {
//            QMap<int, void *> map_device = iter.value();
//            for (int i = 0; i < map_device.count(); i++) {
//                device *dev = (device *)map_device[i];
//                qDebug() << dev->name << dev->id;
//            }
//        }
//        else if (iter.key() == "led")
//        {
//            QMap<int, void *> led_device = iter.value();
//            for (int i = 0; i < led_device.count(); i++) {
//                led *led1 = (led *)led_device[i];
//                qDebug() << led1->name << led1->device << led1->dev_id << led1->variable;
//            }
//        }
//        else if (iter.key() == "key")
//        {
//            QMap<int, void *> key_device = iter.value();
//            for (int i = 0; i < key_device.count(); i++) {
//                key *key1 = (key *)key_device[i];
//                qDebug() << key1->name << key1->device << key1->dev_id<< key1->variable;
//            }
//        }

//        iter++;
//    }
}

void ConfigFile::doit2(char *text)
{
    char *out;cJSON *json;

    json=cJSON_Parse(text);
    if (!json) {printf("Error before: [%s]\n",cJSON_GetErrorPtr());}
    else
    {
        /*
        out=cJSON_Print(json);
        cJSON_Delete(json);
        printf("%s\n",out);
        free(out);
        */
        printJson(json);
        cJSON_Delete(json);
    }
}

void ConfigFile::dofile2(char *filename)
{
    FILE *f;long len;char *data;

    f=fopen(filename,"rb");fseek(f,0,SEEK_END);len=ftell(f);fseek(f,0,SEEK_SET);
    data=(char*)malloc(len+1);fread(data,1,len,f);fclose(f);
    doit2(data);
    free(data);
}

void ConfigFile::read_config_file(char *config_file)
{
    dofile2("../tests/a.txt");
}

QMap<int, void *> ConfigFile::getDevice(QString str)
{
    return device_map[str];
}

void *ConfigFile::get_gui_info_byname(QString name)
{
    QMap<int, void *> leds = getDevice("led");
    for (int i = 0; i < leds.count(); i++)
    {
         led *info = (led *)leds[i];
         //qDebug() << info->name << dev_driver->data_save.name["name"];
         if (info->name["name"] == name)
         {
            return info;
         }
    }

    QMap<int, void *> keys = getDevice("key");
    for (int i = 0; i < keys.count(); i++)
    {
         key *info = (key *)keys[i];
         //qDebug() << info->name << dev_driver->data_save.name["name"];
         if (info->name["name"] == name)
         {
            return info;
         }
    }
    return 0;
}
