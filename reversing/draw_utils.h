#ifndef DRAW_UTILS_H
#define DRAW_UTILS_H

#include "rev_utils.h"
#include<windows.h>
#include<vector>
using namespace std;

void drawcircle(HDC& hdc,fpos p,int r);

void drawbox(HDC& hdc,int u,int d,int r,int l);

void drawpol(HDC& hdc,vector<fpos> ps);


#endif
