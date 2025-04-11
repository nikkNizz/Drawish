#ifndef RICHEDITOR_H
#define RICHEDITOR_H

#include <QDialog>
#include <qtextformat.h>

namespace Ui {
class richEditor;
}

class richEditor : public QDialog
{
    Q_OBJECT

public:
    explicit richEditor(QWidget *parent = nullptr);

    QPixmap rtfPix;
    bool passed = false;

    ~richEditor();

private slots:
    void on_boldButton_clicked();

    void on_italicButton_clicked();

    void on_underlinButton_clicked();

    void on_fontComboBox_currentFontChanged(const QFont &f);

    void on_sizeSpin_valueChanged(int arg1);

    void on_blackButton_clicked();

    void on_fntColorButton_clicked();

    void on_ClearBtn_clicked();

    void on_scrshotButton_clicked();

    void on_openButton_clicked();

    void on_saveButton_clicked();

    void on_bgcolorButton_clicked();

    void on_whiteButton_clicked();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    Ui::richEditor *ui;
};

#endif // RICHEDITOR_H
