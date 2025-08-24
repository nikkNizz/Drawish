#include "tracer.h"
#include "geometric.h"
#include <QPainter>
#include <qevent.h>
#include <QPoint>

tracer::tracer(QWidget *parent)  : QLabel{parent}{
    resetGeometry();
    aPix =QPixmap(this->size());
    aPix.fill(QColor(255,255,255,0));
    setPixmap(aPix);
    mousePos= QPoint(0,0);
    red = 255;
    blue = 90;
    drawStep =0;
    // create cursor
    QPixmap cur(9,9);
    cur.fill(QColor(255,255,255,0));
    QPainter p(&cur);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::green);
    p.drawEllipse(0,0,9,9);
    p.setBrush(QColor(255,0,255));
    p.drawEllipse(2,2,4,4);
    this->setCursor(QCursor(cur));
}

void tracer::mousePressEvent(QMouseEvent *event)
{
    prePos = event->pos();    
}

void tracer::mouseMoveEvent(QMouseEvent *event)
{
    drawStep++;
    if(drawStep > 3) drawStep =0;
    if(drawStep == 1){
    mousePos = event->pos();
    QPainter pai(&aPix);
    QPen pen(QColor(red,255,blue));
    pen.setWidth(8);
    pen.setCapStyle(Qt::RoundCap);
    pai.setPen(pen);
    pai.drawLine(prePos, mousePos);
    pai.end();
    setPixmap(aPix);
    prePos = mousePos;
    red = red -5;
    if(red < 100) red = 255;
    blue += 3;
    if(blue > 250) blue = 90;
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
