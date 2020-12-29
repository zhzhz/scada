#ifndef CUSTOM_DATA_H
#define CUSTOM_DATA_H

#include <QObject>
#include "dev_driver.h"
#include "qdatastream.h"

class Custom_data : public QObject
{
    Q_OBJECT
public:
    explicit Custom_data(QObject *parent = nullptr);

    data_exchange l;

    friend QDataStream& operator>>(QDataStream&, Custom_data&);
    friend QDataStream& operator<<(QDataStream&, Custom_data&);

signals:

public slots:
};

#endif // CUSTOM_DATA_H
