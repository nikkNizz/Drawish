#ifndef ZOOMAREA_H
#define ZOOMAREA_H
#include <QWidget>
#include <QLabel>

class zoomArea : public QLabel
{
     Q_OBJECT
public:
    explicit zoomArea(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)override;

signals:
    void zoomChangeColor();
    void changeActiveColor();

};

#endif // ZOOMAREA_H
