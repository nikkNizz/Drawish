#ifndef LINEDIALOG_H
#define LINEDIALOG_H

#include <QDialog>

namespace Ui {
class LineDialog;
}

class LineDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LineDialog(QWidget *parent = nullptr);
    ~LineDialog();
    int res;
    int linex1, liney1, linex2, liney2;

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::LineDialog *ui;
};

#endif // LINEDIALOG_H
