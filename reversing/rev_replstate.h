#ifndef REV_REPLSTATE_H
#define REV_REPLSTATE_H

#include <windows.h>

struct replstate{
	//char lv,sc; //lv,sc .. ���x��,�V�[���B
	int ms,pt; //ms .. �J�n����̕b��,pt .. �|�C���g�B
	int stgidx,stan;
	bool iscleared; //stgidx�̂��N���A���Ă邩�ǂ����B
	bool canplay; //�܂��������ĂȂ��ꍇ������
	void out();
	void get();
	void draw(HDC& hdc);
};




#endif
