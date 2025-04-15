#ifndef PICKPALETTE_H
#define PICKPALETTE_H

#include <QDialog>

namespace Ui {
class PickPalette;
}

class PickPalette : public QDialog
{
    Q_OBJECT

public:
    explicit PickPalette(QPixmap pic, QWidget *parent = nullptr);
    QColor pickd;
    ~PickPalette();

protected:
    void mousePressEvent(QMouseEvent *ev);

private:
    Ui::PickPalette *ui;
    QPixmap pixx;
};

#endif // PICKPALETTE_H
