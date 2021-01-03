#ifndef INPUTFILE_H
#define INPUTFILE_H

#include <QObject>

class InputFile : public QObject
{
    Q_OBJECT
public:
    explicit InputFile(QObject *parent = nullptr);
    Q_INVOKABLE QString readfile(const QString fileName, const QString filePath);

signals:

};

#endif // READFILE_H
