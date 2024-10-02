#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "area.h"
#include "borderright.h"
#include "borderbottom.h"
#include "angolo.h"
#include "geometric.h"
#include "qevent.h"
#include "qscrollarea.h"
#include "selectionarea.h"
#include "shapearea.h"
#include "curvelinearea.h"
#include "zoomarea.h"
#include "dialogsize.h"
#include "dialogeffects.h"
#include "dcolors.h"
#include "viewimage.h"
#include "savecam.h"
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QKeyEvent>
#include <QClipboard>
#include <QMimeData>
#include <QColorDialog>
#include <QRandomGenerator>
#include <QRectF>
#include <QInputDialog>
#include <QCursor>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // set label as drawing area

    ui->textOptionsWidget->setVisible(false);
    ui->lineOptionWidget->setGeometry(80,0,500,31);
    ui->lineOptionWidget->setVisible(false);
    ui->similaritywidget->setGeometry(80,0,500,31);
    ui->similaritywidget->setVisible(false);

    ball = QCursor(QPixmap(":/res/cursor.png"));

    wArea = new Area();

    // set borders
    borderR = new BorderRight(wArea);
    borderB = new BorderBottom(wArea);
    corner = new Angolo(wArea);

    // add default image to area
    pix =QPixmap(sizes::areaWidth, sizes::areaHeight);
    pix.fill(Qt::white);
    wArea->setPixmap(pix);

    historyList.push_back("Original image");
    historyPix.push_back(pix);

    // scroll area
    ui->scrollArea->setWidget(wArea);
    wArea->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
    ui->scrollArea->setWidgetResizable(false);

    // signals
    connect(borderB, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(borderR, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(corner, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(wArea, SIGNAL(endRubber()), this, SLOT(createSelectionFromRubb()));
    connect(wArea, SIGNAL(redraw()), this, SLOT(drawCopy()));
    connect(wArea, SIGNAL(penDraw()), this, SLOT(drawWithPen()));
    connect(wArea, SIGNAL(finishDrawPen()), this, SLOT(showPix()));
    connect(wArea, SIGNAL(savePrevious()), this, SLOT(save_previous()));
    connect(wArea, SIGNAL(readyToFill()), this, SLOT(fill_()));
    connect(wArea, SIGNAL(sprayDraw()), this, SLOT(spray_draw()));
    connect(wArea, SIGNAL(getColor()), this, SLOT(get_color()));
    connect(wArea, SIGNAL(setInfo()), this, SLOT(updateInfo()));
    connect(wArea, SIGNAL(drawShape()), this, SLOT(draw_shape()));
    connect(wArea, SIGNAL(viewZoom()), this, SLOT(view_zoom()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(sizes::isSelectionOn){
        if(ev->key() ==Qt::Key_W || ev->key()== Qt::Key_Q){sizes::selY--;}
        else if(ev->key()== Qt::Key_A){sizes::selX--;}
        else if(ev->key()== Qt::Key_S){sizes::selX++;}
        else if(ev->key()== Qt::Key_Z || ev->key()== Qt::Key_X){sizes::selY++;}
        else if(ev->key()== Qt::Key_Delete){
            QPixmap spix = selectionRect->pixmap();
            spix.fill(Qt::white);
            selectionRect->setPixmap(spix);
        }
        selectionRect->resetGeometry();
    }
    else if(sizes::activeOperation == 3){
        QPainter pai(&pix);
        QColor ncol= sizes::activeColor;
        if(ui->markerButton->isChecked()){
            ncol = QColor(sizes::activeColor.red(), sizes::activeColor.green(), sizes::activeColor.blue(), 32);
        }
        QPen pen(ncol, ui->lineWidthBox->value());

        if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::FlatCap);}
        else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

        pai.setPen(pen);
        int num =1;
        int xFin= sizes::selX;
        int yFin = sizes::selY;
        if(ui->radioButton_2->isChecked()) num=5;
        if(ev->key() ==Qt::Key_W || ev->key()== Qt::Key_Q){ yFin = sizes::selY-num;
        }
        else if(ev->key()== Qt::Key_A){xFin = sizes::selX-num;}
        else if(ev->key()== Qt::Key_S){xFin = sizes::selX+num;}
        else if(ev->key()== Qt::Key_Z || ev->key()== Qt::Key_X){yFin =sizes::selY+num;}

        pai.drawLine(sizes::selX, sizes::selY, xFin, yFin);
        sizes::selX= xFin; sizes::selY = yFin;
        wArea->setPixmap(pix);
    }
    else if(sizes::isShapeOn){
        if(ev->key() ==Qt::Key_W || ev->key()== Qt::Key_Q){sizes::selY--;}
        else if(ev->key()== Qt::Key_A){sizes::selX--;}
        else if(ev->key()== Qt::Key_S){sizes::selX++;}
        else if(ev->key()== Qt::Key_Z || ev->key()== Qt::Key_X){sizes::selY++;}
        shape_area->resetGeometry();
    }
    else if(isCam){
        int aax = save_area->pos().x();
        int aay = save_area->pos().y();
        int aaw = save_area->width();
        int aah = save_area->height();
        if(ev->key() ==Qt::Key_W || ev->key()== Qt::Key_Q){aay--;}
        else if(ev->key()== Qt::Key_A){aax--;}
        else if(ev->key()== Qt::Key_S){aax++;}
        else if(ev->key()== Qt::Key_Z || ev->key()== Qt::Key_X){aay++;}
        save_area->setGeometry(aax, aay, aaw, aah);
    }

}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if(sizes::modify){
        int q = QMessageBox::question(this, "Drawish", "Save image?",QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
        imgSave();
        }
        else if(q==QMessageBox::Cancel){ev->ignore();}
        else{ev->accept();}
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->scrollArea->setGeometry(80,30,this->width()-91, this->height()-120);
    ui->infoWidget->setGeometry(80,this->height()-88,591,41);
}

void MainWindow::reSize()
{   
   if(!sizes::startResize){save_previous("Resize");}
   sizes::startResize=true;
   wArea->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
   if(sizes::isCurveLineAreaOn){
       cl_area->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
   }
   QPixmap pix2(sizes::areaWidth, sizes::areaHeight);
   pix2.fill(Qt::white);
   QPainter p(&pix2);
   p.drawPixmap(0, 0, pix);

   pix =pix2;
   wArea->setPixmap(pix);

   borderB->resetGeometry();
   borderR->resetGeometry();
   corner->resetGeometry();

}

void MainWindow::imgSave()
{
    QString f;
    if(activePathFile== ""){
        QFileDialog dialog(this);
        f =dialog.getSaveFileName(this, "Drawish save...", QDir::homePath(),"Images (*.png *.jpg *.ico *bmp)");
        if(f ==""){return;}
    }else{
     f = activePathFile;
    }

        if(f.endsWith(".jpg", Qt::CaseInsensitive)){ pix.save(f, "jpg");}
        else if(f.endsWith(".ico", Qt::CaseInsensitive)){ pix.save(f, "ico");}
        else if(f.endsWith(".bmp", Qt::CaseInsensitive)){ pix.save(f, "BMP");}
        else{pix.save(f, "PNG");}
        sizes::modify = false;

}

void MainWindow::newImage(QString from)
{

   if(from == "zero"){
       sizes::modify= false;
       sizes::areaHeight=300;
       sizes::areaWidth=200;
       QPixmap npix(200, 300);
       npix.fill(Qt::yellow);
       pix = npix;
       activePathFile="";
   }
   else if(from == "f"){ // open file
      sizes::modify= false;
      QFileDialog dialog(this);
      QString f =dialog.getOpenFileName(this,"Drawish...Select image", QDir::homePath(),"Images (*.png *.jpg *.gif *.bmp *.ico)" );
      QPixmap npix(f);
      if(npix.isNull()){ QMessageBox::information(this, "Drawish", "Unsupported file"); return;}
      activePathFile = f;
      pix = npix;
      sizes::areaHeight=pix.height();
      sizes::areaWidth=pix.width();
   }
   else if(from == "pix"){
       sizes::areaHeight=pix.height();
       sizes::areaWidth=pix.width();
   }
       wArea->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
       wArea->setPixmap(pix);
       borderB->resetGeometry();
       borderR->resetGeometry();
       corner->resetGeometry();

}

void MainWindow::updateInfo()
{
  ui->mouseX_label->setNum(sizes::selX);
  ui->mouseY_label->setNum(sizes::selY);
  if(sizes::isSelectionOn || sizes::isShapeOn){
    ui->selX_label->setNum(sizes::selW);
    ui->selY_label->setNum(sizes::selH);
  }else{
      ui->selX_label->setNum(0);
      ui->selY_label->setNum(0);
  }
  ui->imageW_label->setNum(pix.width());
  ui->imageH_label->setNum(pix.height());
}

void MainWindow::untoggle()
{

    deleteSel();
    ui->selectionAreaButton->setChecked(false);
    ui->drawTextButton->setChecked(false);
    ui->penButton->setChecked(false);
    ui->fillButton->setChecked(false);
    ui->sprayButton->setChecked(false);
    ui->pickerButton->setChecked(false);
    ui->lineButton->setChecked(false);
    ui->shapeButton->setChecked(false);
    ui->curveButton->setChecked(false);

    // hide widgets
    ui->textOptionsWidget->setVisible(false);
    ui->lineOptionWidget->setVisible(false);
    ui->similaritywidget->setVisible(false);
}

void MainWindow::deleteSel()
{
    if(sizes::isShapeOn){
        sizes::isShapeOn = false;
        delete shape_area;
        shape_area =NULL;
    }
    if(sizes::isCurveLineAreaOn){
        sizes::isCurveLineAreaOn = false;
        delete cl_area;
        cl_area = NULL;
    }
    if(isCam){
        delete save_area;
        save_area = NULL;
        isCam = false;
    }
}

void MainWindow::createSelectionFromRubb()
{
    save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
    createSelection();
}


void MainWindow::on_actionNew_triggered()
{
   // ask if modified
   delete selectionRect; selectionRect = NULL;   
   sizes::isSelectionOn= false;

   deleteSel();

    if(sizes::modify){
        int q = QMessageBox::question(this, "Drawish", "Save image?",QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
            imgSave();
            // create new from 0, file, clipbard
            newImage("zero");
        }else if(q == QMessageBox::No){
             newImage("zero");
        }
    }else{
         save_previous("New Image");
         newImage("zero");

    }

}

void MainWindow::on_actionOpen_triggered()
{
    if(sizes::modify){
        int q = QMessageBox::question(this, "Drawish", "Save image?",QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
            imgSave();
            // create new from 0, file, clipboard
            newImage("f");
        }else if(q == QMessageBox::No){
             newImage("f");
        }
}else{ save_previous("Open"); newImage("f");}
}


void MainWindow::on_actionSave_triggered()
{
    if(sizes::modify ){
        int q = QMessageBox::question(this, "Drawish", "Save image?",QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
        imgSave();
        }
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog dialog(this);
    QString f =dialog.getSaveFileName(this,"Drawish save image", QDir::homePath());
    if(f.endsWith(".jpg", Qt::CaseInsensitive)){ pix.save(f, "jpg");}
    else if(f.endsWith(".ico", Qt::CaseInsensitive)){ pix.save(f, "ico");}
    else if(f.endsWith(".bmp", Qt::CaseInsensitive)){ pix.save(f, "BMP");}
    else{pix.save(f, "PNG");}
}


void MainWindow::on_actionClose_triggered()
{
    this->close();
}


void MainWindow::on_undoButton_clicked()
{
    if(!prePix.isNull()){
      pix=prePix;
      newImage("pix");
    }
}

void MainWindow::on_selectionAreaButton_clicked()
{
    if(sizes::activeOperation==1){ // deactivate

        sizes::activeOperation=0;
        wArea->setCursor(Qt::ArrowCursor);
        if(sizes::isSelectionOn){
            sizes::isSelectionOn=false;
            drawCopy();
        }else{
           sizes::isSelectionOn=false;
           delete selectionRect;
           selectionRect = NULL;
        }

    }
    else {         // activate
        if(sizes::activeOperation ==2 ){
            sizes::isSelectionOn=false;
            delete selectionRect;
            selectionRect = NULL;
        }
        sizes::activeOperation=1;
        wArea->setCursor(Qt::CrossCursor);
        untoggle();
        ui->selectionAreaButton->setChecked(true);
    }
}

void MainWindow::createSelection()
{

    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    if(sizes::activeOperation != 2){
        QPixmap selPix = pix.copy(sizes::selX , sizes::selY , sizes::selW , sizes::selH );
        if(ui->actionTransparent_selection->isChecked()){
            selPix=addTransparency(selPix, 0,255,255,255);
        }
      selectionRect->setPixmap(selPix);
      QPixmap blank(sizes::selW ,sizes::selH );
      blank.fill(Qt::white);
      QPainter p(&pix);
      p.drawPixmap(sizes::selX ,sizes::selY, blank);
      wArea->setPixmap(pix);
    }
    sizes::isSelectionOn=true;
    selectionRect->show();
    updateInfo();
    connect(selectionRect, SIGNAL(setInfo()), this, SLOT(updateInfo()));

}

void MainWindow::drawCopy()
{    
    save_previous("Copy selection");
    QPixmap selectedImage(selectionRect->pixmap());
    QPainter p(&pix);
    p.drawPixmap(sizes::selX ,sizes::selY, selectedImage.scaled(sizes::selW, sizes::selH));
    wArea->setPixmap(pix);
    delete selectionRect;
    selectionRect = NULL;
    updateInfo();
}


void MainWindow::on_actionSelect_all_triggered()
{
    sizes::selX =0;
    sizes::selY =0;
    sizes::selH = sizes::areaHeight;
    sizes::selW = sizes::areaWidth;
    save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
    createSelection();
}


void MainWindow::on_actionCopy_triggered()
{
    if(sizes::isSelectionOn){
        prePix = pix;
        sizes::startResize=false;
        QClipboard *p_Clipboard = QApplication::clipboard();
        p_Clipboard->setPixmap(selectionRect->pixmap());

    }
}

void MainWindow::on_actionCopy_selection_no_clipboard_triggered()
{
    if(sizes::isSelectionOn){
        prePix = pix;
        sizes::startResize=false;
        QPixmap copied=selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
        drawCopy();
        selectionRect = new selectionArea(wArea);
        selectionRect->resetGeometry();
        if(ui->actionTransparent_selection->isChecked()){
            copied=addTransparency(copied, 0,255,255,255);
        }
        selectionRect->setPixmap(copied);
        selectionRect->show();
        updateInfo();
    }
}


void MainWindow::on_actionPaste_from_clipboard_triggered()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasImage()) {
            QPixmap cpix =(qvariant_cast<QPixmap>(mimeData->imageData()));
            pasteImg(cpix);
    }
}
void MainWindow::pasteImg(QPixmap passedPix)
{
    if(sizes::isSelectionOn){     delete selectionRect;    selectionRect = NULL;}
    int wi = passedPix.width();
    int he = passedPix.height();
    if(wi > wArea->width()){ sizes::areaWidth = wi+8;}
    if(he > wArea->height()){sizes::areaHeight = he+8;}
    reSize();
    sizes::selX =0;
    sizes::selY =0;
    sizes::selH = he;
    sizes::selW = wi;

    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    selectionRect->setPixmap(passedPix.scaled(sizes::selW, sizes::selH));
    sizes::isSelectionOn=true;
    sizes::startResize=false;
    selectionRect->show();
}


QPixmap MainWindow::addTransparency(QPixmap passedPix, int opacity, int red, int green, int blue)
{
    QImage sPix = passedPix.toImage();
    sPix = sPix.convertToFormat(QImage::Format_ARGB32);
    QColor k(red, green, blue);
    for(int row=0; row< sPix.width(); ++row){
       for(int coloumn =0; coloumn < sPix.height(); ++ coloumn){
           if(sPix.pixelColor(row, coloumn)== k){
               sPix.setPixelColor(row, coloumn, QColor(255,255,255, opacity));
           }
       }
    }
    return QPixmap::fromImage(sPix);
}


void MainWindow::on_actionPaste_from_file_triggered()
{
    QFileDialog dialog(this);
    QString f =dialog.getOpenFileName(this,"Drawish...Select image", QDir::homePath(),"Images (*.png *.jpg *.gif *.bmp *.ico)" );
    QPixmap fpix(f);
    pasteImg(fpix);
}


void MainWindow::on_actionCopy_selection_to_file_triggered()
{
    if(sizes::isSelectionOn){
        QPixmap selPix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
        QFileDialog dialog(this);
        QString f =dialog.getSaveFileName(this,"Drawish save image", QDir::homePath());
        if(f.endsWith(".jpg", Qt::CaseInsensitive)){ selPix.save(f, "jpg");}
        else if(f.endsWith(".ico", Qt::CaseInsensitive)){ selPix.save(f, "ico");}
        else if(f.endsWith(".bmp", Qt::CaseInsensitive)){ selPix.save(f, "BMP");}
        else{selPix.save(f, "PNG");}
    }
}

//  draw text
void MainWindow::on_drawTextButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    wArea->setCursor(Qt::ArrowCursor);

    if(ui->drawTextButton->isChecked()){
        untoggle();
        ui->drawTextButton->setChecked(true);
        sizes::activeOperation = 2;
        ui->textOptionsWidget->setVisible(true);
    }else{
        //...
        delete selectionRect;
        selectionRect =NULL;
        sizes::activeOperation = 0;
        ui->textOptionsWidget->setVisible(false);
    }
}


