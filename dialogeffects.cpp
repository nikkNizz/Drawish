#include "dialogeffects.h"

#include "ui_dialogeffects.h"
#include <QPixmap>


DialogEffects::DialogEffects(QWidget *parent, QPixmap ePix) :
    QDialog(parent),
    ui(new Ui::DialogEffects)
{
    ui->setupUi(this);
    ui->labelThumb->setPixmap(ePix.scaled(200,170));
    origPix = ePix;
    newPix = origPix;
}

DialogEffects::~DialogEffects()
{
    delete ui;
}


QImage DialogEffects::gamma( double exp)
{
    QImage qImgRet= origPix.toImage();
    for (int y = 0; y < qImgRet.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(qImgRet.scanLine(y));
        for (int x = 0; x < qImgRet.width(); ++x) {
            QRgb &rgb = line[x];

            const double r = qRed(rgb) / 255.0;
            const double g = qGreen(rgb) / 255.0;
            const double b = qBlue(rgb) / 255.0;
            qImgRet.setPixelColor(x, y,
              QColor(
                255 * std::pow(r, exp),
                255 * std::pow(g, exp),
                255 * std::pow(b, exp)));
        }
    }
    return qImgRet;
}


void DialogEffects::on_gammaSlider_sliderReleased()
{
    int value = ui->gammaSlider->value();
    QImage imgg;
    if(value < 50){
        double exp = value /10;  //0-49  = 0 - 1/5
        exp = 1/exp;
        imgg= gamma(exp);
    }
    else{
        double exp = value -49;  //50-99 = 1 - 5
        exp = exp /10;
        imgg = gamma(exp);
    }
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));

}


void DialogEffects::on_valSlider_sliderReleased()
{    
    int value = ui->valSlider->value();
    int vb = value*2.5;

    QImage imgg;
    imgg = contrast(vb);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));

}

QImage DialogEffects::contrast(int v)
{
     QImage qImgRet= origPix.toImage();
     double F = (259*(v+255)) / (255 *(259-v));
     for (int y = 0; y < qImgRet.height(); ++y) {
         QRgb *line = reinterpret_cast<QRgb*>(qImgRet.scanLine(y));
         for (int x = 0; x < qImgRet.width(); ++x) {
             QRgb &rgb = line[x];

             int r = qRed(rgb) ;
             int g = qGreen(rgb);
             int b = qBlue(rgb);

             r = (F * (r-128) +128);
             g =  (F * (g-128) +128);
             b = (F * (b-128) +128);
             if(r < 0) r =0;
             else if(r > 255)r = 255;
             if(g < 0) g =0;
             else if(g > 255)g = 255;
             if(b < 0) b =0;
             else if(b > 255)b = 255;
             qImgRet.setPixelColor(x, y, QColor(r,g,b));

         }
     }
     return qImgRet;
}

QImage DialogEffects::highlightEffect(double v)
{
    QImage Img= origPix.toImage();
    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 0; x < Img.width(); ++x) {
            QRgb &rgb = line[x];

            int r = qRed(rgb) ;
            int g = qGreen(rgb);
            int b = qBlue(rgb);
            if( r > g && r > b){
                r *= v;
                if(r > 255)r = 255;
                else if(r < 0) r = 0;
            }
            else if(g > r && g > b){
                g *= v;
                if(g > 255)g = 255;
                else if(g < 0) g = 0;
            }
            else if(b > r && b > g){
                b *=v;
                if(b > 255)b = 255;
                else if(b < 0) b = 0;
            }
                Img.setPixelColor(x, y, QColor(r,g,b));
            }
        }
    return Img;
}


void DialogEffects::on_highlightPlusButton_clicked()
{
    QImage imgg = highlightEffect(1.1);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}


void DialogEffects::on_highlightMinusButton_clicked()
{
    QImage imgg = highlightEffect(0.9);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}

void DialogEffects::on_pushButton_3_clicked()
{
    // update image
    origPix = newPix;
}


void DialogEffects::on_redSlider_sliderReleased()
{
    int value = ui->redSlider->value();
    QImage imgg;
    // 0-49   -255..1   50-99  1..255
    if(value < 50){
        value = 50 - value;
        value = value * -5;

    }else if(value > 50){
        value = value - 50;
        value = value * 5;
    }
    imgg = addColor(0, value);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
}

