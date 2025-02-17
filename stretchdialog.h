#ifndef STRETCHDIALOG_H
#define STRETCHDIALOG_H

#include "stretchview.h"
#include <QDialog>


namespace Ui {
class StretchDialog;
}

class StretchDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StretchDialog(QWidget *parent = nullptr, QPixmap ePix= QPixmap(200,170));
    ~StretchDialog();
    QPixmap epix;
    int res;

protected:


private slots:
    void on_oKbtn_clicked();

    void on_cancelBtn_clicked();

    void on_checkBox_stateChanged(int arg1);

private:
    Ui::StretchDialog *ui;
    stretchView *sv;

};

#endif // STRETCHDIALOG_H
