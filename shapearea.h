#ifndef SHAPEAREA_H
#define SHAPEAREA_H

#include <QObject>
#include <QWidget>
#include <QLabel>

class shapeArea : public QLabel
{
    Q_OBJECT
public:
    explicit shapeArea(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void redrawForCopy();
    void resetGeometry(bool redraw = false);
    void drawSomething();

signals:
    void setInfo();

private slots:
    //void redrawLine(int to_x, int to_y, int from_x,int from_y );
    QBrush bru();


private:
    bool pressed=false;
    QString whereExp; // top, bottom, left right
    int preX, preY;
    int diffx, diffy;
    bool useZoom = true ;

};

#endif // SHAPEAREA_H
