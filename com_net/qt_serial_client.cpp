#include "qt_serial_client.h"
#include "qdebug.h"


Qt_serial_client::Qt_serial_client()
{
    //创建串口对象
    serial = new QSerialPort;
    //设置串口名
    serial->setPortName("COM5");
    //设置波特率
    serial->setBaudRate(QSerialPort::Baud9600);
    //设置数据位数
    serial->setDataBits(QSerialPort::Data8);
    //设置奇偶校验
    serial->setParity(QSerialPort::NoParity);
    //设置停止位
    serial->setStopBits(QSerialPort::OneStop);
    //设置流控制
    serial->setFlowControl(QSerialPort::NoFlowControl);
    //打开串口
    bool f = serial->open(QIODevice::ReadWrite);

    qDebug() << "打开串口" << f;

    connect(serial, &QSerialPort::readyRead, this, &Qt_serial_client::serialPort_readyRead);
    //connect(serial, SIGNAL(readyRead()), this, SLOT(serialPort_readyRead()));
}

Qt_serial_client::~Qt_serial_client()
{
    delete serial;
}

void Qt_serial_client::write_data(QByteArray &data)
{
    serial->write(data);
}

void Qt_serial_client::serialPort_readyRead()
{
    QByteArray buffer;
    //读取缓冲区数据

    buffer = serial->readAll();
    //qDebug() << "Qt_serial_client::serialPort_readyRead()" << buffer;
    emit data_come(buffer);
}
