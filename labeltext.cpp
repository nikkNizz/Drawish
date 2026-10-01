#include "labeltext.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QTextEdit>
#include <QMessageBox>

labelText::labelText(QWidget *parent)  : QLabel{parent}
 {
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setText("𝥮");    //< ^ >
    setIndent(0);
    setMinimumWidth(12);
    setMinimumHeight(12);

    //  ------
    txe = new QTextEdit(this);
    txe->setWordWrapMode(QTextOption::WrapAnywhere);
    txe->setStyleSheet("background:transparent; ") ;//border:1px solid");
    txe->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    txe->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(txe, &QTextEdit::textChanged, this, &labelText::expandTextArea);

 }

 void labelText::mousePressEvent(QMouseEvent *event)
 {
     if(event->button()== Qt::RightButton){
        int q= QMessageBox::question(this, "Drawish", tr("Load the previous text?"), QMessageBox::Yes | QMessageBox::No);
         if(q == QMessageBox::Yes){
            txe->setText(sizes::savedTxt);
         }
         return;
     }

     mouseX_old = event->globalPosition().x();
     mouseY_old = event->globalPosition().y();

 }

 void labelText::mouseMoveEvent(QMouseEvent *event)
 {
     int px = event->globalPosition().x();
     int py = event->globalPosition().y();
     int diffx = px - mouseX_old;
     int diffy = py - mouseY_old;
     if(event->pos().x() < (12*sizes::zoomLevel)){  // mouse left border= move
        sizes::selX = sizes::selX + diffx;
        sizes::selY = sizes::selY + diffy;
        setCursor(Qt::SizeAllCursor);
     }else{
         if(event->pos().y() < 7){ // up
             setCursor(Qt::SizeVerCursor);
             sizes::selY += diffy;
             sizes::selH -= diffy;
         }
         else if(event->pos().y() > (sizes::selH-7)){
             setCursor(Qt::SizeVerCursor);
             sizes::selH += diffy;
         }
         else if(event->pos().x() > (sizes::selW-7)){
             setCursor(Qt::SizeHorCursor);
             sizes::selW += diffx;
         }
     }
     resetGeometry();
     mouseX_old = px;
     mouseY_old = py;

 }


 void labelText::resetGeometry()
 {
     if(sizes::selX < 0) sizes::selX =0;
     setGeometry(sizes::selX, sizes::selY, sizes::selW, sizes::selH);
     int sc1 = 0;

     if(sizes::zoomLevel > 1.00){
         sc1 = 3 * sizes::zoomLevel;
     }
     else if(sizes::zoomLevel < 1.00){
         sc1 = -3 * sizes::zoomLevel;
     }
     txe->setGeometry((12 * sizes::zoomLevel)+sc1, (6 * sizes::zoomLevel)+ sc1, sizes::selW -(18*sizes::zoomLevel), sizes::selH -(12*sizes::zoomLevel));
 }

 void labelText::formatText()
 {
     QFont tFont(ffont);
     tFont.setBold(fbold);
     tFont.setItalic(fitalic);
     tFont.setUnderline(funderline);
     //tFont.setStyleStrategy(QFont::NoAntialias);

     int sizeText = fsize * sizes::zoomLevel;
     if(sizeText < 4){sizeText = 4;}
     tFont.setPixelSize(sizeText);
     ffont = tFont;
     expandTextArea();

     txe->setFont(tFont);
     txe->setTextColor(sizes::activeColor);
     txe->setText(txe->toPlainText());

     if(falignCenter){
         txe->selectAll();
         txe->setAlignment(Qt::AlignCenter);
     }else{
         txe->setAlignment(Qt::AlignLeft);
     }

 }

 void labelText::clearText()
 {
     txe->clear();
 }

 void labelText::saveText()
 {
     sizes::savedTxt = txe->toPlainText();
 }

 void labelText::expandTextArea()
 {
     QFontMetrics fm(ffont);
     QString txt = txe->toPlainText();
     int n = txt.count("\n") + 2;
     sizes::selH = (n * fm.height());

     if(sizes::selH < 100) sizes::selH = 100;

     resetGeometry();
 }




