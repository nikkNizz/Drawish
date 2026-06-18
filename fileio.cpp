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
    QPixmap oneColor(220, 24);
    oneColor.fill(col);

    // find color name
    QString colorNames = readFile(":/res/listcolors.txt");
    colorNames.replace("\r", "");

    QStringList colorList = colorNames.split("\n");
    QStringList colorLine;
    int refRed = col.red();
    int refGreen = col.green();
    int refBlue = col.blue();
    int red, green, blue;
    int max = 1000;
    QString found ="f";
    foreach (QString row, colorList) {
        if(row.startsWith("--end")) break;
        colorLine = row.split("|");
        red =colorLine.at(1).toInt();
        green = colorLine.at(2).toInt();
        blue = colorLine.at(3).toInt();
        red = abs(red - refRed) + abs(green-refGreen) + abs(blue-refBlue);
        if(red < max){
            max = red;
            found = colorLine.at(0);
        }
    }
    if(max > 0){ found = "~" + found;}
    // ----------------------

    QPainter pai(&oneColor);
    if(refRed > 128){
        pai.setPen(Qt::black);
    }else{
        pai.setPen(Qt::white);
    }
    pai.setFont(QFont("Helvetica", 12));
    pai.drawText(4,14, found);
    pai.end();

    if(QFile::exists(path) == false){
        oneColor.save(path);
    }else{
        QPixmap oldPalette(path);
        int len = oldPalette.height();
        if(len > 480){
            len = 456;
        }
        oldPalette = oldPalette.copy(0, 0, 220, len);

        QPixmap newPalette(220, len + 24);
        newPalette.fill(Qt::white);
        QPainter p(&newPalette);
        p.drawPixmap(0,0, oneColor);
        p.drawPixmap(0,24, oldPalette);
        p.end();
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

