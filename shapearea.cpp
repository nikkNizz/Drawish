#include "shapearea.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>


shapeArea::shapeArea(QWidget *parent)  : QLabel{parent}
{
    this->setCursor(Qt::SizeAllCursor);
    this->setFrameStyle(QFrame:: Box| QFrame::Raised);
    this->setMouseTracking(true);
    resetGeometry();
}

void shapeArea::mousePressEvent(QMouseEvent *event)
{
    pressed= true;
    preX=event->pos().x();
    preY=event->pos().y();
    int gx = event->globalPosition().x();
    int gy = event->globalPosition().y();
    diffx = gx-sizes::selX;
    diffy = gy-sizes::selY;
}

void shapeArea::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();


    if(!pressed){
        if(x < 5 ){ this->setCursor(Qt::OpenHandCursor); whereExp="left";}
        else if(x > this->width()-5){this->setCursor(Qt::OpenHandCursor); whereExp="right";}
        else if( y > this->height()-5){this->setCursor(Qt::OpenHandCursor); whereExp="bottom";}
        else if(y < 5){this->setCursor(Qt::OpenHandCursor);whereExp="top"; }
        else{ this->setCursor(Qt::SizeAllCursor); whereExp="move";}
    }else{
        if(whereExp == "move"){
           sizes::selX  = event->globalPosition().x()-diffx;
           sizes::selY  = event->globalPosition().y()-diffy;
           resetGeometry();
        }
        else if(whereExp== "top"  ){
           int incY = sizes::selY - (event->globalPosition().y()-diffy);
           sizes::selH = sizes::selH + incY;
           sizes::selY = event->globalPosition().y()-diffy;
           resetGeometry();
           if(sizes::activeOperation == 7){
              sizes::shape_y_begin += incY;
              redrawLine(x, y, sizes::shape_x_begin, sizes::shape_y_begin);
           }else{
               drawSomething();
           }
        }

        else if(whereExp== "bottom" ){
           sizes::selH = y;
           resetGeometry();
           if(sizes::activeOperation == 7){
               redrawLine(x, y, sizes::shape_x_begin, sizes::shape_y_begin);
            }else{
               drawSomething();
           }
        }

        else if(whereExp== "left" ){
           int incX = sizes::selX - (event->globalPosition().x()-diffx);
           sizes::selW = sizes::selW + incX;
           sizes::selX = event->globalPosition().x()-diffx;

           resetGeometry();
           if(sizes::activeOperation == 7){
              sizes::shape_x_begin += incX;
              redrawLine(x, y, sizes::shape_x_begin, sizes::shape_y_begin);
            }else{
               drawSomething();
           }

        }
        else if(whereExp== "right" ){
           sizes::selW = x;
           resetGeometry();
           if(sizes::activeOperation == 7){
              redrawLine(x, y, sizes::shape_x_begin, sizes::shape_y_begin);
           }else{
               drawSomething();
           }
        }
    }
    preX=x; preY=y;
}

void shapeArea::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = false;
    sizes::shape_x_end = preX;
    sizes::shape_y_end = preY;
    preX=0;
    preY=0;
    emit setInfo();
}

void shapeArea::resetGeometry()
{
    this->setGeometry(sizes::selX, sizes::selY, sizes::selW, sizes::selH);
}

void shapeArea::redrawLine(int to_x, int to_y, int from_x,int from_y )
{
    if(this->width() < 5 || this->height() < 5)return;
    QPixmap npix(sizes::selW, sizes::selH);
    npix.fill(QColor(255,255,255,0));
    QPainter p(&npix);
    QPen pen2(sizes::activeColor);
    pen2.setWidth(sizes::line_width);
    p.setPen(pen2);
    p.drawLine(from_x, from_y, to_x, to_y);
    this->setPixmap(npix);
}

