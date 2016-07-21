#include "reverser.h"
#include "defines.h"
using namespace std;

#include<cstdio>
#include<cmath>

#define rep(i,n) for(int i=0;i<((int)(n));i++)

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

fpos getfpos(int b){
	if(b==0)return fpos();
	float x = *(float*)(b);
	float y = *(float*)(b+0x4);
	return fpos(y,x);
}

fpos getfposconv(int b){ //位置補正をかけたやつ
	if(b==0)return fpos();
	fpos p = getfpos(b);
	p.x += 128 + 192; p.y += 16;
	return p;
}

void getspped(){ //自機の移動速度
	
	int nec = getenumecx(0x4be3c8,0x430180);
	if(nec!=0){
		int b = *(int*)(nec+0x410);
		if(b!=0){
			ods("sppedp %x",b);
			int d[4] = {0x18,0x20,0x14,0x1c};
			rep(i,4){
				float p = *(float*)(b+d[i]);
				ods("speed %d : %f",i,p);
			}
			
			float p = *(float*)(nec+0x2a18);
			ods("slower : %f",p); 
			// 2.3, 2.3/sqrt(2), 5, 5/sqrt(2)
			return;
		}
	}
	
	ods("speednull");
}

mychara getmydata(){
	int necx;
	mychara res;
	necx = getenumecx(0x4be3c8 + 0x20,0x430210);
	if(necx!=0){
		res.p = getfposconv(necx + 0x1e30);
		res.col = getfpos(necx + 0x3d8);
	}
	necx = getenumecx(0x4be3c8,0x430180);
	if(necx!=0){
		int b = *(int*)(necx+0x410);
		if(b!=0){
			//ods("sppedp %x",b);
			int d[4] = {0x18,0x20,0x14,0x1c};
			// 2.3, 2.3/sqrt(2), 5, 5/sqrt(2)
			rep(i,4){
				res.sp[i] = *(float*)(b+d[i]);
				ods("speed %d : %f",i,res.sp[i]);
			}
			
			res.slor = *(float*)(necx+0x2a18);
			ods("slower : %f",res.slor); 
		}
		
		res.fil = *(float*)(necx + 0xb80 + 0x1e3c);
	}
	return res;
}

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

//bool bo = false;


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
				//ためしに -> 合ってるー。
				res.push_back(getbfp(nbp));
				nbp = *(int*)(nbp+0x358);
			}
		}
	}
	return res;
}

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
	ag0 = ag0 + getfpos(r + 0x1e0); //表示分だけずれる
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
	ods("laserecxs %x ",necx);
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


void getcorn(){ //0x430450のecxの. あたり判定につかわれてる謎のやつ。
	int b = 0x4c4e70;
	b = *(int*)(b);
	if(b==0){
		ods("cornull");
		return;
	}
	b += 0x3a8;
	ods("corp %x",b);
	rep(i,4){
		int t = i>=2?i+1:i;
		float p = *(float*)(b + 4*t);
		ods("corn %d: %f",i,p);
	}
	//x y x y. 微妙にずれか？ (+16とか, 128 + 192 とかいりそう。)
}

void getrevdata(revdata& r){
	
	r.my = getmydata();
	r.bos = getbosdata();
	int bup = getenumecx(0x4be3c8 + 0x20,0x405a00);
	r.buls = enubal(bup);
	
	int lap = getenumecx(0x4be3c8,0x41db00);
	r.lasers = enulaser(lap);
	
	//ods("myp: %f %f",r.myp.x,r.myp.y);
	//getcorn();
	//getspped();
}

void drawcircle(HDC& hdc,fpos p,int r){
	Arc(hdc , p.x-r , p.y-r , p.x+r , p.y+r , p.x , p.y-r , p.x , p.y-r);
}

void drawbox(HDC& hdc,int u,int d,int r,int l){
	MoveToEx(hdc , l, u  , NULL);
	LineTo(hdc , r , u);
	LineTo(hdc , r , d);
	LineTo(hdc , l , d);
	LineTo(hdc , l , u);
}

