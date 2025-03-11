#ifndef FIGURES_H
#define FIGURES_H

#include <QDialog>

namespace Ui {
class figures;
}

class figures : public QDialog
{
    Q_OBJECT

public:
    explicit figures(int x1, int y1, QWidget *parent = nullptr);
    ~figures();
    int res;
    bool center;

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    bool mesur();

private:
    Ui::figures *ui;
};

#endif // FIGURES_H
