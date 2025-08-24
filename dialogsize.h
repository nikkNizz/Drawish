#ifndef DIALOGSIZE_H
#define DIALOGSIZE_H

#include <QDialog>

namespace Ui {
class DialogSize;
}

class DialogSize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSize(QWidget *parent = nullptr, int pixW =0, int pixH =0, double Zoom =1.0);
    ~DialogSize();

    int returned; // 1 = resize   2 = scale
    int pixWW, pixHH;

private slots:
    void on_newHeight_lineEdit_textEdited(const QString &arg1);

    void on_newWidth_lineEdit_textEdited(const QString &arg1);

    void on_wPerc_lineEdit_textEdited(const QString &arg1);

    void on_hPerc_lineEdit_textEdited(const QString &arg1);

    void on_resizeButton_clicked();

    void on_scaleButton_clicked();

    void on_createSelectionButton_clicked();

    void saveSelData();


private:
    Ui::DialogSize *ui;
    QString preTextW, preTextH, prePercW, prePercH;
    double dZoom;


};

#endif // DIALOGSIZE_H
