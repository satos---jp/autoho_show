//gccでコンパイルの際には、extern(ry)  を、DllMainにも付けること。

#define EXPORT extern "C" __declspec(dllexport)

#include <windows.h>
#include <cstdio>

#include "defines.h"

#include "head_dlltest.h"
#include "ai_tes.h"

#include "rensya_window.h"
#include "debug_window.h"

#include "reversing/reverser.h"

/*
Joygetposexを乗っ取ります。

*/

typedef MMRESULT (WINAPI *Jgpex)(UINT ,LPJOYINFOEX);//Joygetposexの型

Jgpex mempo;

char ods[200];//OutputDebugString用

DWORD centd=0;
int ct=0,cp=0;


void replstate::out(){
	//sprintf(ods,"%d %d ms %d pt %d",lv,sc,ms,pt);
	//ODS(ods);
	ods("%d %d ms %d pt %d",lv,sc,ms,pt);
}

void getrepst(replstate* st){
	int addr = 0x4c4e74;
	int ap = (int)(*(int*)(addr));
	if(ap==0){
		st->lv=-1;
		st->ms=-1;
		return;
	}
	int ministrpos = (*(int*)(ap + 0x8));
	st->lv = *((char*)(ministrpos + 0x2));
	st->sc = *((char*)(ministrpos + 0x3));
	st->pt = *((int*)(ministrpos + 0x14));
	st->ms = (*(int*)(ap + 0x1c));
	//詳細はreversing/forida/anal を見よ
	
	//ptは死んだときに得られるそうな。
	st->out();
}




HDC nhdc;

//もうこいつだけでいいんじゃないか、という気分になったとさ。

bool sendd=false;
bool makedwin = false;
HWND debhwnd = NULL;
	

MMRESULT WINAPI Joygetposex_wrapper( UINT uJoyID, LPJOYINFOEX pji){

	MMRESULT res = mempo(uJoyID, pji);
	
	
	if(!makedwin){
		
		const char browsername[400]="東方文花帖　～ Shoot the Bullet. ver 1.02a";
		HWND hwnd = GetHwnd(browsername);
		if(hwnd==NULL){
			ods("hwndisnull");
		}
		HINSTANCE hi = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE); //64bit互換性はないらしい。
		if(hi==NULL){
			ods("hiisnull");
		}
		else{
			ods("hi in overrided joypos .. %x",hi);
			//sprintf(ods,"hi in overrided joypos .. %x",hi);
			//ODS(ods);
		}
		
		debhwnd = makewindow(hi); //できない。
		ods("debhwnd .. %x",debhwnd);
		//sprintf(ods,"debhwnd .. %x",debhwnd);
		//ODS(ods);
		
		//Sleep(10000);
		
		//ODS("sleeped");
		makedwin=true;
	}
	
	const char browsername[400]="東方文花帖　～ Shoot the Bullet. ver 1.02a";
	HWND hwnd = GetHwnd(browsername);


	revdata red;
	getrevdata(red);
	if(hwnd!=NULL){
		//ODS("cancap");
		HDC ghdc = CopyWindow(hwnd);
		debhwnd = GetHwnd("toho_debug_window");
		if(debhwnd!=NULL){
			sprintf(ods,"finddeb %x",debhwnd);
			//ODS(ods);

				
			red.draw(ghdc);
			PostMessage(debhwnd,WM_APP,0,((LPARAM)ghdc));
		}
		else{
			ods("can'finddeb");
		}
	}
	else{
		ods("can'tcap");
	}

	//ai_conductionによって、AIに制御させる。
	//red, には、ゲーム画面のlevel(lv),scene(sc),得点(pt),経過時間(ms)が入っている。
	//jo は、joipadのデータ。これで操作する。
	//red は、reverseによって分かった、敵機位置、自機位置、弾位置などが入っている。
	
	res=JOYERR_NOERROR;
	pji->dwFlags = JOY_RETURNALL;
	pji->dwButtons=0;
	pji->dwButtonNumber=3;
	
	
	replstate st;
	getrepst(&st);
	
	
	joydata jo;
	ai_conduction(&st,&jo,red);
	
	pji->dwXpos=jo.x;
	pji->dwYpos=jo.y;
	pji->dwButtons=jo.b;

	//pji->dwXpos=500;
	//pji->dwYpos=500;
	//pji->dwButtons=0;

	return res;
}


/*
g++ -c dlltest.cpp
g++ -shared -o dlltest.dll dlltest.o
g++ main.cpp
*/

PROC* gotpos = (PROC*)(4805224);

void WINAPI attach(){

  	OutputDebugString("override joyGetDevCapsA");
	//0x00495268 が joygetposex の got
	DWORD dummy;
	VirtualProtect(gotpos, sizeof(gotpos), PAGE_EXECUTE_READWRITE, &dummy);
	//ここで、書き込むとこのプロテクトを外してる。
	mempo = (Jgpex)(*gotpos);
	(*gotpos) = (PROC)Joygetposex_wrapper;
	VirtualProtect(gotpos, sizeof(gotpos), PAGE_EXECUTE_READ, &dummy);
	
	
	//あとは 0x419410 からのやつをいじる。
	//uint で、 xmin,xmax,ymin,ymax で
	
	//ODS("writed func");
	
	int stpi = (0x004bdf04); //xminのポインタ
	VirtualProtect((void*)stpi, 4 * sizeof(UINT), PAGE_EXECUTE_READWRITE, &dummy);
	//0 1000 0 1000 にする。
	for(int i=0;i<4;i++){
		UINT* p = (UINT*)(stpi + i * sizeof(UINT)); 
		(*p) = (UINT)((i%2)*1000);
	}
	
	VirtualProtect((void*)stpi, 4 * sizeof(unsigned int), dummy, &dummy);
	
	//ODS("writed struct");
	
	ai_init();
	
	const char browsername[400]="東方文花帖　～ Shoot the Bullet. ver 1.02a";
	HWND hwnd = GetHwnd(browsername);
	if(hwnd==NULL){
		ods("hwndisnull");
	}
	HINSTANCE hi = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE); //64bit互換性はないらしい。
	if(hi==NULL){
		ods("hiisnull");
	}
	else{
		ods("hi in attach.. %x",hi);
	}
}

void WINAPI detach_joygetposex(){
	if(gotpos ==NULL)return;

	DWORD dummy;
	VirtualProtect(gotpos, sizeof(gotpos), PAGE_EXECUTE_READWRITE, &dummy);
	//もっぺん外す
	(*gotpos) = (PROC)mempo;
	//sprintf(ods,"detach ..*respo .. %x ",*gprespo);
	//ODS(ods);
	SendMessage(debhwnd,WM_CLOSE,0,0); //きかない。
	
	Sleep(1000);
	//なんか、1秒程度待つと落ちない。
	//謎、まったくもって謎！
	VirtualProtect(gotpos, sizeof(gotpos), PAGE_EXECUTE_READ, &dummy);
	//もっぺんはめる。
}


EXPORT BOOL WINAPI DllMain(HANDLE hModule, DWORD reasonForCall, LPVOID lpReserved){
	switch (reasonForCall){
	case DLL_PROCESS_ATTACH:
		OutputDebugString("[*]Dll Attach");
		attach();
		break;
	case DLL_PROCESS_DETACH:
		OutputDebugString("[*]Dll Detach");
		detach_joygetposex();
		break;
	}
	return TRUE;
}
