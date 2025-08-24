#ifndef SHAPEAREA_H
#define SHAPEAREA_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class shapeArea : public QLabel
{
    Q_OBJECT
public:
    explicit shapeArea(QWidget *parent = nullptr, double Zoomy = 1.0);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resetGeometry(bool redraw = false);
    double Zoom;

signals:
    void setInfo();

private slots:
    //void redrawLine(int to_x, int to_y, int from_x,int from_y );
    QBrush bru();
    void drawSomething();

private:
    bool pressed=false;
    QString whereExp; // top, bottom, left right
    int preX, preY;
    int diffx, diffy;


};

#endif // SHAPEAREA_H
