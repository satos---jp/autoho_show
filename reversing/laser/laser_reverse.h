#ifndef LASER_REVERSE_H
#define LASER_REVERSE_H

#include "../rev_utils.h"
#include<windows.h>
#include<vector>
using namespace std;

struct laser{
	fpos p,v; //基点位置、基点の移動速度
	float xt,yt,tt,vt;
	void draw(HDC& hdc);
};

vector<laser> enulaser(int necx);

#endif
