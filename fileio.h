#ifndef FILEIO_H
#define FILEIO_H
#include <QString>

class fileIO
{
public:
    fileIO();
    void createFile(QString content, QString path);
    QString readFile(QString path);
};

#endif // FILEIO_H
