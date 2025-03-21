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
#include "stretchdialog.h"
#include "fileio.h"
#include "figures.h"
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
#include <QProcess>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    version_info = "0.9.8";
    // 0.9.8: correct undo selection  ; del selection when camera; add to recent file not modified too
    // pen transparency to 16         ; fill with semi transpar. ; multicolor pen
    // replace color option           ; draw diag with keyboard  ;


    isLinux = false;
#ifdef Q_OS_LINUX
    isLinux = true;
#endif


// set label as drawing area
    ui->textOptionsWidget->setVisible(false);
    ui->lineOptionWidget->setGeometry(80,0,552,31);
    ui->lineOptionWidget->setVisible(false);
    ui->similaritywidget->setGeometry(80,0,552,31);
    ui->similaritywidget->setVisible(false);
    ui->widgetPick->setGeometry(80,0,552,31);
    ui->widgetPick->setVisible(false);

    wArea = new Area();

    // set borders
    borderR = new BorderRight(wArea);
    borderB = new BorderBottom(wArea);
    corner = new Angolo(wArea);

    // open passed file (from command line)
        if(sizes::passedFile != "" && QFile::exists(sizes::passedFile)){
        QPixmap npix;

        if(sizes::passedFile.endsWith("pdf", Qt::CaseInsensitive) && isLinux){
             npix = openPdf(sizes::passedFile);

        }else{
             npix = QPixmap(sizes::passedFile);
             activePathFile = sizes::passedFile;
             addToRecent(activePathFile);
        }
            if(npix.isNull()){
                QMessageBox::information(this, "Drawish", tr("Unsupported file"));
                 // set default image to area
                pix =QPixmap(sizes::areaWidth-8, sizes::areaHeight-8);
                pix.fill(Qt::white);
            }else{

            pix = npix;
            sizes::areaHeight=pix.height()+8;
            sizes::areaWidth=pix.width()+8;
            borderB->resetGeometry();
            borderR->resetGeometry();
            corner->resetGeometry();
            }
        }else{
             // set default image to area
        pix =QPixmap(sizes::areaWidth-8, sizes::areaHeight-8);
        pix.fill(Qt::white);
        }
        wArea->setPixmap(pix);
        wArea->setAlignment(Qt::AlignTop);

    historyList.push_back(tr("Original image"));
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
    ui->greenButton->setStyleSheet("background-color: lime");
    ui->blueButton->setStyleSheet("background-color: blue");
    ui->yellowButton->setStyleSheet("background-color: yellow");
    ui->magentaButton->setStyleSheet("background-color: magenta");
    ui->cyanButton->setStyleSheet("background-color: cyan");
#endif

    readConfig();

    // signals
    connect(borderB, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(borderR, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(corner, SIGNAL(sizeChange()), this, SLOT(reSize()) );
    connect(wArea, SIGNAL(endRubber()), this, SLOT(createSelectionFromRubb()));
    connect(wArea, SIGNAL(redraw()), this, SLOT(drawCopy()));
    connect(wArea, SIGNAL(penDraw()), this, SLOT(drawWithPen()));
    connect(wArea, SIGNAL(finishDrawPen()), this, SLOT(showPix()));
    connect(wArea, SIGNAL(drawFirstPoint()), this, SLOT(draw_first_point()));
    connect(wArea, SIGNAL(readyToFill()), this, SLOT(fill_()));
    connect(wArea, SIGNAL(sprayDraw()), this, SLOT(spray_draw()));
    connect(wArea, SIGNAL(getColor()), this, SLOT(get_color()));
    connect(wArea, SIGNAL(setInfo()), this, SLOT(updateInfo()));
    connect(wArea, SIGNAL(drawShape()), this, SLOT(draw_shape()));
    connect(wArea, SIGNAL(viewZoom()), this, SLOT(view_zoom()));

    historyCount = 1;
    updateInfo();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()){event->acceptProposedAction();}
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls()){
            foreach(QUrl url, event->mimeData()->urls()){
                QFileInfo f(url.toLocalFile());
                if(sizes::modify){
                    save_previous(tr("Open"));
                    int q = QMessageBox::question(this, "Drawish", tr("Save image?"),QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
                    if(q == QMessageBox::Yes){
                        imgSave();
                    }
                 }
                //open image

                 QPixmap npix;
                 if(f.filePath().endsWith(".pdf", Qt::CaseInsensitive) && isLinux){
                    npix = openPdf(f.filePath());
                 }else{
                     npix = QPixmap(f.filePath());
                     if(npix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file")); return;}
                     activePathFile = f.filePath();
                     addToRecent(activePathFile);
                 }
                save_previous(tr("Open"));

                if(!npix.isNull()) pix = npix;

                sizes::areaHeight = pix.height()+8;
                sizes::areaWidth  = pix.width()+8;

                areaSize();
                wArea->setPixmap(pix);

                borderR->resetGeometry();
                borderB->resetGeometry();
                corner->resetGeometry();
                updateInfo();
            }
        }
}

QPixmap MainWindow::openPdf(QString fileName)
{
    QPixmap zpix;
    int num = QInputDialog::getInt(this, "Drawish", tr("Enter page number to convert"), 1);
    QFileInfo f(fileName);
    QByteArray result;
    QStringList arguments;
    arguments << f.filePath() << "/tmp/" + f.baseName() << "-png" << "-f" << QString::number(num) << "-l" << QString::number(num);
    QProcess pdftoppm;
    pdftoppm.start("pdftoppm", arguments);
    bool started = pdftoppm.waitForStarted();
    if(!started){ QMessageBox::question(this, "Drawish", tr("Error executing command")); }
    pdftoppm.waitForFinished();
    result = pdftoppm.readAllStandardOutput();
    zpix = QPixmap("/tmp/" + f.baseName() + "-" + QString::number(num) + ".png");
    if(zpix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file"));
        return QPixmap();
    }
    QFile::remove("/tmp/" + f.baseName() + "-" + QString::number(num) + ".png");
    activePathFile = "";
    return zpix;
}

void MainWindow::readConfig()
{
    QString user = QDir::homePath();
    configPath = user + "/AppData/Roaming/DrawishConfig.txt";
    if(isLinux){
        QDir c(user + "/.config");
        if(!c.exists()){
            c.mkdir(user + "/.config");
        }
        configPath = user + "/.config/DrawishConfig.txt";
    }

    fileIO fio;
    if(!QFile::exists(configPath)){
        fio.createFile("<recent></recent><links>www.remove.bg\nperchance.org/ai-text-to-image-generator\ncloudconvert.com/image-converter\nphotocartoon.net\nwww.iconfinder.com</links>", configPath);
    }

    QString config = fio.readFile(configPath);
    int init = config.indexOf("<recent>",0, Qt::CaseInsensitive) + 8;
    int endit = config.indexOf("</recent>", init,Qt::CaseInsensitive);
    configRecent = config.mid(init, endit-init);
    QStringList recents = configRecent.split("\n");
    if(recents.count() > 0){
    // create actions
    int maxR = recents.count();
    if(maxR > 10) maxR = 10;
    for(int i =0; i < maxR; ++i){
        if(recents.at(i) !=""){
            QAction *actionRecent;
            actionRecent = new QAction(this);
            actionRecent->setObjectName(recents.at(i));
            actionRecent->setText(recents.at(i));
            ui->menuRecent->addAction(actionRecent);
            connect(actionRecent, SIGNAL(triggered()), this, SLOT(open_file()));
        }
    }
    }
    init = config.indexOf("<links>",0, Qt::CaseInsensitive) + 7;
    endit = config.indexOf("</links>", init,Qt::CaseInsensitive);
    configLinks =config.mid(init, endit-init);
    QStringList links = configLinks.split("\n");
    if(links.count() > 0){
        // create actions
        int maxR = links.count();
        if(maxR > 10) maxR = 10;
        for(int i =0; i < maxR; ++i){
            if(links.at(i) !=""){
                QAction *actionRecent;
                actionRecent = new QAction(this);
                actionRecent->setObjectName(links.at(i));
                actionRecent->setText(links.at(i));
                ui->menuLinks->addAction(actionRecent);
                connect(actionRecent, SIGNAL(triggered()), this, SLOT(open_link()));
            }
        }
    }
}

void MainWindow::open_file()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString url = sender()->objectName();
        on_actionSave_triggered();
        newImage("f", url);
    }
}

