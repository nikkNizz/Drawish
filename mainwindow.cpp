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
#include "savecam.h"
#include "linedialog.h"
#include "stretchdialog.h"
#include "fileio.h"
#include "figures.h"
#include "richeditor.h"
#include "pickpalette.h"
#include "serialize.h"
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
#include <QTimer>
#include <QScrollBar>
#include <QSysInfo>
#include <qpainterpath.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    version_info = "2.3";
    // 2.3: BUG autoratation button; add invert color if not active color (dColors);
    // value for transparency (pen); add zoom shortcut; translator in if
    // add sprayDraw at first point; pen width for spray; change cursor
    // default path; better square pen; add average color; bug in zoom cursor

    setGeometry(50,80,790,487);


    isLinux = false;
#ifdef Q_OS_LINUX
    isLinux = true;
#endif

    ui->textOptionsWidget->setVisible(false);
    ui->lineOptionWidget->setGeometry(80,0,552,31);
    ui->lineOptionWidget->setVisible(false);
    ui->similaritywidget->setGeometry(80,0,552,31);
    ui->similaritywidget->setVisible(false);
    ui->widgetPick->setGeometry(80,0,552,31);
    ui->widgetPick->setVisible(false);

// set label as drawing area
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
             ui->labelActiveFile->setText( activePathFile);
             addToRecent(activePathFile);
        }
            if(npix.isNull()){
                QMessageBox::information(this, "Drawish", tr("Unsupported file"));
                removeFromRecent(activePathFile);
                 // set default image to area
                pix =QPixmap(sizes::areaWidth-8, sizes::areaHeight-8);
                pix.fill(Qt::white);
            }else{

            pix = npix;
            sizes::areaHeight=pix.height()+8;
            sizes::areaWidth=pix.width()+8;
            PlaceEdges();
            }
        }else{
             // set default image to area
        pix =QPixmap(sizes::areaWidth-8, sizes::areaHeight-8);
        pix.fill(Qt::white);
        }
        showPix();
        wArea->setAlignment(Qt::AlignTop);

    historyList.push_back(tr("Original image"));
    historyPix.push_back(pix);
    historyPix.push_back(QPixmap());

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
    ui->prefColorButton->setStyleSheet("background-color: white");
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
    connect(wArea, SIGNAL(setPaste()), this, SLOT(on_actionPaste_from_clipboard_triggered()));

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
                     ui->labelActiveFile->setText( activePathFile);
                     addToRecent(activePathFile);
                 }
                save_previous(tr("Open"));

                if(!npix.isNull()) pix = npix;

                sizes::areaHeight = (pix.height() * zoomLevel) + 8;
                sizes::areaWidth  = (pix.width() * zoomLevel) + 8;

                areaSize();
                showPix();

                PlaceEdges();
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
    ui->labelActiveFile->setText("File:*");
    return zpix;
}

