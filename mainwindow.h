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

    void dragEnterEvent(QDragEnterEvent *event)override;
    void dropEvent(QDropEvent *event) override;
    ~MainWindow();

protected:
virtual void keyPressEvent(QKeyEvent *ev) override;
void closeEvent(QCloseEvent *ev) override;
void resizeEvent(QResizeEvent *event) override;

private slots:
  void reSize();
  void areaSize();
  void raiseBorders();
  void imgSave();
  QString ChooseImg();
  void savePix(QPixmap pixToSave, QString f);
  void newImage(QString from, QString path="");
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
  void draw_first_point();
  void colorEraser();
  void showPix();
  void save_previous(QString tx="");
  void fill_();
  void fillInsideBlack();
  bool isSimil(QColor k1, QColor k2, int affinity);
  void spray_draw();
  void get_color();
  void set_activeColor(int R, int G, int B, int A=255);
  void createShapeArea();
  void draw_shape();
  void finish_curve();
  void finish_lines();
  void view_zoom();
  void zoom_change_color();
  void rotation(int a);
  void mirror(bool horizontal, bool vertical);
  QPixmap openPdf(QString fileName);
  void readConfig();
  QString midstring(QString from, QString tag);
  void open_file();
  void open_link();
  void addToRecent(QString pf);
  void removeFromRecent(QString pf);
  QStringList askForValues();
  double graphicFactor(QStringList sl, double graphDim);
  void addGraphic(QPixmap gr);
  QPen configPen(QColor& ncol, int alpha=12);
  void autoRotation();


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

  void on_actionCopy_selection_to_file_triggered();

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

  void on_fillButton_clicked();

  void on_similaritySlider_valueChanged(int value);

  void on_sprayButton_clicked();

  void on_pickerButton_clicked();

  void on_actionCopy_selection_no_clipboard_triggered();

  void on_lineButton_clicked();

  void on_lineWidthBox_valueChanged(int arg1);

  void on_shapeButton_clicked();

  void on_shapesCombo_currentIndexChanged(int index);

  void on_curveButton_clicked();

  void on_rotateLeftButton_clicked();

  void on_rotateRightButton_clicked();

  void on_actionEffects_triggered();

  void on_actionColors_triggered();

  void on_actionMirror_triggered();

  void on_actionMirror_vertically_triggered();

  void on_actionTo_greyscale_triggered();

  void on_actionReduce_to_RGB_triggered();

  void on_actionInvert_colors_triggered();

  void on_actionCreate_triggered();

  void on_actionSave_image_triggered();

  void on_actionDelete_camera_triggered();

  void on_historyCombo_activated(int index);

  void on_actionAbout_triggered();

  void on_actionCreate_Line_triggered();

  void on_actionGithub_triggered();

  void on_actionMerge_selection_to_image_triggered();

  void on_comboBox_activated(int index);

  void on_textEdit_textChanged();

  void on_actionbase64_triggered();

  void on_actionTo_Pdf_triggered();

  void on_actionAdd_as_selection_triggered();

  void on_actionAdd_right_triggered();

  void on_actionAdd_bottom_triggered();

  void on_actionStretch_area_triggered();

  void on_actionTo_sepia_triggered();

  void on_actionSet_triggered();

  void on_actionSizes_2_triggered();

  void on_actionQuadruple_the_pixels_2_triggered();

  void on_actionDivide_by_5_triggered();

  void on_actionZoom_2_triggered();

  void on_actionIncrement_10_triggered();  

  void on_conn_Curve_clicked();

  void on_actionAdd_link_triggered();

  void on_sizeLine_textChanged(const QString &arg1);

  void on_actionHorizontal_bars_triggered();

  void on_actionVertical_bars_triggered();

  void on_actionPie_triggered();

  void on_actionSet_as_restore_point_triggered();

  void on_actionRestore_triggered();

  void on_actionCreate_shape_triggered();

  void on_autoRotationButton_clicked();

  void on_comboPen_activated(int index);

  void on_actionTransparent_selection_toggled(bool arg1);

  void on_rtfButton_clicked();

  void on_actionMouse_correctio_triggered(bool checked);

  void on_boldButton_clicked();

  void on_italicButton_clicked();

  void on_underlineButton_clicked();

  void on_fontComboBox_currentFontChanged(const QFont &f);

  void on_actionSet_as_favorite_color_triggered();

  void on_prefColorButton_clicked();

  void on_actionPick_color_from_an_image_triggered();

  void on_comboPen_highlighted(int index);

  void on_resetFavColorButton_clicked();

  void on_actionDesktop_shortcut_triggered();

  void on_actionCreate_mask_triggered();

  void createLineArea(int op);

  void on_comboLines_currentIndexChanged(int index);

  void on_comboLines_highlighted(int index);

  private:
    Ui::MainWindow *ui;
    QString version_info;
    Area *wArea;
    BorderRight *borderR;
    BorderBottom *borderB;
    Angolo *corner;
    QPixmap pix, prePix;
    QString activePathFile;
    selectionArea *selectionRect=nullptr;
    shapeArea *shape_area;
    curveLineArea *cl_area;
    saveCam *save_area;
    bool isCam = false;
    zoomArea *zoom_area=nullptr;
    int posXofZoom, posYofZoom;
    QStringList historyList;
    QList <QPixmap> historyPix;
    int historyCount;
    bool isLinux;
    QString nextColumnToAdd="0593716482";
    QString configRecent, configLinks, configPath;
    QPixmap toRestore;
    QColor fav = Qt::white;
    //------------------------------
    QPixmap preRotatePix;
    int wPreRotate, hPreRotate;
    int preAngle =0;
    bool goRotate = false;
    //-------------------------------


};
#endif // MAINWINDOW_H
