#include "geometric.h"

QString sizes::passedFile ="";
int sizes::areaHeight=328;
int sizes::areaWidth= 488;
bool sizes::startResize=false;
int sizes::activeOperation=0;
int sizes::selX=0;
int sizes::selY=0;
int sizes::selW=10;
int sizes::selH=20;
bool sizes::isSelectionOn=false;
bool sizes::isShapeOn=false;
bool sizes::modify = false;
QColor sizes::activeColor=QColor(0,0,0);
int sizes::line_width=6;
int sizes::shape_x_begin=40;
int sizes::shape_y_begin=40;
int sizes::shape_x_end=0;
int sizes::shape_y_end=0;
QString sizes::activeShape="squ";
int sizes::FillStyle = 0;
bool sizes::isCurveLineAreaOn = false;
int sizes::zoomx =0;
int sizes::zoomy =0;
bool sizes::zoomEdited=false;
int sizes::lineXEnd = 0;
int sizes::lineYEnd = 0;
bool sizes::curveStretch = false;
QString sizes::savedRtf = "";
bool sizes::mouseCorrection = false;
bool sizes::isArrow = false;
bool sizes::isRotating = false;
QString sizes::selectData ="0 0 0 0";
