#include "figures.h"
#include "ui_figures.h"
#include "geometric.h"
#include <qmessagebox.h>

figures::figures(int x1, int y1, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::figures)
{
    ui->setupUi(this);

    res =0;
    center = false;
    ui->linePosx->setText(QString::number(x1));
    ui->linePosy->setText(QString::number(y1));
    ui->lineWidth->setText(QString::number(sizes::lineXEnd));
    ui->lineHeight->setText(QString::number(sizes::lineYEnd));

}

figures::~figures()
{
    delete ui;
}

void figures::on_pushButton_3_clicked()
{
    // cancel
    res =0;
    close();
}


void figures::on_pushButton_clicked()
{
    // rect
    bool b = mesur();

    res = 1;
    close();

}


void figures::on_pushButton_2_clicked()
{
    // ellipse
    bool b= mesur();

    res = 2;
    close();
}

bool figures::mesur()
{
    sizes::lineXEnd = ui->lineWidth->text().toInt(); // width
    sizes::lineYEnd = ui->lineHeight->text().toInt();  // height
    sizes::shape_x_begin = ui->linePosx->text().toInt(); // x
    sizes::shape_y_begin = ui->linePosy->text().toInt(); // y

    if(ui->checkDrawCenter->isChecked()){ center = true;}
    else{ center = false;}
    return true;
}

