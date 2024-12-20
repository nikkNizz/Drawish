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
#include "linedialog.h"
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
#include <QDesktopServices>
#include <QBuffer>
#include <QPrinter>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    version_info = "Version:  0.8\nEmail:  nicNiz@libero.it";
    // set label as drawing area

    ui->textOptionsWidget->setVisible(false);
    ui->lineOptionWidget->setGeometry(80,0,552,31);
    ui->lineOptionWidget->setVisible(false);
    ui->similaritywidget->setGeometry(80,0,552,31);
    ui->similaritywidget->setVisible(false);

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

    // Qpalette works in Linux for buttons bgcolors
#ifdef Q_OS_WIN
    ui->colorActiveButton->setStyleSheet("background-color: black");
    ui->blackButton->setStyleSheet("background-color: black");
    ui->whiteButton->setStyleSheet("background-color: white");
    ui->greyButton->setStyleSheet("background-color: grey");
    ui->redButton->setStyleSheet("background-color: red");
    ui->greenButton->setStyleSheet("background-color: green");
    ui->blueButton->setStyleSheet("background-color: blue");
    ui->yellowButton->setStyleSheet("background-color: yellow");
    ui->magentaButton->setStyleSheet("background-color: magenta");
    ui->cyanButton->setStyleSheet("background-color: cyan");
#endif

    // signals
    connect(borderB, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(borderR, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(corner, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(wArea, SIGNAL(endRubber()), this, SLOT(createSelectionFromRubb()));
    connect(wArea, SIGNAL(redraw()), this, SLOT(drawCopy()));
    connect(wArea, SIGNAL(penDraw()), this, SLOT(drawWithPen()));
    connect(wArea, SIGNAL(doublePenDraw()), this, SLOT(drawWithDoublePen()));
    connect(wArea, SIGNAL(finishDrawPen()), this, SLOT(showPix()));
    connect(wArea, SIGNAL(drawFirstPoint()), this, SLOT(draw_first_point()));
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
            QPixmap spix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
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

        if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::SquareCap);}
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
        pai.end();
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
   areaSize();
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
   updateInfo();
}


void MainWindow::areaSize()
{
     wArea->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
     if(pix.width() * pix.height() < 250000){
                 sizes::stopShow =6;
             }
             else if(pix.width() * pix.height() < 1000000){
                 sizes::stopShow=10;
             }
             else{
                 sizes::stopShow = 15;
             }
}

void MainWindow::raiseBorders()
{
    borderB->raise();
    borderR->raise();
    corner->raise();
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
       savePix(pix, f);
       sizes::modify = false;
}


void MainWindow::savePix(QPixmap pixToSave, QString f)
{
    if(f.endsWith(".jpg", Qt::CaseInsensitive)){ pixToSave.save(f, "jpg");}
    else if(f.endsWith(".ico", Qt::CaseInsensitive)){ pixToSave.save(f, "ico");}
    else if(f.endsWith(".bmp", Qt::CaseInsensitive)){ pixToSave.save(f, "BMP");}
    else{pixToSave.save(f, "PNG");}
}