void MainWindow::open_link()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString url = sender()->objectName();
        if(!url.startsWith("http")){ url = "https://" + url;}
        QDesktopServices::openUrl(QUrl(url));
    }
}

void MainWindow::addToRecent(QString pf)
{
    if(!configRecent.contains(pf)){
        configRecent = pf + "\n" + configRecent;
        if(configRecent.count("\n") > 10){
            int lastN = configRecent.lastIndexOf("\n");
            configRecent = configRecent.mid(0, lastN);
        }
    }
}

QStringList MainWindow::askForValues()
{
   QString x = QInputDialog::getMultiLineText(this, "Drawish", tr("Enter Positive values (One per line!)"));
    return x.split("\n");

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

        QPen pen(configPen(ncol));

        pai.setPen(pen);
        int num =1;
        int xFin= sizes::selX;
        int yFin = sizes::selY;
        if(ui->radioButton_2->isChecked()) num=5;
        if(ev->key() ==Qt::Key_W ){ yFin = sizes::selY-num;
        }
        else if(ev->key()== Qt::Key_A){xFin = sizes::selX-num;}
        else if(ev->key()== Qt::Key_S){xFin = sizes::selX+num;}
        else if(ev->key()== Qt::Key_Z){yFin =sizes::selY+num;}
        else if(ev->key()== Qt::Key_Q){xFin = sizes::selX -num; yFin =sizes::selY-num;}
        else if(ev->key()== Qt::Key_E){xFin = sizes::selX +num; yFin =sizes::selY-num;}
        else if(ev->key()== Qt::Key_X){xFin = sizes::selX +num; yFin =sizes::selY+num;}
        else if(ev->key()== Qt::Key_Less){xFin = sizes::selX -num; yFin =sizes::selY+num;}



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
   if ((ev->key() == Qt::Key_Z)  && (ev->modifiers().testFlag(Qt::ControlModifier))){
       if(ui->historyCombo->count() > 0) {
           if(ui->historyCombo->count() - historyCount < 0){ historyCount = 1;}
           on_historyCombo_activated(ui->historyCombo->count()-historyCount);
       }
       historyCount++;
   }

}

