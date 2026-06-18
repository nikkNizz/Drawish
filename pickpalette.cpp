#include "pickpalette.h"
#include "ui_pickpalette.h"
#include "geometric.h"
#include <QMouseEvent>


PickPalette::PickPalette(QPixmap pic, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PickPalette)
{
    pickd = sizes::activeColor;
    ui->setupUi(this);
    ui->viewLabel->setPixmap(pic);
    ui->viewLabel->setGeometry(0,0, pic.width(), pic.height());
    pixx = pic;
}

PickPalette::~PickPalette()
{
    delete ui;
}

void PickPalette::mousePressEvent(QMouseEvent *ev)
{
    int ax = ev->pos().x();
    int ay = ev->pos().y();
    pickd= pixx.toImage().pixelColor(ax, ay);
    close();
}
