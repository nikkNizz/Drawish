#include "selectionarea.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QMenu>
#include <QAction>


selectionArea::selectionArea(QWidget *parent)  : QLabel{parent}
{
    setCursor(Qt::SizeAllCursor);
    setFrameStyle(QFrame::Box | QFrame::Raised);
    setMouseTracking(true);
    setScaledContents(true);
    setAlignment(Qt::AlignTop | Qt::AlignLeft);  // for text
    setIndent(0);
    setMinimumWidth(5);
    setMinimumHeight(5);
    resetGeometry();
    createActions();
    menu =new QMenu(this);
    menu->addAction(copy);
    menu->addAction(tr("Cancel"));

}

void selectionArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button()== Qt::RightButton){
        menu->exec((QPoint(event->globalPosition().x(),event->globalPosition().y())));
    }
    else{
    pressed= true;
      preX=event->pos().x();
      preY=event->pos().y();
      int gx = event->globalPosition().x();
      int gy = event->globalPosition().y();
      diffx = gx-sizes::selX;
      diffy = gy-sizes::selY;
    }
}

void selectionArea::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    double ratio = sizes::selW / sizes::selH;

    if(!pressed){
        if(x < 8 ){ this->setCursor(Qt::SizeHorCursor); whereExp="left";}
        else if(x > this->width()-8){this->setCursor(Qt::SizeHorCursor); whereExp="right";}
        else if( y > this->height()-8){this->setCursor(Qt::SizeVerCursor); whereExp="bottom";}
        else if(y < 8){this->setCursor(Qt::SizeVerCursor);whereExp="top"; }
        else{ this->setCursor(Qt::SizeAllCursor); whereExp="move"; }
    }else{
        if(whereExp == "move"){
           sizes::selX  = event->globalPosition().x()-diffx;
           sizes::selY  = event->globalPosition().y()-diffy;
           setFrameStyle(QFrame::NoFrame);
        }
        else if(whereExp== "top" && preY > y){
           sizes::selY-=2;
           sizes::selH+=2.00;
           if(sizes::keepSelAspectRatio){
               sizes::selW = sizes::selW + 2 * ratio;
           }
        }
        else if(whereExp== "top" && preY < y){
           sizes::selY++;
           sizes::selH--;
           if(sizes::keepSelAspectRatio){
               sizes::selW = sizes::selW - ratio;
           }
        }
        else if(whereExp== "bottom" && preY > y){
           sizes::selH--;
            if(sizes::keepSelAspectRatio){
                sizes::selW = sizes::selW - ratio;
            }
        }
        else if(whereExp== "left" && preX < x){
           sizes::selX++;
           sizes::selW--;
           if(sizes::keepSelAspectRatio){
               sizes::selH = sizes::selH - (1/ratio);
           }
        }
        else if(whereExp== "left" && preX > x){
            sizes::selX-=2;
            sizes::selW+=2.00;
            if(sizes::keepSelAspectRatio){
                sizes::selH = sizes::selH + 2* (1/ ratio);
            }
        }
        else if(whereExp== "bottom" && preY < y){
           sizes::selH+=2.00;
            if(sizes::keepSelAspectRatio){
                sizes::selW = sizes::selW + 2* ratio;
            }
        }
        else if(whereExp== "right" && preX < x){
           sizes::selW+=2.00;
            if(sizes::keepSelAspectRatio){
                sizes::selH = sizes::selH + 2* (1/ratio);
            }

        }
        else if(whereExp== "right" && preX > x){
           sizes::selW--;
            if(sizes::keepSelAspectRatio){
                sizes::selH = sizes::selH - (1/ratio);
            }
        }
        resetGeometry();
    }
    preX=x; preY=y;
}

void selectionArea::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = false;
    preX=0;
    preY=0;
    setFrameStyle(QFrame::Box | QFrame::Raised);
    emit setInfo();
}

void selectionArea::resetGeometry()
{
    setGeometry(sizes::selX, sizes::selY, sizes::selW, sizes::selH);
}

void selectionArea::createActions()
{
    copy =new QAction(tr("Copy"), this);
    connect(copy, SIGNAL(triggered()), this, SLOT(copyClip()));
}

void selectionArea::copyClip()
{
    emit setCopy();
}

