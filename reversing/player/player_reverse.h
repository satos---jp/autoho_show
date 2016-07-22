#ifndef PLAYER_REVERSE_H
#define PLAYER_REVERSE_H

#include "../rev_utils.h"
#include<windows.h>

struct mychara{
	fpos p,col;
	float sp[4];
	float slor; //毒霧とかで遅くなるやつ
	float fil; //カメラの充填率
	void draw(HDC& hdc);
	void normalize();
	double distfromwall();
};

mychara getmydata();

#endif
