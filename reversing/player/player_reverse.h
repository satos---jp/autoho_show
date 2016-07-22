#ifndef PLAYER_REVERSE_H
#define PLAYER_REVERSE_H

#include "../rev_utils.h"
#include<windows.h>

struct mychara{
	fpos p,col;
	float sp[4];
	float slor; //“Å–¶‚Æ‚©‚Å’x‚­‚È‚é‚â‚Â
	float fil; //ƒJƒƒ‰‚Ì[“U—¦
	void draw(HDC& hdc);
	void normalize();
	double distfromwall();
};

mychara getmydata();

#endif
