#include "dialogeffects.h"

#include "ui_dialogeffects.h"
#include <QPixmap>
#include <qmessagebox.h>


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
void DialogEffects::on_contrastPlus_clicked()
{
    QImage imgg = contrast(1);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}


void DialogEffects::on_contrastLess_clicked()
{
    QImage imgg = contrast(-1);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}



QImage DialogEffects::contrast(int v)
{
    QImage qImgRet= origPix.toImage();

    int lighter = qImgRet.pixelColor(0,0).lightness();
    int darker = lighter;
    int p;

    for (int y = 0; y < qImgRet.height(); ++y) {

            for (int x = 0; x < qImgRet.width(); ++x) {
            p = qImgRet.pixelColor(x, y).lightness();
                if( p > lighter) lighter =p;
                if( p < darker) darker =p;
            }
    }
    int medium= (lighter + darker) / 2 ;
    QColor k;
    for (int y = 0; y < qImgRet.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(qImgRet.scanLine(y));
        for (int x = 0; x < qImgRet.width(); ++x) {
            QRgb &rgb = line[x];
            k = QColor(rgb);
            p= k.lightness();
            if( v == 1){
            if(p > medium) k = k.lighter(105);
            else if( p < medium) k = k.darker(110);
            }
            else{
                if(p > medium) k = k.darker(110);
                else if( p < medium) k = k.lighter(105);
            }
           qImgRet.setPixelColor(x, y, k);
        }
    }
    return qImgRet;

}

QImage DialogEffects::highlightEffect(int v)
{
    QImage Img= origPix.toImage();
    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 0; x < Img.width(); ++x) {
            QRgb &rgb = line[x];
            QColor k(qRed(rgb), qGreen(rgb), qBlue(rgb));
            if(v > 0){
                   k = k.lighter(105);
            }
            else{
                k = k.darker(105);
            }
            Img.setPixelColor(x, y, k);
            }
        }
    return Img;
}


void DialogEffects::on_highlightPlusButton_clicked()
{
    QImage imgg = highlightEffect(1);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}


void DialogEffects::on_highlightMinusButton_clicked()
{
    QImage imgg = highlightEffect(-1);
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


void DialogEffects::on_saturationPlusButton_2_clicked()
{
    QImage imgg = setSaturationHue(1);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}


void DialogEffects::on_saturationMinusButton_2_clicked()
{
    QImage imgg = setSaturationHue(-1);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}

QImage DialogEffects::setSaturationHue(int v)
{
    QImage Img= origPix.toImage();
    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 0; x < Img.width(); ++x) {
            QRgb &rgb = line[x];
            QColor k(qRed(rgb), qGreen(rgb), qBlue(rgb));
            QColor hsvC = k.toHsv();
            int sat = hsvC.saturation();
            int hue = hsvC.hue();

            if(v ==1){
                sat += 2;
            }
            else if(v == -1){
                sat -= 2;
            }
            else if(v == 2){
                hue += 2;
            }
            else if(v == -2){
                hue -= 2;
            }
            if(sat > 255) sat = 255;
            else if(sat < 0) sat = 0;
            if(hue > 359) hue = 0;
            else if(hue < 0) hue = 0;
            hsvC.setHsv(hue, sat, hsvC.value());
            Img.setPixelColor(x, y, hsvC.toRgb());
        }
    }
    return Img;
}

void DialogEffects::on_huePlus_clicked()
{
    QImage imgg = setSaturationHue(2);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
}

void DialogEffects::on_hueMinus_clicked()
{
    QImage imgg = setSaturationHue(-2);
    newPix = QPixmap::fromImage(imgg);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
    if(ui->autoUpdateCheck->isChecked()){origPix = newPix;}
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



void DialogEffects::on_histograButton_clicked()
{
    QImage Img = origPix.toImage();

    // create sumList
    QList <int> sumList;
    for(int i=0; i < 256 ; ++i){ sumList.append(0);}
    // loop pixel in image
    int v =0;
    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 0; x < Img.width(); ++x) {
            QRgb &rgb = line[x];
            QColor k(rgb);
            //QColor k(qRed(rgb), qGreen(rgb), qBlue(rgb));
            QColor hsvC = k.toHsv();
            v = hsvC.value();
            sumList.replace(v, sumList.at(v) + 1);
        }
    }
    // progressive sum (from 1)
    for(int i = 1; i < 256 ; ++i ){
        sumList.replace(i, sumList.at(i-1) + sumList.at(i));
    }
    // find first non-zero value
    int firstVal=0;
    for(int i=0; i < 256; ++i ){
        if(sumList.at(i) > 0){
            firstVal = sumList.at(i);
            break;
        }
    }
    // find denominator for next operation
    int denominator = (Img.width() * Img.height()) -1;

    // find new hsv value & replace
    double z =0.0;
    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 0; x < Img.width(); ++x) {
            QRgb &rgb = line[x];
            QColor k(rgb);
            QColor hsvC = k.toHsv();
            v = hsvC.value();
            z = double((sumList.at(v) - firstVal)) / double(denominator) ;
            v = z * 255;

            hsvC.setHsv(hsvC.hue(), hsvC.saturation(), v);
            Img.setPixelColor(x, y, hsvC.toRgb());
        }
    }
    newPix = QPixmap::fromImage(Img);
    ui->labelThumb->setPixmap( newPix.scaled(200,170));
}
