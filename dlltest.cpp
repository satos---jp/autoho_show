//gcc�ŃR���p�C���̍ۂɂ́Aextern(ry)  ���ADllMain�ɂ��t���邱�ƁB

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
Joygetposex��������܂��B

*/

typedef MMRESULT (WINAPI *Jgpex)(UINT ,LPJOYINFOEX);//Joygetposex�̌^

Jgpex mempo;

char ods[200];//OutputDebugString�p

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
	//�ڍׂ�reversing/forida/anal ������
	
	//pt�͎��񂾂Ƃ��ɓ����邻���ȁB
	st->out();
}




HDC nhdc;

//�������������ł����񂶂�Ȃ����A�Ƃ����C���ɂȂ����Ƃ��B

bool sendd=false;
bool makedwin = false;
HWND debhwnd = NULL;
	

MMRESULT WINAPI Joygetposex_wrapper( UINT uJoyID, LPJOYINFOEX pji){

	MMRESULT res = mempo(uJoyID, pji);
	
	
	if(!makedwin){
		
		const char browsername[400]="�������Ԓ��@�` Shoot the Bullet. ver 1.02a";
		HWND hwnd = GetHwnd(browsername);
		if(hwnd==NULL){
			ods("hwndisnull");
		}
		HINSTANCE hi = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE); //64bit�݊����͂Ȃ��炵���B
		if(hi==NULL){
			ods("hiisnull");
		}
		else{
			ods("hi in overrided joypos .. %x",hi);
			//sprintf(ods,"hi in overrided joypos .. %x",hi);
			//ODS(ods);
		}
		
		debhwnd = makewindow(hi); //�ł��Ȃ��B
		ods("debhwnd .. %x",debhwnd);
		//sprintf(ods,"debhwnd .. %x",debhwnd);
		//ODS(ods);
		
		//Sleep(10000);
		
		//ODS("sleeped");
		makedwin=true;
	}
	
	const char browsername[400]="�������Ԓ��@�` Shoot the Bullet. ver 1.02a";
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

	//ai_conduction�ɂ���āAAI�ɐ��䂳����B
	//red, �ɂ́A�Q�[����ʂ�level(lv),scene(sc),���_(pt),�o�ߎ���(ms)�������Ă���B
	//jo �́Ajoipad�̃f�[�^�B����ő��삷��B
	//red �́Areverse�ɂ���ĕ��������A�G�@�ʒu�A���@�ʒu�A�e�ʒu�Ȃǂ������Ă���B
	
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
	//0x00495268 �� joygetposex �� got
	DWORD dummy;
	VirtualProtect(gotpos, sizeof(gotpos), PAGE_EXECUTE_READWRITE, &dummy);
	//�����ŁA�������ނƂ��̃v���e�N�g���O���Ă�B
	mempo = (Jgpex)(*gotpos);
	(*gotpos) = (PROC)Joygetposex_wrapper;
	VirtualProtect(gotpos, sizeof(gotpos), PAGE_EXECUTE_READ, &dummy);
	
	
	//���Ƃ� 0x419410 ����̂��������B
	//uint �ŁA xmin,xmax,ymin,ymax ��
	
	//ODS("writed func");
	
	int stpi = (0x004bdf04); //xmin�̃|�C���^
	VirtualProtect((void*)stpi, 4 * sizeof(UINT), PAGE_EXECUTE_READWRITE, &dummy);
	//0 1000 0 1000 �ɂ���B
	for(int i=0;i<4;i++){
		UINT* p = (UINT*)(stpi + i * sizeof(UINT)); 
		(*p) = (UINT)((i%2)*1000);
	}
	
	VirtualProtect((void*)stpi, 4 * sizeof(unsigned int), dummy, &dummy);
	
	//ODS("writed struct");
	
	ai_init();
	
	const char browsername[400]="�������Ԓ��@�` Shoot the Bullet. ver 1.02a";
	HWND hwnd = GetHwnd(browsername);
	if(hwnd==NULL){
		ods("hwndisnull");
	}
	HINSTANCE hi = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE); //64bit�݊����͂Ȃ��炵���B
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
	//�����؂�O��
	(*gotpos) = (PROC)mempo;
	//sprintf(ods,"detach ..*respo .. %x ",*gprespo);
	//ODS(ods);
	SendMessage(debhwnd,WM_CLOSE,0,0); //�����Ȃ��B
	
	Sleep(1000);
	//�Ȃ񂩁A1�b���x�҂Ɨ����Ȃ��B
	//��A�܂����������ē�I
	VirtualProtect(gotpos, sizeof(gotpos), PAGE_EXECUTE_READ, &dummy);
	//�����؂�͂߂�B
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
