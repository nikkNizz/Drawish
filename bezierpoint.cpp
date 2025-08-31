#include "bezierpoint.h"
#include <qcolor.h>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

bezierPoint::bezierPoint(QWidget *parent, int ax, int ay, QString tx) : QLabel{parent}
{
    setText(tx);    
    setCursor(Qt::SizeAllCursor);
    setFrameStyle(QFrame::Box | QFrame::Raised);
    pPoint = QPoint(ax, ay);
    setGeometry(pPoint.x(), pPoint.y(), 21,21);
}

void bezierPoint::mousePressEvent(QMouseEvent *event)
{
    globX1 = event->globalPosition().x();
    globY1 = event->globalPosition().y();
}

void bezierPoint::mouseMoveEvent(QMouseEvent *event)
{
    int posx = event->globalPosition().x() - globX1;
    int posy =event->globalPosition().y() - globY1;
    pPoint = QPoint(pPoint.x()+posx, pPoint.y()+posy);
    setGeometry(pPoint.x(), pPoint.y(), 21,21);
    globX1 = event->globalPosition().x() ;
    globY1 =event->globalPosition().y();
    emit curving();
}
