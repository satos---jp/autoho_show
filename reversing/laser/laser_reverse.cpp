#include "laser_reverse.h"
#include "../draw_utils.h"

#include<cmath>
#define rep(i,n) for(int i=0;i<((int)(n));i++)




laser getlasp(int r){
	//ods("laserr %x ",r);

	laser res;
	//return res;
	if(r==0)return res;
	float cr = 8.0;
	float ct = *(float*)(r+0x34); //rad.and arg4
	res.tt = ct;
	
	res.vt = *(float*)(r+0x6c);
	
	/*
	fpos ag0 = fpos(cr*cos(ct),cr*sin(ct)) + getfpos(r + 0x1c);
	ag0 = ag0 + getfpos(0x4c4e70 + 0x1e30); //4c6ca0
	ag0 = ag0 + getfpos(r + 0x1e0); //•\Ž¦•ª‚¾‚¯‚¸‚ê‚é
	res.p = ag0;
	*/
	
	res.p = getfpos(r + 0x1c);
	res.p.x += 128.0 +192.0;
	res.p.y += 16.0;
	
	res.v = getfpos(r + 0x5c);

	float ag8 = *(float*)(r+0x3c);
	if(ag8>32)ag8 -= 16;
	else ag8/= 2.0;
	res.xt = ag8; 
	
	res.yt = *(float*)(r+0x38) - 16.0;
	
	return res;
}

vector<laser> enulaser(int necx){
	//ods("laserecxs %x ",necx);
	vector<laser> res;
	if(necx==0)return res;
	necx = *(int*)(necx+0x8);
	//int d = 0;
	while(necx){
		res.push_back(getlasp(necx));
		necx = *(int*)(necx+0x8);
		//d++;
		//if(d>10)break;
	}
	return res;
}

void laser::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	//ods("%f %f : %f %f %f",p.y,p.x,xt,yt,tt);
	
	vector<fpos> ps;
	ps.push_back(fpos(xt,8.0));
	ps.push_back(fpos(xt,yt+8.0));
	ps.push_back(fpos(-xt,yt+8.0));
	ps.push_back(fpos(-xt,8.0));
	
	rep(i,ps.size()){
		float y=ps[i].y,x=ps[i].x;
		ps[i]=fpos(y*cos(tt)+x*sin(tt),x*cos(tt)-y*sin(tt));
		ps[i] = ps[i] + p;
	}
	
	HPEN yellowpen = CreatePen(PS_SOLID , 0 ,RGB(255,255,0)); //3
	//HPEN bluepen = CreatePen(PS_SOLID , 0 ,RGB(0,0,255)); //3
	//DeleteObject(bluepen);
	
	SelectObject(hdc , yellowpen);
	drawpol(hdc,ps);
	//drawcircle(hdc,p,10);
	//drawcircle(hdc,p,5);
	
	/*
	SelectObject(hdc , bluepen);
	drawcircle(hdc,p2,10);
	*/
	
	DeleteObject(yellowpen);
}
