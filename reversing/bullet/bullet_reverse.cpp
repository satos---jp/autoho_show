#include "bullet_reverse.h"
#include "../draw_utils.h"
#define rep(i,n) for(int i=0;i<((int)(n));i++)


bullet getbfp(int b){
	bullet res;
	if(b==0)return res;
	res.p = getfposconv(b + 0x2d0);
	res.sp = getfpos(b + 0x2dc);
	res.col = getfpos(b + 0x30c);
	return res;
}

vector<bullet> enubal(int necx){
	//ods("bs %x ",necx);
	vector<bullet> res;
	if(necx==0)return res;
	for(int i=0;i<6;i++){
		int nbp = *(int*)(necx+i*4+4);
		for(;;){
			if(nbp==0)break;
			else{
				//int n = *(int*)(nbp+0x22c);
				//ods("%x",nbp);
				//res.push_back(getbfp(nbp+0x14c));
				//ÇΩÇﬂÇµÇ… -> çáÇ¡ÇƒÇÈÅ[ÅB
				res.push_back(getbfp(nbp));
				nbp = *(int*)(nbp+0x358);
			}
		}
	}
	return res;
}

void bullet::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	drawbox(hdc,p.y-col.y/2,  p.y+col.y/2,  p.x-col.x/2,  p.x+col.x/2);
	MoveToEx(hdc , p.x, p.y  , NULL);
	LineTo(hdc , p.x + sp.x*10 , p.y + sp.y*10);
}


void bullet::step(){
	p = p + sp;
}