void MainWindow::closeEvent(QCloseEvent *ev)
{
    if(activePathFile != "")  addToRecent(activePathFile);
    fileIO fio;
    fio.createFile("<recent>" + configRecent + "</recent><links>" + configLinks + "</links>" , configPath);

    if(sizes::modify){
        int q = QMessageBox::question(this, "Drawish", tr("Save image?"),QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
            imgSave();
            fio.createFile("<recent>" + configRecent + "</recent><links>" + configLinks + "</links>" , configPath);
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
   if(!sizes::startResize){save_previous(tr("Resize"));}
   sizes::startResize=true;
   areaSize();
   if(sizes::isCurveLineAreaOn){
       cl_area->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
   }
   QPixmap pix2(sizes::areaWidth-8, sizes::areaHeight-8);
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
        f =dialog.getSaveFileName(this, tr("Drawish save..."), QDir::homePath(),"Images (*.png *.jpg *.ico *bmp)");
        if(f ==""){return;}
    }else{
     f = activePathFile;

    }
    addToRecent(f);
       savePix(pix, f);
       sizes::modify = false;
}

QString MainWindow::ChooseImg()
{
    QFileDialog dialog(this);
    return dialog.getOpenFileName(this,tr("Drawish...Select image"), QDir::homePath() );
}


void MainWindow::savePix(QPixmap pixToSave, QString f)
{
    if(f.endsWith(".jpg", Qt::CaseInsensitive)){ pixToSave.save(f, "jpg");}
    else if(f.endsWith(".ico", Qt::CaseInsensitive)){ pixToSave.save(f, "ico");}
    else if(f.endsWith(".bmp", Qt::CaseInsensitive)){ pixToSave.save(f, "BMP");}
    else{pixToSave.save(f, "PNG");}
}

void MainWindow::newImage(QString from, QString path)
{

   if(from == "zero"){
       sizes::modify= false;
       sizes::areaHeight=328;
       sizes::areaWidth=488;
       QPixmap npix(480, 320);
       npix.fill(Qt::white);
       pix = npix;
       activePathFile="";
   }

   else if(from == "f"){ // open file

      QFileDialog dialog(this);
      QString f = path;
      if(f == ""){
       f =dialog.getOpenFileName(this, tr("Drawish...Select image"), QDir::homePath() );
       if(f ==""){return;}
      }

      QPixmap npix;
      if(f.endsWith(".pdf", Qt::CaseInsensitive) && isLinux){
          npix = openPdf(f);
      }else{
          npix = QPixmap(f);
          if(npix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file")); return;}
          activePathFile = f;
          addToRecent(activePathFile);
      }
      sizes::modify= false;

      if(npix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file")); return;}
      pix = npix;
      sizes::areaHeight=pix.height()+8;
      sizes::areaWidth=pix.width()+8;
   }
   else if(from == "pix"){
       sizes::areaHeight=pix.height()+8;
       sizes::areaWidth=pix.width()+8;
   }
       areaSize();
       wArea->setPixmap(pix);
       borderB->resetGeometry();
       borderR->resetGeometry();
       corner->resetGeometry();
       updateInfo();
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
        pai.end();
        if(ui->comboPen->currentIndex()==3 && wh > 5){
            QPainter pai2(&kursor);
            pai2.setPen(Qt::gray);
            pai2.drawLine(1, 1, kursor.width()-1, kursor.height()-1);
        }
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
    ui->conn_Curve->setChecked(false);
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
    ui->widgetPick->setVisible(false);
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
        int q = QMessageBox::question(this, "Drawish", tr("Save image?"),QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
            imgSave();
            newImage("zero");
        }else if(q == QMessageBox::No){
             newImage("zero");
        }
    }else{
         save_previous(tr("New Image"));
         newImage("zero");
    }
}

void MainWindow::on_actionOpen_triggered()
{
    delete selectionRect; selectionRect = NULL;
    sizes::isSelectionOn= false;
    if(sizes::modify){
        save_previous(tr("Open"));
        int q = QMessageBox::question(this, "Drawish", tr("Save image?"), QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
            imgSave();
            newImage("f");
        }else if(q == QMessageBox::No){             
             newImage("f");
        }
     }else{  newImage("f");}
}


void MainWindow::on_actionSave_triggered()
{
    if(sizes::modify ){
        int q = QMessageBox::question(this, "Drawish", tr("Save image?"), QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
        imgSave();
        }
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QFileDialog dialog(this);
    QString f =dialog.getSaveFileName(this, tr("Drawish save image"), QDir::homePath());
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
      historyCount =1;
    }
}


// selection
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
    }else{
        ui->textEdit->setFocus();
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
    sizes::selH = sizes::areaHeight-8;
    sizes::selW = sizes::areaWidth-8;
    save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
    untoggle();
    sizes::activeOperation= 1;
    ui->selectionAreaButton->setChecked(true);

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
    }else if(mimeData->hasText()){
        QPixmap pxm(pix.width(), pix.height());
        pxm.fill(QColor(255,255,255,0));
        QPainter p(&pxm);
        QPen pen(Qt::black);
        p.setPen(pen);
        QFont font("arial", 18);
        p.setFont(font);
        p.drawText(20,20 , clipboard->text());
        pasteImg(pxm);
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

    untoggle();
    ui->selectionAreaButton->setChecked(true);
    sizes::activeOperation =1;
    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    selectionRect->setPixmap(passedPix.scaled(sizes::selW, sizes::selH));
    sizes::isSelectionOn=true;
    sizes::startResize=false;
    selectionRect->show();
    QMessageBox::information(this, "Drawish", tr("The image is in a selection"));
    raiseBorders();
}


void MainWindow::on_actionMerge_selection_to_image_triggered()
{
    if(sizes::isSelectionOn == false){
        QMessageBox::information(this, "Drawish", tr("No area selected"));
        return;
    }
    QPixmap selectedPix(selectionRect->pixmap().scaled(sizes::selW, sizes::selH));
    QPixmap underPix = pix.copy(sizes::selX, sizes::selY, sizes::selW, sizes::selH);
    QImage selectedImage = selectedPix.toImage();
    QImage underImage = underPix.toImage();

    QMessageBox msgBox(this);
    msgBox.setText(tr("Levels"));
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

void MainWindow::on_actionAdd_as_selection_triggered() // paste from file
{
    QString f = ChooseImg();
    if(f ==""){return;}
    QPixmap fpix(f);
    if(!fpix.isNull()){
        pasteImg(fpix);
    }else{
        QMessageBox::information(this, "Drawish", tr("Invalid image!"));
    }
}

void MainWindow::on_actionAdd_right_triggered()  // paste from file
{
    QString f = ChooseImg();
    if(f ==""){return;}
    QPixmap fpix(f);
    if(fpix.isNull()) {
        QMessageBox::information(this, "Drawish", tr("Invalid image!"));
        return;
    }
    save_previous(tr("Paste right"));
    int wImg = fpix.width();
    int hImg = fpix.height();
    sizes::areaWidth += wImg;
    if(hImg > sizes::areaHeight-8){ sizes::areaHeight = hImg+8; }
    reSize();
    QPainter p(&pix);
    int v= (sizes::areaWidth-8-wImg) + 1 ;
    p.drawPixmap(v, 0, fpix);
    showPix();
}


void MainWindow::on_actionAdd_bottom_triggered()  // paste from file
{
    QString f = ChooseImg();
    if(f ==""){return;}
    QPixmap fpix(f);
    if(fpix.isNull()) {
        QMessageBox::information(this, "Drawish", tr("Invalid image!"));
        return;
    }
    save_previous(tr("Paste bottom"));
    int wImg = fpix.width();
    int hImg = fpix.height();
    sizes::areaHeight += fpix.height();
    if(wImg > sizes::areaWidth-8){ sizes::areaWidth = wImg+8; }
    reSize();
    QPainter p(&pix);
    int v= (sizes::areaHeight-8-hImg) + 1 ;
    p.drawPixmap(0, v, fpix);
    showPix();
}



void MainWindow::on_actionCopy_selection_to_file_triggered()
{
    if(sizes::isSelectionOn){
        QPixmap selPix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
        QFileDialog dialog(this);
        QString f =dialog.getSaveFileName(this, tr("Drawish save image"), QDir::homePath());
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
        QMessageBox::information(this, "Drawish", tr("Click a point on the canvas, before"));
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
        save_previous(tr("Text"));
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
    sizes::activeColor = QColorDialog::getColor(Qt::gray, this, tr("Drawish choose color"));
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

QPen MainWindow::configPen(QColor &ncol, int alpha)
{
    // combopen: 0 round, 1 square, 2 flat, 3 nib, 4 rand round, 5 random square, 6 random red
    // 7 random green, 8 random blue
    ncol = sizes::activeColor;
    int jColor= ui->comboPen->currentIndex();
    if(jColor > 3){ // random
         quint32 red = QRandomGenerator::global()->bounded(128) ;
         quint32 green = QRandomGenerator::global()->bounded(128) ;
         quint32 blue = QRandomGenerator::global()->bounded(128) ;
         if(jColor == 6){ green = red * 0.6 ; blue = red * 0.6; red += 128;}
         else if(jColor == 7){ red = green *0.6; blue = green * 0.6; green += 128;}
         else if(jColor == 8){ red = blue * 0.6; green = blue *0.6; blue += 128;}
         else{ red *= 2; green *= 2; blue *=2; }
         ncol = QColor(red, green, blue);
    }

    if(ui->markerButton->isChecked()){
        ncol.setAlpha(alpha);
    }
    QPen pen1(ncol, sizes::line_width);
    if(jColor == 3) pen1.setWidth(1); // nib
    else if(jColor == 1 || jColor == 5){ pen1.setCapStyle(Qt::SquareCap);}
    else if(jColor ==2)                { pen1.setCapStyle(Qt::FlatCap);}
    else                               { pen1.setCapStyle(Qt::RoundCap);}
    return pen1;
}



void MainWindow::drawWithPen(){

    updateInfo();
    QPainter pai(&pix);
    QColor ncol = sizes::activeColor;
    QRect reg(qMin(sizes::shape_x_begin,sizes::shape_x_end), qMin(sizes::shape_y_begin,sizes::shape_y_end), abs(sizes::shape_x_end-sizes::shape_x_begin), abs(sizes::shape_y_end-sizes::shape_y_begin));

    // stylus
    if(ui->comboPen->currentIndex()== 3){
        int corners = sizes::line_width / 2;
        QPen pen(configPen(ncol));
        pai.setPen(pen);
        QBrush br(ncol);
        pai.setBrush(br);
        QPolygon poly;
        poly << QPoint(sizes::shape_x_begin + corners, sizes::shape_y_begin + corners) << QPoint(sizes::shape_x_begin - corners, sizes::shape_y_begin - corners) << QPoint(sizes::shape_x_end - corners, sizes::shape_y_end - corners) << QPoint(sizes::shape_x_end + corners, sizes::shape_y_end + corners);
        pai.drawPolygon(poly);
        pai.setClipRegion(reg);
    }else{
        // normal pen
        QPen pen(configPen(ncol));
        pai.setPen(pen);
        pai.drawLine(sizes::shape_x_begin, sizes::shape_y_begin, sizes::shape_x_end, sizes::shape_y_end);
        pai.setClipRegion(reg);
    }

    sizes::shape_x_begin = sizes::shape_x_end ;
    sizes::shape_y_begin = sizes::shape_y_end;

    wArea->setPixmap(pix);

}

void MainWindow::draw_first_point()
{
    if(sizes::activeOperation == 5){
        save_previous("Spray");
        return;
    }
    else{
        save_previous(tr("Pen"));
    }
    updateInfo();
    QPainter pai(&pix);
    QColor ncol = sizes::activeColor;

    QPen pen(configPen(ncol));

    pai.setPen(pen);
    pai.drawPoint(sizes::shape_x_begin, sizes::shape_y_begin);
    pai.end();
    wArea->setPixmap(pix);
}


void MainWindow::on_conn_Curve_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    wArea->setCursor(Qt::ArrowCursor);
    if(ui->conn_Curve->isChecked()){
        untoggle();
        ui->conn_Curve->setChecked(true);
        sizes::activeOperation = 10;
        sizes::shape_x_begin = -1;
        sizes::shape_y_begin = -1;
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


//-----------------------------------------------
void MainWindow::showPix()
{
    wArea->setPixmap(pix);
}

void MainWindow::save_previous(QString tx)
{
    historyCount = 1;
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
    save_previous(tr("Fill"));
    updateInfo();
    QColor ncol = sizes::activeColor;
    if(ui->semiTranspFillButton->isChecked()){
        ncol.setAlpha(64);
    }
    QImage img = pix.toImage();
    img =img.convertToFormat(QImage::Format_ARGB32);
    QList<int> coord;
    coord.push_back(sizes::selX);
    coord.push_back(sizes::selY);
    QColor oldColor= QColor::fromRgb(img.pixel(sizes::selX, sizes::selY));
    if(oldColor == sizes::activeColor)return;
    if(oldColor == Qt::white && sizes::activeColor == QColor(255,255,255,0)){ return; }
    img.setPixelColor(sizes::selX, sizes::selY, ncol);
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
                img.setPixelColor(posx, posy-1, ncol);
            }
        }
        if(posx-1 > -1 ){
            if(isSimil(QColor::fromRgb(img.pixel(posx-1, posy)), oldColor, similarity)){
                coord.push_back(posx-1);
                coord.push_back(posy);
                img.setPixelColor(posx-1, posy, ncol);
            }
        }

        if(posx+1 < img.width() ){
            if(isSimil(QColor::fromRgb(img.pixel(posx+1, posy)), oldColor, similarity)){
                coord.push_back(posx+1);
                coord.push_back(posy);
                img.setPixelColor(posx+1, posy, ncol);
            }
        }
        if(posy+1 < img.height() ){
            if(isSimil(QColor::fromRgb(img.pixel(posx, posy+1)) ,oldColor, similarity)){
                coord.push_back(posx);
                coord.push_back(posy+1);
                img.setPixelColor(posx, posy+1, ncol);
            }
        }
    }
    pix = QPixmap::fromImage(img);
    showPix();

}

void MainWindow::fillInsideBlack()
{
    save_previous(tr("Fill inside black"));
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
    //pai.setClipRegion(QRect(sizes::selX -20, sizes::selY-20, sizes::selX + 20, sizes::selY+20));

    wArea->setPixmap(pix);
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
        ui->widgetPick->setVisible(true);
        sizes::activeOperation = 6;
    }else{
        ui->widgetPick->setVisible(false);
        sizes::activeOperation = 0;
    }
}

void MainWindow::get_color()
{
    updateInfo();
    QImage img = pix.toImage();
    if(ui->colorBalanceCheck->isChecked()){
        QColor colorRif = QColor::fromRgba(img.pixel(sizes::selX, sizes::selY));
        double deltaR =  double(sizes::activeColor.red()) / double(colorRif.red());
        double deltaG = double(sizes::activeColor.green()) / double(colorRif.green());
        double deltaB = double(sizes::activeColor.blue()) / double(colorRif.blue());
        int newr, newg, newb;
        for(int ih=0; ih < img.height(); ++ih){
            for(int iw =0; iw < img.width(); ++iw){
                colorRif = img.pixelColor(iw,ih);
                newr = colorRif.red() * deltaR;
                newg = colorRif.green() * deltaG;
                newb = colorRif.blue() * deltaB;
                if(newr > 255)newr = 255;
                if(newg > 255)newg = 255;
                if(newb > 255)newb = 255;
                img.setPixelColor(iw, ih, QColor(newr, newg, newb));
            }
        }
        save_previous(tr("Color balance"));
        pix = QPixmap::fromImage(img);
        showPix();
    }else{
    sizes::activeColor = QColor::fromRgba(img.pixel(sizes::selX, sizes::selY));
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
    }
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
    QColor ncol = sizes::activeColor;

    QPainter pai(&selPix);
    QPen pen(configPen(ncol, 32));

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
        save_previous(tr("Line"));
        updateInfo();
        QColor ncol = sizes::activeColor;

        QPainter pai(&pix);
        QPen pen(configPen(ncol, 32));

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
        save_previous(tr("Shape"));
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
    shapeNames << "squ" << "rec" << "cir" << "ell" << "tri" << "rou" << "sta" << "aup" << "ari" << "ado" << "ale" << "aul" << "aur" << "abr" << "abl" << "crp" << "crx" << "sar";
    sizes::activeShape = shapeNames.at(index);
    if(ui->shapeButton->isChecked() == false){
        int q = QMessageBox::question(this,"Drawish", tr("Do you want to activate shapes?"), QMessageBox::Yes|QMessageBox::No);
        if(q == QMessageBox::Yes){
            untoggle();
             ui->lineOptionWidget->setVisible(true);
            ui->shapeButton->setChecked(true);
            sizes::activeOperation = 8;
        }
    }
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
    save_previous(tr("Connected lines"));
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
    save_previous(tr("Curve line"));
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
        QMessageBox::information(this, "Drawish", tr("No selection"));
        return;
    }   

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
      updateInfo();
}


