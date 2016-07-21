#ifndef REVERSER_H
#define REVERSER_H

#include <vector>
#include <windows.h>

struct fpos{
	float y,x;
	fpos(){
		y=x=0;
	}
	fpos(float iy,float ix){
		y=iy; x=ix;
	}
	fpos const operator+(const fpos& a){
		return fpos(a.y+y,a.x+x);
	}
	fpos const operator-(const fpos& a){
		return fpos(y-a.y,x-a.x);
	}
	bool const operator==(const fpos& a){
		return (y==a.y && x==a.x);
	}
	bool const operator<(const fpos& a){
		if(y!=a.y)return (y<a.y);
		return (x<a.x);
	}
	double dist(fpos p);
};

struct mychara{
	fpos p,col;
	float sp[4];
	float slor; //“Å–¶‚Æ‚©‚Å’x‚­‚È‚é‚â‚Â
	float fil; //ƒJƒƒ‰‚Ì[“U—¦
	void draw(HDC& hdc);
	void normalize();
};

struct boschara{
	fpos p,col,sp;
	void draw(HDC& hdc);
};

struct bullet{
	fpos p,sp,col;
	void draw(HDC& hdc);
};

struct laser{
	fpos p,v; //Šî“_ˆÊ’uAŠî“_‚ÌˆÚ“®‘¬“x
	float xt,yt,tt,vt;
	void draw(HDC& hdc);
};

struct revdata{
	boschara bos;
	mychara my;
	std::vector<bullet> buls;
	std::vector<laser> lasers;
	
	void draw(HDC& hdc);
	
	void step();
	void select(double d);
	bool isdie();
};

void getrevdata(revdata& r);

#endif