QImage DialogEffects::addColor(int color, int v)
{
    QImage Img= origPix.toImage();
    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 0; x < Img.width(); ++x) {
            QRgb &rgb = line[x];

            int r = qRed(rgb) ;
            int g = qGreen(rgb);
            int b = qBlue(rgb);
            if( color == 0){
                r += v;
                if(r > 255)r = 255;
                else if(r < 0) r = 0;
            }
            else if(color == 1){
                g += v;
                if(g > 255)g = 255;
                else if(g < 0) g = 0;
            }
            else if(color == 2){
                b +=v;
                if(b > 255)b = 255;
                else if(b < 0) b = 0;
            }
                Img.setPixelColor(x, y, QColor(r,g,b));
            }
        }
    return Img;
}

void DialogEffects::on_greenSlider_sliderReleased()
{
    int value = ui->greenSlider->value();
    QImage imgg;
    // 0-49   -255..1   50-99  1..255
    if(value < 50){
        value = 50 - value;
        value = value * -5;

    }else if(value > 50){
        value = value - 50;
        value = value * 5;
    }
    imgg = addColor(1, value);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
}


void DialogEffects::on_blueSlider_sliderReleased()
{
    int value = ui->blueSlider->value();
    QImage imgg;
    // 0-49   -255..1   50-99  1..255
    if(value < 50){
        value = 50 - value;
        value = value * -5;

    }else if(value > 50){
        value = value - 50;
        value = value * 5;
    }
    imgg = addColor(2, value);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
}


void DialogEffects::on_noiseMinusButton_clicked()
{
    QImage imgg = noise();
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck_2->isChecked()){origPix = newPix;}
}

QImage DialogEffects::noise()
{
    QImage Img= origPix.toImage();

    if(Img.width() < 3 || Img.height() < 3)return Img;

    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 1; x < Img.width(); ++x) {
            QRgb &rgb = line[x-1];
            QRgb &rgb2 = line[x];

            int r = qRed(rgb) ;
            int g = qGreen(rgb);
            int b = qBlue(rgb);
            int r1 = qRed(rgb2) ;
            int g1 = qGreen(rgb2);
            int b1 = qBlue(rgb2);
            int ar = abs(r-r1);
            int ag = abs(g-g1);
            int ab = abs(b-b1);
            ar = ar + ag + ab;
            if(ar !=0 && ar < 10 ){
                r1 =r;
                g1 = g;
                b1 = b;
            }
            else if(ar != 0 && ar < 19){
                r1 = (r1 + r)/2;
                g1 = (g1 + g)/2;
                b1 = (b1 + b)/2;
            }

                Img.setPixelColor(x, y, QColor(r1,g1,b1));
            }
        }
    return Img;
}



void DialogEffects::on_blurSlider_2_sliderReleased()
{
    int radius = ui->blurSlider_2->value(); // 3 - 10
    QImage Img = origPix.toImage();

    if(Img.width() < radius+1){ return;}
    if(Img.height() < radius+1){ return;}

    for (int y = radius; y < Img.height(); ++y) {
            for (int x = radius; x < Img.width(); ++x) {
                int totr =0;
                int totg =0;
                int totb =0;
                int totpix=0;
                for(int z =0; z < radius; ++z){
                    totr += Img.pixelColor(x-z, y).red();
                    totg += Img.pixelColor(x-z, y).green();
                    totb += Img.pixelColor(x-z, y).blue();
                    totpix++;
                }
                for(int z =0; z < radius; ++z){
                    totr += Img.pixelColor(x, y-z).red();
                    totg += Img.pixelColor(x, y-z).green();
                    totb += Img.pixelColor(x, y-z).blue();
                    totpix++;
                }
                totr = totr/totpix;
                totg = totg/totpix;
                totb = totb/totpix;
                int central = radius/2;
                Img.setPixelColor(x-central, y-central, QColor(totr,totg,totb));
                }
            }

        newPix = QPixmap::fromImage(Img);
        ui->labelThumb->setPixmap( newPix.scaled(200,170));
}


void DialogEffects::on_pushButton_2_clicked() // cancel
{
    modifPix = origPix;
    res =QDialog::Rejected;
    close();

}


void DialogEffects::on_pushButton_clicked() // ok
{

    modifPix = origPix;
    res =QDialog::Accepted;
    close();
}


void DialogEffects::on_pixelizeButton_clicked()
{
     QImage Img = origPix.toImage();
     if(Img.width() < 6 || Img.height() < 6){return;}
     for (int y = 0; y < Img.height(); ++y) {
          for (int x = 5; x < Img.width(); x+=6) {
             QColor k= Img.pixelColor(x,y);
             for(int i=0; i < 6; ++i){
                 Img.setPixelColor(x-i,y, k);
             }
         }
     }
     newPix = QPixmap::fromImage(Img);
     ui->labelThumb->setPixmap( newPix.scaled(200,170));
     if(ui->autoUpdateCheck_2->isChecked()){origPix = newPix;}
}


