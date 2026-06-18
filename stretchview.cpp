#include "stretchview.h"
#include "geometric.h"
#include <QMouseEvent>
#include <qpainter.h>

stretchView::stretchView(QWidget *parent, QPixmap pxm) : QLabel{parent}
{
    w = pxm.width();
    h = pxm.height();
    x = 100;
    y = 100;
    this->setFrameStyle(QFrame:: Box);
    this->setCursor(Qt::SizeAllCursor);
    this->setMouseTracking(true);
    this->setMinimumWidth(20);
    this->setMinimumHeight(20);
    this->setPixmap(pxm);
    resetGeometry();
    sPix = pxm;


}

void stretchView::mousePressEvent(QMouseEvent *event)
{
    pressed= true;
    preX=event->pos().x();
    preY=event->pos().y();


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

    }else{
         if(whereExp== "top" && preY > y1){
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

    if(sizes::curveStretch==0){
        stretch();
    }
    else{
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
    }

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
     QPainter p(&newPix);
    for(int ax=0; ax < sPix.width(); ++ax){
        newPix1 = sPix.copy(ax, 0, 1, sPix.height());
        int d = lineCoord[ax];
        if( d == 0 && ax > 0){ d = lineCoord[ax-1];}
        if(ax > 0 && ax < sPix.width() -1){
            int coerent =abs(d -lineCoord[ax-1] );
            if(coerent > 1){ d = (lineCoord[ax-1] + lineCoord[ax+1]) / 2;}
        }

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

        pai.drawEllipse(0,0, newPix1.width(), newPix1.height());

    pai.end();
    QPainter pai2(&newPix2);
    pai2.setPen(pen);

        pai2.drawEllipse(0, 0, newPix2.width(), newPix2.height());

    pai2.end();
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
                    lineCoord.append(int(diffH) - aheight);
                }
                else if(bottomLine == 0){
                    lineCoord.append(aheight);
                }
                else if(bottomLine == 2){
                    lineCoord.append(h- (int(diffH) - aheight));
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
                    lineCoord.append(int(diffH) - aheight);
                }
                else if(bottomLine == 0){
                    lineCoord.append(aheight);

                }
                else if(bottomLine == 2){
                    lineCoord.append(h- (int(diffH) - aheight));
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

        pai.drawEllipse(0,0, newPix1.width(), newPix1.height());

    pai.end();
    QPainter pai2(&newPix2);
    pai2.setPen(pen);

        pai2.drawEllipse(0, 0, newPix2.width(), newPix2.height());

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
                    lineCoord.append(int(diffW/2) - awidth);
                }               
                else if(side == 2){
                    lineCoord.append( awidth);
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
                    lineCoord.append(int(diffW/2) - awidth);
                }               
                else if(side == 2){
                    lineCoord.append( awidth);
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
    QPainter p(&newPix);
    for(int ay=0; ay < sPix.height(); ++ay){
        newPix1 = sPix.copy(0, ay, sPix.width(), 1);
        int d = lineCoord[ay];
        if( d == 0 && ay > 0){ d = lineCoord[ay-1];}

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
    p.end();
    sPix = newPix;
}

void stretchView::stretch()
{
    int diffDim = 0;
    double perc= 0.1;
    QPixmap pix1, pix2, shearedPix;
    QTransform transform, transform2;
    //-----------------
    if(data.startsWith("vert")){
        pix1 = sPix.copy(0, 0, x1, sPix.height());
        pix2 = sPix.copy(x1, 0, sPix.width() - x1, sPix.height());
        diffDim = abs(y1 - sPix.height());
        perc = double(diffDim) / double(sPix.height());
        perc = perc / 4;

    }else{
        pix1 = sPix.copy(0, 0, sPix.width(), y1);
        pix2 = sPix.copy(0, y1, sPix.width(), sPix.height()-y1 );
        diffDim = abs(x1 - sPix.width());
        perc = double(diffDim) / double(sPix.width());
        perc = perc / 4;

    }
    //----------------
    if(data == "vert_top_up"){        
        transform.shear(0.0, -perc);
        QRect rect1 = transform.mapRect(pix1.rect());
        transform2.shear(0.0, perc);
        QRect rect2 = transform2.mapRect(pix2.rect());
        shearedPix = QPixmap(rect1.width() +rect2.width(), qMax(rect1.height(), rect2.height()));
        shearedPix.fill(Qt::transparent);
        QPainter p(&shearedPix);
        p.drawPixmap(0,0, pix1.transformed(transform));
        p.drawPixmap(x1, 0, pix2.transformed(transform2));
        p.end();
    }

    else if(data == "vert_bott_down"){
        transform.shear(0.0, perc);
        QRect rect1 = transform.mapRect(pix1.rect());
        transform2.shear(0.0, -perc);
        QRect rect2 = transform2.mapRect(pix2.rect());
        shearedPix = QPixmap(rect1.width() +rect2.width(), qMax(rect1.height(), rect2.height()));
        shearedPix.fill(Qt::transparent);
        QPainter p(&shearedPix);

        if(rect1.height() > rect2.height()){
            p.drawPixmap(0,0, pix1.transformed(transform));
            p.drawPixmap(x1, shearedPix.height()- rect2.height(), pix2.transformed(transform2));
            p.end();
        }else{
            int dy = shearedPix.height()- rect1.height();
            p.drawPixmap(0,dy, pix1.transformed(transform));
            p.drawPixmap(x1, 0, pix2.transformed(transform2));
            p.end();
        }
    }
    else if(data =="hor_right_right"){
        transform.shear( perc, 0.0);
        QRect rect1 = transform.mapRect(pix1.rect());
        transform2.shear(-perc, 0.0);
        QRect rect2 = transform2.mapRect(pix2.rect());
        shearedPix = QPixmap(qMax(rect1.width() ,rect2.width()), (rect1.height() + rect2.height()));
        shearedPix.fill(Qt::transparent);
        QPainter p(&shearedPix);
        if(rect1.width() > rect2.width()){
            p.drawPixmap(0,0, pix1.transformed(transform));
            p.drawPixmap(shearedPix.width()- rect2.width(), y1, pix2.transformed(transform2));
            p.end();
        }else{
            int dx = shearedPix.width()- rect1.width();
            p.drawPixmap(dx, 0, pix1.transformed(transform));
            p.drawPixmap(0, y1, pix2.transformed(transform2));
            p.end();
        }
    }

    else if(data == "hor_left_left"){
        transform.shear( -perc, 0.0);
        QRect rect1 = transform.mapRect(pix1.rect());
        transform2.shear(perc, 0.0);
        QRect rect2 = transform2.mapRect(pix2.rect());
        shearedPix = QPixmap(qMax(rect1.width() ,rect2.width()), (rect1.height() + rect2.height()));
        shearedPix.fill(Qt::transparent);
        QPainter p(&shearedPix);
        p.drawPixmap(0,0, pix1.transformed(transform));
        p.drawPixmap(0, y1, pix2.transformed(transform2));
        p.end();
    }
    else {
        return;
    }

    sPix = shearedPix;
    w = sPix.width();
    h = sPix.height();
    x = 100;
    y = 100;
    resetGeometry();
    this->setPixmap(sPix);

}