void MainWindow::on_textLine_textEdited(const QString &arg1)
{
    if(!sizes::isSelectionOn){
        QMessageBox::information(this, "Drawish", "Click a point on the canvas, before");
        return;
    }
    QFont tFont(ui->fontComboBox->currentFont());
    if(ui->boldButton->isChecked()){ tFont.setBold(true);}
    else{tFont.setBold(false);}
    if(ui->italicButton->isChecked()){ tFont.setItalic(true);}
    else{tFont.setItalic(false);}
    if(ui->underlineButton->isChecked()){ tFont.setUnderline(true);}
    else{tFont.setUnderline(false);}
    int sizeText = ui->sizeLine->text().toInt();
    if(sizeText < 4){sizeText = 16;}
    tFont.setPixelSize(sizeText);

    selectionRect->setFont(tFont);
    selectionRect->setStyleSheet("color:" + sizes::activeColor.name());
    selectionRect->setText(arg1);
}

void MainWindow::on_confirmTextButton_clicked()
{
    if(ui->textLine->text() != ""){
        save_previous("Text");
        QPainter p(&pix);
        QFont tFont(ui->fontComboBox->currentFont());
        if(ui->boldButton->isChecked()){ tFont.setBold(true);}
        else{tFont.setBold(false);}
        if(ui->italicButton->isChecked()){ tFont.setItalic(true);}
        else{tFont.setItalic(false);}
        if(ui->underlineButton->isChecked()){ tFont.setUnderline(true);}
        else{tFont.setUnderline(false);}
        int sizeText = ui->sizeLine->text().toInt();
        if(sizeText < 4){sizeText = 16; ui->sizeLine->setText("16");}
        tFont.setPixelSize(sizeText);
        p.setFont(tFont);
        p.setPen(sizes::activeColor);
        p.drawText(sizes::selX+2, sizes::selY+sizeText+2, ui->textLine->text());
        wArea->setPixmap(pix);
        selectionRect->resetGeometry();
    }

}

