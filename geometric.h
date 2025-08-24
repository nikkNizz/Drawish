#ifndef GEOMETRIC_H
#define GEOMETRIC_H
#include <QColor>
#include <QString>
#include <QPoint>

struct sizes
{
  static QString passedFile;
  static int areaWidth;
  static int areaHeight;
  static bool startResize;
  static int activeOperation; //1=selection  2=text 3=pen 4=fill 5=spray  6=picker 7=line 8=shape 9=curve
                              //10= conn_curve 11 = connLine 12=stretch 13= freehandSel 14= tracer
// selection geometry
  static int selX;
  static int selY;
  static float selW;
  static float selH;
  //----------------
  static bool isSelectionOn;
  static bool isShapeOn;
  static QColor activeColor;
  static bool modify;
  static int line_width;
  static int shape_x_begin;
  static int shape_y_begin;
  static int shape_x_end;
  static int shape_y_end;
  static QString activeShape;
  static int FillStyle;
  static bool isCurveLineAreaOn;
  static int zoomx;
  static int zoomy;
  static bool zoomEdited;
  static int lineXEnd;
  static int lineYEnd;
  static bool curveStretch;
  static QString savedRtf;
  static bool mouseCorrection;
  static bool isArrow;
  static bool isRotating;
  static QString selectData;
  static QList<QPoint> freeSelPoints;
  static QPoint autoPoint;
};

#endif // GEOMETRIC_H
