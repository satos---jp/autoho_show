#ifndef RENSYA_WINDOW_H
#define RENSYA_WINDOW_H

#include <Windows.h>
HDC CopyWindow(HWND hwnd);
//hwndのwindowのコピーしたHDCを返す。
HWND GetHwnd(const char* wname);
//window名が、のやつのをとる。

#endif