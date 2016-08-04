#include "reverser.h"
#include "../defines.h"
#include "rev_utils.h"
using namespace std;

#include<cstdio>
#include<cmath>

#define rep(i,n) for(int i=0;i<((int)(n));i++)

/*
void getcorn(){ //0x430450のecxの. あたり判定につかわれてる謎のやつ。
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
	//x y x y. 微妙にずれか？ (+16とか, 128 + 192 とかいりそう。)
}
//使ってないっぽい、かな...
*/

//revdataをまとめる。
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
	//背後を黒にする一般的なテク
	SelectObject(hdc , blackbrush);
	Rectangle(hdc,0,0,640,480);
	*/
	
	//自機描画
	HPEN yellowpen = CreatePen(PS_SOLID , 0 ,RGB(255,255,0)); //3
	SelectObject(hdc , yellowpen);
	my.draw(hdc);
	
	//敵機描画
	HPEN bluepen = CreatePen(PS_SOLID , 0 ,RGB(0,0,255)); //3
	SelectObject(hdc , bluepen);
	bos.draw(hdc);
	
	//敵弾描画
	HPEN greenpen = CreatePen(PS_SOLID , 0 ,RGB(0,255,0)); //3
	SelectObject(hdc , greenpen);
	int ls = buls.size();
	//if(ls>10)ls=10;
	rep(i,ls)buls[i].draw(hdc);
	
	//レーザー描画
	SelectObject(hdc , yellowpen);
	ls = lasers.size();
	rep(i,ls)lasers[i].draw(hdc);
	
	//画面情報描画
	HPEN blackpen = CreatePen(PS_SOLID , 0 ,RGB(0,0,0)); //3
	SelectObject(hdc , blackpen);
	state.draw(hdc);
	
	
	DeleteObject(blackbrush);
	DeleteObject(yellowpen);
	DeleteObject(bluepen);
	DeleteObject(greenpen);
	DeleteObject(blackpen);
}

//盤面を1フレーム分進める。
void revdata::step(){
	bos.step();
	rep(i,buls.size())buls[i].step();
}

//自機から距離d以内の弾のみ考慮することにより
//計算速度を上げる。
void revdata::select(double d){
	vector<bullet> tb;
	rep(i,buls.size()){
		bullet b = buls[i];
		if(my.p.dist(b.p) < d)tb.push_back(b);
	}
	swap(buls,tb);
}

//「中心pa,あたりca」の四角形と「中心pb,あたりcb」の四角形が
//当たっているかを調べる。
//すぐ下のrevdata::isdie()で使われる。
//弾とのcolでは、弾側の当たりは入ってる値の半分。
//本体側のあたりはそのまま。
bool iscol(fpos pa,fpos ca,fpos pb,fpos cb){
	if(abs(pa.y-pb.y) - 0.1 > ca.y + cb.y/2.0)return false; //1でよいかな？ 0.3かな？
	if(abs(pa.x-pb.x) - 0.1 > ca.x + cb.x/2.0)return false;
	return true;
}

//死んでしまってるかを調べる。
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