void MainWindow::newImage(QString from)
{

   if(from == "zero"){
       sizes::modify= false;
       sizes::areaHeight=320;
       sizes::areaWidth=480;
       QPixmap npix(480, 320);
       npix.fill(Qt::white);
       pix = npix;
       activePathFile="";
   }
   else if(from == "f"){ // open file
      sizes::modify= false;
      QFileDialog dialog(this);
      QString f =dialog.getOpenFileName(this,"Drawish...Select image", QDir::homePath() );
      if(f ==""){return;}
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
       areaSize();
       wArea->setPixmap(pix);
       borderB->resetGeometry();
       borderR->resetGeometry();
       corner->resetGeometry();
}

QCursor MainWindow::rectCursor()
{
        int wh = ui->lineWidthBox->value()+2;
        QPixmap kursor(wh,wh);
        kursor.fill(Qt::black);
        // ------
        QPixmap fillCurs(wh-2, wh-2);
        fillCurs.fill(Qt::white);
        // -----
        QPainter pai(&kursor);
        pai.drawPixmap(1,1,fillCurs);
        return QCursor(kursor);

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
    ui->doublePen->setChecked(false);
    ui->fillButton->setChecked(false);
    ui->sprayButton->setChecked(false);
    ui->pickerButton->setChecked(false);
    ui->lineButton->setChecked(false);
    ui->shapeButton->setChecked(false);
    ui->curveButton->setChecked(false);
    ui->connLine->setChecked(false);

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
    if(f ==""){return;}
    savePix(pix, f);
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
    if(sizes::activeOperation != 2){  // selection area for text
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
    raiseBorders();
    updateInfo();
    connect(selectionRect, SIGNAL(setInfo()), this, SLOT(updateInfo()));
}


void MainWindow::drawCopy()
{    
    QPixmap selectedImage(selectionRect->pixmap());
    QPainter p(&pix);  
    p.drawPixmap(sizes::selX ,sizes::selY, selectedImage.scaled(sizes::selW, sizes::selH));
    wArea->setPixmap(pix);
    delete selectionRect;
    selectionRect = NULL;
    save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
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
        p_Clipboard->setPixmap(selectionRect->pixmap().scaled(sizes::selW, sizes::selH));
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
        raiseBorders();
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
    QMessageBox::information(this, "Drawish", "The image is in a selection");
    raiseBorders();
}


void MainWindow::on_actionMerge_selection_to_image_triggered()
{
    if(sizes::isSelectionOn == false){
        QMessageBox::information(this, "Drawish", "No area selected");
        return;
    }
    QPixmap selectedPix(selectionRect->pixmap().scaled(sizes::selW, sizes::selH));
    QPixmap underPix = pix.copy(sizes::selX, sizes::selY, sizes::selW, sizes::selH);
    QImage selectedImage = selectedPix.toImage();
    QImage underImage = underPix.toImage();

    QMessageBox msgBox(this);
    msgBox.setText("Levels");
    msgBox.addButton("50/50", QMessageBox::AcceptRole);
    msgBox.addButton("30/70", QMessageBox::RejectRole);
    msgBox.addButton("70/30", QMessageBox::ActionRole);
    int ret = msgBox.exec();

    int perc1=50;
    int perc2=50;
    if(ret == QMessageBox::RejectRole){ perc1 = 30; perc2 = 70;}
    else if(ret == QMessageBox::ActionRole){perc1 = 70; perc2 = 30;}
    int r1,r2,g1,g2,b1,b2;
    for(int xx= 0;  xx < sizes::selW; ++xx){
        for(int yy=0; yy < sizes::selH; ++yy){
            r1 = selectedImage.pixelColor(xx,yy).red();
            r2 = underImage.pixelColor(xx,yy).red();
            r1 = (r1 * perc1 + r2 * perc2)/100;
            g1 = selectedImage.pixelColor(xx,yy).green();
            g2 = underImage.pixelColor(xx,yy).green();
            g1 = (g1 * perc1 + g2 * perc2)/100;
            b1 = selectedImage.pixelColor(xx,yy).blue();
            b2 = underImage.pixelColor(xx,yy).blue();
            b1 = (b1 * perc1 + b2 * perc2)/100;
            selectedImage.setPixelColor(xx, yy, QColor(r1,g1,b1));
        }
    }
    selectionRect->setPixmap(QPixmap::fromImage(selectedImage));
    updateInfo();
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
    QString f =dialog.getOpenFileName(this,"Drawish...Select image", QDir::homePath() );
    if(f ==""){return;}
    QPixmap fpix(f);
    if(!fpix.isNull()){
        pasteImg(fpix);
    }else{
        QMessageBox::information(this, "Drawish", "Invalid image!");
    }
}

void MainWindow::on_actionCopy_selection_to_file_triggered()
{
    if(sizes::isSelectionOn){
        QPixmap selPix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
        QFileDialog dialog(this);
        QString f =dialog.getSaveFileName(this,"Drawish save image", QDir::homePath());
        if(f ==""){return;}
        savePix(selPix, f);

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
        delete selectionRect;
        selectionRect =NULL;
        sizes::activeOperation = 0;
        ui->textOptionsWidget->setVisible(false);
    }
}

void MainWindow::on_textEdit_textChanged()
{
    if(!sizes::isSelectionOn){
        wArea->setFocus();
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
    selectionRect->setText(ui->textEdit->toPlainText());
}


void MainWindow::on_confirmTextButton_clicked()
{
    if(ui->textEdit->toPlainText() != ""){
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
        p.drawText(QRect(sizes::selX+2, sizes::selY+2, sizes::selW, sizes::selH), ui->textEdit->toPlainText());
        wArea->setPixmap(pix);
        selectionRect->resetGeometry();
    }

}

//  SET COLORS....

void MainWindow::set_activeColor(int R, int G, int B, int A)
{
    sizes::activeColor= QColor(R,G,B, A);
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
}

void MainWindow::on_blackButton_clicked()
{
    set_activeColor(0,0,0);

}


void MainWindow::on_whiteButton_clicked()
{
     set_activeColor(255,255,255);

}


void MainWindow::on_greyButton_clicked()
{
     set_activeColor(128,128,128);

}


void MainWindow::on_redButton_clicked()
{
     set_activeColor(255,0,0);

}


void MainWindow::on_greenButton_clicked()
{
     set_activeColor(0,255,0);
}


void MainWindow::on_blueButton_clicked()
{
     set_activeColor(0,0,255);
}


void MainWindow::on_yellowButton_clicked()
{
     set_activeColor(255,255,0);
}


void MainWindow::on_magentaButton_clicked()
{
     set_activeColor(255,0,255);
}


void MainWindow::on_cyanButton_clicked()
{
     set_activeColor(0,255,255);
}


void MainWindow::on_transparentButton_clicked()
{
     set_activeColor(255,255,255,0);
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
        wArea->setCursor(rectCursor());
        sizes::activeOperation = 3;
        ui->lineOptionWidget->setVisible(true);
    }else{
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

    if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::SquareCap);}
    else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

    pai.setPen(pen);
    pai.drawLine(sizes::shape_x_begin, sizes::shape_y_begin, sizes::shape_x_end, sizes::shape_y_end);

    repeatShow++;
    sizes::shape_x_begin = sizes::shape_x_end;
    sizes::shape_y_begin = sizes::shape_y_end;
    if(repeatShow== sizes::stopShow){
    wArea->setPixmap(pix);
    repeatShow=1;
    }
}

void MainWindow::drawWithDoublePen()
{
    updateInfo();
    QPainter pai(&pix);
    QColor ncol = sizes::activeColor;
    if(ui->markerButton->isChecked()){
        ncol = QColor(sizes::activeColor.red(), sizes::activeColor.green(), sizes::activeColor.blue(), 32);
    }

    QPen pen(ncol, sizes::line_width);

    if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::SquareCap);}
    else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

    pai.setPen(pen);
    pai.drawLine(sizes::shape_x_begin, sizes::shape_y_begin, sizes::shape_x_end, sizes::shape_y_end);
    pai.end();

    QPainter pai2(&pix);
    QPen pen2(Qt::black, sizes::line_width);

    if(ui->flatcapButton->isChecked()){ pen2.setCapStyle(Qt::SquareCap);}
    else if(ui->roundcapButton->isChecked()){pen2.setCapStyle(Qt::RoundCap);}

    pai2.setPen(pen2);
    int d= sizes::line_width*1.2;
    if(sizes::shape_x_begin < sizes::shape_x_end){
        if(sizes::shape_y_begin < sizes::shape_y_end){
             pai2.drawLine(sizes::shape_x_begin +d, sizes::shape_y_begin, sizes::shape_x_end +d, sizes::shape_y_end);
        }
        else if(sizes::shape_y_begin > sizes::shape_y_end){
            pai2.drawLine(sizes::shape_x_begin -d , sizes::shape_y_begin, sizes::shape_x_end -d, sizes::shape_y_end );
        }
        else if(sizes::shape_y_begin == sizes::shape_y_end){
            pai2.drawLine(sizes::shape_x_begin, sizes::shape_y_begin -d,sizes::shape_x_end, sizes::shape_y_end-d );
        }

    }
    else if(sizes::shape_x_begin > sizes::shape_x_end){
        if(sizes::shape_y_begin < sizes::shape_y_end){
             pai2.drawLine(sizes::shape_x_begin +d, sizes::shape_y_begin, sizes::shape_x_end +d, sizes::shape_y_end);
        }
        else if(sizes::shape_y_begin > sizes::shape_y_end){
            pai2.drawLine(sizes::shape_x_begin -d , sizes::shape_y_begin, sizes::shape_x_end -d, sizes::shape_y_end );
        }
        else if(sizes::shape_y_begin == sizes::shape_y_end){
            pai2.drawLine(sizes::shape_x_begin, sizes::shape_y_begin -d,sizes::shape_x_end, sizes::shape_y_end-d );
        }
    }
    else if(sizes::shape_x_begin == sizes::shape_x_end){
        if(sizes::shape_y_begin < sizes::shape_y_end){
             pai2.drawLine(sizes::shape_x_begin +d, sizes::shape_y_begin, sizes::shape_x_end +d, sizes::shape_y_end);
        }
        else if(sizes::shape_y_begin > sizes::shape_y_end){
            pai2.drawLine(sizes::shape_x_begin -d , sizes::shape_y_begin, sizes::shape_x_end -d, sizes::shape_y_end );
        }
    }

    repeatShow++;
    sizes::shape_x_begin = sizes::shape_x_end;
    sizes::shape_y_begin = sizes::shape_y_end;
    if(repeatShow== sizes::stopShow){
    wArea->setPixmap(pix);
    repeatShow=1;
    }
}

