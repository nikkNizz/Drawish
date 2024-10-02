#ifndef SAVECAM_H
#define SAVECAM_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QSize>

class saveCam : public QLabel
{
     Q_OBJECT
public:
   explicit saveCam(QWidget *parent = nullptr, QSize sz=QSize(10,10));
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;



private:
    int ax, ay, gx, gy;
};

#endif // SAVECAM_H
