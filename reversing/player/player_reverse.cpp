#include "player_reverse.h"
#include "../draw_utils.h"

#define rep(i,n) for(int i=0;i<((int)(n));i++)

mychara getmydata(){
	int necx;
	mychara res;
	
	//位置と当たりを得る
	necx = getenumecx(0x4be3c8 + 0x20,0x430210);
	if(necx!=0){
		res.p = getfposconv(necx + 0x1e30);
		res.col = getfpos(necx + 0x3d8);
	}
	
	//速度を得る
	necx = getenumecx(0x4be3c8,0x430180);
	if(necx!=0){
		int b = *(int*)(necx+0x410);
		if(b!=0){
			//ods("sppedp %x",b);
			int d[4] = {0x18,0x20,0x14,0x1c};
			// 2.3, 2.3/sqrt(2), 5, 5/sqrt(2)
			rep(i,4){
				res.sp[i] = *(float*)(b+d[i]);
				//ods("speed %d : %f",i,res.sp[i]);
			}
			
			res.slor = *(float*)(necx+0x2a18);
			//ods("slower : %f",res.slor); 
		}
		
		res.fil = *(float*)(necx + 0xb80 + 0x1e3c);
	}
	return res;
}

void mychara::normalize(){
	//y .. 48.0 to 452.0
	//x .. 136.0 to 504.0
	
	p.y = max(48.0,min(452.0,(double)p.y));
	p.x = max(136.0,min(504.0,(double)p.x));
}

//壁からのキョリが近すぎるのはやばい。
double mychara::distfromwall(){
	//y .. 48.0 to 452.0
	//x .. 136.0 to 504.0
	double res = 1000;
	res = min(res,min(abs(p.y-48.0),abs(452.0-p.y)));
	res = min(res,min(abs(p.x-136.0),abs(504.0-p.x)));
	return res;
}

void mychara::draw(HDC &hdc){
	//ods("%f %f : %f %f : %f %f",p.y,p.x,sp.y,sp.x,col.y,col.x);
	//ods("mychara %f %f : %f %f : %f",p.y,p.x,col.y,col.x,fil);
	drawbox(hdc,p.y-col.y/2,  p.y+col.y/2,  p.x-col.x/2,  p.x+col.x/2);
}

