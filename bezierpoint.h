#ifndef BEZIERPOINT_H
#define BEZIERPOINT_H

#include <qlabel.h>
#include <QWidget>
#include <QPoint>

class bezierPoint : public QLabel
{
    Q_OBJECT
public:
    explicit bezierPoint(QWidget *parent = nullptr, int ax=0, int ay =0, QString tx="1");
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    QPoint pPoint;

signals:
    void curving();

private:

    int globX1, globY1;
};

#endif // BEZIERPOINT_H
