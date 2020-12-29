#ifndef OUTPUTFILE_H
#define OUTPUTFILE_H

#include <QObject>

class OutputFile : public QObject
{
    Q_OBJECT
public:
    explicit OutputFile(QObject *parent = nullptr);
    Q_INVOKABLE bool exportToFile(const QString context, const QString fileName, const QString filePath);
signals:

};

#endif // OUTPUTFILE_H