void MainWindow::draw_first_point()
{
    if(sizes::activeOperation == 5){
        save_previous("Spray");
        return;
    }
    if(sizes::activeOperation == 10){
        save_previous("Double pen");
    }else{
        save_previous("Pen");
    }
    updateInfo();
    QPainter pai(&pix);
    QColor ncol = sizes::activeColor;
    if(ui->markerButton->isChecked()){
        ncol = QColor(sizes::activeColor.red(), sizes::activeColor.green(), sizes::activeColor.blue(), 32);
    }

    QPen pen(ncol, sizes::line_width);

    if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::SquareCap);}
    else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

    pai.setPen(pen);
    pai.drawPoint(sizes::shape_x_begin, sizes::shape_y_begin);
    pai.end();
    if(sizes::activeOperation == 10){
        QPainter pai2(&pix);
        QPen pen2(Qt::black, sizes::line_width);

        if(ui->flatcapButton->isChecked()){ pen2.setCapStyle(Qt::SquareCap);}
        else if(ui->roundcapButton->isChecked()){pen2.setCapStyle(Qt::RoundCap);}

        pai2.setPen(pen2);
        pai2.drawPoint(sizes::shape_x_begin+sizes::line_width/2, sizes::shape_y_begin-sizes::line_width/2);
    }
    wArea->setPixmap(pix);
}

