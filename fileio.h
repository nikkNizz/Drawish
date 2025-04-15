#ifndef FILEIO_H
#define FILEIO_H
#include <QString>
#include <QColor>

class fileIO
{
public:
    fileIO();
    void createFile(QString content, QString path);
    void saveFav_color(QColor col, QString path);
    QString readFile(QString path);
};

#endif // FILEIO_H
