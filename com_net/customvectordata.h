#ifndef CUSTOMVECTORDATA_H
#define CUSTOMVECTORDATA_H

#include <QObject>
#include "dev_driver.h"
#include "qdatastream.h"

class CustomVectorData : public QObject
{
    Q_OBJECT
public:
    explicit CustomVectorData(QObject *parent = nullptr);

    QVector<data_exchange> l;

    friend QDataStream& operator>>(QDataStream&, CustomVectorData&);
    friend QDataStream& operator<<(QDataStream&, CustomVectorData&);

signals:

};

#endif // CUSTOMVECTORDATA_H
