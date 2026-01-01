#ifndef CURVELINEAREA_H
#define CURVELINEAREA_H

#include "bezierpoint.h"
#include <QObject>
#include <QWidget>
#include <QLabel>

class curveLineArea : public QLabel
{
    Q_OBJECT
public:
    explicit curveLineArea(QWidget *parent = nullptr, double zoomy =1.0);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resetGeometry();

signals:
    void finishCurve();
    void finishLines();

private slots:
    void Curving();

private:
    int countClicks=0;
    bool changeColor;
    double Zoom;
    QPixmap tPix;
    QPoint startPoint, centerPoint1, centerPoint2, endPoint;
    bezierPoint *bez1, *bez2, *bez3, *bez4, *bez5, *bez6;
    QLabel *infoDeg;

};

#endif // CURVELINEAREA_H