//  SET COLORS....
void MainWindow::on_blackButton_clicked()
{
    sizes::activeColor= QColor(0,0,0);
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_whiteButton_clicked()
{
     sizes::activeColor= QColor(255,255,255);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_greyButton_clicked()
{
     sizes::activeColor= QColor(128,128,128);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_redButton_clicked()
{
     sizes::activeColor= QColor(255,0,0);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_greenButton_clicked()
{
     sizes::activeColor= QColor(0,255,0);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_blueButton_clicked()
{
     sizes::activeColor= QColor(0,0,255);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_yellowButton_clicked()
{
     sizes::activeColor= QColor(255,255,0);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_magentaButton_clicked()
{
     sizes::activeColor= QColor(255,0,255);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_cyanButton_clicked()
{
     sizes::activeColor= QColor(0,255,255);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}


void MainWindow::on_transparentButton_clicked()
{
     sizes::activeColor= QColor(255,255,255,0);
     ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
     ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}

void MainWindow::on_addColorButton_clicked()
{
    sizes::activeColor = QColorDialog::getColor(Qt::gray, this, "Drawish choose color");
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}
//  END COLORS

void MainWindow::on_penButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }

    if(ui->penButton->isChecked()){
        untoggle();
        ui->penButton->setChecked(true);
         wArea->setCursor(ball);
        sizes::activeOperation = 3;
        ui->lineOptionWidget->setVisible(true);
        if(pix.width() * pix.height() < 250000){
            stopRepeatShow=6;
        }
        else if(pix.width() * pix.height() < 1000000){
            stopRepeatShow=10;
        }
        else{
            stopRepeatShow = 15;
        }
    }else{
        //...
        sizes::activeOperation = 0;
        wArea->setCursor(Qt::ArrowCursor);
        ui->lineOptionWidget->setVisible(false);

    }
}

void MainWindow::drawWithPen(){

    updateInfo();
    QPainter pai(&pix);
    QColor ncol = sizes::activeColor;
    if(ui->markerButton->isChecked()){
        ncol = QColor(sizes::activeColor.red(), sizes::activeColor.green(), sizes::activeColor.blue(), 32);
    }
    QPen pen(ncol, sizes::line_width);

    if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::FlatCap);}
    else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

    pai.setPen(pen);
    pai.drawLine(sizes::shape_x_begin, sizes::shape_y_begin, sizes::shape_x_end, sizes::shape_y_end);

    repeatShow++;
    sizes::shape_x_begin = sizes::shape_x_end;
    sizes::shape_y_begin = sizes::shape_y_end;
    if(repeatShow== stopRepeatShow){
    wArea->setPixmap(pix);
    repeatShow=1;
    }
}

void MainWindow::showPix()
{

    wArea->setPixmap(pix);
}

void MainWindow::save_previous(QString tx)
{
    if(sizes::activeOperation == 3){
         historyList.push_back("Pen");
    }
    else if(sizes::activeOperation == 5){
        historyList.push_back("Spray");
    }
    else{  historyList.push_back(tx); }

    prePix = QPixmap();
    historyPix.push_back(pix);
    if(historyList.count() > 8){
        historyList.removeFirst();
        historyPix.removeFirst();
    }
    ui->historyCombo->clear();

    ui->historyCombo->addItems(historyList);
   // ui->historyCombo->removeItem(ui->historyCombo->count()-1);
    sizes::modify = true;
}

void MainWindow::on_flatcapButton_clicked()
{
    ui->roundcapButton->setChecked(false);
}


void MainWindow::on_roundcapButton_clicked()
{
    ui->flatcapButton->setChecked(false);
}

//  FILL
void MainWindow::on_fillButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }

    wArea->setCursor(Qt::ArrowCursor);
    if(ui->fillButton->isChecked()){
        untoggle();
        ui->fillButton->setChecked(true);
        sizes::activeOperation = 4;
        ui->similaritywidget->setVisible(true);
    }else{
        //...
        sizes::activeOperation = 0;
        ui->similaritywidget->setVisible(false);
    }
}

void MainWindow::fill_()
{
    save_previous("Fill");
    updateInfo();
    QImage img = pix.toImage();
    img =img.convertToFormat(QImage::Format_ARGB32);
    QList<int> coord;
    coord.push_back(sizes::selX);
    coord.push_back(sizes::selY);
    QColor oldColor= QColor::fromRgb(img.pixel(sizes::selX, sizes::selY));
    if(oldColor == sizes::activeColor)return;
    if(oldColor == Qt::white && sizes::activeColor == QColor(255,255,255,0)){ return; }
    img.setPixelColor(sizes::selX, sizes::selY, sizes::activeColor);
    int posx, posy;
    int similarity= ui->similaritySlider->value();
    while(coord.count()>0){
        posx = coord[0];
        posy = coord[1];
        coord.remove(0,2);
        if(posy-1 > -1 ){
            if(isSimil(QColor::fromRgb(img.pixel(posx, posy-1)), oldColor, similarity)){
                coord.push_back(posx);
                coord.push_back(posy-1);
                img.setPixelColor(posx, posy-1, sizes::activeColor);
            }
        }
        if(posx-1 > -1 ){
            if(isSimil(QColor::fromRgb(img.pixel(posx-1, posy)), oldColor, similarity)){
                coord.push_back(posx-1);
                coord.push_back(posy);
                img.setPixelColor(posx-1, posy, sizes::activeColor);
            }
        }

        if(posx+1 < img.width() ){
            if(isSimil(QColor::fromRgb(img.pixel(posx+1, posy)), oldColor, similarity)){
                coord.push_back(posx+1);
                coord.push_back(posy);
                img.setPixelColor(posx+1, posy, sizes::activeColor);
            }
        }
        if(posy+1 < img.height() ){
            if(isSimil(QColor::fromRgb(img.pixel(posx, posy+1)) ,oldColor, similarity)){
                coord.push_back(posx);
                coord.push_back(posy+1);
                img.setPixelColor(posx, posy+1, sizes::activeColor);
            }
        }
    }
    pix = QPixmap::fromImage(img);
    showPix();

}

bool MainWindow::isSimil(QColor k1, QColor k2, int affinity)
{
  affinity = affinity/2;
  int diffr = k1.red() - k2.red();
  if(abs(diffr) > affinity)return false;
  int diffg = k1.green() - k2.green();
  if(abs(diffg) > affinity)return false;
  int diffb =k1.blue() - k2.blue();
  if(abs(diffb)> affinity)return false;
  diffr = abs(diffr) + abs(diffg) + abs(diffb);
  if(diffr > affinity*2 )return false;
  return true;
}

void MainWindow::on_similaritySlider_valueChanged(int value)
{
    ui->similarityLabel->setNum(value);
}

// spray
void MainWindow::on_sprayButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }

    if(ui->sprayButton->isChecked()){
        untoggle();
        ui->sprayButton->setChecked(true);
        sizes::activeOperation = 5;
        wArea->setCursor(ball);
        if(pix.width() * pix.height() < 250000){
            stopRepeatShow=6;
        }
        else if(pix.width() * pix.height() < 1000000){
            stopRepeatShow=10;
        }
        else{
            stopRepeatShow = 15;
        }

    }else{
        //...
        sizes::activeOperation = 0;
        wArea->setCursor(Qt::ArrowCursor);
        }
}

