#include "stretchview.h"
#include <QMouseEvent>
#include <QMessageBox>

stretchView::stretchView(QWidget *parent, QPixmap pxm) : QLabel{parent}
{
    w = pxm.width(); h = pxm.height();
    x = 80; y = 100;
    this->setFrameStyle(QFrame:: Box);
    this->setCursor(Qt::SizeAllCursor);
    this->setMouseTracking(true);
    this->setMinimumWidth(10);
    this->setMinimumHeight(10);
    this->setPixmap(pxm);
    resetGeometry();
    sPix = pxm;

}

void stretchView::mousePressEvent(QMouseEvent *event)
{
    pressed= true;
    preX=event->pos().x();
    preY=event->pos().y();
    int gx = event->globalPosition().x();
    int gy = event->globalPosition().y();
    diffx = gx-x;
    diffy = gy-y;
}

void stretchView::mouseMoveEvent(QMouseEvent *event)
{
    x1 = event->pos().x();
    y1 = event->pos().y();
    if(!pressed){
        if(x1 < 5 ){ this->setCursor(Qt::SizeHorCursor); whereExp="left";}
        else if(x1 > this->width()-5){this->setCursor(Qt::SizeHorCursor); whereExp="right";}
        else if( y1 > this->height()-5){this->setCursor(Qt::SizeVerCursor); whereExp="bottom";}
        else if(y1 < 5){this->setCursor(Qt::SizeVerCursor);whereExp="top"; }
        else{ this->setCursor(Qt::SizeAllCursor); whereExp="move";}
    }else{
        if(whereExp == "move"){
           x  = event->globalPosition().x()-diffx;
           y  = event->globalPosition().y()-diffy;
        }
        else if(whereExp== "top" && preY > y1){
           y--;
           h++;
           data = "vert_top_up";
        }
        else if(whereExp== "top" && preY < y1){
           y++;
           h--;
           data = "vert_top_down";
        }
        else if(whereExp== "bottom" && preY > y1){
           h--;
           data = "vert_bott_up";
        }
        else if(whereExp== "left" && preX < x1){
           x++;
           w--;
           data = "hor_left_right";
        }
        else if(whereExp== "left" && preX > x1){
            x--;
            w++;
            data = "hor_left_left";
        }
        else if(whereExp== "bottom" && preY < y1){
           h++;
           data = "vert_bott_down";
        }
        else if(whereExp== "right" && preX < x1){
          w++;
          data = "hor_right_right";
        }
        else if(whereExp== "right" && preX > x1){
           w--;
           data = "hor_right_left";

        }
        resetGeometry();
    }
    preX = x1; preY = y1;
}

void stretchView::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = false;
    x1 = event->pos().x();
    y1 = event->pos().y();
    preX=0;
    preY=0;
    deform();
    data ="";
}

void stretchView::resetGeometry()
{
    setGeometry(x, y, w, h);
}

void stretchView::deform()
{
    QPixmap img2(w,h);
    img2.fill(Qt::white);
    QImage pix1 = sPix.toImage();
    QImage pix2 = img2.toImage();
    if(data.startsWith( "vert" )){
        int pixH = sPix.height();
        double diffHeight = h - sPix.height();
        int pixelsToAdd = 0;
        QColor k;
        double step = diffHeight / double(x1);
        for(int iLeft = 0; iLeft < x1; ++iLeft){
            pixelsToAdd = iLeft * step;
            for(int iTop =0; iTop < pixH; ++iTop){
                k = pix1.pixelColor(iLeft, iTop);
                if ( k == Qt::transparent){k = Qt::white;}
                if(data == "vert_bott_down" || data == "vert_bott_up" ){
                  pix2.setPixelColor(iLeft, pixelsToAdd + iTop, k);
                }
                else{
                  pix2.setPixelColor(iLeft, diffHeight -pixelsToAdd + iTop, k);
                }
            }
        }
        for(int iLeft = x1; iLeft < sPix.width(); ++iLeft){
            step = diffHeight / double((sPix.width() - x1));
            pixelsToAdd = (iLeft-x1) * step;
            for(int iTop =0; iTop < pixH; ++iTop){
                k = pix1.pixelColor(iLeft, iTop);
                if ( k == Qt::transparent){k = Qt::white;}
                if(data == "vert_bott_down" || data == "vert_bott_up" ){
                   pix2.setPixelColor(iLeft, iTop + diffHeight - pixelsToAdd , k);
                }
                else{
                   pix2.setPixelColor(iLeft, iTop + pixelsToAdd , k);
                }
            }
        }
        sPix = QPixmap::fromImage(pix2);
        this->setPixmap(sPix);
    }    

    else if(data.startsWith( "hor" )){
        int pixW = sPix.width();
        double diffWidth = w - sPix.width();
        int pixelsToAdd = 0;
        QColor k;
        double step = diffWidth / double(y1);
        for(int jTop = 0; jTop < y1; ++jTop){
            pixelsToAdd = jTop * step;
            for(int iLeft =0; iLeft < pixW; ++iLeft){
                k = pix1.pixelColor( iLeft, jTop);
                if ( k == Qt::transparent){k = Qt::white;}
                if(data == "hor_left_left" || data == "hor_left_right"){
                    pix2.setPixelColor(iLeft + diffWidth - pixelsToAdd,  jTop, k);
                }
                else{
                    pix2.setPixelColor(iLeft + pixelsToAdd,  jTop, k);
                }
            }
        }
        for(int jTop = y1; jTop < sPix.height(); ++jTop){
            step = diffWidth / double((sPix.height() - y1));
            pixelsToAdd = (jTop-y1) * step;
            for(int iLeft =0; iLeft < pixW; ++iLeft){
                k = pix1.pixelColor(iLeft, jTop);
                if ( k == Qt::transparent){k = Qt::white;}
                if(data == "hor_left_left" || data == "hor_left_right"){
                    pix2.setPixelColor( iLeft + pixelsToAdd , jTop,  k);
                }
                else{
                    pix2.setPixelColor( iLeft + diffWidth - pixelsToAdd , jTop,  k);
                }
            }
        }
        sPix = QPixmap::fromImage(pix2);
        this->setPixmap(sPix);
    }

}
