#include "serialize.h"
#include "ui_serialize.h"

#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QFile>
#include <qevent.h>
#include <QMimeData>
#include <QPainter>

Serialize::Serialize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Serialize)
{
    ui->setupUi(this);

    findFfmpeg();

}

void Serialize::dragEnterEvent(QDragEnterEvent *event)
{
     if(event->mimeData()->hasUrls()){event->acceptProposedAction();}
}

void Serialize::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls()){
        foreach(QUrl url, event->mimeData()->urls()){
            QFileInfo f(url.toLocalFile());
            ui->linePastePath->setText( f.filePath());
        }
    }
}

Serialize::~Serialize()
{
    delete ui;
}

void Serialize::on_buttonChooseFolder_clicked()
{
    QString orig = QFileDialog::getExistingDirectory(this, tr("Open folder") );
    ui->lineEdit_origin->setText( orig);
    origDir = QDir(orig);
}

void Serialize::findFfmpeg() // add suffix
{
    QProcess ffmpeg;
    QStringList arguments;
    arguments << "-h" ;
    ffmpeg.start("ffmpeg", arguments);
    bool started = ffmpeg.waitForStarted();
    if(!started){ return; }
    ffmpeg.waitForFinished(10000);
    QByteArray result = ffmpeg.readAllStandardOutput();
    if(result.length() > 4){
        ui->comboSuffix->addItem("GIF");
        ui->comboSuffix->addItem("TIFF");
        ui->comboSuffix->addItem("WEBP");
    }
}

QString Serialize::createDestinationFolder()
{
    QString Name = origDir.absolutePath() + "_Drawish";
    int num =1;
    bool go=true;
    QString Name2;
    while(go){
        Name2 = Name + "_" + QString::number(num);
        if(QDir(Name2).exists() == false){
            QDir().mkdir(Name2);
            break;
        }
        num++;
    }
    return Name2;
}


void Serialize::toGrayscale()
{
    QImage img = actualPix.toImage();
    img = img.convertToFormat(QImage::Format_Grayscale8);
    actualPix = QPixmap::fromImage(img);
}

void Serialize::contrast()
{
    QImage qImgRet= actualPix.toImage();

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
    int v = ui->spinContrast->value();
    for (int y = 0; y < qImgRet.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(qImgRet.scanLine(y));
        for (int x = 0; x < qImgRet.width(); ++x) {
            QRgb &rgb = line[x];
            k = QColor(rgb);
            p= k.lightness();
            if( v > 0){
                if(p > medium) k = k.lighter(100 + v);
                else if( p < medium) k = k.darker(100+v);
            }
            else{
                if(p > medium) k = k.darker(100-v);
                else if( p < medium) k = k.lighter(100-v);
            }
            qImgRet.setPixelColor(x, y, k);
        }
    }
    actualPix =QPixmap::fromImage(qImgRet);
}

void Serialize::brightness()
{
    QImage Img= actualPix.toImage();
    int v = ui->spinBright->value();
    for (int y = 0; y < Img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(Img.scanLine(y));
        for (int x = 0; x < Img.width(); ++x) {
            QRgb &rgb = line[x];
            QColor k(qRed(rgb), qGreen(rgb), qBlue(rgb));
            if(v > 0){
                k = k.lighter(100+v);
            }
            else{
                k = k.darker(100-v);
            }
            Img.setPixelColor(x, y, k);
        }
    }
    actualPix =QPixmap::fromImage(Img);
}

void Serialize::crop(QString errOnFile)
{
    bool ok;
    int cx = ui->lineCropx->text().toInt(&ok);
    if(!ok){
        errors += tr("\nCrop: integer required: ") + errOnFile;
        return;
    }
    int cy = ui->lineCropy->text().toInt(&ok);
    if(!ok){
        errors += tr("\nCrop: integer required: ") + errOnFile;
        return;
    }

    int cw = ui->lineCropw->text().toInt();
    int ch = ui->lineCroph->text().toInt();
    if(cw < 1 || ch < 1){
        errors += tr("\nCrop: invalid size: ") + errOnFile;
        return;
    }
    int pixWidth = actualPix.width();
    int pixHeight = actualPix.height();
    if(cx > pixWidth -2 || cy > pixHeight-2){
        errors += tr("\nCrop: coordinates out of range: ") + errOnFile;
        return;
    }
    if((cx + cw) > pixWidth){
        cw = pixWidth - cx ;
        errors += tr("\nCrop: width out of bounds: ") + errOnFile;
    }
    if((cy + ch) > pixHeight){
        ch = pixHeight - ch ;
        errors += tr("\nCrop: height out of bounds: ") + errOnFile;
    }
    actualPix = actualPix.copy(cx, cy, cw, ch);

}

