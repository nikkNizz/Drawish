#include "savecam.h"
#include <QSize>
#include <QMouseEvent>

saveCam::saveCam(QWidget *parent, QSize sz)  : QLabel{parent}
{
    QPixmap npix(sz.width(), sz.height());
    npix.fill(QColor(255,255,255,0));
    this->setCursor(Qt::SizeAllCursor);
    this->setFrameStyle(QFrame:: Box| QFrame::Raised);
    this->setPixmap(npix);
}

void saveCam::mousePressEvent(QMouseEvent *event)
{
    gx = event->globalPosition().x();
    gy = event->globalPosition().y();
}

void saveCam::mouseMoveEvent(QMouseEvent *event)
{
    ax = event->globalPosition().x() - gx;
    ay = event->globalPosition().y() - gy;
    setGeometry(this->pos().x()+ax, this->pos().y()+ay, this->width(), this->height());
    gx = event->globalPosition().x();
    gy = event->globalPosition().y();

}

