#ifndef VIEWIMAGE_H
#define VIEWIMAGE_H

#include "qlabel.h"
#include <QDialog>

namespace Ui {
class viewImage;
}

class viewImage : public QDialog
{
    Q_OBJECT

public:
    explicit viewImage(QWidget *parent = nullptr, QPixmap ePix= QPixmap(200,170));
    ~viewImage();

protected:
   void resizeEvent(QResizeEvent *event) override;

private slots:
   void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::viewImage *ui;
    QLabel *viewLabel;
    QPixmap Pix;
};

#endif // VIEWIMAGE_H
