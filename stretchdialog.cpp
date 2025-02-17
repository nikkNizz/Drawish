#include "stretchdialog.h"
#include "ui_stretchdialog.h"
#include "stretchview.h"
#include "geometric.h"

StretchDialog::StretchDialog(QWidget *parent, QPixmap ePix) :
    QDialog(parent),
    ui(new Ui::StretchDialog)
{
    ui->setupUi(this);
    sizes::curveStretch = false;
    res =0;
    sv = new stretchView(this, ePix);
    sv->show();
}

StretchDialog::~StretchDialog()
{
    delete ui;
}

void StretchDialog::on_oKbtn_clicked()
{
    res = QDialog::Accepted;
    epix = sv->pixmap();
    close();
}


void StretchDialog::on_cancelBtn_clicked()
{
    res = QDialog::Rejected;
    close();
}

void StretchDialog::on_checkBox_stateChanged(int arg1)
{
    if(ui->checkBox->isChecked())sizes::curveStretch = true;
    else sizes::curveStretch = false;
}
