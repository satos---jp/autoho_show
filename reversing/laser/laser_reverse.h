#ifndef LASER_REVERSE_H
#define LASER_REVERSE_H

#include "../rev_utils.h"
#include "../player/player_reverse.h"

#include<windows.h>
#include<vector>
using namespace std;

struct laser{
	fpos p,v; //��_�ʒu�A��_�̈ړ����x
	float tt,vt; //���[�U�[�p�A���[�U�[�p���x
	float xcol,ycol; //�c�A�������̂����蔻��
	void draw(HDC& hdc);
	void step();
	bool iscol(mychara& mc);
};

vector<laser> enulaser();

#endif
