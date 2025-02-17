#ifndef STRETCHVIEW_H
#define STRETCHVIEW_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPixmap>

class stretchView : public QLabel
{
    Q_OBJECT

public:
    explicit stretchView(QWidget *parent = nullptr, QPixmap pxm= QPixmap(100,100));
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resetGeometry();

    int x, y, w, h;
    QPixmap sPix;

private slots:
    void deform();

private:
    bool pressed=false;
    QString whereExp; // top, bottom, left right
    int preX, preY;
    int diffx, diffy;
    QString data;
    int x1, y1;

};

#endif // STRETCHVIEW_H
