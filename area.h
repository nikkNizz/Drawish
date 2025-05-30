#ifndef AREA_H
#define AREA_H

#include <QWidget>
#include <QLabel>
#include <QRubberBand>

class Area : public QLabel
{
        Q_OBJECT

public:
   explicit Area(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


signals:
    void endRubber();
    void redraw();
    void penDraw();
    void drawFirstPoint();
    void finishDrawPen();
    void readyToFill();
    void sprayDraw();
    void getColor();
    void setInfo();
    void drawShape();
    void viewZoom();


    private:
    QRubberBand *rubb;
    int originX, originY;
    bool isRubb = false;
    bool rightPressed=false;
    int six =0;

};

#endif // AREA_H
