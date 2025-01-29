#ifndef DIALOGEFFECTS_H
#define DIALOGEFFECTS_H

#include <QDialog>
#include <QImage>

namespace Ui {
class DialogEffects;
}

class DialogEffects : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEffects(QWidget *parent = nullptr, QPixmap ePix= QPixmap(200,170));
    ~DialogEffects();
    QPixmap modifPix;
    int res;

private slots:
    QImage gamma(double exp);
    QImage contrast(int v);
    QImage highlightEffect(double v);
    QImage addColor(int color, int v);
    void on_gammaSlider_sliderReleased();

    void on_valSlider_sliderReleased();

    void on_highlightPlusButton_clicked();


    void on_highlightMinusButton_clicked();

    void on_pushButton_3_clicked();

    void on_redSlider_sliderReleased();

    void on_greenSlider_sliderReleased();

    void on_blueSlider_sliderReleased();

    void on_noiseMinusButton_clicked();

    QImage noise();

    void on_blurSlider_2_sliderReleased();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pixelizeButton_clicked();


private:
    Ui::DialogEffects *ui;
    QPixmap origPix;
    QPixmap newPix;
};

#endif // DIALOGEFFECTS_H