// double pen

void MainWindow::on_doublePen_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }

    if(ui->doublePen->isChecked()){
        untoggle();
        ui->doublePen->setChecked(true);
        wArea->setCursor(rectCursor());
        sizes::activeOperation = 10;
        ui->lineOptionWidget->setVisible(true);
    }else{
        sizes::activeOperation = 0;
        wArea->setCursor(Qt::ArrowCursor);
        ui->lineOptionWidget->setVisible(false);
    }
}

//-----------------------------------------------
void MainWindow::showPix()
{
    wArea->setPixmap(pix);
}

void MainWindow::save_previous(QString tx)
{
    historyList.push_back(tx);
    prePix = QPixmap();
    historyPix.push_back(pix);
    if(historyList.count() > 8){
        historyList.removeFirst();
        historyPix.removeFirst();
    }
    ui->historyCombo->clear();
    ui->historyCombo->addItems(historyList);
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
        sizes::activeOperation = 0;
        ui->similaritywidget->setVisible(false);
    }
}

void MainWindow::fill_()
{
    if(ui->fillInsideBlack_check->isChecked()){
        fillInsideBlack();
        return;
    }
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

void MainWindow::fillInsideBlack()
{
    save_previous("Fill inside black");
    updateInfo();
    QImage img = pix.toImage();
    img =img.convertToFormat(QImage::Format_ARGB32);
    QList<int> coord;
    coord.push_back(sizes::selX);
    coord.push_back(sizes::selY);
    QColor oldColor= QColor::fromRgb(img.pixel(sizes::selX, sizes::selY));
    if(oldColor == Qt::black)return;
    img.setPixelColor(sizes::selX, sizes::selY, Qt::black);
    int posx, posy;

    while(coord.count()>0){
        posx = coord[0];
        posy = coord[1];
        coord.remove(0,2);
        if(posy-1 > -1 ){
            if(QColor::fromRgb(img.pixel(posx, posy-1)) != QColor(0,0,0)){
                coord.push_back(posx);
                coord.push_back(posy-1);
                img.setPixelColor(posx, posy-1, Qt::black);
            }
        }
        if(posx-1 > -1 ){
            if(QColor::fromRgb(img.pixel(posx-1, posy)) != QColor(0,0,0)){
                coord.push_back(posx-1);
                coord.push_back(posy);
                img.setPixelColor(posx-1, posy, Qt::black);
            }
        }

        if(posx+1 < img.width() ){
            if(QColor::fromRgb(img.pixel(posx+1, posy)) != QColor(0,0,0)){
                coord.push_back(posx+1);
                coord.push_back(posy);
                img.setPixelColor(posx+1, posy, Qt::black);
            }
        }
        if(posy+1 < img.height() ){
            if(QColor::fromRgb(img.pixel(posx, posy+1)) != QColor(0,0,0)){
                coord.push_back(posx);
                coord.push_back(posy+1);
                img.setPixelColor(posx, posy+1, Qt::black);
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
        wArea->setCursor(rectCursor());
    }else{
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
    if(repeatShow== sizes::stopShow){
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

    if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::SquareCap);}
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

        if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::SquareCap);}
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
    raiseBorders();
}

void MainWindow::on_lineWidthBox_valueChanged(int arg1)
{
    sizes::line_width = arg1;
    if(sizes::activeOperation ==3 || sizes::activeOperation ==5 || sizes::activeOperation == 10){
      wArea->setCursor(rectCursor());
    }
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


// connected lines
void MainWindow::on_connLine_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    wArea->setCursor(Qt::ArrowCursor);
    if(ui->connLine->isChecked()){
        untoggle();
        ui->connLine->setChecked(true);
        sizes::activeOperation = 11;
        sizes::shape_x_begin = -1;
        sizes::shape_y_begin = -1;
        ui->lineOptionWidget->setVisible(true);
        cl_area = new curveLineArea(wArea);
        connect(cl_area, SIGNAL(finishLines()), this, SLOT(finish_lines()));
        sizes::isCurveLineAreaOn = true;
        cl_area->show();
    }else{
        sizes::activeOperation = 0;
        ui->lineOptionWidget->setVisible(false);
        if(sizes::isCurveLineAreaOn){
            sizes::isCurveLineAreaOn = false;
            delete cl_area;
            cl_area =NULL;
        }
    }
}

void MainWindow::finish_lines()
{
    save_previous("Connected lines");
    if(sizes::isCurveLineAreaOn){
      QPixmap cPix = cl_area->pixmap();
      QPainter p(&pix);
      p.drawPixmap(0,0, cPix);
      showPix();
      delete cl_area;
      cl_area =NULL;
      // continue
      cl_area = new curveLineArea(wArea);
      connect(cl_area, SIGNAL(finishLines()), this, SLOT(finish_lines()));
      cl_area->show();
    }
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
      // put in a visible area if possible
      int zx = sizes::selX+2;
      int zy = sizes::selY;
      if(zx > wArea->width()-238 ){zx = wArea->width()-239;}
      if(zx < 10){zx =10;}
      if(zy > wArea->height()-238){ zy = wArea->height()-239;}
      if(zy < 0){zy =0;}
      zoom_area->setGeometry(zx, zy , 238,238);
      zoom_area->show();
      raiseBorders();
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
    QPixmap pxx = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
    int newDim = sizes::selW + sizes::selH;
    QPixmap rotPix(newDim, newDim);
    rotPix.fill(QColor(255,255,255,0));
    QPainter painter(&rotPix);
    QTransform transform;
    if(a == 1){                       // right
     transform.translate(newDim * 0.5, newDim * 0.3);
    }else{                            // left
     transform.translate(newDim * 0.3, newDim * 0.3);
    }
        transform.rotate(ui->RotatioAngleSpin->value() * a);
        painter.setTransform(transform);

    painter.drawPixmap(0, 0, pxx);
    sizes::selH = newDim;
    sizes::selW = newDim;
    selectionRect->resetGeometry();
    selectionRect->setPixmap(rotPix);
    updateInfo();
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
    QPixmap sPix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH).transformed(tf);
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
        areaSize();
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
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
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
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
    }

    dColors dCol(this, Epix);
    dCol.setModal(true);
    dCol.exec();
    // update active color
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
    //---
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
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
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
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
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
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
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
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
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
        savePix(camPix, f);
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
        untoggle();
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


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "Drawish", version_info);
}


