#include "rev_replstate.h"
#include "defines.h"

void replstate::out(){
	//sprintf(ods,"%d %d ms %d pt %d",lv,sc,ms,pt);
	//ODS(ods);
	ods("%d %d ms %d pt %d",lv,sc,ms,pt);
}

void replstate::get(){
	int addr = 0x4c4e74;
	int ap = (int)(*(int*)(addr));
	if(ap==0){
		lv=-1;
		ms=-1;
		return;
	}
	int ministrpos = (*(int*)(ap + 0x8));
	lv = *((char*)(ministrpos + 0x2));
	sc = *((char*)(ministrpos + 0x3));
	pt = *((int*)(ministrpos + 0x14));
	ms = (*(int*)(ap + 0x1c));
	//Ú×‚Íreversing/forida/anal ‚ğŒ©‚æ
	
	//pt‚Í€‚ñ‚¾‚Æ‚«‚É“¾‚ç‚ê‚é‚»‚¤‚ÈB
	//st->out();
}