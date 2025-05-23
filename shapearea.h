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
    void resetGeometry();

signals:
    void setInfo();

private slots:
    
    QBrush bru();
    void drawSomething();

private:
    bool pressed=false;
    QString whereExp; // top, bottom, left right
    int preX, preY;
    int diffx, diffy;

};

#endif // SHAPEAREA_H