void revdata::draw(HDC &hdc){
	
	
	HBRUSH blackbrush = CreateSolidBrush(RGB(0,0,0)); //3
	/*
	//背後を黒にする一般的なテク
	SelectObject(hdc , blackbrush);
	Rectangle(hdc,0,0,640,480);
	*/
	
	//自機描画
	HPEN yellowpen = CreatePen(PS_SOLID , 0 ,RGB(255,255,0)); //3
	SelectObject(hdc , yellowpen);
	my.draw(hdc);
	
	//敵機描画
	HPEN bluepen = CreatePen(PS_SOLID , 0 ,RGB(0,0,255)); //3
	SelectObject(hdc , bluepen);
	bos.draw(hdc);
	
	//敵弾描画
	HPEN greenpen = CreatePen(PS_SOLID , 0 ,RGB(0,255,0)); //3
	SelectObject(hdc , greenpen);
	int ls = buls.size();
	//if(ls>10)ls=10;
	rep(i,ls)buls[i].draw(hdc);
	
	
	DeleteObject(blackbrush);
	DeleteObject(yellowpen);
	DeleteObject(bluepen);
	DeleteObject(greenpen);
	
	/*
	
	int ls = lasers.size();
	rep(i,ls)lasers[i].draw(hdc);
	*/
}

void bullet::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	drawbox(hdc,p.y-col.y/2,  p.y+col.y/2,  p.x-col.x/2,  p.x+col.x/2);
	MoveToEx(hdc , p.x, p.y  , NULL);
	LineTo(hdc , p.x + sp.x*10 , p.y + sp.y*10);
}

void mychara::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	ods("mychara %f %f : %f %f : %f",p.y,p.x,col.y,col.x,fil);
	drawbox(hdc,p.y-col.y/2,  p.y+col.y/2,  p.x-col.x/2,  p.x+col.x/2);
}

void boschara::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	ods("%f %f : %f %f",p.y,p.x,col.y,col.x);
	drawbox(hdc,p.y-col.y/2,  p.y+col.y/2,  p.x-col.x/2,  p.x+col.x/2);
	MoveToEx(hdc , p.x, p.y  , NULL);
	LineTo(hdc , p.x + sp.x*10 , p.y + sp.y*10);
}


void drawpol(HDC& hdc,vector<fpos> ps){
	int ls = ps.size();
	MoveToEx(hdc , ps[ls-1].x , ps[ls-1].y , NULL);
	rep(i,ls){
		LineTo(hdc , ps[i].x , ps[i].y);
	}
}

void laser::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	ods("%f %f : %f %f %f",p.y,p.x,xt,yt,tt);
	
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


void revdata::step(){
	if(bos.sp.dist(fpos())<=1000.0){
		bos.p = bos.p + bos.sp;
	}
	rep(i,buls.size()){
		bullet& b = buls[i];
		b.p = b.p + b.sp;
	}
}

void revdata::select(double d){
	vector<bullet> tb;
	rep(i,buls.size()){
		bullet b = buls[i];
		if(my.p.dist(b.p) < d)tb.push_back(b);
	}
	swap(buls,tb);
}

bool iscol(fpos pa,fpos ca,fpos pb,fpos cb){
	if(abs(pa.y-pb.y) - 0.3 > ca.y + cb.y)return false; //1でよいかな？
	if(abs(pa.x-pb.x) - 0.3 > ca.x + cb.x)return false;
	return true;
}

bool revdata::isdie(){
	if(iscol(my.p,my.col,bos.p,bos.col))return true;
	rep(i,buls.size()){
		bullet& b = buls[i];
		if(iscol(my.p,my.col, b.p,b.col))return true;
	}
	return false;
}

double fpos::dist(fpos p){
	return sqrt((p.x-x)*(p.x-x)+(p.y-y)*(p.y-y));
}


void mychara::normalize(){
	//y .. 48.0 to 452.0
	//x .. 136.0 to 504.0
	
	p.y = max(48.0,min(452.0,(double)p.y));
	p.x = max(136.0,min(504.0,(double)p.x));
}