void MainWindow::on_actionQuadruple_the_pixels_triggered()
{
  if(sizes::isSelectionOn){
      QImage img = selectionRect->pixmap().scaled(sizes::selW, sizes::selH).toImage();
      sizes::selH *=2;
      sizes::selW *=2;
       QPixmap bigPix(sizes::selW, sizes::selH);
       QImage bigImage = bigPix.toImage();
       int nx=0;
       int ny =0;
       for (int y = 0; y < img.height(); ++y) {
           QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
           for (int x = 0; x < img.width(); ++x) {
               QRgb &rgb = line[x];
               nx = x * 2;
               ny = y * 2;
               bigImage.setPixelColor(nx, ny, QColor(rgb));
               bigImage.setPixelColor(nx+1, ny, QColor(rgb));
               bigImage.setPixelColor(nx, ny+1, QColor(rgb));
               bigImage.setPixelColor(nx+1, ny+1, QColor(rgb));
           }
       }
       bigPix = QPixmap::fromImage(img);
       selectionRect->resetGeometry();
       selectionRect->setPixmap(bigPix);

  }
  else{
  untoggle();
  save_previous("Quadruple");
  sizes::areaWidth *=2;
  sizes::areaHeight *=2;
  QPixmap bigPix(sizes::areaWidth, sizes::areaHeight);
  QImage bigImage = bigPix.toImage();
  QImage img = pix.toImage();
  int nx=0;
  int ny =0;
  for (int y = 0; y < img.height(); ++y) {
      QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
      for (int x = 0; x < img.width(); ++x) {
          QRgb &rgb = line[x];
          nx = x * 2;
          ny = y * 2;
          bigImage.setPixelColor(nx, ny, QColor(rgb));
          bigImage.setPixelColor(nx+1, ny, QColor(rgb));
          bigImage.setPixelColor(nx, ny+1, QColor(rgb));
          bigImage.setPixelColor(nx+1, ny+1, QColor(rgb));
      }
  }
  pix = QPixmap::fromImage(bigImage);
  areaSize();
  showPix();
  borderB->resetGeometry();
  borderR->resetGeometry();
  corner->resetGeometry();
  }
  updateInfo();
}


