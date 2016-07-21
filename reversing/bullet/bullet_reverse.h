#ifndef BULLET_REVERSE_H
#define BULLET_REVERSE_H

#include "../rev_utils.h"
#include<windows.h>

#include<vector>
using namespace std;

struct bullet{
	fpos p,sp,col;
	void draw(HDC& hdc);
};

vector<bullet> enubal(int necx);



#endif
