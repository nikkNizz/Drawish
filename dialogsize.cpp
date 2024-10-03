#include "dialogsize.h"
#include "ui_dialogsize.h"
#include "geometric.h"

DialogSize::DialogSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSize)
{
    ui->setupUi(this);
    ui->OriginalSize_label->setText(QString::number(sizes::areaWidth)+ " x " + QString::number(sizes::areaHeight));
    preTextW = QString::number(sizes::areaWidth);
    preTextH = QString::number(sizes::areaHeight);
    prePercW ="100";
    prePercH ="100";
    ui->newWidth_lineEdit->setText(preTextW);
    ui->newHeight_lineEdit->setText(preTextH);
    ui->wPerc_lineEdit->setText("100");
    ui->hPerc_lineEdit->setText("100");
}

DialogSize::~DialogSize()
{
    delete ui;

}


void DialogSize::on_newHeight_lineEdit_textEdited(const QString &arg1)
{
    QString aarg1 = arg1;
    if(aarg1 == ""){ aarg1 ="0";}
    bool ok;
    double nh = aarg1.toDouble(&ok);
    if(!ok){
        ui->newHeight_lineEdit->setText( preTextH);
    }else{
        preTextH = aarg1;
        double ah = sizes::areaHeight;
        double div = nh / ah ;
        div = div *100;
        ui->hPerc_lineEdit->setText(QString::number(div, 'g', 6));
        if(ui->keep_checkBox->isChecked()){
            ui->wPerc_lineEdit->setText( QString::number(div, 'g', 6));
            int nw = sizes::areaWidth * (div /100);
            ui->newWidth_lineEdit->setText(QString::number(nw));
        }
    }
}

void DialogSize::on_newWidth_lineEdit_textEdited(const QString &arg1)
{
    QString aarg1 = arg1;
    if(aarg1 == ""){ aarg1 ="0";}
    bool ok;
    double nw = aarg1.toDouble(&ok);
    if(!ok){
        ui->newWidth_lineEdit->setText( preTextW);
    }else{
        preTextW = aarg1;
        double aw = sizes::areaWidth;
        double div = nw / aw ;
        div = div *100;
        ui->wPerc_lineEdit->setText(QString::number(div, 'g', 6));
        if(ui->keep_checkBox->isChecked()){
            ui->hPerc_lineEdit->setText( QString::number(div, 'g',6));
            int nh = sizes::areaHeight * (div /100);
            ui->newHeight_lineEdit->setText(QString::number(nh));
        }
    }
}


void DialogSize::on_wPerc_lineEdit_textEdited(const QString &arg1)
{
    QString aarg1 = arg1;
    if(aarg1 == ""){ aarg1 ="0";}
    bool ok;
    double nw = aarg1.toDouble(&ok);
    if(!ok){
        ui->newWidth_lineEdit->setText( prePercW);
    }else{
        prePercW = aarg1;
        double aw = sizes::areaWidth;
        double div = nw * aw ;
        div = div /100;
        ui->newWidth_lineEdit->setText(QString::number(int(div)));
        if(ui->keep_checkBox->isChecked()){
            ui->hPerc_lineEdit->setText( aarg1);
            double nh = sizes::areaHeight * nw /100 ;
            ui->newHeight_lineEdit->setText(QString::number(int(nh)));
        }
    }
}


void DialogSize::on_hPerc_lineEdit_textEdited(const QString &arg1)
{
    QString aarg1 = arg1;
    if(aarg1 == ""){ aarg1 ="0";}
    bool ok;
    double nh = aarg1.toDouble(&ok);
    if(!ok){
        ui->newHeight_lineEdit->setText( prePercH);
    }else{
        prePercH = aarg1;
        double ah = sizes::areaHeight;
        double div = nh * ah ;
        div = div /100;
        ui->newHeight_lineEdit->setText(QString::number(int(div)));
        if(ui->keep_checkBox->isChecked()){
            ui->wPerc_lineEdit->setText( aarg1);
            double nw = sizes::areaWidth * nh/100 ;
            ui->newWidth_lineEdit->setText(QString::number(int(nw)));
        }
    }
}


void DialogSize::on_resizeButton_clicked()
{
    sizes::areaWidth = ui->newWidth_lineEdit->text().toInt();
    sizes::areaHeight = ui->newHeight_lineEdit->text().toInt();
    returned = 1;
    close();
}


void DialogSize::on_scaleButton_clicked()
{
    sizes::areaWidth = ui->newWidth_lineEdit->text().toInt();
    sizes::areaHeight = ui->newHeight_lineEdit->text().toInt();
    returned = 2;
    close();
}


void DialogSize::on_createSelectionButton_clicked()
{
   bool ok;
   int xs = ui->newXsel_lineEdit->text().toInt(&ok);
   if(!ok){ ui->newXsel_lineEdit->setText(""); ui->newXsel_lineEdit->setFocus(); return; }

   int ys = ui->newYsel_lineEdit->text().toInt(&ok);
   if(!ok){ ui->newYsel_lineEdit->setText(""); ui->newYsel_lineEdit->setFocus(); return; }

   int ws = ui->newWsel_lineEdit->text().toInt(&ok);
   if(!ok){ ui->newWsel_lineEdit->setText(""); ui->newWsel_lineEdit->setFocus(); return; }

   int hs = ui->newHsel_lineEdit->text().toInt(&ok);
   if(!ok){ ui->newHsel_lineEdit->setText(""); ui->newHsel_lineEdit->setFocus(); return; }

   sizes::selX = xs;
   sizes::selY = ys;
   sizes::selW = ws;
   sizes::selH = hs;

   returned =3;
   close();
}
