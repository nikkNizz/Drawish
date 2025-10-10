#ifndef AREA_H
#define AREA_H

#include <QWidget>
#include <QLabel>
#include <QMenu>
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
    void setPaste();

private slots:
    void createMenu();
    void createActions();
    void actPaste();
    void actZoom();

    private:
    QRubberBand *rubb;
    int originX, originY;
    bool isRubb = false;
    bool rightPressed=false;
    int six =0;
    QMenu *menu;
    QAction *Paste; QAction *Zoomy;


};

#endif // AREA_H