void MainWindow::spray_draw()
{
    updateInfo();
    QPainter pai(&pix);
    QPen pen(sizes::activeColor, 1);

    pai.setPen(pen);
    // copied from kolourpaint code (^_^)-------------------------------
    for(int i=0; i< 10; ++i){
         int dx= (QRandomGenerator::global()->generate() % 20 )-10;
         int dy= (QRandomGenerator::global()->generate() % 20 )-10;
         if((dx*dx) +(dy*dy) > 100){continue;}
         pai.drawPoint(sizes::selX+dx, sizes::selY+dy);
    }//  ----------------------------------------------------------------
    repeatShow++;
    if(repeatShow== stopRepeatShow){
    wArea->setPixmap(pix);
    repeatShow=1;
    }
}

//picker

void MainWindow::on_pickerButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    wArea->setCursor(Qt::ArrowCursor);
    if(ui->pickerButton->isChecked()){
        untoggle();
        ui->pickerButton->setChecked(true);
        sizes::activeOperation = 6;
    }else{
        //...
        sizes::activeOperation = 0;
    }
}

void MainWindow::get_color()
{
    updateInfo();
    QImage img = pix.toImage();
    sizes::activeColor = QColor::fromRgba(img.pixel(sizes::selX, sizes::selY));
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}

