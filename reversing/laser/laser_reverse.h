#ifndef LASER_REVERSE_H
#define LASER_REVERSE_H

#include "../rev_utils.h"
#include<windows.h>
#include<vector>
using namespace std;

struct laser{
	fpos p,v; //��_�ʒu�A��_�̈ړ����x
	float xt,yt,tt,vt;
	void draw(HDC& hdc);
};

vector<laser> enulaser(int necx);

#endif
