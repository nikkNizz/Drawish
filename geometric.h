#ifndef GEOMETRIC_H
#define GEOMETRIC_H
#include <QColor>
#include <QString>

struct sizes
{
  static int areaWidth;
  static int areaHeight;
  static bool startResize;
  static int activeOperation; //1=selection  2=text 3=pen 4=fill 5=spray  6=picker 7=line 8=shape 9=curve
// selection geometry
  static int selX;
  static int selY;
  static int selW;
  static int selH;
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
};

#endif // GEOMETRIC_H
