#ifndef CURVELINEAREA_H
#define CURVELINEAREA_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class curveLineArea : public QLabel
{
    Q_OBJECT
public:     
    explicit curveLineArea(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void finishCurve();
    void finishLines();

private slots:


private:
    int countClicks=0;
    QPixmap tPix;
    QPoint startPoint, centerPoint1, centerPoint2, endPoint;

};

#endif // CURVELINEAREA_H
