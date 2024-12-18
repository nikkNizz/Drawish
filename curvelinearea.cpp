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
    if(sizes::activeOperation == 11){
        if(sizes::shape_x_begin == -1 && sizes::shape_y_begin == -1){
            sizes::shape_x_begin = event->pos().x();
            sizes::shape_y_begin = event->pos().y();
            QPainter p(&tPix);
            QPen pen(sizes::activeColor);
            pen.setWidth(sizes::line_width);
            p.setPen(pen);
            p.drawPoint(event->pos());
            this->setPixmap(tPix);
        }else{
            sizes::shape_x_end = event->pos().x();
            sizes::shape_y_end = event->pos().y();
            QPainter p(&tPix);
            QPen pen(sizes::activeColor);
            pen.setWidth(sizes::line_width);
            p.setPen(pen);
            p.drawLine(sizes::shape_x_begin, sizes::shape_y_begin, sizes::shape_x_end, sizes::shape_y_end);
            this->setPixmap(tPix);
            sizes::shape_x_begin = sizes::shape_x_end;
            sizes::shape_y_begin = sizes::shape_y_end;
            p.end();
            emit finishLines();
        }

    }
    else if(sizes::activeOperation == 9){
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
    //}else{
        p.end();
        countClicks=0;
        emit finishCurve();
        return;
   }
    countClicks++;
    }
}

