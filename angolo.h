#ifndef ANGOLO_H
#define ANGOLO_H
#include <QWidget>
#include <QLabel>

class Angolo : public QLabel
{
    Q_OBJECT
public:
  explicit Angolo(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resetGeometry();

signals:
    void sizeChange();

private:
    int exx, exy;
};

#endif // ANGOLO_H
