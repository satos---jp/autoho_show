#ifndef BOSS_REVERSE_H
#define BOSS_REVERSE_H

#include "../rev_utils.h"
#include<windows.h>

struct boschara{
	fpos p,col,sp;
	void get();
	void draw(HDC& hdc);
	void step();
};

#endif