void Serialize::pasteImage(QPixmap ppix, QString errOnFile)
{
    bool ok;
    int px = ui->linePastex->text().toInt(&ok);
    if(!ok){
        errors += tr("\nPaste image: integer required: ") + errOnFile ;
        return;
    }
    int py = ui->linePastey->text().toInt(&ok);
    if(!ok){
        errors += tr("\nPaste image: integer required: ") + errOnFile;
        return;
    }
    QPainter pa(&actualPix);
    pa.drawPixmap(px, py, ppix);
    pa.end();
}

void Serialize::rotate()
{
    int d = ui->comboRotateOnly->currentIndex();
    int a = -90;
    if(ui->comboRotateDirection->currentIndex()== 1){ a = 90;}
    int w = actualPix.width();
    int h = actualPix.height();
    QPixmap npix(h, w);
    bool ok = false;
    if(d == 0){
        ok = true;
    }else if(d == 1 && h > w){  // vertical img only
            ok = true;
    }else if(d == 2 && w > h){  // horiz. img only
            ok = true;
    }
    if(ok){
        QTransform tf;
        tf.rotate(a);
        npix = actualPix.transformed(tf);
        actualPix = npix;
    }
}


void Serialize::scale(QString errOnFile)
{
    int nw = ui->lineScalew->text().toInt();
    int nh = ui->lineScaleh->text().toInt();
    if(nw < 1 || nh < 1){
        errors += tr("\nScale: integer required: ") + errOnFile;
        return;
    }
    actualPix = actualPix.scaled(nw, nh);

}

void Serialize::convert(QString f)
{
    QString suffix =ui->comboSuffix->currentText();
    if(suffix == "BMP"){actualPix.save(destination + "/" + f + ".bmp", "bmp");}
    else if(suffix == "ICO"){actualPix.save(destination + "/" + f + ".ico", "ico"); }
    else if(suffix == "JPEG"){actualPix.save(destination + "/" + f + ".jpeg", "jpeg"); }
    else if(suffix == "PNG"){actualPix.save(destination + "/" + f + ".png", "png"); }
    //---------------------------------------------------------------------------
    else if(suffix == "GIF" || suffix == "TIFF" || suffix == "WEBP"){
        actualPix.save(QDir::homePath() + "/tmp___img___dra.png", "png");
        QProcess ffmpeg;
        QStringList arguments;
        arguments << "-i" << QDir::homePath() + "/tmp___img___dra.png" << destination + "/" + f + "." + suffix.toLower() ;
        ffmpeg.start("ffmpeg", arguments);
        bool started = ffmpeg.waitForStarted();
        if(!started){ return; }
        ffmpeg.waitForFinished(5000);
    }
}

void Serialize::formFactor()
{
    QString ratio = ui->comboBox->currentText();
    QStringList ratios = ratio.split(":");
    int nw = ratios.at(0).toInt();
    int nh = ratios.at(1).toInt();
    int ow = actualPix.width();
    int oh = actualPix.height();
    double area = ow * oh;
    double rapp = double(nw) * double(nh);
    rapp = sqrt(area / rapp);
    nw = rapp * nw;
    nh = rapp * nh;
    if(nw < 4)nw =4;
    if(nh < 4)nh =4;
    actualPix = actualPix.scaled(nw, nh);
}


void Serialize::on_buttonOk_clicked()
{

    QStringList imgPaths;
    if(ui->lineEdit_origin->text() == ""){
        QMessageBox::warning(this, "Drawish", tr("The source folder does not exist"));
        return;
    }
    if(origDir.exists()){
        imgPaths = origDir.entryList();
    }else{
        QMessageBox::warning(this, "Drawish", tr("The source folder does not exist"));
        return;
    }

    destination = createDestinationFolder();

    bool isPix=true;
    QPixmap pastePix;
    if(ui->checkPaste->isChecked()){
        pastePix =QPixmap(ui->linePastePath->text());
        if(pastePix.isNull()){
            errors += tr("\nPaste image: invalid image ! ") ;
            isPix = false;
        }
    }

    foreach (QString f, imgPaths) {
        actualPix =QPixmap(origDir.absolutePath() + "/" + f);
        if(!actualPix.isNull()){
            if(ui->checkGrayscale->isChecked()){ toGrayscale();}
            if(ui->checkContrast->isChecked()){ contrast(); }
            if(ui->checkBright->isChecked()){ brightness(); }
            if(ui->checkCrop->isChecked()){ crop(f); }
            if(ui->checkPaste->isChecked() && isPix){ pasteImage(pastePix, f); }
            if(ui->checkRotate->isChecked()){ rotate(); }
            if(ui->checkScale->isChecked()){
                if(ui->radioFormFactor->isChecked() == false){ scale(f);}
                else{ formFactor();}
            }
            QFileInfo fi(origDir.absolutePath() + "/" + f);
            f = fi.baseName();
            if(ui->checkConvert->isChecked()){
                convert(f);
            }else{
                actualPix.save(destination + "/" + f + ".png", "png");
            }
        }
    }
    ui->textErrors->setPlainText(tr("Created folder: ") + destination + "\n" + errors);
}

