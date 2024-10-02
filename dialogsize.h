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
    explicit DialogSize(QWidget *parent = nullptr);
    ~DialogSize();

    int returned; // 1 = resize   2 = scale

private slots:
    void on_newHeight_lineEdit_textEdited(const QString &arg1);

    void on_newWidth_lineEdit_textEdited(const QString &arg1);

    void on_wPerc_lineEdit_textEdited(const QString &arg1);

    void on_hPerc_lineEdit_textEdited(const QString &arg1);

    void on_resizeButton_clicked();

    void on_scaleButton_clicked();

    void on_createSelectionButton_clicked();

private:
    Ui::DialogSize *ui;
    QString preTextW, preTextH, prePercW, prePercH;

};

#endif // DIALOGSIZE_H
