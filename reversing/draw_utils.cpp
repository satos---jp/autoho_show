#include "draw_utils.h"

#define rep(i,n) for(int i=0;i<((int)(n));i++)

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

void drawpol(HDC& hdc,vector<fpos> ps){
	int ls = ps.size();
	MoveToEx(hdc , ps[ls-1].x , ps[ls-1].y , NULL);
	rep(i,ls){
		LineTo(hdc , ps[i].x , ps[i].y);
	}
}
