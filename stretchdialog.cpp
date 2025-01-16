#include "stretchdialog.h"
#include "ui_stretchdialog.h"
#include "stretchview.h"

StretchDialog::StretchDialog(QWidget *parent, QPixmap ePix) :
    QDialog(parent),
    ui(new Ui::StretchDialog)
{
    ui->setupUi(this);

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


