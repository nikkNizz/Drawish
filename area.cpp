#include "area.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QMenu>
#include <QAction>

Area::Area(QWidget *parent)  : QLabel{parent}
{
    createMenu();
}

void Area::mousePressEvent(QMouseEvent *event)
{
    if(sizes::isRotating) return;
    if(event->button()== Qt::RightButton){
        sizes::selX = event->pos().x();
        sizes::selY = event->pos().y();
        if(sizes::zoomEdited) emit viewZoom();
        else{
           menu->exec((QPoint(event->globalPosition().x(),event->globalPosition().y())));
        }
    }
    else if(event->button()== Qt::LeftButton){
        rightPressed = false;
    if(sizes::isSelectionOn && sizes::activeOperation != 2 ){
        sizes::isSelectionOn = false;
        emit redraw();
    }
    else if(sizes::activeOperation ==1 && sizes::isSelectionOn == false){

    originX = event->pos().x();
    originY = event->pos().y();
    if(!isRubb){

        rubb = new QRubberBand(QRubberBand::Rectangle, this);
        isRubb= true;
        rubb->setGeometry(originX, originY, 0,0);
        rubb->show();

    }
    }
    else if(sizes::activeOperation == 2 && sizes::isSelectionOn == false ){ // text
        sizes::selX = event->pos().x();
        sizes::selY = event->pos().y();
        sizes::selH = 100.00;
        sizes::selW = 210.00;
        emit endRubber();   // reuse this
    }
    else if(sizes::activeOperation == 3 || sizes::activeOperation == 5){
        sizes::selX = event->pos().x();
        sizes::selY = event->pos().y();

        sizes::shape_x_begin = sizes::selX;
        sizes::shape_y_begin = sizes::selY;        
        emit drawFirstPoint();

    }

    else if(sizes::activeOperation == 4){ // fill        
        sizes::selX = event->pos().x();
        sizes::selY = event->pos().y();
        emit readyToFill();
    }
    else if(sizes::activeOperation == 6){ // picker
        sizes::selX = event->pos().x();
        sizes::selY = event->pos().y();
        emit getColor();
    }
    else if(sizes::activeOperation == 8 ){ // line shape
        if(sizes::isShapeOn == false ){
          sizes::selX = event->pos().x();
          sizes::selY = event->pos().y();
        }
        emit drawShape();
    }

    else{
        sizes::selX = event->pos().x();
        sizes::selY = event->pos().y();

    }
    }
    emit setInfo();
}

void Area::mouseMoveEvent(QMouseEvent *event)
{
   if(sizes::isRotating) return;
   if(rightPressed){
       sizes::selX = event->pos().x();
       sizes::selY = event->pos().y();
       emit setInfo();
   }
   // if left click
   else{

   if(sizes::activeOperation == 3){
        if(!sizes::mouseCorrection){ six = 6; }
        if(six == 6){
            sizes::selX = event->pos().x();
            sizes::selY = event->pos().y();
            sizes::shape_x_end = sizes::selX;
            sizes::shape_y_end = sizes::selY;
            emit penDraw();
            six =0;
        }
        six++;
    }

   else if(sizes::activeOperation == 5){
       sizes::selX = event->pos().x();
       sizes::selY = event->pos().y();
       emit sprayDraw();
   }

   else if(sizes::activeOperation==1){
    if(isRubb){
       sizes::isSelectionOn=true;
       int zx = event->pos().x();
       int zy = event->pos().y();

       int zw = abs(originX-zx);
       int zh = abs(originY- zy);
       if(zx < originX && zy  < originY){ rubb->setGeometry(QRect(zx, zy, zw, zh));}
       else if( zx > originX && zy < originY){rubb->setGeometry(QRect(originX, zy, zw, zh));}
       else if(zx < originX && zy > originY){ rubb->setGeometry(QRect(zx, originY, zw, zh));}
       else{ rubb->setGeometry(QRect(originX, originY, zw, zh).normalized()) ;}
       sizes::selW = zw;
       sizes::selH = zh;
       emit setInfo();
   }
   }
   else{
       sizes::selX = event->pos().x();
       sizes::selY = event->pos().y();
       emit setInfo();
   }
   }
}

void Area::mouseReleaseEvent(QMouseEvent *event)
{
     if(isRubb){
         sizes::selX = rubb->x();
         sizes::selY = rubb->y();
         sizes::selW = double(rubb->width());
         sizes::selH = double(rubb->height());
         isRubb= false;
         delete rubb;
         emit endRubber();
     }
   else if(sizes::activeOperation == 3 || sizes::activeOperation == 5 ){
         if(!rightPressed) emit finishDrawPen();
     }
}

void Area::createMenu()
{
    createActions();
    menu = new QMenu(this);
    menu->addAction(Paste);
    menu->addAction(Zoomy);
    menu->addSeparator();
    menu->addAction(tr("Cancel"));
}

void Area::createActions()
{
    Paste =new QAction(tr("Paste"), this);
   connect(Paste, SIGNAL(triggered()), this, SLOT(actPaste()));
    Zoomy = new QAction(tr("Show/Hide Grid"));
   connect(Zoomy, SIGNAL(triggered()), this, SLOT(actZoom()));
}

void Area::actPaste()
{
    emit setPaste();
}

void Area::actZoom()
{
    rightPressed = true;
    // create zoom label
    emit viewZoom();
}

