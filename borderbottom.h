#ifndef BORDERBOTTOM_H
#define BORDERBOTTOM_H

#include <QWidget>
#include <QLabel>

class BorderBottom : public QLabel
{
        Q_OBJECT
public:
    explicit BorderBottom(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resetGeometry();

signals:
    void sizeChange();

private:
   int exP;
};

#endif // BORDERBOTTOM_H