void MainWindow::on_actionDivide_by_4_triggered()
{
    if(sizes::isSelectionOn){
        QImage img = selectionRect->pixmap().scaled(sizes::selW, sizes::selH).toImage();
        sizes::selH /=2;
        sizes::selW /=2;
        QPixmap smallPix(sizes::selW, sizes::selH);
        QImage smallImage = smallPix.toImage();
        for (int y = 0; y < img.height()-1; y +=2) {
            QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
            for (int x = 0; x < img.width()-1; x +=2) {
                QRgb &rgb = line[x];
                smallImage.setPixelColor(x/2, y/2, QColor(rgb));
            }
        }
        smallPix = QPixmap::fromImage(img);
        selectionRect->resetGeometry();
        selectionRect->setPixmap(smallPix);
    }
    else{
    untoggle();
    save_previous("Divide by 4");
    sizes::areaWidth /=2;
    sizes::areaHeight /=2;
    QPixmap smallPix(sizes::areaWidth, sizes::areaHeight);
    QImage smallImage = smallPix.toImage();
    QImage img = pix.toImage();

    for (int y = 0; y < img.height(); y +=2) {
        QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
        for (int x = 0; x < img.width(); x +=2) {
            QRgb &rgb = line[x];
            smallImage.setPixelColor(x/2, y/2, QColor(rgb));
        }
    }
    pix = QPixmap::fromImage(smallImage);
   areaSize();
    showPix();
    borderB->resetGeometry();
    borderR->resetGeometry();
    corner->resetGeometry();
    }
    updateInfo();
}


