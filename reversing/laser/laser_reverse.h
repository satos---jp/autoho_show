#ifndef LASER_REVERSE_H
#define LASER_REVERSE_H

#include "../rev_utils.h"
#include "../player/player_reverse.h"

#include<windows.h>
#include<vector>
using namespace std;

struct laser{
	fpos p,v; //基点位置、基点の移動速度
	float tt,vt; //レーザー角、レーザー角速度
	float xcol,ycol; //縦、横方向のあたり判定
	void draw(HDC& hdc);
	void step();
	bool iscol(mychara& mc);
};

vector<laser> enulaser();

#endif