//--------------------------------------------------------------------------------------------

// shapes
void MainWindow::on_lineButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    wArea->setCursor(Qt::ArrowCursor);
    if(ui->lineButton->isChecked()){
        untoggle();
        ui->lineButton->setChecked(true);
        sizes::activeOperation = 7;
        ui->lineOptionWidget->setVisible(true);
    }else{
        //...
        sizes::activeOperation = 0;
        ui->lineOptionWidget->setVisible(false);
        if(sizes::isShapeOn){
          delete shape_area;
          shape_area =NULL;
          sizes::isShapeOn =false;
        }
    }
}

void MainWindow::createShapeArea()
{    
    sizes::selH =80;
    sizes::selW =80;
    sizes::shape_x_begin= 40;
    sizes::shape_y_begin= 40;
    shape_area = new shapeArea(wArea);
    shape_area->resetGeometry();
    QPixmap selPix(80,80);

    selPix.fill(QColor(255,255,255,0));
    QPainter pai(&selPix);
    QPen pen(sizes::activeColor, sizes::line_width);

    if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::FlatCap);}
    else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

    pai.setPen(pen);

    pai.drawPoint(40,40);
    shape_area->setPixmap(selPix);
    sizes::isShapeOn=true;
    shape_area->show();
    updateInfo();
    connect(shape_area, SIGNAL(setInfo()), this, SLOT(updateInfo()));
}

void MainWindow::draw_shape()
{
    // line -- shape
    if(sizes::activeOperation == 7 && sizes::isShapeOn){
        save_previous("Line");
        updateInfo();
        QPainter pai(&pix);
        QPen pen(sizes::activeColor, sizes::line_width);

        if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::FlatCap);}
        else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

        pai.setPen(pen);
        pai.drawLine(sizes::selX + sizes::shape_x_begin, sizes::selY + sizes::shape_y_begin, sizes::selX + sizes::shape_x_end, sizes::selY +sizes::shape_y_end);
        wArea->setPixmap(pix);
        sizes::isShapeOn = false;
        delete shape_area;
        shape_area =NULL;
    }
    else if(sizes::activeOperation >6 && sizes::activeOperation < 9 && sizes::isShapeOn ==false){
        createShapeArea();
    }    
    else if(sizes::activeOperation == 8 && sizes::isShapeOn == true){
        save_previous("Shape");
        updateInfo();
        QPainter pai(&pix);
        QPixmap sPix= shape_area->pixmap();
        pai.drawPixmap(sizes::selX+2, sizes::selY, shape_area->width(), shape_area->height(), sPix);
        wArea->setPixmap(pix);
        sizes::isShapeOn = false;
        delete shape_area;
        shape_area =NULL;
    }


}

