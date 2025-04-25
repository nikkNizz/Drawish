#include "fileio.h"
#include <QTextStream>
#include <QFile>
#include <QColor>
#include <QDir>
#include <qpainter.h>
#include <qpixmap.h>


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

void fileIO::saveFav_color(QColor col, QString path)
{

    QPixmap oneColor(40, 32);
    oneColor.fill(col);

    if(QFile::exists(path) == false){
        oneColor.save(path);
    }else{
        QPixmap oldPalette(path);
        int len = oldPalette.width();
        if(len > 639){
            len = 600;
        }
        oldPalette = oldPalette.copy(0, 0, len, 32);
        QPixmap newPalette(len+40, 40);
        newPalette.fill(Qt::white);
        QPainter p(&newPalette);
        p.drawPixmap(39,0, oldPalette);
        p.end();
        QPainter p2(&newPalette);
        p2.drawPixmap(0,0, oneColor);
        p2.end();
        newPalette.save(path);
    }
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
