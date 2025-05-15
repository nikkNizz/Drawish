#include "shapearea.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>

shapeArea::shapeArea(QWidget *parent)  : QLabel{parent}
{
    this->setCursor(Qt::SizeAllCursor);
    this->setFrameStyle(QFrame:: Box| QFrame::Raised);
    this->setMouseTracking(true);
    resetGeometry();
    drawSomething();
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
    emit setInfo();
}

void shapeArea::mouseMoveEvent(QMouseEvent *event)
{
    int x = event->pos().x();
    int y = event->pos().y();

    if(!pressed){
        if(x < 8 ){ this->setCursor(Qt::OpenHandCursor); whereExp="left";}
        else if(x > this->width()-8){this->setCursor(Qt::OpenHandCursor); whereExp="right";}
        else if( y > this->height()-8){this->setCursor(Qt::OpenHandCursor); whereExp="bottom";}
        else if(y < 8){this->setCursor(Qt::OpenHandCursor);whereExp="top"; }
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
           drawSomething();

        }

        else if(whereExp== "bottom" ){
           sizes::selH = y;
           resetGeometry();
           drawSomething();

        }

        else if(whereExp== "left" ){
           int incX = sizes::selX - (event->globalPosition().x()-diffx);
           sizes::selW = sizes::selW + incX;
           sizes::selX = event->globalPosition().x()-diffx;

           resetGeometry();
           drawSomething();


        }
        else if(whereExp== "right" ){
           sizes::selW = x;
           resetGeometry();
           drawSomething();

        }
    }
    emit setInfo();
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

QBrush shapeArea::bru()
{
    QBrush br;
    br.setColor(sizes::activeColor);
    if(sizes::FillStyle == 0){
        br.setStyle(Qt::NoBrush);
    }
    else if(sizes::FillStyle == 1) { br.setStyle(Qt::SolidPattern); }
    else if(sizes::FillStyle == 2) { br.setStyle(Qt::Dense1Pattern); }
    else if(sizes::FillStyle == 3) { br.setStyle(Qt::Dense2Pattern); }
    else if(sizes::FillStyle == 4) { br.setStyle(Qt::Dense3Pattern); }
    else if(sizes::FillStyle == 5) { br.setStyle(Qt::Dense4Pattern); }
    else if(sizes::FillStyle == 6) { br.setStyle(Qt::Dense5Pattern); }
    else if(sizes::FillStyle == 7) { br.setStyle(Qt::Dense6Pattern); }
    else if(sizes::FillStyle == 8) { br.setStyle(Qt::Dense7Pattern); }
    else if(sizes::FillStyle == 9) { br.setStyle(Qt::HorPattern); }
    else if(sizes::FillStyle == 10) { br.setStyle(Qt::VerPattern); }
    else if(sizes::FillStyle == 11) { br.setStyle(Qt::CrossPattern); }
    else if(sizes::FillStyle == 12) { br.setStyle(Qt::BDiagPattern); }
    else if(sizes::FillStyle == 13) { br.setStyle(Qt::FDiagPattern); }
    else if(sizes::FillStyle == 14) { br.setStyle(Qt::DiagCrossPattern); }
    return br;
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
        pen2.setCapStyle(Qt::RoundCap);
        pen2.setJoinStyle(Qt::MiterJoin);
        p.setPen(pen2);
        p.setBrush(bru());

        if(sizes::activeShape == "squ"){
            int side = (sizes::selW < sizes::selH) ? sizes::selW : sizes::selH;
            p.drawRect(sizes::line_width+2, sizes::line_width+2, side-14-sizes::line_width, side-14-sizes::line_width);
        }
        else if(sizes::activeShape == "rec"){
            p.drawRect(sizes::line_width+2, sizes::line_width+2, ww-14-sizes::line_width, hh-14-sizes::line_width);
        }
        else if(sizes::activeShape == "cir"){
            int side = (sizes::selW < sizes::selH) ? sizes::selW : sizes::selH;
            p.drawEllipse(sizes::line_width+2, sizes::line_width+2, side-14-sizes::line_width, side-14-sizes::line_width);
        }
        else if(sizes::activeShape == "ell"){
            p.drawEllipse(sizes::line_width+2, sizes::line_width+2, ww-14-sizes::line_width, hh-14-sizes::line_width);
        }
        else if(sizes::activeShape == "tri"){
            QPolygon poly;
            poly << QPoint(ww*0.1, hh*0.9) << QPoint(ww*0.9, hh*0.9) << QPoint(ww*0.5, hh*0.1) << QPoint(ww*0.1, hh*0.9);
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "crp"){
             QPolygon poly;
            poly << QPoint(ww*0.5, hh*0.1) << QPoint(ww*0.5, hh*0.9) << QPoint(ww*0.5, hh*0.5) << QPoint(ww*0.1, hh*0.5) << QPoint(ww * 0.9, hh *0.5) << QPoint(ww * 0.5, hh * 0.5);
             p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "crx"){
            QPolygon poly;
            poly << QPoint(ww*0.1, hh*0.1) << QPoint(ww*0.9, hh*0.9) << QPoint(ww*0.5, hh*0.5) << QPoint(ww*0.9, hh*0.1) << QPoint(ww * 0.1, hh *0.9) << QPoint(ww * 0.5, hh * 0.5);
            p.drawPolygon(poly);
        }
        else if(sizes::activeShape == "rou"){
            p.drawRoundedRect(sizes::line_width+2, sizes::line_width+2, ww-14-sizes::line_width, hh-14-sizes::line_width, 12,12);
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
            int x1 = ww*0.34;
            int y1 = hh*0.9;
            int x2 = ww*0.34;
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
            int y1 = hh*0.34;
            int x2 = ww*0.66;//
            int y2 = hh*0.34;
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
            int x1 = ww*0.34;
            int y1 = hh*0.1;
            int x2 = ww*0.34;
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
            int x2 = ww*0.34;
            int y2 = hh*0.1;
            int x3 = ww*0.34;
            int y3 = hh*0.34;
            int x4 = ww*0.9;
            int y4 = hh*0.34;
            int x5 = ww*0.9;
            int y5 = hh*0.66;
            int x6 = ww*0.34;
            int y6 = hh*0.66;
            int x7 = ww *0.34;
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

        else if(sizes::activeShape == "sol"){

            QPoint p1(sizes::line_width +2, hh-sizes::line_width-2);
            QPoint p2(sizes::line_width +2, hh *0.3);
            QPoint p3(ww*0.3, sizes::line_width+2);
            QPoint p4(ww-sizes::line_width-2, sizes::line_width+2);
            QPoint p5(ww -sizes::line_width-2 , hh*0.7);
            QPoint p6(ww*0.7, hh -sizes::line_width-2);
            QPoint p7(ww*0.3, hh*0.7);
            QPoint p8(ww*0.7, hh*0.3);
            QList<QLine> Lines;
            Lines << QLine(p1,p2) << QLine(p2,p3) << QLine(p3,p4)
                  << QLine(p4,p5) << QLine(p5,p6) << QLine(p6,p1) << QLine(p8,p4)
                  << QLine(p1,p7) << QLine(p7,p3) << QLine(p2,p8) << QLine(p8,p6) << QLine(p7,p5);
            p.drawLines(Lines);

        }
        this->setPixmap(npix);
    }
}