void MainWindow::on_lineWidthBox_valueChanged(int arg1)
{
    sizes::line_width = arg1;
}

//  shapes
void MainWindow::on_shapeButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    wArea->setCursor(Qt::ArrowCursor);
    if(ui->shapeButton->isChecked()){
        untoggle();
        ui->shapeButton->setChecked(true);
        sizes::activeOperation = 8;
        ui->lineOptionWidget->setVisible(true);
    }else{
        //...
        sizes::activeOperation = 0;
        ui->lineOptionWidget->setVisible(false);
        if(sizes::isShapeOn){
          delete shape_area;
          shape_area =NULL;
          sizes::isShapeOn =false;
        }
    }
}


void MainWindow::on_shapesCombo_currentIndexChanged(int index)
{
    QStringList shapeNames;
    shapeNames << "squ" << "rec" << "cir" << "ell" << "tri" << "rou" << "sta" << "aup" << "ari" << "ado" << "ale" << "aul" << "aur" << "abr" << "abl";
    sizes::activeShape = shapeNames.at(index);

}

void MainWindow::on_fillCheck_clicked(bool checked)
{
    sizes::toFill =checked;
}

//  curve line
void MainWindow::on_curveButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    wArea->setCursor(Qt::ArrowCursor);
    if(ui->curveButton->isChecked()){
        untoggle();
        ui->curveButton->setChecked(true);
        sizes::activeOperation = 9;
        ui->lineOptionWidget->setVisible(true);
        cl_area = new curveLineArea(wArea);
        connect(cl_area, SIGNAL(finishCurve()), this, SLOT(finish_curve()));
        sizes::isCurveLineAreaOn = true;
        cl_area->show();
    }else{
        //...
        sizes::activeOperation = 0;
        ui->lineOptionWidget->setVisible(false);
        if(sizes::isCurveLineAreaOn){
            sizes::isCurveLineAreaOn = false;
            delete cl_area;
            cl_area =NULL;
        }
    }
}

void MainWindow::finish_curve()
{
    save_previous("Curve line");
    if(sizes::isCurveLineAreaOn){
      QPixmap cPix = cl_area->pixmap();
      QPainter p(&pix);
      p.drawPixmap(0,0, cPix);
      showPix();
      delete cl_area;
      cl_area =NULL;
      // continue
      cl_area = new curveLineArea(wArea);
      connect(cl_area, SIGNAL(finishCurve()), this, SLOT(finish_curve()));
      cl_area->show();
    }
}

// ZOOM
void MainWindow::view_zoom()
{
    if(zoom_area == nullptr){
      sizes::zoomEdited = false;
      zoom_area = new zoomArea(wArea);
      posXofZoom = sizes::selX-8;
      posYofZoom = sizes::selY-8;
      QPixmap cPix = pix.copy(posXofZoom, posYofZoom, 17, 17);
      QImage img= cPix.toImage();
      QPixmap zoomPix(238,238);
      QColor k;
      for(int py=0; py < img.height(); ++ py){
          for(int px=0; px < img.width(); ++px){
              k= img.pixelColor(px, py);
              QPainter p(&zoomPix);
              QBrush br;
              br.setColor(k);
              br.setStyle(Qt::SolidPattern);
              p.setBrush(br);
              if(px == 8 && py ==8){
                  p.drawEllipse(px*14, py*14, 14,14);
              }else{
                  p.drawRect(px*14, py*14, 14,14);
              }
          }
      }
      connect(zoom_area, SIGNAL(zoomChangeColor()), this, SLOT(zoom_change_color()) );
      zoom_area->setPixmap(zoomPix);
      // put ia visible area if possible
      int zx = sizes::selX+2;
      int zy = sizes::selY;
      if(zx > wArea->width()-238 ){zx = wArea->width()-239;}
      if(zx < 10){zx = 10;}
      if(zy > wArea->height()-238){ zy = wArea->height()-239;}
      if(zy < 0){zy =0;}
      zoom_area->setGeometry(zx, zy , 238,238);
      zoom_area->show();
    }
    else{
        // copy zoomed image
        if(sizes::zoomEdited){ save_previous("Zoom");}
        QImage img = zoom_area->pixmap().toImage();
        QPixmap cPix(17,17);
        QColor k;
        for(int ix=0; ix < 17; ++ix){
           for(int iy =0; iy <17; ++iy){
               k = img.pixelColor((ix*14)+7, (iy*14)+7);
               QPainter p(&cPix);
               QPen pen(k);
               p.setPen(pen);
               p.drawPoint(ix,iy);
           }
        }        
        QPainter pai(&pix);
        pai.drawPixmap(posXofZoom, posYofZoom, cPix);
        showPix();
        delete zoom_area;
        zoom_area = nullptr;
    }
}

void MainWindow::zoom_change_color()
{
    QPixmap zPix = zoom_area->pixmap();
    int squareColx = sizes::zoomx / 14;
    int squareColy = sizes::zoomy / 14;
    squareColx *=14;
    squareColy *=14;
    QPainter p(&zPix);
    QBrush br;
    br.setColor(sizes::activeColor);
    br.setStyle(Qt::SolidPattern);
    p.setBrush(br);
    p.drawRect(squareColx,squareColy, 14,14);
    zoom_area->setPixmap(zPix);
}

//-----------------------------
// rotate

void MainWindow::on_rotateLeftButton_clicked()
{
    rotation(-1);
}

void MainWindow::on_rotateRightButton_clicked()
{
   rotation(1);
}

