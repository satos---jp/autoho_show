#ifndef REV_REPLSTATE_H
#define REV_REPLSTATE_H

#include <windows.h>

struct replstate{
	//char lv,sc; //lv,sc .. レベル,シーン。
	int ms,pt; //ms .. 開始からの秒数,pt .. ポイント。
	int stgidx,stan;
	bool iscleared; //stgidxのをクリアしてるかどうか。
	bool canplay; //まだ解放されてない場合がある
	void out();
	void get();
	void draw(HDC& hdc);
};




#endif
