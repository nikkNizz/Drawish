#ifndef LABELTEXT_H
#define LABELTEXT_H

#include <QWidget>
#include <QLabel>
#include <QTextEdit>

class labelText : public QLabel
{
    Q_OBJECT
public:
    explicit labelText(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resetGeometry();
    QTextEdit *txe;
    QFont ffont = QFont("Helvetica");
    bool fbold = false, fitalic=false, funderline=false, falignCenter=false;
    int fsize = 12;
    void formatText();
    void clearText();
    void saveText();

private slots:
    void expandTextArea();

private:
    int mouseX_old, mouseY_old;


};

#endif // LABELTEXT_H