void MainWindow::readConfig()
{

    QString user = QDir::homePath();
    QDir c(user + "/Drawish_Data");
    if(!c.exists()){
        c.mkdir(user + "/Drawish_Data");
    }
    configPath = user + "/Drawish_Data/DrawishConfig.txt";

    fileIO fio;
    if(!QFile::exists(configPath)){
        fio.createFile("<recent></recent><links>www.remove.bg\nperchance.org/ai-text-to-image-generator\ncloudconvert.com/image-converter\nphotocartoon.net\nwww.iconfinder.com</links>", configPath);
    }

    QString config = fio.readFile(configPath);

    configRecent = midstring(config, "recent");
    if(configRecent != ""){
    QStringList recents = configRecent.split("\n");
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

    configLinks =midstring(config, "links");
    if(configLinks != ""){
    QStringList links = configLinks.split("\n");
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
    // set values if present

    QString k = midstring(config, "penWidth");
    if(k == "")  k = "6";
    ui->lineWidthBox->setValue(k.toInt());
    k = midstring(config, "textSize");
    if(k.toInt() == 0) k = "16";
    ui->sizeLine->setText(k);
    k = midstring(config, "transSel");
    if(k == "false"){ ui->actionTransparent_selection->setChecked(false);}
    k = midstring(config, "textFont");
    if(k != ""){
    for(int i=0; i < ui->fontComboBox->count(); ++i){
        if(ui->fontComboBox->itemText(i) == k){
            ui->fontComboBox->setCurrentFont(k);
            break;
        }
    }
    }
    k = midstring(config, "textStyle");
    if(k.contains("b", Qt::CaseInsensitive)){ ui->boldButton->setChecked(true);}
    if(k.contains("i", Qt::CaseInsensitive)){ ui->italicButton->setChecked(true);}
    if(k.contains("u", Qt::CaseInsensitive)){ ui->underlineButton->setChecked(true);}
    k = midstring(config, "degrees");
    if(k.toInt() > 0 && k.toInt() < 360){ ui->RotatioAngleSpin->setValue(k.toInt());}
    int myW = this->width();
    int myH = this->height();
    int myX = this->pos().x();
    int myY = this->pos().y();
    k =midstring(config, "windowWidth");
    if(k.toInt() > 50){ myW = k.toInt();}
    k =midstring(config, "windowHeight");
    if(k.toInt() > 50){ myH = k.toInt();}
    defaultPathDialog = midstring(config, "defaultPathDialog");//***
    setGeometry(myX, myY, myW, myH);

}

QString MainWindow::midstring(QString from, QString tag)
{
    int init = from.indexOf("<" + tag + ">", 0, Qt::CaseInsensitive) + tag.length()+2;
    int endit = from.indexOf("</" + tag + ">", 0, Qt::CaseInsensitive);
    if(init == -1 || endit == -1) return "";
    return from.mid(init, endit-init);
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
        // remove or move to top
        QStringList recents= configRecent.split("\n");
        int a = recents.indexOf(pf);
        if(a > -1) recents.removeAt(a);
        if(recents.count() > 10) recents.mid(0,9);
        configRecent =pf + "\n" + recents.join("\n");

}

void MainWindow::removeFromRecent(QString pf)
{
    QStringList recents= configRecent.split("\n");
    int a = recents.indexOf(pf);
    if(a > -1) recents.removeAt(a);
    configRecent =recents.join("\n");
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
        if(selectionRect->frameStyle() != QFrame::NoFrame){
            removeSelectionBorder();
        }
        if(ev->key() ==Qt::Key_W || ev->key()== Qt::Key_Q){            
            sizes::selY--;}
        else if(ev->key()== Qt::Key_A){
            sizes::selX--;}
        else if(ev->key()== Qt::Key_S){
            sizes::selX++;}
        else if(ev->key()== Qt::Key_Z || ev->key()== Qt::Key_X){
            sizes::selY++;}
        else if(ev->key()== Qt::Key_Delete){
            if(passPath == true){
                // del only path
                QImage freeSelImg = freeSelPix.toImage();
                QColor k;
                for(int x=0; x < freeSelImg.width(); ++x){
                    for(int y=0; y < freeSelImg.height(); ++y){
                        k = freeSelImg.pixelColor(x, y);
                        if(k == Qt::blue || k == Qt::yellow || k == Qt::black){
                            freeSelImg.setPixelColor(x,y, Qt::white);
                        }
                    }
                }
                QPainter pai(&selectionPix);
                freeSelPix = QPixmap::fromImage( freeSelImg);
                pai.drawPixmap(0, 0, freeSelPix);

            }else{
                 selectionPix.fill(Qt::white);
            }
            selectionRect->setPixmap(selectionPix.scaled(sizes::selW, sizes::selH));
        }
        else if(ev->key() == Qt::Key_Plus && sizes::activeOperation == 1){
            on_actionCopy_selection_no_clipboard_triggered();
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
        if(ev->key() ==Qt::Key_W ){ yFin = sizes::selY - num;
        }
        else if(ev->key()== Qt::Key_A){xFin = (sizes::selX) - num ;}
        else if(ev->key()== Qt::Key_S){xFin = (sizes::selX) + num ;}
        else if(ev->key()== Qt::Key_Z){yFin = (sizes::selY) + num;}
        else if(ev->key()== Qt::Key_Q){xFin = (sizes::selX) - num;
                                       yFin = (sizes::selY) - num;}
        else if(ev->key()== Qt::Key_E){xFin = (sizes::selX) + num;
                                       yFin = (sizes::selY) - num;}
        else if(ev->key()== Qt::Key_X){xFin = (sizes::selX) + num;
                                       yFin = (sizes::selY) + num;}
        else if(ev->key()== Qt::Key_Less){xFin = (sizes::selX) - num;
                                         yFin = (sizes::selY) + num;}

        pai.drawLine(sizes::selX , sizes::selY, xFin, yFin);
        pai.end();
        sizes::selX= xFin;
        sizes::selY = yFin;
        showPix();
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
   updateInfo();
}


void MainWindow::closeEvent(QCloseEvent *ev)
{
    if(activePathFile != "")  addToRecent(activePathFile);
    QStringList recents = configRecent.split("\n");
    if(recents.count() > 11){
        recents = recents.mid(0,10);
        configRecent = recents.join("\n");
    }
    // save some values: penWidth, textSize, textFont, textStyle, degrees, width height
    QString val="\n<penWidth>" + QString::number(sizes::line_width) + "</penWidth>\n";
    if(ui->actionTransparent_selection->isChecked() == false){
        val += "<transSel>false</transSel>\n";
    }else{
        val += "<transSel>true</transSel>\n";
    }
    val += "<textSize>" + ui->sizeLine->text() + "</textSize>\n";
    val += "<textFont>" + ui->fontComboBox->currentText() + "</textFont>\n";
    QString textstyle ="";
    if(ui->boldButton->isChecked()) textstyle += "b";
    if(ui->italicButton->isChecked()) textstyle += "i";
    if(ui->underlineButton->isChecked()) textstyle += "u";
    val += "<textStyle>" + textstyle + "</textStyle>\n";
    val +=  "<degrees>" + QString::number(ui->RotatioAngleSpin->value()) + "</degrees>\n"; //***
    val += "<windowWidth>" + QString::number(this->width()) + "</windowWidth>\n"; //***
    val += "<windowHeight>" + QString::number(this->height()) + "</windowHeight>\n"; //***
    val += "<defaultPathDialog>" + defaultPathDialog + "</defaultPathDialog>\n"; //***


    fileIO fio;
    fio.createFile("<recent>" + configRecent + "</recent><links>" + configLinks + "</links>" + val , configPath);

    if(sizes::modify){
        int q = QMessageBox::question(this, "Drawish", tr("Save image?"),QMessageBox::Yes| QMessageBox::No | QMessageBox::Cancel );
        if(q == QMessageBox::Yes){
            imgSave();
            fio.createFile("<recent>" + configRecent + "</recent><links>" + configLinks + "</links>" + val, configPath);
        }
        else if(q==QMessageBox::Cancel){ev->ignore();}
        else{ev->accept();}
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->scrollArea->setGeometry(80,30,this->width()-91, this->height()-120);
    ui->infoWidget->setGeometry(80,this->height()-88, 600, 60);
}

void MainWindow::reSize()
{   
   if(!sizes::startResize){save_previous(tr("Resize"));}
   sizes::startResize=true;
   areaSize();

   QPixmap pix2((sizes::areaWidth - 8) / zoomLevel, (sizes::areaHeight -8) / zoomLevel);
   pix2.fill(Qt::white);
   QPainter p(&pix2);
   p.drawPixmap(0, 0, pix);

   pix =pix2;
   showPix();

   PlaceEdges();
   if(sizes::isCurveLineAreaOn){
       cl_area->resetGeometry();
   }
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

void MainWindow::PlaceEdges()
{
    borderB->resetGeometry();
    borderR->resetGeometry();
    corner->resetGeometry();
}


void MainWindow::imgSave()
{
    QString f;
    if(activePathFile== ""){
        QStringList suffix;
        suffix << ".png" << ".jpeg" << ".ico" << ".bmp" << ".ppm" << ".xbm" << ".xpm";
        QString ext = QInputDialog::getItem(this, "Drawish", tr("Format"), suffix);
        QFileDialog dialog(this);
        if(QDir(defaultPathDialog).exists() == false){defaultPathDialog = QDir::homePath();}
        f =dialog.getSaveFileName(this, tr("Drawish save..."), defaultPathDialog);
        if(f ==""){return;}
        f = f + ext;
        activePathFile = f ;
        ui->labelActiveFile->setText( activePathFile);
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
    if(QDir(defaultPathDialog).exists() == false){defaultPathDialog = QDir::homePath();}
    return dialog.getOpenFileName(this,tr("Drawish...Select image"), defaultPathDialog );
}


void MainWindow::savePix(QPixmap pixToSave, QString f)
{
    if(f.endsWith(".jpeg", Qt::CaseInsensitive)){ pixToSave.save(f, "jpeg");}
    else if(f.endsWith(".jpg", Qt::CaseInsensitive)){ pixToSave.save(f, "jpg");}
    else if(f.endsWith(".ico", Qt::CaseInsensitive)){ pixToSave.save(f, "ico");}
    else if(f.endsWith(".bmp", Qt::CaseInsensitive)){ pixToSave.save(f, "BMP");}
    else if(f.endsWith(".png", Qt::CaseInsensitive)){ pixToSave.save(f, "png");}
    else if(f.endsWith(".ppm", Qt::CaseInsensitive)){ pixToSave.save(f, "ppm");}
    else if(f.endsWith(".xbm", Qt::CaseInsensitive)){ pixToSave.save(f, "xbm");}
    else if(f.endsWith(".xpm", Qt::CaseInsensitive)){ pixToSave.save(f, "xpm");}

    else{pixToSave.save(f + ".png", "PNG");}
}

void MainWindow::newImage(QString from, QString path)
{

   if(from == "zero"){
       sizes::modify= false;
       sizes::areaHeight= (320 * zoomLevel) + 8;
       sizes::areaWidth= (480 * zoomLevel) + 8;
       QPixmap npix(480, 320);
       npix.fill(Qt::white);
       pix = npix;
       activePathFile="";
       ui->labelActiveFile->setText("File:*");
   }

   else if(from == "f"){ // open file

      QFileDialog dialog(this);
      QString f = path;
      if(f == ""){
          if(QDir(defaultPathDialog).exists() == false){defaultPathDialog = QDir::homePath();}
       f =dialog.getOpenFileName(this, tr("Drawish...Select image"), defaultPathDialog );
       if(f ==""){return;}
      }

      QPixmap npix;
      if(f.endsWith(".pdf", Qt::CaseInsensitive) && isLinux){
          npix = openPdf(f);
      }else{
          npix = QPixmap(f);
          if(npix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file"));
              removeFromRecent(f);
              return;
          }
          activePathFile = f;
          ui->labelActiveFile->setText( activePathFile);
          addToRecent(activePathFile);
      }
      sizes::modify= false;

      if(npix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file"));
          removeFromRecent(f);
          return;
      }
      pix = npix;
      sizes::areaHeight= (pix.height() * zoomLevel) + 8;
      sizes::areaWidth = (pix.width() * zoomLevel) + 8;
   }
   else if(from == "pix"){
       sizes::areaHeight= (pix.height() * zoomLevel) + 8;
       sizes::areaWidth = (pix.width() * zoomLevel) + 8;
   }
       areaSize();
       showPix();
       PlaceEdges();
       updateInfo();
}

void MainWindow::on_actionDefault_path_for_file_picker_triggered()
{
    QString p= QInputDialog::getText(this, "Drawish", tr("Enter name of an existing folder"));
    if(p != "") defaultPathDialog = p;
}


QCursor MainWindow::rectCursor()
{
        int wh = (ui->lineWidthBox->value() * zoomLevel) + 2;
        QPixmap kursor(wh,wh);
        kursor.fill(QColor(0,0,0,64));//***
        // ------
        QPixmap fillCurs(wh-2, wh-2);
        fillCurs.fill(QColor(255,255,255,128));
        // -----
        QPainter pai(&kursor);
        pai.drawPixmap(1,1,fillCurs);
        pai.end();
        if(ui->comboPen->currentIndex()==4 && wh > 5){
            QPainter pai2(&kursor);
            pai2.setPen(Qt::gray);
            pai2.drawLine(1, 1, kursor.width()-1, kursor.height()-1);
        }
        else if(ui->comboPen->currentIndex()==3 && wh > 5){
            QPainter pai2(&kursor);
            pai2.setPen(Qt::gray);
            pai2.drawLine(kursor.width()-1, 1, 1, kursor.height()-1);
        }
        return QCursor(kursor);

}

void MainWindow::updateInfo()
{

  if(sizes::activeOperation == 2 && sizes::isSelectionOn){
        ui->textEdit->setFocus();
  }else if(sizes::isSelectionOn){
      selectionPix = selectionPix.scaled(sizes::selW / zoomLevel, sizes::selH / zoomLevel);
  }

  ui->mouseX_label->setNum(sizes::selX);
  ui->mouseY_label->setNum(sizes::selY);
  if(sizes::isSelectionOn || sizes::isShapeOn){
    ui->selX_label->setNum(int(sizes::selW));
    ui->selY_label->setNum(int(sizes::selH));
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
    ui->freeSelectionAreaButton->setChecked(false);
    ui->penTracer->setChecked(false);

    // hide widgets
    ui->textOptionsWidget->setVisible(false);
    ui->lineOptionWidget->setVisible(false);
    ui->similaritywidget->setVisible(false);
    ui->widgetPick->setVisible(false);
    // default mouse
    if(sizes::activeOperation == 1){
        wArea->setCursor(Qt::CrossCursor);
    }else{
        wArea->setCursor(Qt::ArrowCursor);
    }
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
    if(traceArea != NULL){
        delete traceArea;
        traceArea = NULL;
    }
}

void MainWindow::removeSelectionBorder()
{
    if(!sizes::isSelectionOn) return;
    if(selectionRect->frameStyle() == QFrame::NoFrame){
        selectionRect->setFrameStyle(QFrame::Box | QFrame::Raised);
    }else{
        selectionRect->setFrameStyle(QFrame::NoFrame);
        QTimer::singleShot(1000, this, SLOT(removeSelectionBorder()));
    }
}

void MainWindow::createSelectionFromRubb()
{
    save_previous("Image");
    //save_previous( selectionCoords());
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
    if(sizes::isSelectionOn){
    delete selectionRect; selectionRect = NULL;
    sizes::isSelectionOn= false;
    }
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
    QStringList suffix;
    suffix << ".png" << ".jpeg" << ".ico" << ".bmp" << ".ppm" << ".xbm" << ".xpm";
    QString ext = QInputDialog::getItem(this, "Drawish", tr("Format"), suffix);
    if(QDir(defaultPathDialog).exists() == false){defaultPathDialog = QDir::homePath();}
    QFileDialog dialog(this);
    QString f =dialog.getSaveFileName(this, tr("Drawish save image"), defaultPathDialog);
    if(f ==""){return;}
    f = f + ext;
    addToRecent(f);
    savePix(pix, f);
}


void MainWindow::on_actionClose_triggered()
{
    this->close();
}


void MainWindow::on_undoButton_clicked()
{
    if(!prePix.isNull()){
        if(sizes::isSelectionOn){
            sizes::isSelectionOn=false;
            delete selectionRect;
            selectionRect = NULL;
        }
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
        untoggle();
        wArea->setCursor(Qt::CrossCursor);        
        ui->selectionAreaButton->setChecked(true);
    }
}

void MainWindow::createSelection(bool fromUndo)
{
    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    if(sizes::activeOperation != 2){  // !selection area for text
        if( !fromUndo){
          selectionPix = pix.copy(sizes::selX / zoomLevel , sizes::selY / zoomLevel, sizes::selW / zoomLevel , sizes::selH / zoomLevel );
        }
        //else is passed  from :on_historyCombo_activated()
        if(ui->actionTransparent_selection->isChecked()){
            selectionPix=addTransparency(selectionPix, 0,255,255,255);
        }
      selectionRect->setPixmap(selectionPix);
      QPixmap blank(sizes::selW/zoomLevel ,sizes::selH/zoomLevel);
      blank.fill(Qt::white);
      QPainter p(&pix);
      p.drawPixmap(sizes::selX/zoomLevel ,sizes::selY/zoomLevel, blank);
      showPix();
      if( !fromUndo){      save_previous(selectionCoords()); }
    }else{
        ui->textEdit->setFocus();
    }
    sizes::isSelectionOn=true;
    restSelX = 0.0;
    restSelY = 0.0;
    selectionRect->show();
    raiseBorders();
    updateInfo();
    connect(selectionRect, SIGNAL(setInfo()), this, SLOT(updateInfo()));
    connect(selectionRect, SIGNAL(setCopy()), this, SLOT(on_actionCopy_triggered()));
}


void MainWindow::drawCopy()
{
    save_previous(selectionCoords());
    if(sizes::activeOperation != 2){
        QPainter p(&pix);
        double px = double(sizes::selX) + restSelX; // avoids one-pixel deviations
        px = px / zoomLevel;
        double py = double(sizes::selY) + restSelY;
        py = py / zoomLevel;
        p.drawPixmap(px, py, selectionPix.scaled((sizes::selW)/zoomLevel, (sizes::selH)/zoomLevel));
        showPix();
    }
    passPath = false;
    delete selectionRect;
    selectionRect = NULL;
    updateInfo();
}


void MainWindow::on_actionSelect_all_triggered()
{
    sizes::selX =0;
    sizes::selY =0;
    sizes::selH = double(sizes::areaHeight) - 8.00;
    sizes::selW = double(sizes::areaWidth) - 8.00;
    save_previous("Image");
    save_previous( selectionCoords());
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
        p_Clipboard->setPixmap(selectionPix.scaled(sizes::selW / zoomLevel, sizes::selH / zoomLevel));
    }
}

void MainWindow::on_actionCopy_selection_no_clipboard_triggered()
{
    if(sizes::isSelectionOn){
        prePix = pix;
        sizes::startResize=false;        
        drawCopy();
        selectionRect = new selectionArea(wArea);
        selectionRect->resetGeometry();
        if(ui->actionTransparent_selection->isChecked()){
            selectionPix=addTransparency(selectionPix, 0,255,255,255);
        }
        selectionRect->setPixmap(selectionPix);
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
        if(clipboard->text() == "") return;
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
    if(wi > pix.width()){ sizes::areaWidth = wi + 8;}
    if(he > pix.height()){sizes::areaHeight = he + 8;}
    reSize();
    sizes::selX =ui->scrollArea->horizontalScrollBar()->value();
    sizes::selY =ui->scrollArea->verticalScrollBar()->value();
    sizes::selH = double(he) * zoomLevel;
    sizes::selW = double(wi) * zoomLevel;

    untoggle();
    if(ui->actionTransparent_selection->isChecked()){
       passedPix = addTransparency(passedPix, 0, 255,255,255);
    }
    ui->selectionAreaButton->setChecked(true);
    sizes::activeOperation =1;
    wArea->setCursor(Qt::CrossCursor);
    selectionPix = passedPix;
    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    selectionRect->setPixmap(passedPix.scaled(sizes::selW, sizes::selH));
    sizes::isSelectionOn=true;
    sizes::startResize=false;
    selectionRect->show();
    if((sizes::selW * sizes::selH) > (sizes::areaWidth * sizes::areaHeight * 0.6) ){
        QMessageBox::information(this, "Drawish", tr("The image is in a selection"));
    }
    raiseBorders();
}

void MainWindow::on_actionPaste_over_file_triggered()
{
    deleteSel();
    if(sizes::isSelectionOn){
        sizes::isSelectionOn = false;
        drawCopy();
    }
    QFileDialog dialog;
    if(QDir(defaultPathDialog).exists() == false){defaultPathDialog = QDir::homePath();}
    QString f = dialog.getOpenFileName(this, tr("Drawish...Select image"), defaultPathDialog );
    if(f ==""){return;}
    QPixmap npix(f);
    if(npix.isNull()){
        QMessageBox::information(this, "Drawish", tr("Usupported format"));
        removeFromRecent(f);
        return;
    }
    int pasteX = QInputDialog::getInt(this, "Drawish", tr("Enter the x coordinate"), remSelX);
    int pasteY = QInputDialog::getInt(this, "Drawish", tr("Enter the y coordinate"), remSelY);

        activePathFile = f;
        ui->labelActiveFile->setText( activePathFile);
        addToRecent(activePathFile);
        save_previous(tr("Paste over file"));
    sizes::modify= true;
    // add pix to npix
    int maxW = pix.width();
    int maxH = pix.height();
    if((maxW + pasteX) > npix.width()){maxW = npix.width() - pasteX;}
    if((maxH + pasteY) > npix.height()){maxH = npix.height() - pasteY;}
    pix = pix.copy(0,0,maxW, maxH);
    QPainter pai(&npix);
    pai.drawPixmap(pasteX, pasteY, pix);

    pix = npix;

    sizes::areaHeight= (pix.height() * zoomLevel) + 8;
    sizes::areaWidth = (pix.width() * zoomLevel) + 8;
    areaSize();

    showPix();
    PlaceEdges();
    updateInfo();

}


void MainWindow::on_actionMerge_selection_to_image_triggered()
{
    if(sizes::isSelectionOn == false){
        QMessageBox::information(this, "Drawish", tr("No area selected"));
        return;
    }
    QPixmap selectedPix(selectionPix.scaled(sizes::selW / zoomLevel, sizes::selH / zoomLevel));
    QPixmap underPix = pix.copy(sizes::selX / zoomLevel, sizes::selY / zoomLevel, sizes::selW / zoomLevel, sizes::selH / zoomLevel);
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
    int sw = sizes::selW / zoomLevel;
    int sh = sizes::selH / zoomLevel;
    for(int xx= 0;  xx < sw; ++xx){
        for(int yy=0; yy < sh; ++yy){
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
    selectionPix = QPixmap::fromImage(selectedImage);
    selectionRect->setPixmap(selectionPix);
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

void MainWindow::on_actionTransparent_selection_toggled(bool arg1)
{
    if(sizes::isSelectionOn == false) return;
    if(arg1 == true){
        selectionPix = addTransparency(selectionPix, 0, 255,255,255);
    }else if(arg1 == false){
        QImage img = selectionPix.toImage();
        for(int h=0; h < img.width(); ++h){
            for(int v =0; v < img.height(); ++v){
                QColor c = img.pixelColor(h,v);
                if(c.red()==0 && c.green() ==0 && c.blue()==0 && c.alpha()==0){
                    img.setPixelColor(h,v, Qt::white);
                }
            }
        }
        selectionPix = QPixmap::fromImage(img);
    }
    selectionRect->setPixmap(selectionPix);
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
    sizes::areaWidth += (wImg * zoomLevel) ;
    if(hImg > (sizes::areaHeight / zoomLevel) - 8){ sizes::areaHeight = (hImg * zoomLevel) + 8; }
    reSize();
    QPainter p(&pix);
    int v= ((sizes::areaWidth / zoomLevel) - 8 - wImg) + 1 ;
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
    sizes::areaHeight += (hImg * zoomLevel);
    if(wImg > (sizes::areaWidth / zoomLevel) - 8){ sizes::areaWidth = (wImg * zoomLevel) + 8; }
    reSize();
    QPainter p(&pix);
    int v= ((sizes::areaHeight / zoomLevel) - 8 - hImg) + 1 ;
    p.drawPixmap(0, v, fpix);
    showPix();
}



void MainWindow::on_actionCopy_selection_to_file_triggered()
{
    if(sizes::isSelectionOn){
        QPixmap selPix = selectionPix.scaled(sizes::selW / zoomLevel, sizes::selH / zoomLevel);
        QFileDialog dialog(this);
        if(QDir(defaultPathDialog).exists() == false){defaultPathDialog = QDir::homePath();}
        QString f =dialog.getSaveFileName(this, tr("Drawish save image"), defaultPathDialog);
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
    if(ui->drawTextButton->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
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
    if(sizes::activeOperation !=2) return; // prevent crash at start
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
    int sizeText = ui->sizeLine->text().toInt() * zoomLevel;
    if(sizeText < 4){sizeText = 4;}
    tFont.setPixelSize(sizeText);;

    selectionRect->setFont(tFont);
    selectionRect->setStyleSheet("color:" + sizes::activeColor.name());
    selectionRect->setText(ui->textEdit->toPlainText());

}


void MainWindow::on_confirmTextButton_clicked()
{
    if(!sizes::isSelectionOn){
        QMessageBox::information(this, "Drawish", tr("Click a point on the canvas, before"));
        return;
    }
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
        if(sizeText < 4){sizeText = 4; ui->sizeLine->setText("4");}
        tFont.setPixelSize(sizeText);
        p.setFont(tFont);
        p.setPen(sizes::activeColor);
        int xpos = selectionRect->x() / zoomLevel;
        int ypos = selectionRect->y() / zoomLevel;
        p.drawText(QRect(xpos+2, ypos+2, sizes::selW, sizes::selH), ui->textEdit->toPlainText());
        showPix();
        selectionRect->resetGeometry();
    }

}

void MainWindow::on_boldButton_clicked()
{
    on_textEdit_textChanged();
}


void MainWindow::on_italicButton_clicked()
{
    on_textEdit_textChanged();
}


void MainWindow::on_underlineButton_clicked()
{
    on_textEdit_textChanged();
}


void MainWindow::on_fontComboBox_currentFontChanged(const QFont &f)
{
    on_textEdit_textChanged();
}


//  SET COLORS....

void MainWindow::set_activeColor(int R, int G, int B, int A)
{
    if(R > -1) sizes::activeColor= QColor(R,G,B, A);
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
    if(sizes::activeOperation == 2 && sizes::isSelectionOn){
        on_textEdit_textChanged();
    }
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
   QColor kk = QColorDialog::getColor(Qt::gray, this, tr("Drawish choose color"));
   set_activeColor(kk.red(), kk.green(), kk.blue());
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
    // combopen: 0 round, 1 square, 2 flat, 3 slash, 4 backslash, 5 fusion, 6 rand round, 7 random square, 8 random red
    // 9 random green, 10 random blue, 11 eraser

    int jColor= ui->comboPen->currentIndex();

    if(jColor > 5 && jColor < 11){ // random but not eraser
         quint32 red = QRandomGenerator::global()->bounded(128) ;
         quint32 green = QRandomGenerator::global()->bounded(128) ;
         quint32 blue = QRandomGenerator::global()->bounded(128) ;         
         if(jColor == 8){ green = red * 0.6 ; blue = red * 0.6; red += 128;}
         else if(jColor == 9){ red = green *0.6; blue = green * 0.6; green += 128;}
         else if(jColor == 10){ red = blue * 0.6; green = blue *0.6; blue += 128;}
         else{
             red = red *2; green = green * 2; blue = blue *2;
         }
         ncol = QColor(red, green, blue);
    }
    else if(jColor == 5){ // fusion

        int df = sizes::line_width /2;
        int a = sizes::selX -df;
        int c = sizes::selY -df;

        QPixmap pxm= pix.copy(a, c, sizes::line_width, sizes::line_width);
        QImage img = pxm.toImage();
        int redSum =0;
        int greenSum = 0;
        int blueSum =0;
        for(int i =0; i < sizes::line_width; ++i){
            for(int j =0; j < sizes::line_width; ++j){
                redSum = img.pixelColor(i, j).red() + redSum;
                greenSum = img.pixelColor(i, j).green() + greenSum;
                blueSum = img.pixelColor(i, j).blue() + blueSum;
            }
        }
        df = sizes::line_width * sizes::line_width;
        ncol = QColor(redSum/df, greenSum / df, blueSum/df);
    }
    if(ui->markerButton->isChecked()){
        ncol.setAlpha(alpha);
    }

    QPen pen1(ncol, sizes::line_width);
    if(jColor == 3 || jColor == 4 || jColor ==1 ) pen1.setWidth(1); // nib or square
    else if(jColor == 7){ pen1.setCapStyle(Qt::SquareCap);}
    else if(jColor ==1 ||  jColor == 2){ pen1.setCapStyle(Qt::FlatCap);}
    else                               { pen1.setCapStyle(Qt::RoundCap);}

    return pen1;
}


void MainWindow::drawWithPen(){

    updateInfo();

    QColor ncol = sizes::activeColor;

    // stylus
    int penIndex = ui->comboPen->currentIndex();
    if(penIndex == 3 || penIndex == 4){
        QPainter pai(&pix);
        int corners = sizes::line_width / 2;
        QPen pen(configPen(ncol));
        pen.setWidth(0);
        pai.setPen(pen);
        QBrush br(ncol);
        pai.setBrush(br);
        QPolygon poly;
        if(penIndex == 4){
        poly << QPoint(sizes::shape_x_begin/zoomLevel + corners, sizes::shape_y_begin/zoomLevel + corners) << QPoint(sizes::shape_x_begin/zoomLevel - corners, sizes::shape_y_begin/zoomLevel - corners) << QPoint(sizes::shape_x_end/zoomLevel - corners, sizes::shape_y_end/zoomLevel - corners) << QPoint(sizes::shape_x_end/zoomLevel + corners, sizes::shape_y_end/zoomLevel + corners);
        }else{
        poly << QPoint(sizes::shape_x_begin/zoomLevel + corners, sizes::shape_y_begin/zoomLevel - corners) << QPoint(sizes::shape_x_begin/zoomLevel - corners, sizes::shape_y_begin /zoomLevel+ corners) << QPoint(sizes::shape_x_end/zoomLevel - corners, sizes::shape_y_end /zoomLevel+ corners) << QPoint(sizes::shape_x_end /zoomLevel+ corners, sizes::shape_y_end/zoomLevel - corners);
        }
        pai.drawPolygon(poly);
    }
    else if(penIndex == 11){
        colorEraser();
    }

    else{
        // normal pen or marker
        QPainter pai(&pix);
        QPen pen;
        if(ui->markerButton->isChecked()){
            int t = 70 / sizes::line_width ;
            pen =QPen(configPen(ncol, t));
        }else{
            pen= QPen(configPen(ncol));
        }
       pai.setPen(pen);
        if(penIndex == 1){ // square
            int ptx1 = sizes::shape_x_begin/zoomLevel; int pty1 = sizes::shape_y_begin/zoomLevel;
            int ptx2 = sizes::shape_x_end/zoomLevel; int pty2 = sizes::shape_y_end/zoomLevel;
            int corners = sizes::line_width / 2;
            QBrush br(ncol);
            pai.setBrush(br);
            QPolygon poly;
            QPoint p1, p2, p3, p4, p5, p6;
            if(ptx2 > ptx1 && pty2 <= pty1){
                p1 = QPoint( ptx1 - corners, pty1-corners);
                p2 = QPoint( ptx1-corners, pty1+corners);
                p3 = QPoint( ptx1 + corners, pty1+corners);
                p4 = QPoint( ptx2+corners, pty2+corners);
                p5 = QPoint( ptx2 + corners, pty2-corners);
                p6 = QPoint( ptx2-corners, pty2-corners);
            }
            else if(ptx2 > ptx1 && pty2 > pty1){
                p1 = QPoint( ptx1 + corners, pty1-corners);
                p2 = QPoint( ptx1-corners, pty1-corners);
                p3 = QPoint( ptx1 - corners, pty1+corners);
                p4 = QPoint( ptx2-corners, pty2+corners);
                p5 = QPoint( ptx2 + corners, pty2+corners);
                p6 = QPoint( ptx2+corners, pty2-corners);
            }
            else if(ptx2 <= ptx1 && pty2 <= pty1){
                p1 = QPoint( ptx1 + corners, pty1-corners);
                p2 = QPoint( ptx1+corners, pty1+corners);
                p3 = QPoint( ptx1 - corners, pty1+corners);
                p4 = QPoint( ptx2-corners, pty2+corners);
                p5 = QPoint( ptx2 - corners, pty2-corners);
                p6 = QPoint( ptx2+corners, pty2-corners);
            }
            else if(ptx2 <= ptx1 && pty2 > pty1){
                p1 = QPoint( ptx1 - corners, pty1-corners);
                p2 = QPoint( ptx1+corners, pty1-corners);
                p3 = QPoint( ptx1 + corners, pty1+corners);
                p4 = QPoint( ptx2+corners, pty2+corners);
                p5 = QPoint( ptx2 - corners, pty2+corners);
                p6 = QPoint( ptx2-corners, pty2-corners);
            }
            poly << p1 << p2 << p3 << p4 << p5 << p6;
            pai.drawPolygon(poly);
        }
        // end square
        else{

          pai.drawLine(sizes::shape_x_begin/zoomLevel, sizes::shape_y_begin/zoomLevel, sizes::shape_x_end/zoomLevel, sizes::shape_y_end/zoomLevel);
        }
            pai.end();

    }
       sizes::shape_x_begin = sizes::shape_x_end ;
       sizes::shape_y_begin = sizes::shape_y_end ;

    showPix();
}

void MainWindow::draw_first_point()
{
    if(sizes::activeOperation == 5){        
        save_previous("Spray");
        spray_draw();
        return;
    }
    else{
        if(ui->comboPen->currentIndex()== 11){
            save_previous(tr("Color eraser"));
            colorEraser();
            return;
        }
        else{ save_previous(tr("Pen"));}
    }
    updateInfo();
    QPainter pai(&pix);
    QColor ncol = sizes::activeColor;

    QPen pen(configPen(ncol));
    pen.setWidth(sizes::line_width);
    pai.setPen(pen);
    pai.drawPoint(sizes::shape_x_begin / zoomLevel, sizes::shape_y_begin/ zoomLevel);
    pai.end();
    sizes::selX = sizes::shape_x_begin / zoomLevel;
    sizes::selY = sizes::shape_y_begin / zoomLevel;
    showPix();
}

void MainWindow::colorEraser()
{
    int ax = (sizes::shape_x_begin / zoomLevel) - (sizes::line_width/2);
    int ay = (sizes::shape_y_begin / zoomLevel) - (sizes::line_width/2);
    int similarity= ui->similaritySlider->value();
    QPixmap pxm = pix.copy(ax, ay, sizes::line_width, sizes::line_width );
    QImage img = pxm.toImage();
    for(int ix=0; ix < img.width(); ++ix){
        for(int iy=0; iy < img.height(); ++iy){
            if(isSimil(QColor(img.pixelColor(ix, iy)), sizes::activeColor, similarity)){
                img.setPixelColor(ix, iy, fav);
            }
        }
    }
    pxm = QPixmap::fromImage(img);
    QPainter p(&pix);
    p.drawPixmap(ax, ay, pxm);
    p.end();
}

// tracer area
void MainWindow::on_penTracer_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    if(ui->penTracer->isChecked()){
        untoggle();
        ui->penTracer->setChecked(true);
        wArea->setCursor(rectCursor());
        sizes::activeOperation = 14;
        createTraceArea();
    }else{
        sizes::activeOperation = 0;
        delete traceArea;
        traceArea =NULL;
        wArea->setCursor(Qt::ArrowCursor);
    }
}

//--------------------------------------------------
void MainWindow::on_conn_Curve_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    if(ui->conn_Curve->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
        ui->conn_Curve->setChecked(true);
        sizes::activeOperation = 10;
        sizes::shape_x_begin = -1;
        sizes::shape_y_begin = -1;
        sizes::autoPoint = QPoint(-1,-1);
        ui->lineOptionWidget->setVisible(true);
        cl_area = new curveLineArea(wArea, zoomLevel);
        connect(cl_area, SIGNAL(finishCurve()), this, SLOT(finish_curve()));
        sizes::isCurveLineAreaOn = true;
        cl_area->show();
    }else{
        sizes::activeOperation = 0;
        sizes::autoPoint = QPoint(-1,-1);
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
    wArea->setPixmap(pix.scaled(pix.width() * zoomLevel, pix.height() * zoomLevel));
}

void MainWindow::save_previous(QString tx)
{
    historyCount = 1;
    historyList.push_back(tx);
    prePix = QPixmap();
    historyPix.push_back(pix);
    historyPix.push_back(selectionPix);
    if(historyList.count() > 25){
        historyList.removeFirst();
        historyPix.removeFirst();
        historyPix.removeFirst();
    }
    ui->historyCombo->clear();
    ui->historyCombo->addItems(historyList);
    sizes::modify = true;
}

QString MainWindow::selectionCoords()
{

    return ("Selection*" + QString::number(int(sizes::selX / zoomLevel)) + "*" + QString::number(int(sizes::selY / zoomLevel)) + "*" + QString::number(int(sizes::selW / zoomLevel)) + "*" + QString::number(int(sizes::selH / zoomLevel)) );

}


//  FILL
void MainWindow::on_fillButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    if(ui->fillButton->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
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

    QImage img = pix.toImage();
    img =img.convertToFormat(QImage::Format_ARGB32);
    QList<int> coord;
    coord.push_back(sizes::selX / zoomLevel);
    coord.push_back(sizes::selY / zoomLevel);
    QColor oldColor= QColor::fromRgb(img.pixel(sizes::selX / zoomLevel, sizes::selY / zoomLevel));
    if(oldColor == Qt::white && sizes::activeColor == QColor(255,255,255,0)){
        sizes::activeColor = QColor(0,0,0,0);
    }
    QColor ncol = sizes::activeColor;
    if(ui->semiTranspFillButton->isChecked()){
        ncol.setAlpha(64);
    }

    if(oldColor == sizes::activeColor)return;

    img.setPixelColor(sizes::selX / zoomLevel, sizes::selY / zoomLevel, ncol);
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
    if(oldColor == Qt::white && sizes::activeColor == QColor(0,0,0,0)){
        sizes::activeColor = QColor(255,255,255,0);
    }

}

void MainWindow::fillInsideBlack()
{
    save_previous(tr("Fill inside black"));
    updateInfo();
    QImage img = pix.toImage();
    img =img.convertToFormat(QImage::Format_ARGB32);
    QList<int> coord;
    coord.push_back(sizes::selX / zoomLevel);
    coord.push_back(sizes::selY / zoomLevel);
    QColor oldColor= QColor::fromRgb(img.pixel(sizes::selX / zoomLevel, sizes::selY / zoomLevel));
    if(oldColor == Qt::black)return;
    img.setPixelColor(sizes::selX / zoomLevel, sizes::selY / zoomLevel, Qt::black);
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
    int lw = sizes::line_width * 2;//***

    // copied from kolourpaint code (^_^)-------------------------------***
    for(int i=0; i< sizes::line_width; ++i){
        int dx= (QRandomGenerator::global()->generate() % lw )-lw/2 ;
        int dy= (QRandomGenerator::global()->generate() % lw )-lw/2;
         if((dx*dx) +(dy*dy) > lw*6){continue;} //****

         pai.drawPoint((sizes::selX / zoomLevel) + dx, (sizes::selY / zoomLevel) + dy);
    }
    showPix();
}

//picker

void MainWindow::on_pickerButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }

    if(ui->pickerButton->isChecked()){
        untoggle();
        ui->pickerButton->setChecked(true);
        ui->widgetPick->setVisible(true);
        wArea->setCursor(Qt::ArrowCursor);
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
        QColor colorRif = QColor::fromRgba(img.pixel(sizes::selX / zoomLevel, sizes::selY / zoomLevel));
        double deltaR =  double(sizes::activeColor.red()+1.00) / double(colorRif.red()+1.00);
        double deltaG = double(sizes::activeColor.green()+1.00) / double(colorRif.green()+1.00);
        double deltaB = double(sizes::activeColor.blue()+1.00) / double(colorRif.blue()+1.00);
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
    sizes::activeColor = QColor::fromRgba(img.pixel(sizes::selX / zoomLevel, sizes::selY / zoomLevel));
    ui->colorActiveButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    ui->rgbLabel->setText("Rgb " + QString::number(sizes::activeColor.red()) + " " + QString::number(sizes::activeColor.green())+ " " + QString::number(sizes::activeColor.blue()));
    }
    }

//--------------------------------------------------------------------------------------------

// line

void MainWindow::on_comboLines_currentIndexChanged(int index)
    {
        if(index == 1){sizes::isArrow = true;}
        else{ sizes::isArrow = false; }
        ui->lineButton->setChecked(false);
        createLineArea(0);
        ui->lineButton->setChecked(true);
        if(index < 2){ createLineArea(7);}
        else{createLineArea(11);}
}

void MainWindow::createLineArea(int op)
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }

    if(ui->lineButton->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
        ui->lineButton->setChecked(true);
        sizes::activeOperation = op;
        if(op == 11){
            sizes::shape_x_begin = -1;
            sizes::shape_y_begin = -1;
        }
        ui->lineOptionWidget->setVisible(true);
        cl_area = new curveLineArea(wArea, zoomLevel);
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

void MainWindow::on_lineButton_clicked()
{
    int j = ui->comboLines->currentIndex();
    if(j < 2){createLineArea(7);}
    else if( j == 2){ createLineArea(11); }

}

void MainWindow::createShapeArea()
{    
    sizes::selW = sizes::selH = 80.00 * zoomLevel;

    shape_area = new shapeArea(wArea, zoomLevel);
    sizes::isShapeOn=true;
    shape_area->show();

    updateInfo();
    connect(shape_area, SIGNAL(setInfo()), this, SLOT(updateInfo()));
}

void MainWindow::createTraceArea()
{
    traceArea = new tracer(wArea);
    traceArea->show();
    sizes::lineXEnd =1;
    updateInfo();
    connect(traceArea, SIGNAL(vanish()), this, SLOT(tracer_vanishes()));
}

void MainWindow::tracer_vanishes()
{
    if(sizes::lineXEnd == 1){
        sizes::lineXEnd = 0;
        QTimer::singleShot(1200, this, SLOT(tracer_vanishes()));
    }else{
        delete traceArea;
        traceArea =NULL;
        if(sizes::activeOperation == 14) createTraceArea();
    }
}

void MainWindow::draw_shape()
{
    // shape
    if(sizes::activeOperation == 8 && sizes::isShapeOn ==false){
        createShapeArea();
    }    
    else if(sizes::activeOperation == 8 && sizes::isShapeOn == true){
        save_previous(tr("Shape"));
        updateInfo();
        // repaint shape
        sizes::selH /= zoomLevel;
        sizes::selW /= zoomLevel;
        sizes::selX /= zoomLevel;
        sizes::selY /= zoomLevel;
        shape_area->resetGeometry(true);
        QPainter pai(&pix);
        QPixmap sPix= shape_area->pixmap();
        pai.drawPixmap(sizes::selX+2 , sizes::selY, sPix);
        showPix();
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


void MainWindow::on_quickPenWidth_activated(int index)
{
    int n = ui->quickPenWidth->currentText().toInt();
    ui->lineWidthBox->setValue(n);
}

//  shapes
void MainWindow::on_shapeButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }   
    if(ui->shapeButton->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
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
    shapeNames << "squ" << "rec" << "cir" << "ell" << "tri" << "rou" << "rsq" << "sta" << "aup" << "ari" << "ado" << "ale" << "aul" << "aur" << "abr" << "abl" << "crp" << "crx" << "sol";
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


void MainWindow::finish_lines()
{
    if(sizes::activeOperation == 7){
        if(sizes::isArrow == false){ save_previous(tr("Line"));}
        else{save_previous(tr("Simple arrow"));}
    }
    else if(sizes::activeOperation == 13){
        createFreeSel();
        sizes::isCurveLineAreaOn = false;
        delete cl_area;
        cl_area =NULL;
    }
    else {save_previous(tr("Connected lines"));}
    if(sizes::isCurveLineAreaOn){
      QPixmap cPix = cl_area->pixmap();
      cPix = cPix.scaled((sizes::areaWidth-8) / zoomLevel, (sizes::areaHeight-8) / zoomLevel);
      QPainter p(&pix);
      p.drawPixmap(0,0, cPix);
      showPix();
      delete cl_area;
      cl_area =NULL;
      // continue
      cl_area = new curveLineArea(wArea, zoomLevel);
      connect(cl_area, SIGNAL(finishLines()), this, SLOT(finish_lines()));
      cl_area->show();
    }
}

void MainWindow::createFreeSel()
{
    // draw polygon
    QPixmap cPix = cl_area->pixmap();
    cPix = cPix.scaled((cPix.width() / zoomLevel), (cPix.height() / zoomLevel));
    QPolygon poly(sizes::freeSelPoints);
    // to delete only the path with the delete key----------------
    passPath = true;
    //-------------------------------------------------------------
    QRect polyRect = poly.boundingRect();
    QPainterPath myPath;
    myPath.addPolygon(poly);
    QPainter pai(&cPix);
    QBrush br(Qt::blue);
    br.setStyle(Qt::SolidPattern);
    pai.setBrush(br);
    pai.drawPath(myPath);
    pai.end();
    QPixmap rectPix = cPix.copy(polyRect);
    freeSelPix = rectPix;

    QImage aPix = pix.toImage();
    QImage bPix = rectPix.toImage();
    // copy pixels of pix in  area and set white in pix
    int x1 = polyRect.x();
    int y1 = polyRect.y();
    QColor k, kpix;
    for(int ax = 0; ax < rectPix.width(); ++ax){
        for(int ay =0; ay < rectPix.height(); ++ay){
            // replace in rectPix the blue with pix pixel, and in pix with white
            k = bPix.pixelColor(ax, ay);
            if(k == Qt::blue || k == Qt::yellow || k == Qt::black){
                kpix = aPix.pixelColor(x1+ax, y1+ay);
                bPix.setPixelColor(ax, ay, kpix);
                aPix.setPixelColor(x1+ax, y1+ay, Qt::white);
            }
        }
    }
    // clear vector
    sizes::freeSelPoints.clear();
    // create selection
    // ---- def sizes
    sizes::selX = x1 * zoomLevel;
    sizes::selY = y1 * zoomLevel;
    sizes::selW = double(polyRect.width()) * zoomLevel ;
    sizes::selH = double(polyRect.height()) * zoomLevel;
    // ----- check all
    sizes::activeOperation=1;

    untoggle();
    wArea->setCursor(Qt::CrossCursor);
    ui->selectionAreaButton->setChecked(true);
    selectionPix = QPixmap::fromImage(bPix);
    save_previous(selectionCoords() + "*f");
    // show pix
    pix = QPixmap::fromImage(aPix);
    showPix();
    // ----- set pixmap    
    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    selectionRect->setPixmap(selectionPix);
    sizes::isSelectionOn=true;
    sizes::modify = true;
    restSelX = 0.0;
    restSelY = 0.0;
    selectionRect->show();
    raiseBorders();
    updateInfo();
    connect(selectionRect, SIGNAL(setInfo()), this, SLOT(updateInfo()));
    connect(selectionRect, SIGNAL(setCopy()), this, SLOT(on_actionCopy_triggered()));
    // save history

}

// free hand selection
void MainWindow::on_freeSelectionAreaButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    if(ui->freeSelectionAreaButton->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
        ui->freeSelectionAreaButton->setChecked(true);
        sizes::activeOperation = 13;
        cl_area = new curveLineArea(wArea, zoomLevel);
        connect(cl_area, SIGNAL(finishLines()), this, SLOT(finish_lines()));
        sizes::isCurveLineAreaOn = true;
        cl_area->show();
    }else{
        sizes::activeOperation = 0;
        if(sizes::isCurveLineAreaOn){
            sizes::isCurveLineAreaOn = false;
            delete cl_area;
            cl_area =NULL;
        }
    }
}

//  curve line
void MainWindow::on_curveButton_clicked()
{
    if(sizes::isSelectionOn){
        sizes::isSelectionOn=false;
        drawCopy();
    }
    if(ui->curveButton->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
        ui->curveButton->setChecked(true);
        sizes::activeOperation = 9;
        ui->lineOptionWidget->setVisible(true);
        cl_area = new curveLineArea(wArea, zoomLevel);
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
      cPix = cPix.scaled(pix.width(), pix.height());
      QPainter p(&pix);
      p.drawPixmap(0,0, cPix);
      showPix();
      delete cl_area;
      cl_area =NULL;
      // continue
      cl_area = new curveLineArea(wArea, zoomLevel);
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
      posXofZoom = (sizes::selX / zoomLevel) -8;
      posYofZoom = (sizes::selY / zoomLevel) -8;
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
      connect(zoom_area, SIGNAL(changeActiveColor()), this, SLOT(set_activeColor()) );
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
        sizes::zoomEdited = false;
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
    QTransform tf;
    tf.rotate(ui->RotatioAngleSpin->value() * a);
    if(!sizes::isSelectionOn){
        save_previous(tr("Image rotation"));
        QRect rect = tf.mapRect(QRect(0,0,sizes::areaWidth, sizes::areaHeight));
        sizes::areaWidth = rect.width();
        sizes::areaHeight = rect.height();
        areaSize();
        pix = pix.transformed(tf);
        showPix();
        PlaceEdges();
        raiseBorders();
    }
    else{
        QRect rect = tf.mapRect(QRect(0,0,sizes::selW, sizes::selH));
        sizes::selW =rect.width();
        sizes::selH = rect.height();
        selectionRect->resetGeometry();
        QPixmap sPix = selectionRect->pixmap().transformed(tf);
        selectionRect->setPixmap(sPix);
        selectionPix = sPix.scaled(sizes::selW / zoomLevel, sizes::selH/zoomLevel);
    }
    updateInfo();
}

void MainWindow::on_autoRotationButton_clicked()
{
    if(!sizes::isSelectionOn){
        QMessageBox::information(this, "Drawish", tr("No selection"));
        ui->autoRotationButton->setChecked(false);
        return;
    }
    if(ui->autoRotationButton->isChecked()){        
        ui->autoRotationButton->setIcon(QIcon(":/res/crono2.png"));
        wPreRotate = sizes::selW;
        hPreRotate = sizes::selH;
        preRotatePix = selectionPix;
        sizes::isRotating= true;
        autoRotation();

    }else{
        ui->autoRotationButton->setIcon(QIcon(":/res/crono1.png"));
        sizes::isRotating = false;
        preAngle =0;
        selectionPix = selectionRect->pixmap().scaled(sizes::selW/zoomLevel, sizes::selH/zoomLevel);
        updateInfo();
    }
}

void MainWindow::autoRotation()
{
    if(!sizes::isRotating)return;
    preAngle++;
    if(preAngle > 360) preAngle =0;
    QTransform tf;
    tf.rotate(preAngle);
    QRect rect = tf.mapRect(QRect(0,0, wPreRotate, hPreRotate));
    sizes::selW = double(rect.width());
    sizes::selH = double(rect.height());
    selectionRect->resetGeometry();
    QPixmap sPix = preRotatePix.transformed(tf);
    selectionRect->setPixmap(sPix);
    ui->RotatioAngleSpin->setValue(preAngle);
    QTimer::singleShot(25, this, SLOT(autoRotation()));
}


// ----
//  resize scale
void MainWindow::on_actionSizes_2_triggered()
{

    DialogSize dSize(this, pix.width(), pix.height());
    dSize.setModal(true);
    dSize.exec();
    sizes::areaWidth =  (dSize.pixWW * zoomLevel) + 8;
    sizes::areaHeight = (dSize.pixHH * zoomLevel) + 8;

    if(dSize.returned == 1){
        save_previous(tr("Resize"));
        reSize();
        sizes::startResize = false;
    }
    else if(dSize.returned == 2){
        save_previous(tr("Scale"));
        areaSize();
        if(sizes::isCurveLineAreaOn){
            finish_lines();
        }
        pix = pix.scaled(dSize.pixWW, dSize.pixHH);
        showPix();
        PlaceEdges();
    }
    else if(dSize.returned == 3){
        sizes::selX *= zoomLevel;
        sizes::selY *= zoomLevel;
        sizes::selW *= zoomLevel;
        sizes::selH *= zoomLevel;

        if(sizes::activeOperation==1){
            if(sizes::isSelectionOn){
                QMessageBox::information(this, "Drawish", tr("An area is already selected"));
                return;
            }else{                
                save_previous(selectionCoords() );
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
            save_previous( selectionCoords());
            createSelection();
        }
    }
    updateInfo();
}

//  effects

void MainWindow::on_actionEffects_triggered()
{
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionPix;
    }

    DialogEffects dEffects(this, Epix);
    dEffects.setModal(true);
    dEffects.exec();

    if(dEffects.res == QDialog::Accepted){
         if(sizes::isSelectionOn){
            selectionPix = dEffects.modifPix;
            selectionRect->setPixmap( selectionPix);
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
        Epix = selectionPix;
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
            selectionPix = dCol.origPix;
            selectionRect->setPixmap( selectionPix);
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
        Epix = selectionPix;
    }
    QImage img = Epix.toImage();

    img.mirror(horizontal, vertical);
    if(sizes::isSelectionOn){
        selectionPix = QPixmap::fromImage(img);
        selectionRect->setPixmap(selectionPix);
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
        Epix = selectionPix;
    }
    QImage img = Epix.toImage();
    img = img.convertToFormat(QImage::Format_Grayscale8);
    if(sizes::isSelectionOn){
        selectionPix = QPixmap::fromImage(img);
        selectionRect->setPixmap(selectionPix);
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
        Epix = selectionPix;
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
        selectionPix = QPixmap::fromImage(img);
        selectionRect->setPixmap(selectionPix);
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
        Epix = selectionPix;
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
        selectionPix = QPixmap::fromImage(img);
        selectionRect->setPixmap(selectionPix);
    }else{
      pix = QPixmap::fromImage(img);
      showPix();
    }
}

void MainWindow::on_actionCreate_mask_triggered()
{
    save_previous(tr("Outline"));
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionPix;
    }
    QImage img = Epix.toImage();
    QImage img2= img;

    int v = QInputDialog::getInt(this, "Drawish", tr("Enter limit"), 15, 2, 64);

    for (int y = 0; y < (img.height()-1); ++y) {
        for (int x = 0; x < (img.width()-1); ++x) {
            QColor col1 = img.pixelColor(x,y);
            QColor col3 = img.pixelColor(x+1, y);
            QColor col2 = img.pixelColor(x, y+1);
            int r = col1.red();
            int g = col1.green();
            int b = col1.blue();
            int r2 = col2.red();
            int g2 = col2.green();
            int b2 = col2.blue();
            int r3 = col3.red();
            int g3 = col3.green();
            int b3 = col3.blue();
            if(abs(r-r2) > v || abs(g-g2)> v || abs(b-b2) > v || abs(r-r3) > v || abs(g-g3) > v || abs(b-b3) > v){
                img2.setPixelColor(x,y, Qt::black);
            }else{
                img2.setPixelColor(x,y, Qt::gray);
            }
        }
    }

    if(sizes::isSelectionOn){
        selectionPix = QPixmap::fromImage(img2);
        selectionRect->setPixmap(selectionPix);
    }else{
        pix = QPixmap::fromImage(img2);
        showPix();
    }
}


void MainWindow::on_actionInvert_colors_triggered()
{
    save_previous(tr("Invert colors"));
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionPix;
    }
    QImage img = Epix.toImage();
    img.invertPixels();
    if(sizes::isSelectionOn){
        selectionPix = QPixmap::fromImage(img);
        selectionRect->setPixmap(selectionPix);
    }else{
      pix = QPixmap::fromImage(img);
      showPix();
    }
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
    preWidthCamera = QInputDialog::getInt(this, "Drawish", tr("Enter width"), 60);
    preHeightCamera = QInputDialog::getInt(this, "Drawish", tr("Enter height"), 60);
    if(preWidthCamera < 8 || preHeightCamera < 8){
        QMessageBox::information(this, "Drawish", tr("Too small selection"));
        return;
    }
    save_area = new saveCam(wArea, QSize(preWidthCamera * zoomLevel, preHeightCamera * zoomLevel));
    int camx =ui->scrollArea->horizontalScrollBar()->value();
    int camy =ui->scrollArea->verticalScrollBar()->value();
    save_area->setGeometry(camx, camy, preWidthCamera * zoomLevel, preHeightCamera * zoomLevel);
    save_area->show();
    isCam = true;
}


void MainWindow::on_actionSave_image_triggered()
{
    if(isCam){
        int ax = (save_area->pos().x() / zoomLevel);
        int ay = (save_area->pos().y() / zoomLevel);
        QPixmap camPix = pix.copy(ax, ay, preWidthCamera, preHeightCamera);
        QFileDialog dialog(this);
        if(QDir(defaultPathDialog).exists() == false){defaultPathDialog = QDir::homePath();}
        QString f =dialog.getSaveFileName(this, tr("Drawish save..."), defaultPathDialog, "Images (*.png *.jpg *.ico *bmp)");
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
    if(ui->historyCombo->count() < 1){ return;}
    if(prePix.isNull()) prePix = pix;
    pix = historyPix[index*2];  // 0-2-4 pixmap

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
        sizes::selW = data.at(3).toDouble();
        sizes::selH = data.at(4).toDouble();
        // selection
        sizes::activeOperation=1;
        wArea->setCursor(Qt::CrossCursor);
        untoggle();
        ui->selectionAreaButton->setChecked(true);

        sizes::selX *= zoomLevel;
        sizes::selY *= zoomLevel;
        sizes::selW *= zoomLevel;
        sizes::selH *= zoomLevel;       
        selectionPix = historyPix[index * 2 + 1];  // 1--5
            createSelection(true);       
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
      sizes::selH = sizes::selH / zoomLevel;
      sizes::selW = sizes::selW / zoomLevel;
      QImage img = selectionPix.toImage();
      sizes::selH *= 2.00;
      sizes::selW *= 2.00;
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
       sizes::selH = sizes::selH * zoomLevel;
       sizes::selW = sizes::selW * zoomLevel;
       selectionRect->resetGeometry();
       selectionPix = bigPix;
       selectionRect->setPixmap(bigPix);

  }
  else{
  save_previous(tr("Quadruple"));
  if(sizes::isCurveLineAreaOn){
      if(sizes::activeOperation == 7 || sizes::activeOperation == 11){
          finish_lines();
      }
      else if(sizes::activeOperation == 10){
          finish_curve();
      }
  }
  QPixmap bigPix(pix.width() * 2, pix.height() * 2);
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
  sizes::areaWidth = (pix.width() * zoomLevel) + 8;
  sizes::areaHeight = (pix.height() * zoomLevel) + 8;
  areaSize();
  showPix();
  PlaceEdges();
  }
  updateInfo();
}


void MainWindow::on_actionDivide_by_5_triggered()
{
    if(sizes::isSelectionOn){
        sizes::selH = sizes::selH / zoomLevel;
        sizes::selW = sizes::selW / zoomLevel;
        QImage img = selectionPix.toImage();
        sizes::selH /=2.00;
        sizes::selW /=2.00;
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
        sizes::selH = sizes::selH * zoomLevel;
        sizes::selW = sizes::selW * zoomLevel;
        selectionRect->resetGeometry();
        selectionPix = smallPix;
        selectionRect->setPixmap(smallPix);
    }
    else{
    save_previous(tr("Divide by 4"));
    QPixmap smallPix(pix.width() / 2, pix.height() / 2);
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
    sizes::areaWidth = (pix.width() * zoomLevel) + 8;
    sizes::areaHeight = (pix.height() * zoomLevel) + 8;
    areaSize();
    showPix();
    PlaceEdges();
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
        QPen pen(configPen(ncol, 48));

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
        showPix();
    }
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
    QString outName =QDir::homePath() + "/Drawish_Data/drawish";
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
    int offset = 0;
       while(offset < pix.height()) {
           QRectF source(QPoint(0, offset), printer.pageRect(QPrinter::DevicePixel).size());
           painter.drawPixmap(printer.pageRect(QPrinter::DevicePixel), pix, source);
           offset += printer.pageRect(QPrinter::DevicePixel).height();
           printer.newPage();
       }
    painter.end();
    QMessageBox::information(this, "Drawish", tr("Saved in\n") + cmpName);
}


void MainWindow::on_actionStretch_area_triggered()
{
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionPix;
    }else{
        QMessageBox::information(this, "Drawish", tr("No selection!"));
        return;
    }

    StretchDialog *strd= new StretchDialog(this, Epix);
    strd->setModal(true);
    strd->exec();
    if(strd->res == QDialog::Accepted){
        sizes::selH = double(strd->epix.height()) * zoomLevel;
        sizes::selW = double(strd->epix.width()) * zoomLevel;
        //save_previous(selectionCoords());
        selectionRect->resetGeometry();
        selectionPix = strd->epix;
        selectionRect->setPixmap(selectionPix);
        }
    delete strd;

}


void MainWindow::on_actionSet_triggered() // crop
{
    if(sizes::isSelectionOn){
        remSelX = (sizes::selX / zoomLevel) +0.99;
        remSelY = (sizes::selY / zoomLevel) +0.99;
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


void MainWindow::addChart(QPixmap gr)
{
    save_previous(tr("Create chart"));
    int wImg = gr.width();
    int hImg = gr.height();
    int v = pix.height() + 3;
    sizes::areaHeight += (double(gr.height()) * zoomLevel);
    if(wImg > pix.width()){ sizes::areaWidth = (double(wImg) * zoomLevel) + 8.00; }
    reSize();
    QPainter p(&pix);    
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
    addChart(gPix);
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
    addChart(gPix);
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
    addChart(gPix);

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
    ui->labelRestorePoint->setText("");
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
    QPen pen(configPen(ncol, 48));
    pen.setJoinStyle(Qt::MiterJoin);

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
    showPix();
}

void MainWindow::on_comboPen_activated(int index)
{
    wArea->setCursor(rectCursor());
}


void MainWindow::on_rtfButton_clicked()
{
    richEditor rtf;
    rtf.setModal(true);
    rtf.exec();
    if(rtf.passed){
        // del. text options------------------------
        delete selectionRect;
        selectionRect =NULL;
        sizes::activeOperation = 0;
        ui->textOptionsWidget->setVisible(false);
        //------------------------------------------
        pasteImg(rtf.rtfPix);
    }

}


void MainWindow::on_actionMouse_correctio_triggered(bool checked)
{
    sizes::mouseCorrection = checked;
}

void MainWindow::on_actionSet_as_favorite_color_triggered()
{
    ui->prefColorButton->setStyleSheet("background-color:" + sizes::activeColor.name());
    fav = sizes::activeColor;
    // save to image
    fileIO fio;
    fio.saveFav_color(fav, QDir::homePath() + "/Drawish_Data/drawish_saved_colors.png");
}


void MainWindow::on_prefColorButton_clicked()
{
    set_activeColor(fav.red(), fav.green(), fav.blue(), 255);
}


void MainWindow::on_actionPick_color_from_an_image_triggered()
{
    if(QFile::exists(QDir::homePath() + "/Drawish_Data/drawish_saved_colors.png")){
        QPixmap paletteImg(QDir::homePath() + "/Drawish_Data/drawish_saved_colors.png");
        PickPalette pickPalette(paletteImg);
        pickPalette.exec();
        set_activeColor(pickPalette.pickd.red(),pickPalette.pickd.green(), pickPalette.pickd.blue(), 255);
    }
    else{QMessageBox::information(this, "Drawish", tr("No color saved"));}
}


void MainWindow::on_comboPen_highlighted(int index)
{
    QString tx= ui->comboPen->itemText(index);
    ui->comboPen->setToolTip(tx);
}


void MainWindow::on_resetFavColorButton_clicked()
{
    ui->prefColorButton->setStyleSheet("background-color: #FFFFFF");
    fav = Qt::white;
}




void MainWindow::on_actionDesktop_shortcut_triggered()
{
#ifdef Q_OS_APPLE
    QMessageBox::information(this, "Drawish", tr("System not supported!"));
#endif

#ifdef Q_OS_LINUX

    QByteArray result;
    QProcess proc;
    QStringList args;
    args << "-n" << "1" << "-bc" << "-w" << "500";
    proc.start("top", args);
    bool started = proc.waitForStarted();
    proc.waitForFinished();
    result = proc.readAllStandardOutput();
    QString res = result;
    // search .appimage
    QStringList processes = res.split("\n");
    args.clear();
    QString d="";
    foreach (QString r, processes) {
        if(r.endsWith(".appimage", Qt::CaseInsensitive)){
            if(r.contains("drawish", Qt::CaseInsensitive)){
                int idx = r.indexOf("/");
                d = r.mid(idx, r.length()-idx);
                args.push_back(d);
            }
        }
    }
    if(args.count() == 0){
        QMessageBox::information(this, "Drawish", tr("No appimage called 'Drawish' found!"));
        return;
    }
    else if(args.count() > 1){
        QMessageBox::information(this, "Drawish", tr("Keep only one instance of Drawish open."));
        return;
    }

    install(args.at(0));


#endif

#ifdef Q_OS_WIN
    // find path
    QString myPathApp =QCoreApplication::applicationDirPath();
    QString myFilePath = QCoreApplication::applicationFilePath();
    // create icon
    QPixmap icona(":/res/draw.png");
    if(!QFile::exists(myPathApp + "/drawish.ico")){
        icona.save(myPathApp + "/drawish.ico", "ico");
    }

    // create vbscript
    QString vbs ="set ws= WScript.CreateObject(\"WScript.shell\")\n";
    vbs += "strD = ws.SpecialFolders(\"Desktop\")\n";
    vbs += "set ossl= ws.CreateShortcut(strD & \"\\Drawish.lnk\")\n";
    QString myPath2 = myPathApp.replace("/","\\");
    vbs += "ossl.IconLocation = \"" + myPath2 + "\\drawish.ico\"\n";
    myPathApp.replace("\\","/");
    myPath2 = myFilePath.replace("/", "\\");
    vbs += "ossl.TargetPath = \"" + myPath2 + "\"\n";

    vbs += "ossl.WindowStyle = 1\nossl.Save";
    // create file
    fileIO fio;
    fio.createFile(vbs, myPathApp + "/desktopShortcut.vbs" );

    // launch vbs
    QDesktopServices::openUrl(myPathApp + "/desktopShortcut.vbs");
    QMessageBox::information(this, "Drawish", tr("Add Drawish to desktop"));

#endif

}

void MainWindow::install(QString execPath)
{
    //   create icon
    QString pathIco=QDir::homePath() + "/.local/share/icons/drawish.png";
    QDir myApp;
    QPixmap ikon(":/res/draw.png");
    if(!myApp.exists(QDir::homePath() + "/.local/share/icons")){
        myApp.mkdir(QDir::homePath() + "/.icons");
        pathIco = QDir::homePath() + "/.icons/drawish.png";
        ikon.save(pathIco, "PNG");
    }
    else{
        if(QFile::exists(QDir::homePath() + "/.local/share/icons/drawish.png")== false){
            ikon.save(QDir::homePath() + "/.local/share/icons/drawish.png", "PNG");        }
    }
    //  create desktop file

    QString desk="[Desktop Entry]\nVersion=1.0\nType=Application\nName=Drawish\nComment=Simple and complete drawing app\nExec=" + execPath + " %F\nIcon=" + pathIco + "\nCategories=Graphics\nTerminal=false\nStartupNotify=false" ;
    QDir share(QDir::homePath());
    share.mkpath(".local/share/applications");
    fileIO fio;
    fio.createFile(desk, QDir::homePath() + "/.local/share/applications/Drawish.desktop");
    QMessageBox::information(this, "Drawish", tr("Add Drawish to menu"));
}

void MainWindow::on_comboLines_highlighted(int index)
{
    QString tx= ui->comboLines->itemText(index);
    ui->comboLines->setToolTip(tx);
}

void MainWindow::on_actionGithub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/nikkNizz/Drawish"));
}

void MainWindow::on_actionReadme_and_help_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/nikkNizz/Drawish/blob/main/README.md"));
}


void MainWindow::on_actionWindow_size_triggered()
{
    int ww = QInputDialog::getInt(this, "Drawish", tr("Enter new width"), this->width());
    int hh = QInputDialog::getInt(this, "Drawish", tr("Enter new height"), this->height());
    if(ww < 32 || hh < 32){
        QMessageBox::information(this, "Drawish", tr("Too small size"));
        return;
    }
    setGeometry(pos().x(), pos().y(), ww, hh);
}


void MainWindow::on_actionSerialize_triggered()
{
    Serialize *serial = new Serialize(this);
    serial->setModal(true);
    serial->exec();
}


void MainWindow::on_actionScreenshot_me_triggered()
{
    QPixmap shot = this->grab();
    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setPixmap(shot);
    QMessageBox::information(this, "Drawish", tr("Done!"));
}

// --------------------------------------- zoom
void MainWindow::zoomAll(double zm)
{
    double preZoom = zoomLevel;
    zoomLevel = zm / 100;
    if( preZoom != zoomLevel){
        sizes::areaHeight = (pix.height() *  zoomLevel) + 8;
        sizes::areaWidth = (pix.width() * zoomLevel) + 8;
        showPix();
        areaSize();
        PlaceEdges();
        ui->labelZoomLevel->setText("Zoom " + QString::number(zm) + "%");
        if(sizes::activeOperation == 13){
            on_freeSelectionAreaButton_clicked();
        }
        if(sizes::isSelectionOn){
            if(sizes::activeOperation == 2){
                sizes::selW *= zoomLevel;
                sizes::selH *= zoomLevel;
            }else{
            sizes::selW = double(selectionPix.width()) * zoomLevel;
            sizes::selH = double(selectionPix.height()) * zoomLevel;
            }

            double dX = (double(sizes::selX) + restSelX) / preZoom * zoomLevel;
            double dY = (double(sizes::selY) + restSelY) / preZoom * zoomLevel;
            double fx = dX - int(dX);
            double fy = dY - int(dY);

            restSelX = fx;
            restSelY = fy;
            sizes::selX = dX;
            sizes::selY = dY;

            selectionRect->resetGeometry();
            if(sizes::activeOperation !=2){
                selectionRect->setPixmap(selectionPix);
            }else{
                on_textEdit_textChanged();
            }
        }
        if(sizes::isShapeOn){
            sizes::selW = sizes::selW / preZoom * zoomLevel;
            sizes::selH = sizes::selH / preZoom * zoomLevel;
            sizes::selX = sizes::selX / preZoom * zoomLevel;
            sizes::selY = sizes::selY / preZoom * zoomLevel;
            shape_area->Zoom = zoomLevel;
            shape_area->resetGeometry(true);
        }

        if(isCam){
            int aax = save_area->pos().x() / preZoom * zoomLevel;
            int aay = save_area->pos().y() / preZoom * zoomLevel;
            int aaw = preWidthCamera * zoomLevel;
            int aah = preHeightCamera * zoomLevel;
            save_area->setGeometry(aax, aay, aaw, aah);
        }

        if(sizes::isCurveLineAreaOn){
            if(sizes::activeOperation == 9){
                on_curveButton_clicked();
            }
            else if(sizes::activeOperation == 10){
                on_conn_Curve_clicked();
            }
            else if(sizes::activeOperation == 7 || sizes::activeOperation == 11){
                on_lineButton_clicked();
            }
        }
    }
    if(sizes::activeOperation ==3 || sizes::activeOperation ==5 || sizes::activeOperation == 10){
        wArea->setCursor(rectCursor());
    }
}

void MainWindow::on_action100_triggered()
{
    zoomAll(100.0);
}


void MainWindow::on_action150_triggered()
{
    zoomAll(150.0);
}


void MainWindow::on_action200_triggered()
{
    zoomAll(200.0);
}


void MainWindow::on_action300_triggered()
{
    zoomAll(300.0);
}


void MainWindow::on_action400_triggered()
{
    zoomAll(400.0);
}


void MainWindow::on_action50_triggered()
{
    zoomAll(50.0);
}


void MainWindow::on_action30_triggered()
{
    zoomAll(30.0);
}


void MainWindow::on_actionMore_triggered()
{
    int l = QInputDialog::getInt(this,"Drawish", tr("Enter zoom level (10-400)"), 100, 10, 400);
    zoomAll(double(l));
}

