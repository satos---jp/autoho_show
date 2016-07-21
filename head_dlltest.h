#ifndef DLLTEST_H
#define DLLTEST_H

struct replstate{
	char lv,sc;
	int ms,pt;
	void out();
};

struct joydata{
	int y,x,b;
	//button
	//1 .. Z
	//2 .. X
	//4 .. esc
};


#endif