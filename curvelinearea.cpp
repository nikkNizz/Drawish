#include "curvelinearea.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QPainterPath>

curveLineArea::curveLineArea(QWidget *parent)  : QLabel{parent}
{
  this->setGeometry(0, 0, sizes::areaWidth-8, sizes::areaHeight-8);
  tPix = QPixmap(this->size());
  tPix.fill(QColor(255,255,255,0));
  countClicks =0;
}

void curveLineArea::mousePressEvent(QMouseEvent *event)
{

    if(countClicks ==0){
        startPoint = event->pos();
        QPainter p(&tPix);
        QPen pen(sizes::activeColor);
        pen.setWidth(sizes::line_width);
        p.setPen(pen);
        p.drawPoint(startPoint);
        this->setPixmap(tPix);
    }
    else if(countClicks ==1){
        centerPoint1 = event->pos();
        QPainter p(&tPix);
        QPen pen(sizes::activeColor);
        pen.setWidth(sizes::line_width);
        p.setPen(pen);
        p.drawPoint(centerPoint1);
        this->setPixmap(tPix);
    }
    else if(countClicks ==2){
        centerPoint2 = event->pos();
        QPainter p(&tPix);
        QPen pen(sizes::activeColor);
        pen.setWidth(sizes::line_width);
        p.setPen(pen);
        p.drawPoint(centerPoint2);
        this->setPixmap(tPix);
    }
    else if(countClicks ==3){
        endPoint = event->pos();
        tPix.fill(QColor(255,255,255,0));
        QPainterPath pp;
        pp.moveTo(startPoint);
        pp.cubicTo(centerPoint1, centerPoint2, endPoint);
        QPainter p(&tPix);
        QPen pen(sizes::activeColor);
        pen.setWidth(sizes::line_width);
        p.setPen(pen);
        p.drawPath(pp);
        this->setPixmap(tPix);
    }
    else{
        countClicks=0;
        emit finishCurve();
        return;
    }
    countClicks++;
}


