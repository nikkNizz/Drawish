#include "linedialog.h"
#include "ui_linedialog.h"
#include "geometric.h"
#include <QMessageBox>

LineDialog::LineDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LineDialog)
{
    ui->setupUi(this);
    res = 0;
    ui->linexStart->setText(QString::number(sizes::lineXEnd));
    ui->lineyStart->setText(QString::number(sizes::lineYEnd));
}

LineDialog::~LineDialog()
{
    delete ui;
}

void LineDialog::on_cancelButton_clicked()
{
   res = QDialog::Rejected;
    close();
}


void LineDialog::on_okButton_clicked()
{
    bool ok;
    linex1= ui->linexStart->text().toInt(&ok);
    if(!ok){
        QMessageBox::warning(this, "Drawish", tr("Start x: integer required!"));
        return;
    }
    liney1= ui->lineyStart->text().toInt(&ok);
    if(!ok){
        QMessageBox::warning(this, "Drawish", tr("Start y: integer required!"));
        return;
    }
    if(ui->radioButton->isChecked()){
        linex2=ui->linexEnd->text().toInt(&ok);
        if(!ok){
            QMessageBox::warning(this, "Drawish", tr("End x: integer required!"));
            return;
        }
        liney2= ui->lineyEnd->text().toInt(&ok);
        if(!ok){
            QMessageBox::warning(this, "Drawish", tr("End y: integer required!"));
            return;
        }
        //----------------------------------------
        sizes::lineXEnd = linex2;
        sizes::lineYEnd = liney2;
        //----------------------------------------

    if(linex1 < 0 || linex1 > sizes::areaWidth-1 || linex2 < 0 || linex2 > sizes::areaWidth-1){
        QMessageBox::warning(this, "Drawish", tr("x out of bounds!"));
        return;
    }
    if(liney1 < 0 || liney1 > sizes::areaHeight-1 || liney2 < 0 || liney2 > sizes::areaHeight-1){
        QMessageBox::warning(this, "Drawish", tr("y out of bounds!"));
        return;
    }
    res = 1;
    }
    else{
        linex2=ui->lineAngle->text().toInt(&ok);
        if(!ok){
            QMessageBox::warning(this, "Drawish", tr("Angle: integer required!"));
            return;
        }
        liney2= ui->lineLen->text().toInt(&ok);
        if(!ok){
            QMessageBox::warning(this, "Drawish", tr("Length: integer required!"));
            return;
        }
        if(linex1 < 0 || linex1 > sizes::areaWidth-1 ){
            QMessageBox::warning(this, "Drawish", tr("x out of bounds!"));
            return;
        }
        if(liney1 < 0 || liney1 > sizes::areaHeight-1 ){
            QMessageBox::warning(this, "Drawish", tr("y out of bounds!"));
            return;
        }
        if(linex2 < 0){ linex2 =0; }
        else if(linex2 > 360){ linex2 = linex2 % 360; }
        sizes::shape_x_begin = linex2; // angle
        sizes::shape_y_begin = liney2;  // length
        res = 2;
    }

    close();
}