void MainWindow::deg90(int a)
{
    if(!sizes::isSelectionOn){
        QMessageBox::information(this, "Drawish", tr("No selection"));
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
void MainWindow::on_actionSizes_2_triggered()
{
    DialogSize dSize;
    dSize.setModal(true);
    dSize.exec();
    if(dSize.returned == 1){
        save_previous(tr("Resize"));
        reSize();
        sizes::startResize = false;
    }

    else if(dSize.returned == 2){
        save_previous(tr("Resize"));
        areaSize();
        if(sizes::isCurveLineAreaOn){
            cl_area->setGeometry(0,0, sizes::areaWidth, sizes::areaHeight);
        }
        QPixmap pix2(sizes::areaWidth-8, sizes::areaHeight-8);
        pix2.fill(Qt::white);
        QPainter p(&pix2);
        p.drawPixmap(0, 0, pix.scaled(sizes::areaWidth-8, sizes::areaHeight-8));

        pix =pix2;
        wArea->setPixmap(pix);
        borderB->resetGeometry();
        borderR->resetGeometry();
        corner->resetGeometry();
    }

    else if(dSize.returned == 3){
        if(sizes::activeOperation==1){

            if(sizes::isSelectionOn){
                QMessageBox::information(this, "Drawish", tr("An area is already selected"));
                return;
            }else{
                save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
                createSelection();
            }
            return;
        }
        else {
            if(sizes::activeOperation == 2){
                QMessageBox::information(this, "Drawish", tr("An area is already selected"));
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
             save_previous(tr("Effects"));
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
             save_previous(tr("Colors"));
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
    save_previous(tr("Greyscale"));
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

void MainWindow::on_actionTo_sepia_triggered()
{
    save_previous(tr("Sepia"));
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
    }
    QImage img = Epix.toImage();

    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
           QColor rgb = img.pixelColor(x,y);

        int r = rgb.red();
        int g = rgb.green();
        int b = rgb.blue();
        int nr, ng, nb;
        nr = 0.393 * r + 0.769 * g + 0.189 * b;
        ng = 0.349 * r + 0.686 * g + 0.168 * b;
        nb = 0.272 * r + 0.534 * g + 0.131 * b;
        if(nr > 255) nr = 255;
        if(ng > 255) ng = 255;
        if(nb > 255) nb = 255 ;
        img.setPixelColor(x,y, QColor(nr,ng,nb));
        }
    }
    if(sizes::isSelectionOn){
        selectionRect->setPixmap(QPixmap::fromImage(img));
    }else{
        pix = QPixmap::fromImage(img);
        showPix();
    }
}


void MainWindow::on_actionReduce_to_RGB_triggered()
{
    save_previous(tr("Reduce to rgb"));
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
    save_previous(tr("Invert colors"));
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


void MainWindow::on_actionZoom_2_triggered()
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
    if(sizes::isSelectionOn){
       delete selectionRect; selectionRect = NULL;
       sizes::isSelectionOn= false;
       wArea->setCursor(Qt::ArrowCursor);
    }
    sizes::activeOperation =0;
    int ww = QInputDialog::getInt(this, "Drawish", tr("Enter width"), 60);
    int hh = QInputDialog::getInt(this, "Drawish", tr("Enter height"), 60);
    if(ww < 8 || hh < 8){
        QMessageBox::information(this, "Drawish", tr("Too small selection"));
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
        QString f =dialog.getSaveFileName(this, tr("Drawish save..."), QDir::homePath(),"Images (*.png *.jpg *.ico *bmp)");
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
        sizes::activeOperation=0;
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
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
        // selection
        sizes::activeOperation=1;
        wArea->setCursor(Qt::CrossCursor);
        untoggle();
        ui->selectionAreaButton->setChecked(true);

        //
        createSelection();

    }
    newImage("pix");
    showPix();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "Drawish", "Version:  " + version_info + "\nEmail:  nicNiz@libero.it");
}


void MainWindow::on_actionQuadruple_the_pixels_2_triggered()
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
  //untoggle();
  save_previous(tr("Quadruple"));
  sizes::areaWidth = (sizes::areaWidth * 2) -8;
  sizes::areaHeight =(sizes::areaHeight * 2) -8;
  QPixmap bigPix(sizes::areaWidth-8, sizes::areaHeight-8);
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


void MainWindow::on_actionDivide_by_5_triggered()
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
   // untoggle();
    save_previous(tr("Divide by 4"));
    sizes::areaWidth = (sizes::areaWidth -8) / 2 + 8;
    sizes::areaHeight = (sizes::areaHeight-8) / 2 + 8;
    QPixmap smallPix(sizes::areaWidth-8, sizes::areaHeight-8);
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
    if(lineD.res == 1 || lineD.res == 2){
        save_previous(tr("Line input"));
        QColor ncol = sizes::activeColor;
        QPainter pai(&pix);
        QPen pen(configPen(ncol, 32));

        pai.setPen(pen);
        if(lineD.res == 1) {
            pai.drawLine(lineD.linex1, lineD.liney1, lineD.linex2, lineD.liney2);
        }

        else if(lineD.res == 2){
            QLineF lf;
            lf.setP1(QPoint(lineD.linex1, lineD.liney1));
            lf.setAngle(sizes::shape_x_begin);
            lf.setLength(sizes::shape_y_begin);
            pai.drawLine(lf);
        }
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
        QMessageBox::warning(this, "Drawish", tr("No active cam!"));
        return;
    }
    QPixmap camPix = pix.copy(save_area->pos().x(), save_area->pos().y(), save_area->width(), save_area->height());
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);
    camPix.save(&buffer, "PNG");
    auto const encoded = buffer.data().toBase64();
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(encoded);
    QMessageBox::warning(this, "Drawish", tr("base64 code has been copied to clipboard!"));
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
    printer.setResolution(150);
    printer.setPageSize(QPageSize::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(cmpName);
    printer.setFullPage(true);
    QPainter painter;
       if (! painter.begin(&printer)) {
           QMessageBox::warning(this, "Drawish", tr("Failed to open image!"));
           return ;
       }
    painter.drawPixmap(0,0, sizes::areaWidth-8, sizes::areaHeight-8, pix);
    painter.end();
    QMessageBox::information(this, "Drawish", tr("Saved in\n") + cmpName);
}


void MainWindow::on_actionStretch_area_triggered()
{
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
    }else{
        QMessageBox::information(this, "Drawish", tr("No selection!"));
        return;
    }

    StretchDialog strd(this, Epix);
    strd.setModal(true);
    strd.exec();
    if(strd.res == QDialog::Accepted){
        sizes::selH = strd.epix.height();
        sizes::selW = strd.epix.width();
        selectionRect->resetGeometry();
        selectionRect->setPixmap(strd.epix);
        }

}


