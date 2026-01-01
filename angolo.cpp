#include "angolo.h"
#include "geometric.h"

#include <QMouseEvent>

Angolo::Angolo(QWidget *parent)  : QLabel{parent}
{
    //             x  y   w   h
 this->setGeometry(sizes::areaWidth-8,sizes::areaHeight-8, 8, 8);
 this->setStyleSheet("background-color:#995AFF");
    this->setCursor(Qt::SizeFDiagCursor);
}

void Angolo::mousePressEvent(QMouseEvent *event)
{
    exx = event->globalPosition().x();
    exy = event->globalPosition().y();
}


void Angolo::mouseMoveEvent(QMouseEvent *event)
{
        int nx = event->globalPosition().x() - exx;
        int ny = event->globalPosition().y() - exy;
        sizes::areaHeight += ny;
        sizes::areaWidth += nx;
        if(sizes::areaHeight < 9)sizes::areaHeight =9;
        if(sizes::areaWidth < 9)sizes::areaWidth =9;
        exx = event->globalPosition().x();
        exy = event->globalPosition().y();
        emit sizeChange();

}

void Angolo::mouseReleaseEvent(QMouseEvent *event)
{
  sizes::startResize=false;
}

void Angolo::resetGeometry()
{
this->setGeometry(sizes::areaWidth-8,sizes::areaHeight-8, 8, 8);
}