void MainWindow::on_actionCreate_Line_triggered()
{
    LineDialog lineD;
    lineD.setModal(true);
    lineD.exec();
    if(lineD.res == QDialog::Accepted){
        save_previous("Line input");
        QPainter pai(&pix);
        QPen pen(sizes::activeColor, sizes::line_width);

        if(ui->flatcapButton->isChecked()){ pen.setCapStyle(Qt::SquareCap);}
        else if(ui->roundcapButton->isChecked()){pen.setCapStyle(Qt::RoundCap);}

        pai.setPen(pen);
        pai.drawLine(lineD.linex1, lineD.liney1, lineD.linex2, lineD.liney2);
        wArea->setPixmap(pix);
    }
}

void MainWindow::on_actionGithub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/nikkNizz/Drawish"));
}

void MainWindow::on_comboBox_activated(int index)
{
    sizes::FillStyle = index;
}


void MainWindow::on_actionbase64_triggered()
{
    if(!isCam){
        QMessageBox::warning(this, "Drawish", "Na active cam!");
        return;
    }
    QPixmap camPix = pix.copy(save_area->pos().x(), save_area->pos().y(), save_area->width(), save_area->height());
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    camPix.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(encoded);
    QMessageBox::warning(this, "Drawish", "base64 code has been copied to clipboard!");
}


void MainWindow::on_actionTo_Pdf_triggered()
{
    QString outName =QDir::homePath() + "/drawish";
    if(activePathFile != ""){
        const QFileInfo info(activePathFile);
        outName =QDir::homePath() + "/" + info.fileName();
   }

    int numOf =0;
    bool exists = true;
    QString cmpName = outName + ".pdf";
    while( exists ) {
        if(QFile(cmpName).exists()){
            numOf++;
            cmpName = outName + "_" + QString::number(numOf) +  ".pdf";
        }else{
            break;
        }
    }

    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
       printer.setOutputFileName(cmpName);
       QPainter painter;
       if (! painter.begin(&printer)) {
           QMessageBox::warning(this, "Drawish", "Failed to open image!");
           return ;
       }
       painter.drawPixmap(0,0, sizes::areaWidth, sizes::areaHeight, pix);
       painter.end();
       QMessageBox::information(this, "Drawish", "Saved in\n" + cmpName);
}
