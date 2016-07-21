#include "rev_utils.h"

fpos getfposconv(int b){ //位置補正をかけたやつ
	if(b==0)return fpos();
	fpos p = getfpos(b);
	p.x += 128 + 192; p.y += 16;
	return p;
}


fpos getfpos(int b){
	if(b==0)return fpos();
	float x = *(float*)(b);
	float y = *(float*)(b+0x4);
	return fpos(y,x);
}

int getenumecx(int base,int tofp){
	for(;;){
		int tp = *(int*)(base + 0x14);
		int toecx = *(int*)(base + 0x1c);
		int toecxf = *(int*)(base + 0x4);
		if(toecxf==tofp)return toecx;
		if(tp!=0)base = tp;
		else break;
	}
	return 0;
}
