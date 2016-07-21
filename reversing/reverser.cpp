#include "reverser.h"
#include "../defines.h"
#include "rev_utils.h"
using namespace std;

#include<cstdio>
#include<cmath>

#define rep(i,n) for(int i=0;i<((int)(n));i++)



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




