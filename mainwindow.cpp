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
#include "richeditor.h"
#include "pickpalette.h"
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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    version_info = "1.4.1";
    // 1.5: resize bug !!!; shortcut + ctrl++; style fusion; area cpp show/hide grid; geometric w h values

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
                removeFromRecent(activePathFile);
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
        if(ev->key() ==Qt::Key_W || ev->key()== Qt::Key_Q){sizes::selY--;}
        else if(ev->key()== Qt::Key_A){sizes::selX--;}
        else if(ev->key()== Qt::Key_S){sizes::selX++;}
        else if(ev->key()== Qt::Key_Z || ev->key()== Qt::Key_X){sizes::selY++;}
        else if(ev->key()== Qt::Key_Delete){
            QPixmap spix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
            spix.fill(Qt::white);
            selectionRect->setPixmap(spix);
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
    val +=  "<degrees>" + QString::number(ui->RotatioAngleSpin->value()) + "</degrees>";

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
    ui->infoWidget->setGeometry(80,this->height()-88, 600, 45);
}

void MainWindow::reSize()
{   
   if(!sizes::startResize){save_previous(tr("Resize"));}
   sizes::startResize=true;
   areaSize();
   // if(sizes::isCurveLineAreaOn){
   //     if(sizes::activeOperation == 7 || sizes::activeOperation == 11){
   //         finish_lines();
   //     }
   //     else if(sizes::activeOperation == 10){
   //         finish_curve();
   //     }
   // }
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
        activePathFile = f;
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
    else if(f.endsWith(".png", Qt::CaseInsensitive)){ pixToSave.save(f, "png");}//0.9.12
    else{pixToSave.save(f + ".png", "PNG");}  // 0.9.10
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
          if(npix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file"));
              removeFromRecent(f);
              return;
          }
          activePathFile = f;
          addToRecent(activePathFile);
      }
      sizes::modify= false;

      if(npix.isNull()){ QMessageBox::information(this, "Drawish", tr("Unsupported file"));
          removeFromRecent(f);
          return;
      }
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

  if(sizes::activeOperation == 2 && sizes::isSelectionOn){
        ui->textEdit->setFocus();
  }

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
}

