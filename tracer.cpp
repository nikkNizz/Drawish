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
    red = 255;
    green = 160;
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
    QPen pen(QColor(red,green,160));
    pen.setWidth(8);
    pen.setCapStyle(Qt::RoundCap);
    pai.setPen(pen);
    pai.drawLine(prePos, mousePos);
    pai.end();
        update(QRect(prePos, mousePos).normalized().adjusted(-4, -4, +4, +4));
    prePos = mousePos;
    red = red -(3 * redF);
    if(red < 150) {
        redF = -1;
    }else if(red > 255){
        redF = 1;
    }
    green = green + (4 * greenF);
    if(green > 255) {
        greenF = -1;
    }else if(green < 100 ){
        greenF = 1;
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
