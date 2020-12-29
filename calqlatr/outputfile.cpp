#include "outputfile.h"
#include <QDir>
#include <QTextStream>

OutputFile::OutputFile(QObject *parent) : QObject(parent)
{

}


bool OutputFile::exportToFile(const QString context, const QString fileName, const QString filePath)
{
    bool res = false;
    QDir folder;
    QFile file;

    bool pathCheck = true;
    if(!folder.exists(filePath)) {
        pathCheck = folder.mkpath(filePath);
    }

    if(pathCheck) {
        QString fileUrl = filePath + "/" + fileName;

        file.setFileName(fileUrl);
        file.open(QIODevice::ReadWrite);

        if(file.isOpen()) {
            QTextStream out(&file);

            if(file.size() > 0) {
                if(file.resize(0)) {
                    out << context;
                    out.flush();

                    res = true;
                }
            } else {
                out << context;
                out.flush();

                res = true;
            }
        }

        file.close();
    }

    return res;
}
