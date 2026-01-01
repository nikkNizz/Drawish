#include "borderright.h"
#include "geometric.h"
#include <QMouseEvent>

BorderRight::BorderRight(QWidget *parent)  : QLabel{parent}
{
   this->setGeometry(sizes::areaWidth-8,0,8, sizes::areaHeight-8);
   this->setStyleSheet("background-color:#005AFF");
    this->setCursor(Qt::SizeHorCursor);
}

void BorderRight::mousePressEvent(QMouseEvent *event)
{
    exP = event->globalPosition().x();
}


void BorderRight::mouseMoveEvent(QMouseEvent *event)
{
        int nP = event->globalPosition().x()- exP;
        sizes::areaWidth += nP;
        if(sizes::areaWidth < 9)sizes::areaWidth =9;
        exP = event->globalPosition().x();
        emit sizeChange();
}

void BorderRight::mouseReleaseEvent(QMouseEvent *event)
{
    sizes::startResize=false;
}

void BorderRight::resetGeometry()
{
     this->setGeometry(sizes::areaWidth-8,0,8, sizes::areaHeight-8);
}
