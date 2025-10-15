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
    if(argc == 2){
        if(QString(argv[1]) == "en"){ trsl = false; }
        else{ sizes::passedFile =argv[1];}
    }else if(argc > 2){
        sizes::passedFile =argv[1];
        if(QString(argv[2]) == "en"){ trsl = false; }
    }

    QString local =  QLocale::languageToString(QLocale::system().language());

    QTranslator trs;
    
    if(local == QString("Italian") && trsl == true){
        trs.load(":/res/it_lang.qm");
        a.installTranslator(&trs);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
