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
    int toColor=0; // 0=toYellow 1= togreen 2= toblue 3= tomagenta 4 = toorange
    int red, green, blue;
    int drawStep;
};

#endif // TRACER_H
