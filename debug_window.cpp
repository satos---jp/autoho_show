#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>
#include<algorithm>
#include<cmath>
#include<climits>
#include<string>
#include<set>
#include<map>
using namespace std;
#define rep(i,n) for(int i=0;i<((int)(n));i++)
#define reg(i,a,b) for(int i=((int)(a));i<=((int)(b));i++)
#define irep(i,n) for(int i=((int)(n))-1;i>=0;i--)
#define ireg(i,a,b) for(int i=((int)(b));i>=((int)(a));i--)
typedef long long int lli;
typedef pair<int,int> mp;
#define fir first
#define sec second
#define IINF INT_MAX
#define LINF LLONG_MAX

#include <Windows.h>

#include "debug_window.h"
#include "defines.h"

HDC mhdc=NULL;

#define eprintf(...)  fprintf(stderr,__VA_ARGS__)


LRESULT CALLBACK DebugWndProc(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	HDC hdc;
	PAINTSTRUCT ps;

	//sprintf(ods3,"recieved %x",msg);
	//ODS(ods3);
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_TIMER:
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd , &ps);

		if(mhdc!=NULL){
			
			//eprintf("mhdc .. %x\n",mhdc);

			RECT ws;
			if(GetClientRect(hwnd, &ws )==FALSE){
				fprintf(stderr,"can't get criantrect\n");
				ws.top = ws.left = 0;
				ws.bottom = ws.right = 100;
			}
			
			
			BitBlt(hdc , 0 , 0 , ws.right,ws.bottom , mhdc , 0 , 0 , SRCCOPY);
		}
		else{
			eprintf("mhdc is null!!\n");
		}
		EndPaint(hwnd , &ps);
		return 0;
	case WM_APP:
		eprintf("recieve app befmhdc .. %x\n",mhdc);
		if(mhdc!=NULL){
			DeleteDC(mhdc);
			//これがないと、メモリが増えてえらいことになる。
		}
		
		if(((HDC)lp)==NULL){
			eprintf("recieved mhdc is null!!\n");
		}
		else{
			//eprintf("%x\n",lp);
		}
		mhdc = ((HDC)lp);
		InvalidateRect(hwnd , NULL , FALSE); //へー。
		return 0;
	}
	return DefWindowProc(hwnd , msg , wp , lp);
}

const char* clan = TEXT("TOHODEBUGWND");
HWND makewindow(HINSTANCE hInstance){
	
	//最初に一回だけ作る。

	HWND hwnd;
	for(;;){
		//すでに登録されてないかで。
		/*
		hwnd = CreateWindow(
				TEXT(clan) , TEXT("toho_debug_window") ,
				WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
				CW_USEDEFAULT , CW_USEDEFAULT ,
				CW_USEDEFAULT , CW_USEDEFAULT ,
				NULL , NULL , hInstance NULL , NULL
		);
		*/
		
		hwnd = CreateWindow(
				TEXT(clan) , TEXT("toho_debug_window") ,
				WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
				10, 20 ,
				100 , 100 ,
				NULL , NULL , hInstance /*NULL*/ , NULL
		);

		
		if(hwnd!=NULL){
			ods("success to make hwnd");
			break;
		}
		
		ods("not to registered yet");
		
		WNDCLASS winc;

		winc.style		= CS_HREDRAW | CS_VREDRAW;
		winc.lpfnWndProc	= DebugWndProc;
		winc.cbClsExtra	= winc.cbWndExtra	= 0;
		winc.hInstance		= hInstance;
		//winc.hInstance	= NULL;
		winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
		winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
		winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
		winc.lpszMenuName	= NULL;
		winc.lpszClassName	= clan;

		if (!RegisterClass(&winc)){
			ods("miss to register debug window\n");
			return NULL;
		}
	}

	if (hwnd == NULL){
		ods("miss to make debug window\n");
		return NULL;
	}
	
	ods("created debug window\n");
	
	return hwnd;
}



//こちらは、画像を飛ばされるだけ、とかでどですか。
//別プロセスとはmhdcを共有できないんだよなあ...
//これ、普通に東方上で動かせば向こうのpeekが勝手にやってくれそう。
















