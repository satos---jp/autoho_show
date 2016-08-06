#ifndef CONDUCTOR_H
#define CONDUCTOR_H

#include "reversing/reverser.h"

struct joydata{
	int y,x,b;
	//button
	//1 .. Z
	//2 .. X
	//4 .. esc
};


void ai_conduction(joydata* jo,revdata& rd);
void ai_init();






#endif
