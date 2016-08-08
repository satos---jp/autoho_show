#include "conductor.h"

#include "ai_tes.h"

joydata::joydata(){	neutral(); }
void joydata::neutral(){ y=x=500; b=0; }
void joydata::copy(joydata jo){ y = jo.y; x = jo.x; b = jo.b; }

void joydata::Z(){ b |= 1; }
void joydata::stick(int iy,int ix){
	if(iy<0)y = 500 - 300;
	else if(iy>0)y = 500 + 300;
	if(ix<0)x = 500 - 300;
	else if(ix>0)x = 500 + 300;
}



#define ALLPLAY

#ifdef ALLPLAY

//関数の返り値で与えるのはだめっぽい。(virtualがあるのを実体化できない)
//そもそも大域変数で持てなくなった...ので、毎回作ってdeleteします。


class Status{
public:
	virtual Status* run(joydata* joy,revdata& revd) = 0;
};

class Push : public Status{
	Status* aft;
	joydata tojoy;
	int ct;
public:
	Push(Status* ts,joydata tj){
		aft = ts;
		ct = 0;
		tojoy = tj;
	}
	Status* run(joydata* joy,revdata& revd){
		ods("Push");
		if(ct<2){
			if(ct==0)joy->copy(tojoy);
			else joy->neutral();
			ct++;
			return new Push(*this);
		}
		else{
			return aft;
		}
	}
};

class Wait : public Status{
	Status* aft;
	int memt;
	int ct;
public:
	Wait(Status* ts,int t){
		aft = ts;
		memt = t;
		ct = 0;
	}
	Status* run(joydata* joy,revdata& revd){
		ods("Wait");
		if(ct<memt){
			ct++;
			return new Wait(*this);
		}
		else{
			return aft;
		}
	}
};

class Play : public Status{
public:
	bool zed,waited;
	int mms;
	int ct;
	Play();
	Status* run(joydata* joy,revdata& revd);
};

int stagenum[12] = {
	6,6,8,9,8,8,8,8,8,8,8,0
};

class Select : public Status{
	//int memstg;
	bool inited;
public:
	Select(){
		inited = false;
	}
	Status* nextstage(revdata& revd){
		if(revd.state.stgidx % 10 == stagenum[revd.state.stgidx/10] - 1){
			joydata j1,j2;
			j1.stick(1,0); //上に戻しておく。
			j2.stick(0,1);
			return new Push(new Push(new Select(*this),j2),j1);
		}
		else{
			joydata jo;
			jo.stick(1,0);
			return new Push(new Select(*this),jo);
		}
		
	}
	Status* run(joydata* joy,revdata& revd){
		ods("Select");
		if(revd.state.stgidx < 0){
			return new Select(*this);
		}
		if(!inited){
			//ゲーム終了直後はここに入る。
			inited = true;
			return nextstage(revd);
		}
		if(revd.state.iscleared){
			//クリア済みなので飛ばす
			return nextstage(revd);
		}
		
		//みくりやなのでやる。
		joydata jo;
		jo.Z();
		return new Wait(new Push(new Play(),jo),60);
	}
};

class Finish : public Status{
public:
	Status* run(joydata* joy,revdata& revd){
		ods("Finish");
		if(revd.state.stan==3){ //被弾後脂肪
			joydata j1,j2;
			j2.Z(); j1.stick(1,0);
			return new Push(new Push(new Wait(new Select(),60),j2),j1);
		}
		else if(revd.state.stan==5){ //撮影成功
			joydata jo;
			jo.Z();
			return new Push(new Wait(new Select(),60),jo);
		}
		else return new Finish(*this); //そのまま。
	}
};

Play::Play(){
	zed = false; waited = false; mms = -1;
}
Status* Play::run(joydata* joy,revdata& revd){
	ods("Play");
	if(!zed){
		zed = true;
		joydata jo;
		jo.Z();
		ct = 0;
		return new Push(new Play(*this),jo);
	}
	else if(!waited){
		if(revd.state.ms>=100)waited = true;
		mms = revd.state.ms;
		ct ++;
		if(ct>120){
			//遺憾ながらスタートできないやつ。(まだ選択不能)	
			//なので、次ステージに移る。(もっとスマートにやりたい)
			joydata jo;
			jo.stick(0,1);
			return new Push(new Select(),jo);
		}
		return new Play(*this);
	}
	else{
		if(mms!=revd.state.ms){
			int y=0,x=0,b=0;
			getd(revd,y,x,b);
			joy->y = 500 + y; joy->x = 500 + x; joy->b = b;
			//joy->y=joy->x=0;
			mms = revd.state.ms;
			return new Play(*this);
		}
		else{
			joy->x=joy->y=500;
			joy->b=0;
			
			//nstate = Select();
			return new Wait(new Finish(),120);
		}
	}
}





Status* nstate;

void ai_init(){
	//ods("inited ai");
	nstate = new Select();
}

void ai_conduction(joydata* joy,revdata& revd){
	Status* ts = nstate->run(joy,revd);
	delete nstate;
	nstate = ts;
}


#else

int nstate;
int mms;
int w;
bool fixed;

void ai_init(){
	//ods("inited ai");
	nstate = -1;
	mms = -1;
	
	w = 0;
	fixed = false;
}

void ai_conduction(joydata* jo,revdata& rd){
	//ods("conduct %d",nstate);
	jo->x=jo->y=500;
	jo->b=0;
	
	//ods("myp .. y: %f x: %f\n",rd.my.p.y,rd.my.p.x);
	
	if(nstate==-1){
		if(false && !(rd.state.ms==-1)){ //とりま、外しとく。
			ods("cantstart");
			return;
		}
		else{
			jo->b = 1;
			//初期動
			if(rd.state.ms<0){
				nstate = -1;
				//まだまだ
			}
			else{
				nstate = 0;
				mms = rd.state.ms;
			}
		}
	}
	else{
		jo->b = 0;
		if(nstate<-1)return;
		if(rd.state.ms<0){ //なんらかのエラーが発生している。
			ods("errorocced");
			nstate = -2;
			return;
		}
	}
	
	
	switch(nstate){
	case -1:
		break;
	case 0:
		//いまからZを押して起動させる。
		jo->b = 1;
		nstate = 1;
		break;
	case 1:
		jo->b = 0;
		nstate = 2;
		break;
	case 2:
		//起動するまで待つ
		if(rd.state.ms>=100){
			nstate = 3;
			//起動した。
		}
		break;
	case 3:
		if(rd.state.ms!=mms){
			//無事に動いている
			//とりま、向きのみで。

			int y=0,x=0,b=0;
			getd(rd,y,x,b);
			
			
			//8-2のための乱数措置。200ms
			//ex-2 110ms
			if(rd.state.ms<110){
				x *= ((rand() % 3) - 1);
				y *= ((rand() % 3) - 1);
			}

			jo->y = 500 + y;
			jo->x = 500 + x;
			jo->b = b;
		}
		else{
			//やられた。
			jo->x=jo->y=500;
			jo->b=0;
			nstate = 4;
		}
		break;
	case 4:
		//結果処理をする。
		{
			if(w==0){
			}
			if(w<=60)w++;
			else{
				nstate = 0;
				w=0;
			}
		}
		break;
	default:
		ods("invalid nstate");
	}	
	
	//ods("nstate .. %d fix .. %s",nstate,fixed?"true":"false");
	//ods("ndir x .. %d y .. %d bs .. %d",jo->x,jo->y,jo->b);
	//re->out();
	mms=rd.state.ms;
}

#endif


