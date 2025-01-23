#include "mainwindow.h"
#include "geometric.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString local =  QLocale::languageToString(QLocale::system().language());

    QTranslator trs;
    trs.load(":/it_lang.qm");
    if(local == QString("Italian")){
        a.installTranslator(&trs);
    }
    if(argc > 1){
        sizes::passedFile =argv[1];
    }

    MainWindow w;
    w.show();
    return a.exec();
}
