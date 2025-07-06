#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <QDialog>
#include <qdir.h>

namespace Ui {
class Serialize;
}

class Serialize : public QDialog
{
    Q_OBJECT

public:
    explicit Serialize(QWidget *parent = nullptr);
    void dragEnterEvent(QDragEnterEvent *event)override;
    void dropEvent(QDropEvent *event) override;
    ~Serialize();

private slots:
    void on_buttonChooseFolder_clicked();
    void findFfmpeg();
    QString createDestinationFolder();

    void toGrayscale();
    void contrast();
    void brightness();
    void crop(QString errOnFile);
    void pasteImage(QPixmap ppix, QString errOnFile);
    void rotate();
    void scale(QString errOnFile);
    void convert(QString f);


    void on_buttonOk_clicked();

private:
    Ui::Serialize *ui;

    QPixmap actualPix;
    QDir origDir;
    QString destination;
    QString errors;
};

#endif // SERIALIZE_H
