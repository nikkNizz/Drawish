#include "dcolors.h"
#include "ui_dcolors.h"
#include "geometric.h"
#include <QMessageBox>
#include <qpainter.h>
#include <QInputDialog>
#include <QClipboard>


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
    QString hexr, hexg, hexb;
    hexr.setNum(ui->redSpin->value(), 16);
    hexg.setNum(ui->greenSpin->value(), 16);
    hexb.setNum(ui->blueSpin->value(), 16);
    ui->hexValButton->setText("#" + hexr + " " + hexg + " " + hexb);
}

QString dColors::writeRgb()
{
    QString r = QString::number(ui->redSpin->value());
    QString g = QString::number(ui->greenSpin->value());
    QString b = QString::number(ui->blueSpin->value());
    return r + " " + g + " " + b;
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


void dColors::replaceColors(int opt)
{
     QImage Img = origPix.toImage();
     int minRed = ui->IFRED->value();
     int maxRed = ui->ifred->value();
     if(minRed > maxRed){QMessageBox::information(this, "Drawish", tr("The minimum value of red is higher than the maximum value"));}

     int minGreen = ui->IFGREEN->value();
     int maxGreen = ui->ifgreen->value();
     if(minGreen > maxGreen){QMessageBox::information(this, "Drawish", tr("The minimum value of green is higher than the maximum value"));}

     int minBlue = ui->IFBLUE->value();
     int maxBlue = ui->ifblue->value();
     if(minBlue > maxBlue){QMessageBox::information(this, "Drawish", tr("The minimum value of blue is higher than the maximum value"));}

     int counter =0;
     // 1 repl. active color, 2 +red, 3 - red, 4 +green, 5 -green, 6 +blue, 7 -blue
     // 0 header
     int acolor;
     int diff = 15;
     if(opt > 1){
         diff = QInputDialog::getInt(this, "Drawish", "Enter increment/decrement value ", 15, 0, 255);
     }
     for (int y = 0; y < Img.height(); ++y) {
           for (int x = 0; x < Img.width(); ++x) {
             QColor k= Img.pixelColor(x,y);
             if(k.red() >minRed && k.red() < maxRed && k.green() > minGreen && k.green() < maxGreen && k.blue() > minBlue && k.blue() < maxBlue){
                 switch (opt) {
                 case 1:Img.setPixelColor(x,y, sizes::activeColor);
                     break;
                 case 2:
                     acolor = k.red() + diff;
                     if(acolor > 255) acolor = 255;
                     Img.setPixelColor(x, y, QColor(acolor, k.green(), k.blue()) );
                     break;
                 case 3:
                     acolor = k.red() - diff;
                     if(acolor < 0) acolor = 0;
                     Img.setPixelColor(x, y, QColor(acolor, k.green(), k.blue()) );
                     break;
                 case 4:
                     acolor = k.green() + diff;
                     if(acolor > 255) acolor = 255;
                     Img.setPixelColor(x, y, QColor(k.red(), acolor, k.blue()) );
                     break;
                 case 5:
                     acolor = k.green() - diff;
                     if(acolor < 0) acolor = 0;
                     Img.setPixelColor(x, y, QColor(k.red(), acolor, k.blue()) );
                     break;
                 case 6:
                     acolor = k.blue() + diff;
                     if(acolor > 255) acolor = 255;
                     Img.setPixelColor(x, y, QColor(k.red(), k.green(), acolor) );
                     break;
                 case 7:
                     acolor = k.blue() - diff;
                     if(acolor < 0) acolor = 0;
                     Img.setPixelColor(x, y, QColor(k.red(), k.green(), acolor) );
                      break;
                 }
                 counter++;
             }
         }
     }
     newPix = QPixmap::fromImage(Img);
     ui->labelThumb->setPixmap( newPix.scaled(200,170));
     ui->label_replaced->setText(QString::number(counter) + tr(" pixel replaced"));
     if(ui->checkAuto->isChecked()) origPix = newPix;

}


void dColors::on_pushButton_clicked()// gradient
{
     QImage Img = origPix.toImage();
     QStringList startcols = ui->startColorsLine->text().split(" ");
     if(startcols.count() < 3){
         QMessageBox::information(this, "Drawish", tr("Please, enter 3 values in <gradient from> line "));
         return;
     }
     double startR=0; double startG =0; double startB =0;
     for(int i =0; i < startcols.count(); ++i){
         if(startcols.at(i)== ""){
             QMessageBox::information(this, "Drawish", tr("Please, enter 3 values in <gradient from> line "));
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
             QMessageBox::information(this, "Drawish", tr("Please, enter 3 values in <gradient to> line "));
             return;
         }
         int endR=0; int endG =0; int endB =0;
         for(int i =0; i < endcols.count(); ++i){
             if(endcols.at(i)== ""){
                 QMessageBox::information(this, "Drawish", tr("Please, enter 3 values in <gradient to> line "));
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
              if(ui->linearRadio->isChecked()){
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
              }
              else if(ui->diagonalRadio->isChecked()){
                  QLinearGradient gradient(QPoint(0,0), QPoint(Img.width()-1, Img.height()-1));
                  gradient.setColorAt(0, QColor(startR, startG, startB));
                  gradient.setColorAt(1, QColor(endR, endG, endB));
                  QBrush brush(gradient);
                  QPainter p(&origPix);
                  p.setBrush(brush);
                  p.drawRect(-1,-1, origPix.width()+1, origPix.height()+1);
                  p.end();
                  newPix = origPix;
              }
              else if(ui->RadialRadio->isChecked()){
                  int radius = qMin(Img.width(), Img.height());
                  QRadialGradient gradient(QPoint(Img.width()/2, Img.height()/2), radius/2);                  
                  gradient.setColorAt(0, QColor(startR, startG, startB));
                  gradient.setColorAt(1, QColor(endR, endG, endB));
                  QBrush brush(gradient);

                  QPainter p(&origPix);
                  p.setBrush(brush);
                  p.drawRect(-1,-1, origPix.width()+1, origPix.height()+1);
                  p.end();
                  newPix = origPix;
              }

          ui->labelThumb->setPixmap( newPix.scaled(200,170));
          if(ui->checkAuto->isChecked()) origPix = newPix;
}



void dColors::on_comboReplace_currentIndexChanged(int index)
{
    // 1 repl. active color, 2 +red, 3 - red, 4 +green, 5 -green, 6 +blue, 7 -blue
    // 0 header
    if(index >0){
        replaceColors(index);
    }
    ui->comboReplace->setCurrentIndex(0);
}


void dColors::on_hexValButton_clicked()
{
     QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(ui->hexValButton->text());
     QMessageBox::information(this,"Drawish", "Copied!");
}



void dColors::on_getColorBtn1_clicked()
{
    ui->startColorsLine->setText(writeRgb());
}


void dColors::on_getColorBtn2_clicked()
{
    ui->endColorsLine->setText(writeRgb());
}

