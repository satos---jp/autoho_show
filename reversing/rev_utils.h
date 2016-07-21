#ifndef REV_UTILS_H
#define REV_UTILS_H

#include "../defines.h"

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

fpos getfpos(int b);
int getenumecx(int base,int tofp);


fpos getfposconv(int b);



#endif