void MainWindow::createSelectionFromRubb()
{
    save_previous("Image");
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

void MainWindow::createSelection()
{
    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    if(sizes::activeOperation != 2){  // !selection area for text
        QPixmap selPix = pix.copy(sizes::selX , sizes::selY , sizes::selW , sizes::selH );
        if(ui->actionTransparent_selection->isChecked()){
            selPix=addTransparency(selPix, 0,255,255,255);
        }

      selectionRect->setPixmap(selPix);
      QPixmap blank(sizes::selW ,sizes::selH);
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
    connect(selectionRect, SIGNAL(setCopy()), this, SLOT(on_actionCopy_triggered()));
}


void MainWindow::drawCopy()
{    
    QPixmap selectedImage(selectionRect->pixmap());    
    save_previous("From selection");
    QPainter p(&pix);  
    p.drawPixmap(sizes::selX ,sizes::selY, selectedImage.scaled(sizes::selW, sizes::selH));
    wArea->setPixmap(pix);
    delete selectionRect;
    selectionRect = NULL;
   // save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
    updateInfo();
}


void MainWindow::on_actionSelect_all_triggered()
{
    sizes::selX =0;
    sizes::selY =0;
    sizes::selH = sizes::areaHeight-8;
    sizes::selW = sizes::areaWidth-8;
    save_previous("Image");
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
    if(wi > wArea->width()){ sizes::areaWidth = wi+8;}
    if(he > wArea->height()){sizes::areaHeight = he+8;}
    reSize();
    sizes::selX =ui->scrollArea->horizontalScrollBar()->value();
    sizes::selY =ui->scrollArea->verticalScrollBar()->value();
    sizes::selH = he;
    sizes::selW = wi;

    untoggle();
    if(ui->actionTransparent_selection->isChecked()){        // 0.9.10
       passedPix = addTransparency(passedPix, 0, 255,255,255);
    }
    ui->selectionAreaButton->setChecked(true);
    sizes::activeOperation =1;
    wArea->setCursor(Qt::CrossCursor); //0.9.10
    selectionRect = new selectionArea(wArea);
    selectionRect->resetGeometry();
    selectionRect->setPixmap(passedPix.scaled(sizes::selW, sizes::selH));
    sizes::isSelectionOn=true;
    sizes::startResize=false;
    selectionRect->show();
    if((sizes::selW * sizes::selH) > (sizes::areaWidth * sizes::areaHeight * 0.6) ){
        QMessageBox::information(this, "Drawish", tr("The image is in a selection"));
    }  // 0.9.10
    //ui->scrollArea->ensureVisible(0,0);

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

void MainWindow::on_actionTransparent_selection_toggled(bool arg1)
{
    if(sizes::isSelectionOn == false) return;    // 0.9.10
    QPixmap pxm = selectionRect->pixmap();
    if(arg1 == true){
        pxm = addTransparency(pxm, 0, 255,255,255);
    }else if(arg1 == false){
        QImage img = pxm.toImage();
        for(int h=0; h < img.width(); ++h){
            for(int v =0; v < img.height(); ++v){
                QColor c = img.pixelColor(h,v);
                if(c.red()==0 && c.green() ==0 && c.blue()==0 && c.alpha()==0){
                    img.setPixelColor(h,v, Qt::white);
                }
            }
        }
        pxm = QPixmap::fromImage(img);
    }
    selectionRect->setPixmap(pxm);
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
    int sizeText = ui->sizeLine->text().toInt();
    if(sizeText < 4){sizeText = 4;}
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
        if(sizeText < 4){sizeText = 4; ui->sizeLine->setText("4");}
        tFont.setPixelSize(sizeText);
        p.setFont(tFont);
        p.setPen(sizes::activeColor);
        int xpos = selectionRect->x();
        int ypos = selectionRect->y();
        p.drawText(QRect(xpos+2, ypos+2, sizes::selW, sizes::selH), ui->textEdit->toPlainText());
        wArea->setPixmap(pix);
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
    sizes::activeColor= QColor(R,G,B, A);
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
    // combopen: 0 round, 1 square, 2 flat, 3 nib, 4 fusion 5 rand round, 6 random square, 7 random red
    // 8 random green, 9 random blue, 10 eraser

    int jColor= ui->comboPen->currentIndex();

    if(jColor > 4 && jColor < 10){ // random but not eraser
         quint32 red = QRandomGenerator::global()->bounded(128) ;
         quint32 green = QRandomGenerator::global()->bounded(128) ;
         quint32 blue = QRandomGenerator::global()->bounded(128) ;         
         if(jColor == 7){ green = red * 0.6 ; blue = red * 0.6; red += 128;}
         else if(jColor == 8){ red = green *0.6; blue = green * 0.6; green += 128;}
         else if(jColor == 9){ red = blue * 0.6; green = blue *0.6; blue += 128;}
         else{
             red = red *2; green = green * 2; blue = blue *2;
         }
         ncol = QColor(red, green, blue);
    }
    else if(jColor == 4){ // fusion

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
    if(jColor == 3) pen1.setWidth(1); // nib
    else if(jColor == 1 || jColor == 6){ pen1.setCapStyle(Qt::SquareCap);}
    else if(jColor ==2)                { pen1.setCapStyle(Qt::FlatCap);}
    else                               { pen1.setCapStyle(Qt::RoundCap);}

    return pen1;
}


void MainWindow::drawWithPen(){

    updateInfo();

    QColor ncol = sizes::activeColor;
    QRegion reg(qMin(sizes::shape_x_begin,sizes::shape_x_end), qMin(sizes::shape_y_begin,sizes::shape_y_end), abs(sizes::shape_x_end-sizes::shape_x_begin), abs(sizes::shape_y_end-sizes::shape_y_begin));

    // stylus
    if(ui->comboPen->currentIndex()== 3){
        QPainter pai(&pix);
        int corners = sizes::line_width / 2;
        QPen pen(configPen(ncol));
        pen.setWidth(0);
        pai.setPen(pen);
        QBrush br(ncol);
        pai.setBrush(br);
        QPolygon poly;
        poly << QPoint(sizes::shape_x_begin + corners, sizes::shape_y_begin + corners) << QPoint(sizes::shape_x_begin - corners, sizes::shape_y_begin - corners) << QPoint(sizes::shape_x_end - corners, sizes::shape_y_end - corners) << QPoint(sizes::shape_x_end + corners, sizes::shape_y_end + corners);
        pai.drawPolygon(poly);
        pai.setClipRegion(reg);
    }
    else if(ui->comboPen->currentIndex() == 10){
        colorEraser();
    }
    else if(ui->markerButton->isChecked()){
        int difx = sizes::shape_x_begin - sizes::shape_x_end;
        int dify = sizes::shape_y_begin - sizes::shape_y_end;

        QPen pen(configPen(ncol, 3));
        for(int i=1; i < 10; ++i){
            QPainter pai(&pix);
            pai.setPen(pen);
            //pai.drawLine(sizes::shape_x_begin +(difx/(10-i)), sizes::shape_y_begin +(dify/(10-i)), sizes::shape_x_end, sizes::shape_y_end);
            pai.drawLine(sizes::shape_x_begin +(difx/(10-i)), sizes::shape_y_begin +(dify/(10-i)), sizes::shape_x_begin, sizes::shape_y_begin );

            pai.setClipRegion(reg);
            pai.end();
        }
    }
    else{
        // normal pen
        QPainter pai(&pix);
        QPen pen(configPen(ncol));
        pai.setPen(pen);
        pai.drawLine(sizes::shape_x_begin, sizes::shape_y_begin, sizes::shape_x_end, sizes::shape_y_end);
        pai.setClipRegion(reg);
    }

       sizes::shape_x_begin = sizes::shape_x_end ;
       sizes::shape_y_begin = sizes::shape_y_end ;


    wArea->setPixmap(pix);

}

void MainWindow::draw_first_point()
{

    if(sizes::activeOperation == 5){
        save_previous("Spray");
        return;
    }
    else{
        if(ui->comboPen->currentIndex()== 10){
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

    pai.setPen(pen);
    pai.drawPoint(sizes::shape_x_begin, sizes::shape_y_begin);
    pai.end();
    wArea->setPixmap(pix);
}

void MainWindow::colorEraser()
{
    int ax = sizes::shape_x_begin - (sizes::line_width/2);
    int ay = sizes::shape_y_begin - (sizes::line_width/2);
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
    if(historyList.count() > 25){
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

void MainWindow::on_lineButton_clicked()
{
    int j = ui->comboLines->currentIndex();
    if(j < 2){createLineArea(7);}
    else if( j == 2){ createLineArea(11); }

}

void MainWindow::createShapeArea()
{    
    sizes::selH =80;
    sizes::selW =80;

    shape_area = new shapeArea(wArea);  
    sizes::isShapeOn=true;
    shape_area->show();

    updateInfo();
    connect(shape_area, SIGNAL(setInfo()), this, SLOT(updateInfo()));
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
    shapeNames << "squ" << "rec" << "cir" << "ell" << "tri" << "rou" << "sta" << "aup" << "ari" << "ado" << "ale" << "aul" << "aur" << "abr" << "abl" << "crp" << "crx" << "sol";
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
    else {save_previous(tr("Connected lines"));}
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
    if(ui->curveButton->isChecked()){
        untoggle();
        wArea->setCursor(Qt::ArrowCursor);
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
        borderB->resetGeometry();
        borderR->resetGeometry();
        corner->resetGeometry();
        raiseBorders();
    }
    else{
    QRect rect = tf.mapRect(QRect(0,0,sizes::selW, sizes::selH));
    sizes::selW =rect.width();
    sizes::selH = rect.height();
    selectionRect->resetGeometry();
    QPixmap sPix = selectionRect->pixmap().transformed(tf);
    selectionRect->setPixmap(sPix);
    }
    updateInfo();
}

void MainWindow::on_autoRotationButton_clicked()
{
    if(ui->autoRotationButton->isChecked()){
        if(!sizes::isSelectionOn){
            QMessageBox::information(this, "Drawish", tr("No selection"));
            return;
        }
        ui->autoRotationButton->setIcon(QIcon(":/res/crono2.png"));
        wPreRotate = sizes::selW;
        hPreRotate = sizes::selH;
        preRotatePix = selectionRect->pixmap();

        sizes::isRotating= true;
        autoRotation();

    }else{
        ui->autoRotationButton->setIcon(QIcon(":/res/crono1.png"));

        sizes::isRotating = false;
        preAngle =0;
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
    sizes::selW =rect.width();
    sizes::selH = rect.height();
    selectionRect->resetGeometry();
    QPixmap sPix = preRotatePix.transformed(tf);
    selectionRect->setPixmap(sPix);
    ui->RotatioAngleSpin->setValue(preAngle);
    QTimer::singleShot(25, this, SLOT(autoRotation()));
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
        save_previous(tr("Scale"));
        areaSize();
        if(sizes::isCurveLineAreaOn){
            finish_lines();
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
                save_previous("Image");
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
            save_previous("Image");
            save_previous("Selection*" + QString::number(sizes::selX) + "*" + QString::number(sizes::selY) + "*" + QString::number(sizes::selW) + "*" + QString::number(sizes::selH) );
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

void MainWindow::on_actionCreate_mask_triggered()
{
    save_previous(tr("Outline"));
    QPixmap Epix = pix;
    if(sizes::isSelectionOn){
        Epix = selectionRect->pixmap().scaled(sizes::selW, sizes::selH);
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
        selectionRect->setPixmap(QPixmap::fromImage(img2));
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
  if(sizes::isCurveLineAreaOn){
      if(sizes::activeOperation == 7 || sizes::activeOperation == 11){
          finish_lines();
      }
      else if(sizes::activeOperation == 10){
          finish_curve();
      }
  }
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
        wArea->setPixmap(pix);
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
        if(sizes::isCurveLineAreaOn){
            if(sizes::activeOperation == 7 || sizes::activeOperation == 11){
                finish_lines();
            }
            else if(sizes::activeOperation == 10){
                finish_curve();
            }
        }
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
#ifdef Q_OS_LINUX
    // for lubuntu/kubuntu 25.04
    //QString ap= QCoreApplication::applicationFilePath();
    //install(ap);
    //return;
    //----------------
    // find my process
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


