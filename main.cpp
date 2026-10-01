#include "mainwindow.h"
#include "geometric.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QString>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

    bool trsl = true;
    QString lang = "";
    if(argc == 2){
        if(QString(argv[1]).length() == 2 ){
            trsl = false;
            lang = QString(argv[1]); }
        else{ sizes::passedFile =argv[1];}
    }else if(argc > 2){
        sizes::passedFile =argv[1];
        if(QString(argv[2]).length() == 2){
            trsl = false;
            lang = QString(argv[1]);}
    }

    QString local = QLocale::languageToString(QLocale::system().language());


    if(local == QString("Italian") && trsl == true){
        lang = "it";
    }
    else if(local == QString("French") && trsl == true){
        lang = "fr";
    }
    else if(local == QString("Spanish") && trsl == true){
        lang = "es";
    }

    QTranslator trs;
    if( trs.load(":/res/" + lang + "_lang.qm"))  a.installTranslator(&trs);

    MainWindow w;
    w.show();
    return a.exec();
}
