#include "geometric.h"

int sizes::areaHeight=320;
int sizes::areaWidth= 480;
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
bool sizes::toFill = false;
bool sizes::isCurveLineAreaOn = false;
int sizes::zoomx =0;
int sizes::zoomy =0;
bool sizes::zoomEdited=false;