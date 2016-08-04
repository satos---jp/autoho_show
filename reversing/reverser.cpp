#include "reverser.h"
#include "../defines.h"
#include "rev_utils.h"
using namespace std;

#include<cstdio>
#include<cmath>

#define rep(i,n) for(int i=0;i<((int)(n));i++)

/*
void getcorn(){ //0x430450��ecx��. �����蔻��ɂ����Ă��̂�B
	int b = 0x4c4e70;
	b = *(int*)(b);
	if(b==0){
		ods("cornull");
		return;
	}
	b += 0x3a8;
	ods("corp %x",b);
	rep(i,4){
		int t = i>=2?i+1:i;
		float p = *(float*)(b + 4*t);
		ods("corn %d: %f",i,p);
	}
	//x y x y. �����ɂ��ꂩ�H (+16�Ƃ�, 128 + 192 �Ƃ����肻���B)
}
//�g���ĂȂ����ۂ��A����...
*/

//revdata���܂Ƃ߂�B
void revdata::get(){
	
	my.get();
	bos.get();

	buls = enubal();
	lasers = enulaser();
	
	state.get();
	
	//ods("myp: %f %f",r.myp.x,r.myp.y);
}


void revdata::draw(HDC &hdc){
	
	HBRUSH blackbrush = CreateSolidBrush(RGB(0,0,0)); //3
	/*
	//�w������ɂ����ʓI�ȃe�N
	SelectObject(hdc , blackbrush);
	Rectangle(hdc,0,0,640,480);
	*/
	
	//���@�`��
	HPEN yellowpen = CreatePen(PS_SOLID , 0 ,RGB(255,255,0)); //3
	SelectObject(hdc , yellowpen);
	my.draw(hdc);
	
	//�G�@�`��
	HPEN bluepen = CreatePen(PS_SOLID , 0 ,RGB(0,0,255)); //3
	SelectObject(hdc , bluepen);
	bos.draw(hdc);
	
	//�G�e�`��
	HPEN greenpen = CreatePen(PS_SOLID , 0 ,RGB(0,255,0)); //3
	SelectObject(hdc , greenpen);
	int ls = buls.size();
	//if(ls>10)ls=10;
	rep(i,ls)buls[i].draw(hdc);
	
	//���[�U�[�`��
	SelectObject(hdc , yellowpen);
	ls = lasers.size();
	rep(i,ls)lasers[i].draw(hdc);
	
	//��ʏ��`��
	HPEN blackpen = CreatePen(PS_SOLID , 0 ,RGB(0,0,0)); //3
	SelectObject(hdc , blackpen);
	state.draw(hdc);
	
	
	DeleteObject(blackbrush);
	DeleteObject(yellowpen);
	DeleteObject(bluepen);
	DeleteObject(greenpen);
	DeleteObject(blackpen);
}

//�Ֆʂ�1�t���[�����i�߂�B
void revdata::step(){
	bos.step();
	rep(i,buls.size())buls[i].step();
}

//���@���狗��d�ȓ��̒e�̂ݍl�����邱�Ƃɂ��
//�v�Z���x���グ��B
void revdata::select(double d){
	vector<bullet> tb;
	rep(i,buls.size()){
		bullet b = buls[i];
		if(my.p.dist(b.p) < d)tb.push_back(b);
	}
	swap(buls,tb);
}

//�u���Spa,������ca�v�̎l�p�`�Ɓu���Spb,������cb�v�̎l�p�`��
//�������Ă��邩�𒲂ׂ�B
//��������revdata::isdie()�Ŏg����B
//�e�Ƃ�col�ł́A�e���̓�����͓����Ă�l�̔����B
//�{�̑��̂�����͂��̂܂܁B
bool iscol(fpos pa,fpos ca,fpos pb,fpos cb){
	if(abs(pa.y-pb.y) - 0.1 > ca.y + cb.y/2.0)return false; //1�ł悢���ȁH 0.3���ȁH
	if(abs(pa.x-pb.x) - 0.1 > ca.x + cb.x/2.0)return false;
	return true;
}

//����ł��܂��Ă邩�𒲂ׂ�B
bool revdata::isdie(){
	if(iscol(my.p,my.col,bos.p,bos.col))return true;
	rep(i,buls.size()){
		bullet& b = buls[i];
		if(iscol(my.p,my.col, b.p,b.col))return true;
	}
	rep(i,lasers.size()){
		laser& r = lasers[i];
		if(r.iscol(my))return true;
	}
	return false;
}




