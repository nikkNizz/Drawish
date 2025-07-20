#include "zoomarea.h"
#include "geometric.h"
#include <QMouseEvent>
#include <QMessageBox>

zoomArea::zoomArea(QWidget *parent)  : QLabel{parent}
{

}

void zoomArea::mousePressEvent(QMouseEvent *event)
{
  if(event->button()== Qt::RightButton){
      sizes::zoomx = event->pos().x();
      sizes::zoomy = event->pos().y();
      sizes::zoomEdited = true;
      emit zoomChangeColor();
  }else if(event->button()== Qt::LeftButton){
      QImage px = this->pixmap().toImage();
      QColor k = px.pixelColor(event->pos().x(), event->pos().y());
      QString kk = "Red:" + QString::number(k.red()) + " Green:" + QString::number(k.green()) + " Blue:" + QString::number(k.blue());
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

