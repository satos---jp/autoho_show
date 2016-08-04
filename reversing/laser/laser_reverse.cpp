#include "laser_reverse.h"
#include "../draw_utils.h"

#include<cmath>
#define rep(i,n) for(int i=0;i<((int)(n));i++)




laser getlasp(int r){
	//ods("laserr %x ",r);

	laser res;
	if(r==0)return res;
	res.tt = *(float*)(r+0x34); //rad.and arg4
	res.vt = *(float*)(r+0x6c);
	
	/*
	fpos ag0 = fpos(cr*cos(ct),cr*sin(ct)) + getfpos(r + 0x1c);
	ag0 = ag0 + getfpos(0x4c4e70 + 0x1e30); //4c6ca0
	ag0 = ag0 + getfpos(r + 0x1e0); //表示分だけずれる
	res.p = ag0;
	*/
	
	res.p = getfposconv(r + 0x1c);
	//res.v = getfpos(r + 0x5c);
	res.v = getfpos(r + 0x28);
	//swap(res.v.x,res.v.y);
	

	float ag8 = *(float*)(r+0x3c);
	if(ag8>32)ag8 -= 16;
	else ag8/= 2.0;
	res.xcol = ag8; 
	
	//res.xcol = max(res.xcol,(float)5.0); //避けてもらうために広大広告する。
	
	res.ycol = *(float*)(r+0x38);
	//res.ycol -= 16.0;
	
	return res;
}

vector<laser> enulaser(){
	int necx = getenumecx(0x4be3c8,0x41db00);
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

mychara mmc;

void laser::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	ods("%f %f : %f %f ",p.y,p.x,v.x,v.y);
	
	vector<fpos> ps;
	/*
	//縦方向、合ってないっぽい。
	ps.push_back(fpos(xcol,8.0));
	ps.push_back(fpos(xcol,ycol+8.0));
	ps.push_back(fpos(-xcol,ycol+8.0));
	ps.push_back(fpos(-xcol,8.0));
	*/

	ps.push_back(fpos(xcol/2.0,0.0));
	ps.push_back(fpos(xcol/2.0,ycol));
	ps.push_back(fpos(-xcol/2.0,ycol));
	ps.push_back(fpos(-xcol/2.0,0.0));

	rep(i,ps.size()){
		float y=ps[i].y,x=ps[i].x;
		ps[i]=fpos(y*cos(tt)+x*sin(tt),x*cos(tt)-y*sin(tt));
		ps[i] = ps[i] + p;
	}
	drawpol(hdc,ps);
	
	MoveToEx(hdc , p.x,  p.y  , NULL);
	LineTo(hdc , p.x + v.x * 10.0 ,  p.y + v.y * 10.0);
	
	MoveToEx(hdc , p.x,  p.y  , NULL);
	fpos iv;
	iv.x = cos(tt); iv.y = sin(tt);
	LineTo(hdc , p.x + iv.x * vt *  10.0 ,  p.y + iv.y * vt * 10.0);
	//そもそも回るレーザ―がないのでなんとも...
	
	//drawcircle(hdc,p,10);
	//drawcircle(hdc,p,5);
	
	/*
	fpos dp = mmc.p - p;
	fpos tp;
	tp.x = cos(-tt) * dp.x - sin(-tt) * dp.y;
	tp.y = sin(-tt) * dp.x + cos(-tt) * dp.y;
	tp = tp + p;
	
	MoveToEx(hdc , p.x,  p.y  , NULL);
	LineTo(hdc , tp.x , tp.y);
	*/
	//ods("drawn\n");
}

void laser::step(){
	tt = tt + vt;
	p = p + v;
}

/*
double laser::dist(mychara& mc){
	mmc = mc;
	//mycharaは、pとcolでとれる。
	//sub_430530には、pos,tt,dx,dyの順で引数が入っている。
	fpos dp = mc.p - p;
	fpos tp;
	tp.x = cos(-tt) * dp.x - sin(-tt) * dp.y;
	tp.y = sin(-tt) * dp.x + cos(-tt) * dp.y;
	swap(tp.x,tp.y); //90度回る。

	double mind = 1e10;
	mind = min(mind, ycol - (tp.y - mc.col.y 
	mind = max(mind,0.0);
	return mind;
}
*/
bool laser::iscol(mychara& mc){
	mmc = mc;
	//mycharaは、pとcolでとれる。
	//sub_430530には、pos,tt,dx,dyの順で引数が入っている。
	fpos dp = mc.p - p;
	fpos tp;
	tp.x = cos(-tt) * dp.x - sin(-tt) * dp.y;
	tp.y = sin(-tt) * dp.x + cos(-tt) * dp.y;
	swap(tp.x,tp.y); //90度回る。
	//ods("fr %f %f %f rg %f %f: to %f %f\n",p.y,p.x,tt,ycol,xcol, tp.y,tp.x);
	//ods("dp %f %f\n",dp.y,dp.x);
	if(-3.0 + tp.y - mc.col.y > ycol)return false;
	if(-3.0 + tp.x - mc.col.x > xcol / 2.0)return false;
	if(3.0 + tp.y + mc.col.y < 0)return false;
	if(3.0 + tp.x + mc.col.x < -xcol / 2.0)return false;
	
	return true;
}

/*
[ebp+varC] = (mc.p - p).turn(-tt); //call    sub_41B600 まで。

[ebp+var10] = [ebp+var50] = (turnedp - nazop);
[ebp+var4] = [ebp+var34] = [ebp+var60] = (turnedp + nazop);

comp .. var18 と argc. なにか。
comp .. var14 と arg8/2
comp .. varC  と 0
comp .. -arg8/2とvar8.

*/
