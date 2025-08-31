#include "curvelinearea.h"
#include "geometric.h"
#include "bezierpoint.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QPainterPath>
#include <QMessageBox>

curveLineArea::curveLineArea(QWidget *parent, double zoomy)  : QLabel{parent}
{
  resetGeometry();
  countClicks =0;
  Zoom = zoomy;
}

void curveLineArea::mousePressEvent(QMouseEvent *event)
{
    if(sizes::activeOperation == 7 || sizes::activeOperation == 13){
        if(countClicks == 0){
            countClicks =1;
            startPoint = event->pos();
            sizes::freeSelPoints.append(startPoint/ Zoom);
            changeColor=false;
        }
    }
    QPen pen(sizes::activeColor);
    pen.setWidth(sizes::line_width * Zoom);
    pen.setCapStyle(Qt::RoundCap);
    if(sizes::activeOperation == 11){  // lines
        if(sizes::shape_x_begin == -1 && sizes::shape_y_begin == -1){
            sizes::shape_x_begin = event->pos().x();
            sizes::shape_y_begin = event->pos().y();
            QPainter p(&tPix);
            p.setPen(pen);
            p.drawPoint(event->pos().x(), event->pos().y());
            this->setPixmap(tPix);
        }else{
            sizes::shape_x_end = event->pos().x();
            sizes::shape_y_end = event->pos().y();
            QPainter p(&tPix);
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
        if(bez1 == NULL) countClicks =0;
        if(countClicks ==0){
            bez1 = new bezierPoint(this, event->pos().x(), event->pos().y(), "1");
            connect(bez1, SIGNAL(curving()), this, SLOT(Curving()));
            bez1->show();
        }
        else if(countClicks == 1){
            bez2 = new bezierPoint(this, event->pos().x(), event->pos().y(), "2");
            connect(bez2, SIGNAL(curving()), this, SLOT(Curving()));
            bez2->show();
        }
        else if(countClicks == 2){
            bez3 = new bezierPoint(this, event->pos().x(), event->pos().y(), "3");
            connect(bez3, SIGNAL(curving()), this, SLOT(Curving()));
            bez3->show();
        }
        else if(countClicks == 3){
            bez4 = new bezierPoint(this, event->pos().x(), event->pos().y(), "4");
            connect(bez4, SIGNAL(curving()), this, SLOT(Curving()));
            bez4->show();
        }
        else if(countClicks == 4){
            bez5 = new bezierPoint(this, event->pos().x(), event->pos().y(), "5");
            connect(bez5, SIGNAL(curving()), this, SLOT(Curving()));
            bez5->show();
        }
        else if(countClicks == 5){
            bez6 = new bezierPoint(this, event->pos().x(), event->pos().y(), "X");
            connect(bez6, SIGNAL(curving()), this, SLOT(Curving()));
            bez6->show();
            Curving();
        }
        else{
            int q = QMessageBox::question(this, "Drawish", tr("Do you want to confirm the drawing?"), QMessageBox::Yes | QMessageBox::No);
            if(q ==QMessageBox::Yes){
                countClicks = 0;
                delete bez1; bez1 = NULL;
                delete bez2; bez2 = NULL;
                delete bez3; bez3 = NULL;
                delete bez4; bez4 = NULL;
                delete bez5; bez5 = NULL;
                delete bez6; bez6 = NULL;
                emit finishCurve();
            }
        }
        countClicks++;
    }

    else if(sizes::activeOperation == 10 ){  // curve || conn. curve
    if(countClicks ==0){
            if(sizes::shape_x_begin != -1 && sizes::shape_y_begin != -1 ){
                startPoint = QPoint(sizes::shape_x_begin, sizes::shape_y_begin);

                centerPoint1 = event->pos();
                QPainter p(&tPix);
                p.setPen(pen);
                p.drawPoint(centerPoint1);
                this->setPixmap(tPix);
                countClicks++;
            }
        else{
        startPoint = event->pos();
        QPainter p(&tPix);
        p.setPen(pen);
        p.drawPoint(startPoint);
        this->setPixmap(tPix);
            }

    }
    else if(countClicks ==1){
        centerPoint1 = event->pos();
        QPainter p(&tPix);
        p.setPen(pen);
        p.drawPoint(event->pos());
        this->setPixmap(tPix);
    }
    else if(countClicks ==2){
        if(sizes::activeOperation == 10 && sizes::autoPoint.x() > -1){
            endPoint = event->pos();
            sizes::shape_x_begin = endPoint.x();
            sizes::shape_y_begin = endPoint.y();
            tPix.fill(QColor(255,255,255,0));
            QPainterPath pp;
            pp.moveTo(startPoint);
            pp.cubicTo(sizes::autoPoint, centerPoint1, endPoint);
            QPainter p(&tPix);
            p.setPen(pen);
            p.drawPath(pp);
            this->setPixmap(tPix);
            p.end();
            countClicks=0;
            //--------------------------------------------------
            int oldPointx = endPoint.x() +((endPoint.x() - centerPoint1.x())/2);
            int oldPointy = endPoint.y() + ((endPoint.y() -centerPoint1.y())/2);
            sizes::autoPoint =QPoint(oldPointx, oldPointy);
            //--------------------------------------------------
            emit finishCurve();
            return;

        }
        centerPoint2 = event->pos();
        QPainter p(&tPix);
        p.setPen(pen);
        p.drawPoint(event->pos());
        this->setPixmap(tPix);
    }
    else if(countClicks ==3){
        endPoint = event->pos();
        sizes::shape_x_begin = endPoint.x();
        sizes::shape_y_begin = endPoint.y();
        tPix.fill(QColor(255,255,255,0));
        QPainterPath pp;
        pp.moveTo(startPoint);
        pp.cubicTo(centerPoint1, centerPoint2, endPoint);
        QPainter p(&tPix);
        p.setPen(pen);
        p.drawPath(pp);
        this->setPixmap(tPix);
        p.end();
        countClicks=0;
        if(sizes::activeOperation == 10){
        //--------------------------------------------------
        int oldPointx = endPoint.x() +(endPoint.x() - centerPoint2.x())/2;
        int oldPointy = endPoint.y() + (endPoint.y() - centerPoint2.y())/2;
        sizes::autoPoint =QPoint(oldPointx, oldPointy);
        //--------------------------------------------------
        }
        emit finishCurve();
        return;
   }
    countClicks++;
    }    
}

void curveLineArea::mouseMoveEvent(QMouseEvent *event)
{
    if(sizes::activeOperation == 7 && countClicks == 1){
    endPoint = event->pos();
    tPix.fill(QColor(255,255,255,0));
    QPainter p(&tPix);
    QPen pen(sizes::activeColor);
    pen.setWidth(sizes::line_width * Zoom);
    p.setPen(pen);
    if(sizes::isArrow == false){
        p.drawLine(startPoint, endPoint);
    }
    else{
        int basisLen = abs(startPoint.x() - endPoint.x());  // cosine
        int heightLen = abs(startPoint.y() - endPoint.y());  // sin
        double tan = double(basisLen) / double(heightLen);    // tan
        double atan = qAtan(tan);     // opposite tan in radians
        double deg = qRadiansToDegrees(atan);   // radians to deg.
        double flen = sqrt(basisLen*basisLen + heightLen*heightLen);  // hypotenuse (equal to length arrow)
        int len = flen *0.3;   // length of arrowhead
        QLineF lf, lf1, lf2;

        lf = QLine(startPoint, endPoint);

         if(startPoint.x() <= endPoint.x() && startPoint.y() >= endPoint.y() ){ deg += 90;} // top right
         else if(startPoint.x() >= endPoint.x() && startPoint.y() >= endPoint.y() ){ deg = 90-deg  ;} // top left
         else if(startPoint.x() <= endPoint.x() && startPoint.y() <= endPoint.y() ){ deg = 270-deg;} // bott right
         else if(startPoint.x() >= endPoint.x() && startPoint.y() <= endPoint.y() ){ deg = deg-90;} // bott left


        lf1.setP1(endPoint);
        lf1.setAngle(-deg+15);
        lf1.setLength(len);

        lf2.setP1(endPoint);
        lf2.setAngle(-deg-15);
        lf2.setLength(len);

        QList <QLineF> listlines;
        listlines << lf << lf1 << lf2 ;
        p.drawLines(listlines);

    }
    p.end();
    this->setPixmap(tPix);
    }
    else if(sizes::activeOperation == 13){  // freehand sel.
        endPoint = event->pos();
        QPainter pai(&tPix);
        QPen pen;
        pen.setWidth(2);
        if(changeColor){
            pen.setColor(Qt::blue);
        }else{
            pen.setColor(Qt::yellow);
        }
        changeColor = !changeColor;
        pai.setPen(pen);
        pai.drawLine(startPoint, endPoint);
        pai.end();
        this->setPixmap(tPix);
        startPoint = endPoint;
        sizes::freeSelPoints.append(endPoint / Zoom);
    }
}

void curveLineArea::mouseReleaseEvent(QMouseEvent *event)
{

    if(sizes::activeOperation == 7) emit finishLines();
    if(sizes::activeOperation == 13){
        countClicks =0;
        emit finishLines();
    }
}

void curveLineArea::resetGeometry()
{
  this->setGeometry(0, 0, sizes::areaWidth-8, sizes::areaHeight-8);
  tPix = QPixmap(this->size());
  tPix.fill(QColor(255,255,255,0));
}

void curveLineArea::Curving()
{
    if(countClicks > 4){
        tPix =QPixmap(this->width(), this->height());
        tPix.fill(QColor(255,255,255,0));
        QPainter p(&tPix);
        QPen pen(sizes::activeColor);
        pen.setWidth(sizes::line_width * Zoom);
        pen.setCapStyle(Qt::RoundCap);
        p.setPen(pen);
        QPainterPath pp;
        startPoint = QPoint(bez1->x(), bez1->y());
        pp.moveTo(startPoint);
        centerPoint1 = QPoint(bez2->x(), bez2->y());
        centerPoint2 = QPoint(bez3->x(), bez3->y());
        endPoint = QPoint(bez4->x(), bez4->y());
        pp.cubicTo(centerPoint1, centerPoint2, endPoint);

        p.drawPath(pp);

        int axx = endPoint.x() + (endPoint.x() - centerPoint2.x())/2;
        int ayy = endPoint.y() + (endPoint.y() - centerPoint2.y())/2;
        startPoint = endPoint;
        centerPoint1 = QPoint(axx, ayy);
        centerPoint2 = QPoint(bez5->x(), bez5->y());
        endPoint= QPoint(bez6->x(), bez6->y());
        pp.moveTo(startPoint);
        pp.cubicTo(centerPoint1, centerPoint2, endPoint);
        p.drawPath(pp);
        this->setPixmap(tPix);
        p.end();
    }
}