void MainWindow::on_actionSet_triggered()
{
    if(sizes::isSelectionOn){
        sizes::selX = 0;
        sizes::selY = 0;
        sizes::areaHeight = sizes::selH + 8;
        sizes::areaWidth = sizes::selW + 8;
        selectionRect->resetGeometry();
        reSize();

    }else{
         QMessageBox::information(this, "Drawish", tr("No selection!"));
    }
}


void MainWindow::on_actionIncrement_10_triggered()
{
    QString num1 = nextColumnToAdd.mid(0,1); // QString nextColumnToAdd="0593716482";
    nextColumnToAdd = nextColumnToAdd.mid(1, 9) + num1;
    int num_1 = num1.toInt();

    if(sizes::isSelectionOn){
        QImage img = selectionRect->pixmap().scaled(sizes::selW, sizes::selH).toImage();
        sizes::selH *=1.1;
        sizes::selW *=1.1;
        QPixmap bigPix(sizes::selW, sizes::selH);
        QImage bigImage = bigPix.toImage();
        int nx =0;
        int ny =0;
        int num_2 = num_1;
        for (int y = 0; y < img.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
            nx =0;
            num_1 = num1.toInt();
            for (int x = 0; x < img.width(); ++x) {
                QRgb &rgb = line[x];
                bigImage.setPixelColor(nx, ny, QColor(rgb));
                if(x == num_1){
                    bigImage.setPixelColor(nx+1, ny, QColor(rgb));
                    nx++;
                    num_1 += 10;
                }
                nx++;
            }
            if(y == num_2){

                for (int x2 = 0; x2 < bigImage.width(); ++x2) {
                    QColor k = bigImage.pixelColor(x2, ny);
                    bigImage.setPixelColor(x2, ny+1, k);
                }
                ny++;
                num_2 += 10;
            }
            ny++;
        }
        bigPix = QPixmap::fromImage(img);
        selectionRect->resetGeometry();
        selectionRect->setPixmap(bigPix);

    }
    else{
        //untoggle();
        save_previous(tr("Increment 10%"));
        sizes::areaWidth = (sizes::areaWidth - 8) * 1.1 +8;
        sizes::areaHeight =(sizes::areaHeight - 8) * 1.1 +8;
        QPixmap bigPix(sizes::areaWidth-8, sizes::areaHeight-8);
        QImage bigImage = bigPix.toImage();
        QImage img = pix.toImage();
        int nx =0;
        int ny =0;
        int num_2 = num_1;
        for (int y = 0; y < img.height(); ++y) {
            QRgb *line = reinterpret_cast<QRgb*>(img.scanLine(y));
            nx =0;
            num_1 = num1.toInt();
            for (int x = 0; x < img.width(); ++x) {
                QRgb &rgb = line[x];
                bigImage.setPixelColor(nx, ny, QColor(rgb));
                if(x == num_1){
                    bigImage.setPixelColor(nx+1, ny, QColor(rgb));
                    nx++;
                    num_1 += 10;
                }
                nx++;
            }
            if(y == num_2){

                for (int x2 = 0; x2 < bigImage.width(); ++x2) {
                    QColor k = bigImage.pixelColor(x2, ny);
                    bigImage.setPixelColor(x2, ny+1, k);
                }
                ny++;
                num_2 += 10;
            }
            ny++;
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


void MainWindow::on_actionAdd_link_triggered()
{
    QMessageBox msgBox;
    QPushButton *addButt = msgBox.addButton(tr("Add link"), QMessageBox::ActionRole);
    QPushButton *removeButt = msgBox.addButton(tr("Remove link"), QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == addButt) {
        QString nUrl = QInputDialog::getText(this, "Drawish", tr("Enter url"));
        configLinks = nUrl + "\n" + configLinks;
    } else if (msgBox.clickedButton() == removeButt) {
        QStringList webLinks = configLinks.split("\n");
        QString s = QInputDialog::getItem(this, "Drawish", tr("Select link to remove"), webLinks);
        configLinks.replace(s, "");
        configLinks.replace("\n\n", "\n");
    }

}


void MainWindow::on_sizeLine_textChanged(const QString &arg1)
{
    on_textEdit_textChanged();
}

double MainWindow::graphicFactor(QStringList sl, double graphDim)
{
    double d= sl.at(0).toDouble();
    double d1;
    for(int i=1; i < sl.count(); ++i){
        d1 = sl.at(i).toDouble();
        if(d1 > d) d = d1;
    }
    return graphDim / d;
}


void MainWindow::addGraphic(QPixmap gr)
{
    save_previous(tr("Create chart"));
    int wImg = gr.width();
    int hImg = gr.height();
    sizes::areaHeight += gr.height();
    if(wImg > sizes::areaWidth-8){ sizes::areaWidth = wImg+8; }
    reSize();
    QPainter p(&pix);
    int v= (sizes::areaHeight-8-hImg) + 3 ;
    p.drawPixmap(4, v, gr);
    showPix();
    updateInfo();
}


void MainWindow::on_actionHorizontal_bars_triggered()
{
    // chart
    QStringList vals = askForValues();   
    // find max value &
    // equalize d -> graphicWidth
    double factor = graphicFactor(vals, 360.0);
    // find height for each bar
    const int barDim = 22;
    int graphHeight = (barDim * vals.count()) +4;

    // create empty pixmap
    QPixmap gPix(372, graphHeight);
    gPix.fill(Qt::white);
    // draw rect for each value
    double Len=0.0;

    for(int i=0; i < vals.count(); ++i){
        QPainter pai(&gPix);
        QPen pen(Qt::black, 2);
        pai.setPen(pen);
        QBrush br(QColor(120,150,230));
        pai.setBrush(br);
        Len = vals.at(i).toDouble();
        Len = Len * factor;
        pai.drawRoundedRect(4, barDim*i, Len, 16, 4, 4);
        pai.end();
        QPainter pai2(&gPix);
        QPen pen2(Qt::white, 2);
        pai2.setPen(pen2);
        QFont fo("Helvetica", 8);
        pai2.setFont(fo);
        pai2.drawText(5, (barDim*i)+12, vals.at(i));
        pai2.end();
    }
    //
    addGraphic(gPix);
}


void MainWindow::on_actionVertical_bars_triggered()
{
    QStringList vals = askForValues();
    double factor = graphicFactor(vals, 320.0);
    const int barDim = 22;
    int graphWidth = (barDim * vals.count()) +8;
    QPixmap gPix(graphWidth, 332);
    gPix.fill(Qt::white);
    double Len=0;

    for(int i=0; i < vals.count(); ++i){
        QPainter pai(&gPix);
        QPen pen(Qt::black, 2);
        pai.setPen(pen);
        QBrush br(QColor(120,150,230));
        pai.setBrush(br);
        Len = vals.at(i).toDouble();
        Len = Len * factor;
        pai.drawRoundedRect( barDim*i, 328-Len, 16, Len, 4, 4);
        pai.end();
        QPainter pai2(&gPix);
        QPen pen2(Qt::white, 2);
        pai2.setPen(pen2);
        QFont fo("Helvetica", 8);
        pai2.setFont(fo);
        pai2.drawText((barDim*i)+2, 320, vals.at(i));
        pai2.end();
    }
    //
    addGraphic(gPix);
}


void MainWindow::on_actionPie_triggered()
{
    QStringList vals = askForValues();
    // sum all
    double sum =0.0;
    for(int i=0; i< vals.count(); ++i){
        sum = vals.at(i).toDouble() + sum;
    }
    double factor = 360.0 / sum;
    QPixmap gPix(280, 210);
    gPix.fill(Qt::white);
    double Len =0;
    QRectF rectangle(4.0, 0.0, 200.0, 200.0);
    int startAngle = 0;
    int spanAngle = 0;
    QList <int> blues;
    blues << 240 << 190 << 120 << 50;
    for(int i=0; i < vals.count(); ++i){
        QPainter pai(&gPix);
        QPen pen(Qt::black, 2);
        pai.setPen(pen);
        QBrush br(QColor(220,150, blues.at(i % 4)));
        if(i == 0){ br = QBrush(Qt::yellow);}
        else if(i == 1){ br = QBrush(Qt::red);}
        pai.setBrush(br);
        Len = vals.at(i).toDouble();
        Len *= factor;
        spanAngle = Len *16;
        pai.drawPie(rectangle, startAngle, spanAngle);
        pai.end();
        startAngle += spanAngle;
        QPainter pai2(&gPix);
        QPen pen2(br, 10);
        pai2.setPen(pen2);
        QFont fo("Helvetica", 10);
        pai2.setFont(fo);
        pai2.drawText(220, (i*14)+14, vals.at(i));
        pai2.end();
    }
    addGraphic(gPix);

}


void MainWindow::on_actionSet_as_restore_point_triggered()
{
    if(toRestore.isNull()){
        QMessageBox::information(this, "Drawish", tr("Restore point created"));
    }
    else{
        QMessageBox::information(this, "Drawish", tr("Restore point overwritten"));
    }
    toRestore = pix;
}


void MainWindow::on_actionRestore_triggered()
{
    if(toRestore.isNull()) return;
    save_previous("Restore");
    pix = toRestore;
    newImage("pix");

}


void MainWindow::on_actionCreate_shape_triggered()
{
    int a1 =  ui->mouseX_label->text().toInt();
    int a2 =  ui->mouseY_label->text().toInt();
    figures figure(a1, a2);
    figure.setModal(true);
    figure.exec();
    if(figure.res == 0) return;

    save_previous("Input shape");
    QColor ncol = sizes::activeColor;

    QPainter pai(&pix);
    QPen pen(configPen(ncol, 32));

    pai.setPen(pen);
    sizes::shape_x_begin = sizes::shape_x_begin -(sizes::lineXEnd/2);
    sizes::shape_y_begin = sizes::shape_y_begin -(sizes::lineYEnd/2);
    if(figure.res == 1){
        pai.drawRect(sizes::shape_x_begin, sizes::shape_y_begin, sizes::lineXEnd, sizes::lineYEnd);
    }
    else if(figure.res ==2){
        pai.drawEllipse(sizes::shape_x_begin, sizes::shape_y_begin, sizes::lineXEnd, sizes::lineYEnd);
    }
    pai.end();
    if(figure.center == true){
        int xc = sizes::lineXEnd / 2;
        int yc = sizes::lineYEnd / 2;

        QPainter pai2(&pix);
        QPen pen2(ncol, 2);
        pai2.setPen(pen2);
        pai2.drawPoint(QPoint(sizes::shape_x_begin + xc+1, sizes::shape_y_begin + yc+1));
    }
    wArea->setPixmap(pix);

}
