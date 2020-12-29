#include "inputfile.h"
#include "qfile.h"

InputFile::InputFile(QObject *parent) : QObject(parent)
{

}

QString InputFile::readfile(const QString fileName, const QString filePath)
{
    QString fileUrl = filePath + "/" + fileName;
    QFile file;

    file.setFileName(fileUrl);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray t = file.readAll();
    file.close();
    return QString(t);
}