void MainWindow::rotation(int a)
{
    if(!sizes::isSelectionOn){
        QMessageBox::information(this, "Drawish", "No selection");
        return;
    }    
    // set bigger selectionRect for rotated image !!
   //╔══ ▄▀▄ ══╗
   //  ██▀▀▀██
   // ▄█     █▄
   //  █▄▄▄▄▄█
   //    ▀▄▀

    double cos = sizes::selW/2;
    double sin = sizes::selH/2;
    double hip= (cos*cos)+ (sin*sin);
    hip = sqrt(hip);
    double cosA = cos/hip;
    double sinA = sin/hip;
    QList<double>table;
    table <<0<<0.00<<1.00<<1<<0.02<<1.00<<2<<0.03<<1.00<<3<<0.05<<1.00<<4<<0.07<<1.00<<5<<0.09<<1.00<<6<<0.10<<0.99<<7<<0.12<<0.99<<8<<0.14<<0.99<<9<<0.16<<0.99<<10<<0.17<<0.98<<11<<0.19<<0.98<<12<<0.21<<0.98<<13<<0.22<<0.97<<14<<0.24<<0.97<<15<<0.26<<0.97<<16<<0.28<<0.96<<17<<0.29<<0.96<<18<<0.31<<0.95<<19<<0.33<<0.95<<20<<0.34<<0.94<<21<<0.36<<0.93<<22<<0.37<<0.93<<23<<0.39<<0.92<<24<<0.41<<0.91<<25<<0.42<<0.91<<26<<0.44<<0.90<<27<<0.45<<0.89<<28<<0.47<<0.88<<29<<0.48<<0.87<<30<<0.50<<0.87<<31<<0.52<<0.86<<32<<0.53<<0.85<<33<<0.54<<0.84<<34<<0.56<<0.83<<35<<0.57<<0.82<<36<<0.59<<0.81<<37<<0.60<<0.80<<38<<0.62<<0.79<<39<<0.63<<0.78<<40<<0.64<<0.77<<41<<0.66<<0.75<<42<<0.67<<0.74<<43<<0.68<<0.73<<44<<0.69<<0.72<<45<<0.71<<0.71<<46<<0.72<<0.69<<47<<0.73<<0.68<<48<<0.74<<0.67<<49<<0.75<<0.66<<50<<0.77<<0.64<<51<<0.78<<0.63<<52<<0.79<<0.62<<53<<0.80<<0.60<<54<<0.81<<0.59<<55<<0.82<<0.57<<56<<0.83<<0.56<<57<<0.84<<0.54<<58<<0.85<<0.53<<59<<0.86<<0.52<<60<<0.87<<0.50<<61<<0.87<<0.48<<62<<0.88<<0.47<<63<<0.89<<0.45<<64<<0.90<<0.44<<65<<0.91<<0.42<<66<<0.91<<0.41<<67<<0.92<<0.39<<68<<0.93<<0.37<<69<<0.93<<0.36<<70<<0.94<<0.34<<71<<0.95<<0.33<<72<<0.95<<0.31<<73<<0.96<<0.29<<74<<0.96<<0.28<<75<<0.97<<0.26<<76<<0.97<<0.24<<77<<0.97<<0.22<<78<<0.98<<0.21<<79<<0.98<<0.19<<80<<0.98<<0.17<<81<<0.99<<0.16<<82<<0.99<<0.14<<83<<0.99<<0.12<<84<<0.99<<0.10<<85<<1.00<<0.09<<86<<1.00<<0.07<<87<<1.00<<0.05<<88<<1.00<<0.03<<89<<1.00<<0.02<<90<<1.00<<0.00;
    int angle = ui->RotatioAngleSpin->value() ;
    for(int i=2; i< table.count(); i = i+3){
        if(table[i] <= cosA){
            int grad = table[i-2];
            grad = grad -angle;
            if(grad > 90){ grad = 90-grad;}
            if(grad < 0){ grad = grad *-1;}
            grad = (grad *3)+2;
            double g = table[grad];
            sizes::selW  = hip * g*2;
            break;
        }
    }
    for(int i=1; i< table.count(); i = i+3){
        if(table[i] >= sinA){
            int grad = table[i-1];
            grad =grad +angle;
            if(grad > 90){ grad = 90-grad;}
            if(grad < 0){ grad = grad *-1;}
            grad = (grad *3)+1;
            double g = table[grad];
            sizes::selH  = hip * g*2;
            break;
        }
    }
    selectionRect->resetGeometry();
    //--
    QTransform tf;
    tf.rotate(ui->RotatioAngleSpin->value() * a);
    QPixmap sPix = selectionRect->pixmap().transformed(tf);
    selectionRect->setPixmap(sPix);

}

void MainWindow::deg90(int a)
{
    if(!sizes::isSelectionOn){
        QMessageBox::information(this, "Drawish", "No selection");
        return;
    }

    int ww = sizes::selW;
    int hh = sizes::selH;
    sizes::selW = hh;
    sizes::selH = ww;

    selectionRect->resetGeometry();
    //--
    QTransform tf;
    tf.rotate(a);
    QPixmap sPix = selectionRect->pixmap().transformed(tf);
    selectionRect->setPixmap(sPix);
}

void MainWindow::on_deg90left_clicked()
{
    deg90(-90);
}


void MainWindow::on_deg90right_clicked()
{
    deg90(90);
}

// ----
//  resize image or selection. scale image
void MainWindow::on_actionSizes_triggered()
{
    DialogSize dSize;
    dSize.setModal(true);
    dSize.exec();
    if(dSize.returned == 1){
        save_previous("Resize");
        reSize();
        sizes::startResize = false;
    }

    else if(dSize.returned == 2){
        save_previous("Resize");
        wArea->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
        if(sizes::isCurveLineAreaOn){
            cl_area->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
        }
        QPixmap pix2(sizes::areaWidth, sizes::areaHeight);
        pix2.fill(Qt::white);
        QPainter p(&pix2);
        p.drawPixmap(0, 0, pix.scaled(sizes::areaWidth, sizes::areaHeight));

        pix =pix2;
        wArea->setPixmap(pix);
        borderB->resetGeometry();
        borderR->resetGeometry();
        corner->resetGeometry();
    }

    else if(dSize.returned == 3){
        if(sizes::activeOperation==1){

            if(sizes::isSelectionOn){
                QMessageBox::information(this, "Drawish", "An area is already selected");
                return;
            }else{
                save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
                createSelection();
            }
            return;
        }
        else {
            if(sizes::activeOperation == 2){
                QMessageBox::information(this, "Drawish", "An area is already selected");
                return;
            }
            sizes::activeOperation=1;
            untoggle();
            ui->selectionAreaButton->setChecked(true);
            save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
            createSelection();
        }
    }
}

