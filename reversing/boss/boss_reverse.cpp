#include "boss_reverse.h"
#include "../draw_utils.h"
#define rep(i,n) for(int i=0;i<((int)(n));i++)



void boschara::get(){
	int necx = getenumecx(0x4be3c8,0x416290);
	if(necx!=0){
		int b = necx + 0x4e00 + 0x28a0;
		//ods("bospx : %x",b);
		p = getfposconv(necx + 0x4e00 + 0x28a0);
		col = getfpos(necx + 0x4e00 + 0x28dc);
		sp = getfpos(necx + 0x4e00 + 0x28b8);
		//�Ȃ񂩂ӂ��Ƃ�ł�B���A�����ĂȂ��B
		//������̂�4bded8. �ŁA�ʂ�1.0�ł��ˁ[�A�ƁB
	}
}

void boschara::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	//ods("%f %f : %f %f",p.y,p.x,col.y,col.x);
	drawbox(hdc,p.y-col.y/2,  p.y+col.y/2,  p.x-col.x/2,  p.x+col.x/2);
	MoveToEx(hdc , p.x, p.y  , NULL);
	LineTo(hdc , p.x + sp.x*10 , p.y + sp.y*10);
}

void boschara::step(){
	if(sp.norm()<=1000.0){
		p = p + sp;
	}
}


