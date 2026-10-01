#ifndef SELECTIONAREA_H
#define SELECTIONAREA_H

#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QLabel>

class selectionArea : public QLabel
{
    Q_OBJECT
public:
    explicit selectionArea(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resetGeometry();

signals:
    void setInfo();
    void setCopy();
    void transpose();
    void setAsBrush();

private slots:
    void createActions();
    void copyClip();
    void useAsBrush();


private:
    bool pressed=false;
    QString whereExp; // top, bottom, left right
    int preX, preY;
    int diffx, diffy;
    QMenu *menu;
    QAction *copy; QAction *clear; QAction *asBrush;
    bool USEasBRUSH = false;


};

#endif // SELECTIONAREA_H
