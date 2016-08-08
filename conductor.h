#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "reversing/reverser.h"

struct joydata{
	int y,x,b;
	//button
	//1 .. Z
	//2 .. X
	//4 .. esc
	//x,y .. 500±300 で。 
	joydata(); //無操作で初期化する。
	void neutral(); //ニュートラルにする。
	void copy(joydata jo);
	void Z();
	void stick(int iy,int ix);
};


void ai_conduction(joydata* joy,revdata& revd);
void ai_init();






#endif
