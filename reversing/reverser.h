#ifndef REVERSER_H
#define REVERSER_H

#include <vector>
#include <windows.h>

#include "player/player_reverse.h"
#include "boss/boss_reverse.h"
#include "bullet/bullet_reverse.h"
#include "laser/laser_reverse.h"
#include "rev_replstate.h"

struct revdata{
	boschara bos;
	mychara my;
	std::vector<bullet> buls;
	std::vector<laser> lasers;
	replstate state;
	
	void get();
	void draw(HDC& hdc);
	
	void step();
	void select(double d);
	bool isdie();
};

#endif