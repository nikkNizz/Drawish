#ifndef TRACER_H
#define TRACER_H

#include <QLabel>
#include <QWidget>
#include <QObject>

class tracer : public QLabel
{
    Q_OBJECT
public:
    explicit tracer(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resetGeometry();

signals:
    void vanish();

private:
    QPixmap aPix;
    QPoint mousePos, prePos;
    int red = 255, blue = 128, green = 0;
    int drawStep;
};

#endif // TRACER_H
