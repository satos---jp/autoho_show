#include "rev_replstate.h"
#include "defines.h"
#include "rev_utils.h"

void replstate::out(){
	//sprintf(ods,"%d %d ms %d pt %d",lv,sc,ms,pt);
	//ODS(ods);
	//ods("%d %d ms %d pt %d",lv,sc,ms,pt);
}


void replstate::get(){
	int addr = 0x4c4e74;
	int ap = (int)(*(int*)(addr));
	if(ap==0){
		ms=-1;
		pt=-1;
		//return;
	}
	else{
		int ministrpos = (*(int*)(ap + 0x8));
		//lv = *((char*)(ministrpos + 0x2));
		//sc = *((char*)(ministrpos + 0x3));
		pt = *((int*)(ministrpos + 0x14));
		ms = (*(int*)(ap + 0x1c));
		//Ú×‚Íreversing/forida/anal ‚ðŒ©‚æ
	}
	
	
	int necx = getenumecx(0x4be3c8 + 0x20,0x42a6e0);
	if(necx!=0){
		stan = *(int*)(necx + 0x4);
	}
	else stan= -1;
	
	necx = getenumecx(0x4be3c8 + 0x20,0x445e60);
	//ods("necx bef",necx);
	if(necx!=0){
		stgidx = *(int*)(necx + 0x0bf0);
		//ods("aft",stgidx);
	}
	else stgidx= -1;

	
	//pt‚ÍŽ€‚ñ‚¾‚Æ‚«‚É“¾‚ç‚ê‚é‚»‚¤‚ÈB
	//st->out();
}

void replstate::draw(HDC& hdc){
	char s[256];
	sprintf(s,"pt: %d, ms: %d stan: %d, stgidx: %d",pt,ms,stan,stgidx);
	//TextOut(hdc,100,450,s,strlen(s));
	//TextOut(hdc,100,460,s,strlen(s));
	TextOut(hdc,100,465,s,strlen(s));

	//ods("%d %d ms %d pt %d",lv,sc,ms,pt);
}