//  effects

void MainWindow::on_actionEffects_triggered()
{
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap();
    }

    DialogEffects dEffects(this, Epix);
    dEffects.setModal(true);
    dEffects.exec();

    if(dEffects.res == QDialog::Accepted){
         if(sizes::isSelectionOn){
            selectionRect->setPixmap( dEffects.modifPix);
        }else{
             save_previous("Effects");
             pix =  dEffects.modifPix;
             showPix();
         }
    }

}

//  colors dialog
void MainWindow::on_actionColors_triggered()
{
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap();
    }

    dColors dCol(this, Epix);
    dCol.setModal(true);
    dCol.exec();
    // update active color
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
    //
    if(dCol.res == QDialog::Accepted){
         if(sizes::isSelectionOn){
            selectionRect->setPixmap( dCol.origPix);
        }else{
             save_previous("Colors");
             pix =  dCol.origPix;
             showPix();
         }
    }
}

// other effects

void MainWindow::on_actionMirror_triggered() // horiz
{
   mirror(true,false);
}


void MainWindow::on_actionMirror_vertically_triggered()
{
  mirror(false, true);
}

void MainWindow::mirror(bool horizontal, bool vertical)
{

    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap();
    }
    QImage img = Epix.toImage();

    img.mirror(horizontal, vertical);
    if(sizes::isSelectionOn){
        selectionRect->setPixmap(QPixmap::fromImage(img));
    }else{
      QString versus = "horizontally";
      if(vertical) versus = "vertically";
      save_previous("Mirror " + versus);
      pix = QPixmap::fromImage(img);
      showPix();
    }
}

void MainWindow::on_actionTo_greyscale_triggered()
{
    save_previous("Greyscale");
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap();
    }
    QImage img = Epix.toImage();
    img = img.convertToFormat(QImage::Format_Grayscale8);
    if(sizes::isSelectionOn){
        selectionRect->setPixmap(QPixmap::fromImage(img));
    }else{
      pix = QPixmap::fromImage(img);
      showPix();
    }
}


void MainWindow::on_actionReduce_to_RGB_triggered()
{
    save_previous("Reduce to rgb");
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap();
    }
    QImage img = Epix.toImage();

    for (int y = 0; y < img.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < img.width(); ++x) {
            QRgb &rgb = line[x];

            int r = qRed(rgb);
            int g = qGreen(rgb);
            int b = qBlue(rgb);
            if(r > g && r > b){ img.setPixelColor(x,y, QColor(r,0,0));}
            else if(g > r && g > b){img.setPixelColor(x,y, QColor(0,g,0));}
            else if(b > r && b > g){img.setPixelColor(x,y, QColor(0,0,b));}
            else if(r == g && r >b){img.setPixelColor(x,y, QColor(r,g,0));}
            else if(r == b && r >g){img.setPixelColor(x,y, QColor(r,0,b));}
            else if(b == g && b >r){img.setPixelColor(x,y, QColor(0,g,b));}
        }
    }
    if(sizes::isSelectionOn){
        selectionRect->setPixmap(QPixmap::fromImage(img));
    }else{
      pix = QPixmap::fromImage(img);
      showPix();
    }
}


void MainWindow::on_actionInvert_colors_triggered()
{
    save_previous("Invert colors");
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap();
    }
    QImage img = Epix.toImage();
    img.invertPixels();
    if(sizes::isSelectionOn){
        selectionRect->setPixmap(QPixmap::fromImage(img));
    }else{
      pix = QPixmap::fromImage(img);
      showPix();
    }
}


void MainWindow::on_actionZoom_triggered()
{
    // zoom only view

    viewImage vi(this, pix);
    vi.setModal(true);
    vi.exec();
}


void MainWindow::on_actionCreate_triggered()
{
    // create Camera
    if(isCam) return;
    untoggle();
    sizes::activeOperation =0;
    int ww = QInputDialog::getInt(this, "Drawish", "Enter width", 60);
    int hh = QInputDialog::getInt(this, "Drawish", "Enter height", 60);
    if(ww < 8 || hh < 8){
        QMessageBox::information(this, "Drawish", "Too small selection");
        return;
    }
    save_area = new saveCam(wArea, QSize(ww, hh));
    save_area->show();
    isCam = true;
}


void MainWindow::on_actionSave_image_triggered()
{
    if(isCam){
        QPixmap camPix = pix.copy(save_area->pos().x(), save_area->pos().y(), save_area->width(), save_area->height());
        QFileDialog dialog(this);
        QString f =dialog.getSaveFileName(this, "Drawish save...", QDir::homePath(),"Images (*.png *.jpg *.ico *bmp)");
        if(f ==""){return;}
        if(f.endsWith(".jpg", Qt::CaseInsensitive)){ camPix.save(f, "jpg");}
        else if(f.endsWith(".ico", Qt::CaseInsensitive)){ camPix.save(f, "ico");}
        else if(f.endsWith(".bmp", Qt::CaseInsensitive)){ camPix.save(f, "BMP");}
        else{camPix.save(f, "PNG");}
    }
}


void MainWindow::on_actionDelete_camera_triggered()
{
    if(isCam){
        delete save_area;
        save_area = NULL;
        isCam = false;
    }
}


void MainWindow::on_historyCombo_activated(int index)
{
    if(ui->historyCombo->count() < 1){ return; }
    if(prePix.isNull()) prePix =pix;
    pix = historyPix[index];
    if(sizes::isSelectionOn){
        sizes::isSelectionOn = false;
        delete selectionRect;
        selectionRect = NULL;
    }
    QString comboText = ui->historyCombo->itemText(index);
    if(comboText.startsWith("Selection")){
        QStringList data= comboText.split("*");
        sizes::selX = data.at(1).toInt();
        sizes::selY = data.at(2).toInt();
        sizes::selW = data.at(3).toInt();
        sizes::selH = data.at(4).toInt();
        createSelection();
    }
    newImage("pix");
    showPix();
}

