#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "savecam.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class Area;
class BorderRight;
class BorderBottom;
class Angolo;
class selectionArea;
class shapeArea;
class curveLineArea;
class zoomArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
virtual void keyPressEvent(QKeyEvent *ev) override;
void closeEvent(QCloseEvent *ev) override;
void resizeEvent(QResizeEvent *event) override;

private slots:
  void reSize();
  void imgSave();
  void savePix(QPixmap pixToSave, QString f);
  void newImage(QString from);
  QCursor rectCursor();
  void updateInfo();
  void untoggle();
  void deleteSel();
  void createSelectionFromRubb();
  void createSelection();
  void drawCopy();
  void pasteImg(QPixmap passedPix);
  QPixmap addTransparency(QPixmap passedPix, int opacity, int red, int green, int blue);
  void drawWithPen();
  void showPix();
  void save_previous(QString tx="");
  void fill_();
  bool isSimil(QColor k1, QColor k2, int affinity);
  void spray_draw();
  void get_color();
  void createShapeArea();
  void draw_shape();
  void finish_curve();
  void view_zoom();
  void zoom_change_color();
  void rotation(int a);
  void deg90(int a);
  void mirror(bool horizontal, bool vertical);


  void on_actionNew_triggered();

  void on_actionOpen_triggered();

  void on_actionSave_triggered();

  void on_actionSave_as_triggered();

  void on_actionClose_triggered();

  void on_undoButton_clicked();

  void on_selectionAreaButton_clicked();

  void on_drawTextButton_clicked();

  void on_actionSelect_all_triggered();

  void on_actionCopy_triggered();

  void on_actionPaste_from_clipboard_triggered();

  void on_actionPaste_from_file_triggered();

  void on_actionCopy_selection_to_file_triggered();

  void on_textLine_textEdited(const QString &arg1);

  void on_blackButton_clicked();

  void on_whiteButton_clicked();

  void on_greyButton_clicked();

  void on_redButton_clicked();

  void on_greenButton_clicked();

  void on_blueButton_clicked();

  void on_yellowButton_clicked();

  void on_magentaButton_clicked();

  void on_cyanButton_clicked();

  void on_transparentButton_clicked();

  void on_confirmTextButton_clicked();

  void on_addColorButton_clicked();

  void on_penButton_clicked();

  void on_flatcapButton_clicked();

  void on_roundcapButton_clicked();

  void on_fillButton_clicked();

  void on_similaritySlider_valueChanged(int value);

  void on_sprayButton_clicked();

  void on_pickerButton_clicked();

  void on_actionCopy_selection_no_clipboard_triggered();

  void on_lineButton_clicked();

  void on_lineWidthBox_valueChanged(int arg1);

  void on_shapeButton_clicked();

  void on_shapesCombo_currentIndexChanged(int index);

  void on_fillCheck_clicked(bool checked);

  void on_curveButton_clicked();

  void on_rotateLeftButton_clicked();

  void on_rotateRightButton_clicked();

  void on_actionSizes_triggered();

  void on_actionEffects_triggered();

  void on_actionColors_triggered();

  void on_actionMirror_triggered();

  void on_actionMirror_vertically_triggered();

  void on_actionTo_greyscale_triggered();

  void on_actionReduce_to_RGB_triggered();

  void on_actionInvert_colors_triggered();

  void on_actionZoom_triggered();

  void on_actionCreate_triggered();

  void on_actionSave_image_triggered();

  void on_actionDelete_camera_triggered();

  void on_deg90left_clicked();

  void on_deg90right_clicked();

  void on_historyCombo_activated(int index);

private:
    Ui::MainWindow *ui;
    Area *wArea;
    BorderRight *borderR;
    BorderBottom *borderB;
    Angolo *corner;
    QPixmap pix, prePix;
    QString activePathFile;
    selectionArea *selectionRect=nullptr;
    int repeatShow=1 ;  int stopRepeatShow = 15;
    shapeArea *shape_area;
    curveLineArea *cl_area;
    saveCam *save_area;
    bool isCam = false;
    zoomArea *zoom_area=nullptr;
    int posXofZoom, posYofZoom;
    QStringList historyList;
    QList <QPixmap> historyPix;


};
#endif // MAINWINDOW_H
