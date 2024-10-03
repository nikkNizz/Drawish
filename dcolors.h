#ifndef DCOLORS_H
#define DCOLORS_H

#include <QDialog>

namespace Ui {
class dColors;
}

class dColors : public QDialog
{
    Q_OBJECT

public:
    explicit dColors(QWidget *parent = nullptr, QPixmap ePix= QPixmap(200,170));
    ~dColors();
    int res;
    QPixmap origPix;

private slots:
    void setBgColor();


    void on_redSpin_valueChanged(int arg1);

    void on_greenSpin_valueChanged(int arg1);

    void on_blueSpin_valueChanged(int arg1);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_replaceButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::dColors *ui;    
    QPixmap newPix;
};

#endif // DCOLORS_H