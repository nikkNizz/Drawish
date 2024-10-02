#include "borderbottom.h"
#include "geometric.h"


#include <QMouseEvent>

BorderBottom::BorderBottom(QWidget *parent)  : QLabel{parent}
{
      //             x  y   w   h
   this->setGeometry(0,sizes::areaHeight-8, sizes::areaWidth-8, 8);
   this->setStyleSheet("background-color:#005AFF");
   this->setCursor(Qt::SizeVerCursor);

}

void BorderBottom::mousePressEvent(QMouseEvent *event)
{
     exP = event->globalPosition().y();
}

void BorderBottom::mouseMoveEvent(QMouseEvent *event)
{
        int nP = event->globalPosition().y()- exP;

        sizes::areaHeight +=  nP;
        exP =event->globalPosition().y();
        emit sizeChange();

}

void BorderBottom::mouseReleaseEvent(QMouseEvent *event)
{

    sizes::startResize=false;
}

void BorderBottom::resetGeometry()
{
    this->setGeometry(0,sizes::areaHeight-8, sizes::areaWidth-8, 8);
}

