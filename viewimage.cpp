#include "viewimage.h"
#include "ui_viewimage.h"

viewImage::viewImage(QWidget *parent, QPixmap ePix) :
    QDialog(parent),
    ui(new Ui::viewImage)
{
    ui->setupUi(this);
    Pix = ePix;
    viewLabel = new QLabel();
    viewLabel->setPixmap(Pix.scaled(ePix.width()*2, Pix.height()*2));
    viewLabel->setGeometry(0,0, Pix.width()*2, Pix.height()*2);
    ui->scrollArea->setWidget(viewLabel);
    ui->scrollArea->setWidgetResizable(false);
}

viewImage::~viewImage()
{
    delete ui;
}

void viewImage::resizeEvent(QResizeEvent *event)
{
    ui->scrollArea->setGeometry(10, 40,this->width()-16, this->height()-50);
}

void viewImage::on_comboBox_currentTextChanged(const QString &arg1)
{
    double s = arg1.toDouble();
    viewLabel->setPixmap(Pix.scaled(Pix.width()*s, Pix.height()*s));
    viewLabel->setGeometry(0,0, Pix.width()*s, Pix.height()*s);
}

