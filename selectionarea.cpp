#include "selectionarea.h"
#include "geometric.h"
#include <QMouseEvent>


selectionArea::selectionArea(QWidget *parent)  : QLabel{parent}
{
    this->setCursor(Qt::SizeAllCursor);
    this->setFrameStyle(QFrame:: Box| QFrame::Raised);
    this->setMouseTracking(true);
    this->setScaledContents(true);
    this->setAlignment(Qt::AlignTop | Qt::AlignLeft);  // for text
    this->setIndent(0);
    this->setMinimumWidth(5);
    this->setMinimumHeight(5);
    resetGeometry();
}

void selectionArea::mousePressEvent(QMouseEvent *event)
{
      pressed= true;
      preX=event->pos().x();
      preY=event->pos().y();
      int gx = event->globalPosition().x();
      int gy = event->globalPosition().y();
      diffx = gx-sizes::selX;
      diffy = gy-sizes::selY;
}

void selectionArea::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y=event->pos().y();


    if(!pressed){
        if(x < 5 ){ this->setCursor(Qt::SizeHorCursor); whereExp="left";}
        else if(x > this->width()-5){this->setCursor(Qt::SizeHorCursor); whereExp="right";}
        else if( y > this->height()-5){this->setCursor(Qt::SizeVerCursor); whereExp="bottom";}
        else if(y < 5){this->setCursor(Qt::SizeVerCursor);whereExp="top"; }
        else{ this->setCursor(Qt::SizeAllCursor); whereExp="move";}
    }else{
        if(whereExp == "move"){
           sizes::selX  = event->globalPosition().x()-diffx;
           sizes::selY  = event->globalPosition().y()-diffy;
        }
        else if(whereExp== "top" && preY > y){
           sizes::selY-=2;
           sizes::selH+=2;
        }
        else if(whereExp== "top" && preY < y){
           sizes::selY++;
           sizes::selH--;           
        }
        else if(whereExp== "bottom" && preY > y){
           sizes::selH--;
        }
        else if(whereExp== "left" && preX < x){
           sizes::selX++;
           sizes::selW--;
        }
        else if(whereExp== "left" && preX > x){
            sizes::selX-=2;
            sizes::selW+=2;
        }
        else if(whereExp== "bottom" && preY < y){
           sizes::selH+=2;
        }
        else if(whereExp== "right" && preX < x){
           sizes::selW+=2;
        }
        else if(whereExp== "right" && preX > x){
           sizes::selW--;
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
    emit setInfo();

}

void selectionArea::resetGeometry()
{
    this->setGeometry(sizes::selX, sizes::selY, sizes::selW, sizes::selH);
}
