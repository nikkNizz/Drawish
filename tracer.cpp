#include "tracer.h"
#include "geometric.h"
#include <QPainter>
#include <qevent.h>
#include <QPoint>

tracer::tracer(QWidget *parent)  : QLabel{parent}{
    resetGeometry();
    aPix =QPixmap(this->size());
    aPix.fill(QColor(255,255,255,0));
    update();
    mousePos= QPoint(0,0);
    red = 252;
    green = 128;
    blue = 4;
    drawStep =0;
    // create cursor
    QPixmap cur(9,9);
    cur.fill(QColor(255,255,255,0));
    QPainter p(&cur);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::green);
    p.drawEllipse(0,0,9,9);
    p.setBrush(QColor(200,0,200));
    p.drawEllipse(2,2,4,4);
    this->setCursor(QCursor(cur));
}

void tracer::mousePressEvent(QMouseEvent *event)
{
    prePos = event->pos();    
}

void tracer::mouseMoveEvent(QMouseEvent *event)
{
    drawStep++; // drawstep for fluid pen
    if(drawStep > 3) drawStep =0;
    if(drawStep == 1){
    mousePos = event->pos();
    QPainter pai(&aPix);
    QPen pen(QColor(red,green,blue));
    pen.setWidth(8);
    pen.setCapStyle(Qt::RoundCap);
    pai.setPen(pen);
    pai.drawLine(prePos, mousePos);
    pai.end();
        update(QRect(prePos, mousePos).normalized().adjusted(-4, -4, +4, +4));
    prePos = mousePos;
        if(toColor == 0){ // to yellow  r=252 g = 128 b =4
        green+=4;
            if(green == 252) toColor =1;
        }
        else if(toColor ==1){ // togreen  r = 252 g = 252 b =4
            red -= 4;
            blue += 4;
            if(red == 128) toColor = 2;
        }
        else if(toColor == 2){ //to blue  r = 128 g = 252 b = 128
            red -=4;
            green -=4;
            blue +=4;
            if(red == 4) toColor = 3;
        }
        else if(toColor == 3){ // to magenta  r =4 g =128 b = 252
            red += 8;
            green -= 4;
            if(red == 252) toColor = 4;
        }
        else if(toColor == 4){ // to orange r = 252 g=4 b = 252
            green +=4;
            blue -=8;
            if(blue == 4) toColor =0;
        }

    }
}

void tracer::mouseReleaseEvent(QMouseEvent *event)
{
    emit vanish();
}

void tracer::resetGeometry()
{
    setGeometry(0, 0, sizes::areaWidth-8, sizes::areaHeight-8);
}

void tracer::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect pRect = event->rect();
    painter.drawPixmap(pRect, aPix, pRect);

}
