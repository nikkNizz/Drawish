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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap aPix;
    QPoint mousePos, prePos;
    int red = 255, green = 0;
    int redF =1, greenF =1;
    int drawStep;
};

#endif // TRACER_H
