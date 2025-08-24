#include "stretchview.h"
#include "geometric.h"
#include <QMouseEvent>
#include <qpainter.h>


stretchView::stretchView(QWidget *parent, QPixmap pxm) : QLabel{parent}
{
    w = pxm.width();
    h = pxm.height();
    x = 80;
    y = 100;
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
    resetGeometry();
    if(data == "vert_top_up"){
        setCoords(0);
        copyLines(0);
    }
    else if(data == "vert_top_down"){
        setCoords(1);
        copyLines(1);
    }
    else if(data == "vert_bott_up"){
        setCoords(2);
        copyLines(2);
    }
    else if(data == "vert_bott_down"){
        setCoords(1);
        copyLines(0);
    }
    else if(data =="hor_right_right"){
        setHorCoords(0);
        copyHorlines(0);
    }
    else if(data == "hor_right_left"){
        setHorCoords(0);
        copyHorlines(1);
    }
    else if(data == "hor_left_left"){
        setHorCoords(2);
        copyHorlines(0);
    }
    else if(data == "hor_left_right"){
        setHorCoords(0);
        copyHorlines(3);
    }
    this->setPixmap(sPix);
    data = "";

}

void stretchView::resetGeometry()
{
    setGeometry(x, y, w, h);
}

void stretchView::copyLines(int toScale)
{
    QPixmap newPix(w, h);
    QPixmap newPix1 = QPixmap();
    newPix.fill(Qt::white);
    for(int ax=0; ax < sPix.width(); ++ax){
        newPix1 = sPix.copy(ax, 0, 1, sPix.height());
        int d = lineCoord[ax];
        if( d == 0 && ax > 0){ d = lineCoord[ax-1];}
        QPainter p(&newPix);
        if(toScale == 1){
            newPix1 = newPix1.scaledToHeight(h-d);
            p.drawPixmap(ax, d, newPix1);
        }
        else if(toScale == 0){
            p.drawPixmap(ax, d, newPix1);
        }
        else if(toScale == 2){
            newPix1 = newPix1.scaledToHeight(d);
            p.drawPixmap(ax, 0, newPix1);
        }
    }
    sPix = newPix;
}


void stretchView::setCoords(int bottomLine)
{
    int diffH = (abs(h -sPix.height())) * 2;
    QPixmap newPix1(x1 * 2, diffH);
    QPixmap newPix2((w-x1) * 2, diffH);

    QPainter pai(&newPix1);
    QPen pen(Qt::blue);
    pen.setWidth(2);
    pai.setPen(pen);
    if(sizes::curveStretch){
        pai.drawEllipse(0,0, newPix1.width(), newPix1.height());
    }else{
        pai.drawLine(0, int(newPix1.height()/2), x1, 0);
    }
    QPainter pai2(&newPix2);
    pai2.setPen(pen);
    if(sizes::curveStretch){
        pai2.drawEllipse(0, 0, newPix2.width(), newPix2.height());
    }else{
        pai2.drawLine(int(newPix2.width()/2), 0, newPix2.width()-1, int(newPix2.height() / 2));
    }
    // find blue pixel and add to list
    QImage newImg = newPix1.toImage();
    lineCoord.clear();
    QColor k;
    int m= newImg.width()/2;
    for(int awidth=0; awidth < m; ++awidth){
        for(int aheight=0; aheight < newImg.height(); ++aheight){
            k = newImg.pixelColor(awidth, aheight);
            if( k == Qt::blue){
                if(bottomLine== 1){
                    lineCoord.append(int(diffH/2) - aheight);
                }
                else if(bottomLine == 0){
                    lineCoord.append(aheight);
                }
                else if(bottomLine == 2){
                    lineCoord.append(h- (int(diffH/2) - aheight));
                }
                break;
            }
        }
    }
    newImg = newPix2.toImage();
    m = newImg.width()/2;
    for(int awidth= m; awidth < newImg.width(); ++awidth){
        for(int aheight=0; aheight < newImg.height(); ++aheight){
            k = newImg.pixelColor(awidth, aheight);
            if( k == Qt::blue){
                if(bottomLine == 1){
                    lineCoord.append(int(diffH/2) - aheight);
                }
                else if(bottomLine == 0){
                    lineCoord.append(aheight);
                }
                else if(bottomLine == 2){
                    lineCoord.append(h- (int(diffH/2) - aheight));
                }
                break;
            }
        }
    }
}

void stretchView::setHorCoords(int side)
{
    int diffW = (abs(w -sPix.width())) * 2;
    QPixmap newPix1(diffW , y1 * 2);
    QPixmap newPix2(diffW, (h-y1) * 2);

    QPainter pai(&newPix1);
    QPen pen(Qt::blue);
    pen.setWidth(2);
    pai.setPen(pen);
    if(sizes::curveStretch){
        pai.drawEllipse(0,0, newPix1.width(), newPix1.height());
    }else{
        pai.drawLine(0, 0, int(newPix1.width()/2), int(newPix1.height()/2) );
    }
    pai.end();
    QPainter pai2(&newPix2);
    pai2.setPen(pen);
    if(sizes::curveStretch){
        pai2.drawEllipse(0, 0, newPix2.width(), newPix2.height());
    }else{
        pai2.drawLine(int(newPix2.width()/2), int(newPix2.height()/2), 0, newPix2.height());
    }
    pai2.end();
    // find blue pixel and add to list
    QImage newImg = newPix1.toImage();
    lineCoord.clear();
    QColor k;
    int m = newImg.height()/2;
    for(int aheight=0; aheight < m; ++aheight){
        for(int awidth=0; awidth < newImg.width(); ++awidth){
            k = newImg.pixelColor(awidth, aheight);
            if( k == Qt::blue){
                if(side== 0){
                    if(sizes::curveStretch){lineCoord.append(int(diffW/2) - awidth);}
                    else{lineCoord.append(awidth);}
                }               
                else if(side == 2){
                    if(sizes::curveStretch){lineCoord.append( awidth);}
                    else{lineCoord.append( int(diffW /2) - awidth);}
                }              
                break;
            }
        }
    }
    newImg = newPix2.toImage();
    m = newImg.height()/2;
    for(int aheight= m; aheight < newImg.height(); ++aheight){
        for(int awidth=0; awidth < newImg.width(); ++awidth){
            k = newImg.pixelColor(awidth, aheight);
            if( k == Qt::blue){
                if(side == 0){
                    if(sizes::curveStretch){lineCoord.append(int(diffW/2) - awidth);}
                    else{lineCoord.append(awidth);}
                }               
                else if(side == 2){
                    if(sizes::curveStretch){lineCoord.append( awidth);}
                    else{lineCoord.append( int(diffW/2) - awidth);}
                }                
                break;
            }
        }
    }
}

void stretchView::copyHorlines(int toScale)
{
    QPixmap newPix(w, h);
    QPixmap newPix1 = QPixmap();
    newPix.fill(Qt::white);
    for(int ay=0; ay < sPix.height(); ++ay){
        newPix1 = sPix.copy(0, ay, sPix.width(), 1);
        int d = lineCoord[ay];
        if( d == 0 && ay > 0){ d = lineCoord[ay-1];}
        QPainter p(&newPix);
        if(toScale == 0){
            p.drawPixmap(d, ay, newPix1);
        }
        else if(toScale == 1){
            newPix1 = newPix1.scaledToWidth(w-d);
            p.drawPixmap(0, ay, newPix1);
        }
        else if(toScale == 3){
            newPix1 = newPix1.scaledToWidth(w-d);
            p.drawPixmap(d, ay, newPix1);

        }

    }
    sPix = newPix;
}
