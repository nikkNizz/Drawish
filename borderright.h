#ifndef BORDERRIGHT_H
#define BORDERRIGHT_H

#include <QWidget>
#include <QLabel>

class BorderRight : public QLabel
{
        Q_OBJECT
public:
    explicit BorderRight(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void resetGeometry();
signals:
    void sizeChange();

private:
   int exP;
};

#endif // BORDERRIGHT_H
