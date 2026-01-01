#include "zoomarea.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QMessageBox>

zoomArea::zoomArea(QWidget *parent)  : QLabel{parent}
{
    ax = sizes::selX -8;  // initial position
    ay = sizes::selY -8;
}

void zoomArea::mousePressEvent(QMouseEvent *event)
{
    int evx = event->pos().x();
    int evy = event->pos().y();
  if(event->button()== Qt::RightButton){
      sizes::zoomx = evx;
      sizes::zoomy = evy;
      sizes::zoomEdited = true;
      emit zoomChangeColor();
  }else if(event->button()== Qt::LeftButton){
      QImage px = this->pixmap().toImage();
      QColor k = px.pixelColor(evx, evy);
      int wx = evx / 14;
      int hy = evy / 14;
      wx += ax;
      hy += ay;
      QString kk = "Position ( " + QString::number(wx) + ", " + QString::number(hy) + " )\nRed:" + QString::number(k.red()) + " Green:" + QString::number(k.green()) + " Blue:" + QString::number(k.blue());
      QMessageBox msgBox;
      msgBox.setText(kk);
      msgBox.setInformativeText(tr("Set as active color?"));
      msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
      msgBox.setDefaultButton(QMessageBox::No);
      int ret = msgBox.exec();
      if(ret == QMessageBox::Yes){
          sizes::activeColor= k;
          emit changeActiveColor();
      }
  }
}

void zoomArea::mouseMoveEvent(QMouseEvent *event)
{
    sizes::zoomx = event->pos().x();
    sizes::zoomy = event->pos().y();
    sizes::zoomEdited = true;
    emit zoomChangeColor();
}

