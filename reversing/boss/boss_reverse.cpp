#include "boss_reverse.h"
#include "../draw_utils.h"
#define rep(i,n) for(int i=0;i<((int)(n));i++)



boschara getbosdata(){
	int necx = getenumecx(0x4be3c8,0x416290);
	boschara res;
	if(necx!=0){
		int b = necx + 0x4e00 + 0x28a0;
		ods("bospx : %x",b);
		res.p = getfposconv(necx + 0x4e00 + 0x28a0);
		res.col = getfpos(necx + 0x4e00 + 0x28dc);
		res.sp = getfpos(necx + 0x4e00 + 0x28b8);
		//なんかふっとんでる。が、動いてない。
		//かけるのは4bded8. で、別に1.0ですねー、と。
	}
	return res;
}

fpos getbosxy(int necx){
	if(necx==0)return fpos();
	necx += 0x4e00 + 0x28a0;
	float x = *(float*)(necx);
	float y = *(float*)(necx+0x4);
	x += 320;
	return fpos(y,x);
}



void boschara::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	ods("%f %f : %f %f",p.y,p.x,col.y,col.x);
	drawbox(hdc,p.y-col.y/2,  p.y+col.y/2,  p.x-col.x/2,  p.x+col.x/2);
	MoveToEx(hdc , p.x, p.y  , NULL);
	LineTo(hdc , p.x + sp.x*10 , p.y + sp.y*10);
}




