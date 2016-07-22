#ifndef DEFINES_H
#define DEFINES_H

#include<windows.h>
#include<cstdio>

#define ods(...) {char OutputDebugStringbuffer[256]; sprintf(OutputDebugStringbuffer,__VA_ARGS__); OutputDebugString(OutputDebugStringbuffer);}
//#define ods(...) {}


#endif
