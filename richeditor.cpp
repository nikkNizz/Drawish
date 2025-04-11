#include "richeditor.h"
#include "ui_richeditor.h"
#include "geometric.h"
#include "fileio.h"
#include <QColorDialog>
#include <qfiledialog.h>


richEditor::richEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::richEditor)
{
    ui->setupUi(this);
    ui->textEdit->setStyleSheet("background-color=#ffffff");
    ui->textEdit->setHtml(sizes::savedRtf);
#ifdef Q_OS_WIN
    ui->blackButton->setStyleSheet("background-color: black");
    ui->whiteButton->setStyleSheet("background-color: white");
#endif
}

richEditor::~richEditor()
{
    delete ui;

}


void richEditor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->textEdit->mergeCurrentCharFormat(format);
}

void richEditor::on_boldButton_clicked()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(ui->boldButton->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}


void richEditor::on_italicButton_clicked()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(ui->italicButton->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}


void richEditor::on_underlinButton_clicked()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(ui->underlinButton->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}


void richEditor::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QTextCharFormat fmt;
    fmt.setFont(f);
    mergeFormatOnWordOrSelection(fmt);
}


void richEditor::on_sizeSpin_valueChanged(int arg1)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(arg1);
    mergeFormatOnWordOrSelection(fmt);
}


void richEditor::on_blackButton_clicked()
{
    QTextCharFormat fmt;
    fmt.setForeground(Qt::black);
    mergeFormatOnWordOrSelection(fmt);
}


void richEditor::on_fntColorButton_clicked()
{
    QColor col = QColorDialog::getColor(Qt::gray, this, tr("Drawish choose color"));
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
}

void richEditor::on_bgcolorButton_clicked()
{
    QColor col = QColorDialog::getColor(Qt::gray, this, tr("Drawish choose color"));
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setBackground(col);
    mergeFormatOnWordOrSelection(fmt);
}

void richEditor::on_whiteButton_clicked()
{
    QTextCharFormat fmt;
    fmt.setBackground(Qt::white);
    mergeFormatOnWordOrSelection(fmt);
}


void richEditor::on_ClearBtn_clicked()
{
    ui->textEdit->clear();
}

void richEditor::resizeEvent(QResizeEvent *event)
{
    int xr = ui->textEdit->x();
    int yr = ui->textEdit->y();
    int wr = this->width() - 172;
    int hr = this->height() -32;
    ui->textEdit->setGeometry(xr, yr, wr, hr);
}


void richEditor::on_scrshotButton_clicked()
{
    rtfPix = this->grab(QRect(QPoint(162,11), QSize(ui->textEdit->width()-2, ui->textEdit->height()-2)));
    passed = true;
    sizes::savedRtf = ui->textEdit->document()->toHtml();
    close();
}


void richEditor::on_openButton_clicked()
{
    QFileDialog fileDialog(this, tr("Open File..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);

    if (fileDialog.exec() != QDialog::Accepted)  return;
    const QString f = fileDialog.selectedFiles().first();
    fileIO fio;
    QString data = fio.readFile(f);
    ui->textEdit->setHtml(data);

}


void richEditor::on_saveButton_clicked()
{
    QFileDialog fileDialog(this, tr("Save as..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    if (fileDialog.exec() != QDialog::Accepted)  return;
    const QString f = fileDialog.selectedFiles().first();
    if( f.isEmpty()) return;
    fileIO fio;
    fio.createFile(ui->textEdit->toHtml(), f);
}