void shapeArea::drawSomething()
{
    if(this->width() < 5 || this->height() < 5)return;
    int ww = this->width();
    int hh = this->height();
    if(sizes::activeOperation ==8){
        QPixmap npix(sizes::selW, sizes::selH);
        npix.fill(QColor(255,255,255,0));
        QPainter p(&npix);
        QPen pen2(sizes::activeColor);
        pen2.setWidth(sizes::line_width);
        p.setPen(pen2);
        QBrush br;
        br.setColor(sizes::activeColor);
        br.setStyle(Qt::SolidPattern);
        if(sizes::toFill){ p.setBrush(br);}

        if(sizes::activeShape == "squ"){
            int side = (sizes::selW < sizes::selH) ? sizes::selW : sizes::selH;
            p.drawRect(side*0.1, side*0.1, side*0.8, side*0.8);
        }
        else if(sizes::activeShape == "rec"){
            p.drawRect(ww*0.1, hh*0.1, ww*0.8, hh*0.8);
        }
        else if(sizes::activeShape == "cir"){
            int side = (sizes::selW < sizes::selH) ? sizes::selW : sizes::selH;
            p.drawEllipse(side*0.1,side*0.1, side*0.8, side*0.8);
        }
        else if(sizes::activeShape == "ell"){
            p.drawEllipse(ww*0.1, hh*0.1, ww*0.8, hh*0.8);
        }
        else if(sizes::activeShape == "tri"){
            QPolygon poly;
            poly << QPoint(ww*0.1, hh*0.9) << QPoint(ww*0.9, hh*0.9) << QPoint(ww*0.5, hh*0.1) << QPoint(ww*0.1, hh*0.9);
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "rou"){
            p.drawRoundedRect(ww*0.1, hh*0.1, ww*0.8, hh*0.8, 10,10);
        }
        else if(sizes::activeShape == "sta"){
            int x1 = ww/2;
            int y1 = 0;
            int x2 = ww/2.63;
            int y2 = hh/2.63;
            int x3 = 0;
            int y3 = y2;
            int x4 = ww/3.30;
            int y4 = hh/1.62;
            int x5 = ww/5.26;
            int y5 = hh;
            int x6 = x1;
            int y6 = hh/1.282;
            int x7 = ww -(ww/5.26);
            int y7 = y5;
            int x8 = ww - (ww/3.30);
            int y8 = y4;
            int x9 = ww;
            int y9 = y2;
            int x10 = ww - (ww/2.63);
            int y10 = y2;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) << QPoint(x9, y9) << QPoint(x10, y10);
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "aup"){
            int x1 = ww*0.33;
            int y1 = hh*0.9;
            int x2 = ww*0.33;
            int y2 = hh*0.25;
            int x3 = ww*0.1;
            int y3 = hh*0.25;
            int x4 = ww*0.5;
            int y4 = hh*0.1;
            int x5 = ww*0.9;
            int y5 = hh*0.25;
            int x6 = ww*0.66;
            int y6 = hh*0.25;
            int x7 = ww *0.66;
            int y7 = hh*0.9;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "ari"){
            int x1 = ww*0.1;
            int y1 = hh*0.33;
            int x2 = ww*0.66;//
            int y2 = hh*0.33;
            int x3 = ww*0.66;//
            int y3 = hh*0.1;
            int x4 = ww*0.9;
            int y4 = hh*0.5;
            int x5 = ww*0.66;//
            int y5 = hh*0.9;
            int x6 = ww*0.66;//
            int y6 = hh*0.66;
            int x7 = ww *0.1;
            int y7 = hh*0.66;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "ado"){
            int x1 = ww*0.33;
            int y1 = hh*0.1;
            int x2 = ww*0.33;
            int y2 = hh*0.66;
            int x3 = ww*0.1;
            int y3 = hh*0.66;
            int x4 = ww*0.5;
            int y4 = hh*0.9;
            int x5 = ww*0.9;
            int y5 = hh*0.66;
            int x6 = ww*0.66;
            int y6 = hh*0.66;
            int x7 = ww *0.66;
            int y7 = hh*0.1;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "ale"){
            int x1 = ww*0.1;
            int y1 = hh*0.5;
            int x2 = ww*0.33;
            int y2 = hh*0.1;
            int x3 = ww*0.33;
            int y3 = hh*0.33;
            int x4 = ww*0.9;
            int y4 = hh*0.33;
            int x5 = ww*0.9;
            int y5 = hh*0.66;
            int x6 = ww*0.33;
            int y6 = hh*0.66;
            int x7 = ww *0.33;
            int y7 = hh*0.9;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);

        }
        else if(sizes::activeShape == "aul"){
            int x1 = ww*0.1;
            int y1 = hh*0.1;
            int x2 = ww*0.5;
            int y2 = hh*0.10;
            int x3 = ww*0.36; //corner
            int y3 = hh*0.25; // ..
            int x4 = ww*0.9;
            int y4 = hh*0.75;
            int x5 = ww*0.75;
            int y5 = hh*0.9;
            int x6 = ww*0.25; //corner
            int y6 = hh*0.36; //..
            int x7 = ww *0.1;
            int y7 = hh*0.5;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "aur"){
            int x1 = ww*0.9;
            int y1 = hh*0.1;
            int x2 = ww*0.9;
            int y2 = hh*0.5;
            int x3 = ww*0.75; //corner
            int y3 = hh*0.36; // ..
            int x4 = ww*0.25;
            int y4 = hh*0.9;
            int x5 = ww*0.1;
            int y5 = hh*0.75;
            int x6 = ww*0.64; //corner
            int y6 = hh*0.25; //..
            int x7 = ww *0.5;
            int y7 = hh*0.1;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);

        }
        else if(sizes::activeShape == "abr"){
            int x1 = ww*0.9;
            int y1 = hh*0.9;
            int x2 = ww*0.5;
            int y2 = hh*0.9;
            int x3 = ww*0.64; //corner
            int y3 = hh*0.75; // ..
            int x4 = ww*0.1;
            int y4 = hh*0.25;
            int x5 = ww*0.25;
            int y5 = hh*0.1;
            int x6 = ww*0.75; //corner
            int y6 = hh*0.64; //..
            int x7 = ww *0.9;
            int y7 = hh*0.5;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "abl"){
            int x1 = ww*0.1;
            int y1 = hh*0.9;
            int x2 = ww*0.1;
            int y2 = hh*0.5;
            int x3 = ww*0.25; //corner
            int y3 = hh*0.64; // ..
            int x4 = ww*0.75;
            int y4 = hh*0.1;
            int x5 = ww*0.9;
            int y5 = hh*0.25;
            int x6 = ww*0.36; //corner
            int y6 = hh*0.75; //..
            int x7 = ww *0.5;
            int y7 = hh*0.9;
            int x8 = x1;
            int y8 = y1;
            QPolygon poly;
            poly << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4) << QPoint(x5, y5) << QPoint(x6, y6) << QPoint(x7, y7) << QPoint(x8, y8) ;
            p.drawPolygon(poly);
        }
        this->setPixmap(npix);
    }
}