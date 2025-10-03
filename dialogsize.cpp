#include "dialogsize.h"
#include "ui_dialogsize.h"
#include "geometric.h"

DialogSize::DialogSize(QWidget *parent, int pixW, int pixH, double Zoom) :
    QDialog(parent),
    ui(new Ui::DialogSize)
{
    ui->setupUi(this);
    dZoom = Zoom;
    ui->OriginalSize_label->setText(QString::number(pixW)+ " x " + QString::number(pixH));
    preTextW = QString::number(pixW);
    preTextH = QString::number(pixH);
    pixWW = pixW;
    pixHH = pixH;
    prePercW ="100";
    prePercH ="100";
    ui->newWidth_lineEdit->setText(preTextW);
    ui->newHeight_lineEdit->setText(preTextH);
    ui->wPerc_lineEdit->setText("100");
    ui->hPerc_lineEdit->setText("100");
    // load selection data
    if(sizes::selectData == "0 0 0 0" && sizes::isSelectionOn){
        ui->newXsel_lineEdit->setText(QString::number(sizes::selX / dZoom));
        ui->newYsel_lineEdit->setText(QString::number(sizes::selY / dZoom));
        ui->newWsel_lineEdit->setText(QString::number(sizes::selW / dZoom));
        ui->newHsel_lineEdit->setText(QString::number(sizes::selH / dZoom));
    }
    else{
        QStringList sdata= sizes::selectData.split(" ");
        ui->newXsel_lineEdit->setText(sdata.at(0));
        ui->newYsel_lineEdit->setText(sdata.at(1));
        ui->newWsel_lineEdit->setText(sdata.at(2));
        ui->newHsel_lineEdit->setText(sdata.at(3));
    }
    getRatio(pixW, pixH);
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
        double ah = double(pixHH);
        double div = nh / ah ;
        div = div * 100.00;
        ui->hPerc_lineEdit->setText(QString::number(div, 'g', 6));
        if(ui->keep_checkBox->isChecked()){
            ui->wPerc_lineEdit->setText( QString::number(div, 'g', 6));
            int nw = pixWW * (div /100);
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
        double aw = pixWW;
        double div = nw / aw ;
        div = div * 100.00;
        ui->wPerc_lineEdit->setText(QString::number(div, 'g', 6));
        if(ui->keep_checkBox->isChecked()){
            ui->hPerc_lineEdit->setText( QString::number(div, 'g',6));
            int nh = pixHH * (div /100);
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
        double aw = double(pixWW);
        double div = nw * aw ;
        div = div / 100.00;
        ui->newWidth_lineEdit->setText(QString::number(int(div)));
        if(ui->keep_checkBox->isChecked()){
            ui->hPerc_lineEdit->setText( aarg1);
            double nh = pixHH * nw / 100.00 ;
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
        double ah = pixHH;
        double div = nh * ah ;
        div = div / 100.00;
        ui->newHeight_lineEdit->setText(QString::number(int(div)));
        if(ui->keep_checkBox->isChecked()){
            ui->wPerc_lineEdit->setText( aarg1);
            double nw = pixWW * nh/100 ;
            ui->newWidth_lineEdit->setText(QString::number(int(nw)));
        }
    }
}


void DialogSize::on_resizeButton_clicked()
{
    pixWW = ui->newWidth_lineEdit->text().toInt();
    pixHH = ui->newHeight_lineEdit->text().toInt();
    returned = 1;
    saveSelData();
    close();
}


void DialogSize::on_scaleButton_clicked()
{
    pixWW = ui->newWidth_lineEdit->text().toInt();
    pixHH = ui->newHeight_lineEdit->text().toInt();
    returned = 2;
    saveSelData();
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
    sizes::selW = double(ws);
    sizes::selH = double(hs);

    returned =3;
    saveSelData();
    close();
}

void DialogSize::saveSelData()
{
    QString a = ui->newXsel_lineEdit->text();
    if(a == "") a = "0";
    sizes::selectData = a + " ";
    a = ui->newYsel_lineEdit->text();
    if(a == "") a = "0";
    sizes::selectData += a + " ";
    a = ui->newWsel_lineEdit->text();
    if(a == "") a = "0";
    sizes::selectData += a + " ";
    a = ui->newHsel_lineEdit->text();
    if(a == "") a = "0";
    sizes::selectData += a;
}

void DialogSize::getRatio(int aw, int ah)
{
    double rapp = double(aw) / double(ah);
    int r = 100;
    double z, b;
    for(int i=1; i< 101; ++i){
        z = rapp * double(i);
        b = z - int(z);
        if(b < 0.005){
            r = i;
            break;
        }
    }
    ui->spinRatio1->setValue(int(z));
    ui->spinRatio2->setValue(r);
}

void DialogSize::on_scaleButton_2_clicked() // fit to new aspect ratio (keep area)
{
    int newW = ui->spinRatio1->value();
    int newH = ui->spinRatio2->value();

    double area = pixWW * pixHH;
    double rapp = double(newW) * double(newH);
    rapp = sqrt(area / rapp);
    pixWW = rapp * newW;
    pixHH = rapp * newH;
    if(pixWW < 4)pixWW =4;
    if(pixHH < 4)pixHH =4;
    returned = 2;
    saveSelData();
    close();
}

