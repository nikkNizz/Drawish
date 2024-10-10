#include "dcolors.h"
#include "ui_dcolors.h"
#include "geometric.h"
#include <QMessageBox>


dColors::dColors(QWidget *parent, QPixmap ePix ) :
    QDialog(parent),
    ui(new Ui::dColors)
{
    ui->setupUi(this);
    ui->labelThumb->setPixmap(ePix.scaled(200,170));
    origPix = ePix;
    newPix = origPix;
    // set color
    ui->redSpin->setValue(sizes::activeColor.red());
    ui->greenSpin->setValue(sizes::activeColor.green());
    ui->blueSpin->setValue(sizes::activeColor.blue());
    setBgColor();
}

dColors::~dColors()
{
    delete ui;
}

void dColors::setBgColor()
{

    QString r = QString::number(ui->redSpin->value());
    QString g = QString::number(ui->greenSpin->value());
    QString b = QString::number(ui->blueSpin->value());

    ui->showColorWidget->setStyleSheet("background-color: rgb(" + r + "," + g + "," + b + ")");
    sizes::activeColor=QColor(ui->redSpin->value(), ui->greenSpin->value(),ui->blueSpin->value() );
    spinBoxesActivate = true;
}

void dColors::on_redSpin_valueChanged(int arg1)
{
    if(spinBoxesActivate)    setBgColor();
}


void dColors::on_greenSpin_valueChanged(int arg1)
{
    if(spinBoxesActivate)    setBgColor();
}


void dColors::on_blueSpin_valueChanged(int arg1)
{
    if(spinBoxesActivate)     setBgColor();
}


void dColors::on_pushButton_2_clicked()//update
{
    origPix = newPix;
}


void dColors::on_pushButton_3_clicked()//cancel
{
    res =QDialog::Rejected;
    close();
}


void dColors::on_pushButton_4_clicked()//ok
{
    res =QDialog::Accepted;
    close();
}


void dColors::on_replaceButton_clicked()
{
     QImage Img = origPix.toImage();
     int minRed = ui->IFRED->value();
     int maxRed = ui->ifred->value();
     if(minRed > maxRed){QMessageBox::information(this, "Drawish", "The minimum value of red is higher than the maximum value");}

     int minGreen = ui->IFGREEN->value();
     int maxGreen = ui->ifgreen->value();
     if(minGreen > maxGreen){QMessageBox::information(this, "Drawish", "The minimum value of green is higher than the maximum value");}

     int minBlue = ui->IFBLUE->value();
     int maxBlue = ui->ifblue->value();
     if(minBlue > maxBlue){QMessageBox::information(this, "Drawish", "The minimum value of blue is higher than the maximum value");}


     for (int y = 0; y < Img.height(); ++y) {
           for (int x = 0; x < Img.width(); ++x) {
             QColor k= Img.pixelColor(x,y);
             if(k.red() >minRed && k.red() < maxRed && k.green() > minGreen && k.green() < maxGreen && k.blue() > minBlue && k.blue() < maxBlue){
                  Img.setPixelColor(x,y, sizes::activeColor);
             }
         }
     }
     newPix = QPixmap::fromImage(Img);
     ui->labelThumb->setPixmap( newPix.scaled(200,170));

}


void dColors::on_pushButton_clicked()
{
     QImage Img = origPix.toImage();
     QStringList startcols = ui->startColorsLine->text().split(" ");
     if(startcols.count() < 3){
         QMessageBox::information(this, "Drawish", "Please, enter 3 values in <gradient from> line ");
         return;
     }
     double startR=0; double startG =0; double startB =0;
     for(int i =0; i < startcols.count(); ++i){
         if(startcols.at(i)== ""){
             QMessageBox::information(this, "Drawish", "Please, enter 3 values in <gradient from> line ");
             return;
         }
     }
         startR = startcols.at(0).toDouble();
         startG = startcols.at(1).toDouble();
         startB = startcols.at(2).toDouble();
         if(startR < 0.0)startR=0;
         else if(startR > 255)startR =255;
         if(startG < 0.0)startG=0;
          else if(startG > 255)startG =255;
         if(startB < 0.0)startB=0;
          else if(startB > 255)startB =255;
   //----------------------------------------
         QStringList endcols = ui->endColorsLine->text().split(" ");
         if(endcols.count() < 3){
             QMessageBox::information(this, "Drawish", "Please, enter 3 values in <gradient to> line ");
             return;
         }
         int endR=0; int endG =0; int endB =0;
         for(int i =0; i < endcols.count(); ++i){
             if(endcols.at(i)== ""){
                 QMessageBox::information(this, "Drawish", "Please, enter 3 values in <gradient to> line ");
                 return;
             }
         }
             endR = endcols.at(0).toInt();
             endG = endcols.at(1).toInt();
             endB = endcols.at(2).toInt();
             if(endR < 0.0)endR=0;
             else if(endR > 255)endR =255;
             if(endG < 0.0)endG=0;
              else if(endG > 255)endG =255;
             if(endB < 0.0)endB=0;
              else if(endB > 255)endB =255;
     //--------------------------------------------
          double stepR = (endR - startR) / Img.width();
          double stepG = (endG - startG) / Img.width();
          double stepB = (endB - startB) / Img.width() ;

          for(int x=0; x < Img.width(); ++x){
              for(int y =0; y< Img.height(); ++y){
                   Img.setPixelColor(x, y, QColor(int(startR), int(startG), int(startB)));
              }
              startR += stepR;
              startG += stepG;
              startB += stepB;
          }
          newPix = QPixmap::fromImage(Img);
          ui->labelThumb->setPixmap( newPix.scaled(200,170));

}
