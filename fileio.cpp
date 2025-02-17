#include "fileio.h"
#include <QTextStream>
#include <QFile>

fileIO::fileIO() {}

void fileIO::createFile(QString content, QString path)
{
        QFile ff(path);
        if(!ff.open(QFile::WriteOnly | QFile::Text )){}
        QTextStream out(&ff);
        out << content;
        ff.flush();
        ff.close();

}

QString fileIO::readFile(QString path)
{
    if(QFile::exists(path)){
        QString a="";
        QFile fl(path);
        if(!fl.open(QFile::ReadOnly | QFile::Text)){}
        QTextStream in(&fl);
        a = in.readAll();
        fl.close();
        return a;
    }
    return "";
}
