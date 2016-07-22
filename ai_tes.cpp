#include "ai_tes.h"

#include <windows.h>
#include <cstdio>
#include<cstdlib>
#include<cmath>

#include <vector>
#include<algorithm>
using namespace std;

typedef pair<int,int> mp;
#define fir first
#define sec second

#define rep(i,n) for(int i=0;i<((int)(n)); i++)
#define reg(i,a,b) for(int i=((int)(a));i<=((int)(b)); i++)

#include "defines.h"

/*
g++ -c dlltest.cpp
g++ -c ai_tes.cpp
g++ -shared -o dlltest.dll dlltest.o ai_tes.o
g++ main.cpp
*/

//�Ƃ�܁A�N�����ɂ͂��̃X�e�[�W��I������󋵂Ƃ���B

int nstate;
int mms;
int w;
bool fixed;

void ai_init(){
	ods("inited ai");
	nstate = -1;
	mms = -1;
	
	w = 0;
	fixed = false;
}


bool isokinit(replstate re){
	return (re.ms==-1);
}


vector<pair<mp,fpos> > enummypos(revdata d,fpos nmp){
	//����ԂƂ��Ă��肤�鎩�@�ʒu��񋓂���B
	//mp�ɂ̓��o�[�����������B
	typedef pair<mp,fpos> mfp;
	vector<mfp> res;
	reg(dx,-1,1){
		reg(dy,-1,1){
			fpos sp;
			int ns = -1;
			if(dx!=0)ns++;
			if(dy!=0)ns++;
			if(dx!=0)sp.x = d.my.sp[2+ns] * dx;
			if(dy!=0)sp.y = d.my.sp[2+ns] * dy;
			
			res.push_back(mfp(mp(dy,dx),nmp + sp));
		}
	}
	return res;
}

void getd(revdata d,int& ry,int& rx,int& button){
	ry=rx=button=0;
	//return;
	d.select(50.0);
	//ods("tbulletsize %d",d.buls.size());
	d.step();
	
	if(!d.isdie()){
		//���ʏꍇ�͊Ԃɍ���Ȃ����ۂ��B
		//5-7�Ŋm�F�\�B
		if(d.my.fil>=1.0){
			if(abs(d.my.p.dist(d.bos.p)) < 120.0){
				//�B���̂őł�
				button |= 1;
				//return;
			}
		}
	}
	
	
	
	typedef pair<mp,fpos> mfp;
	vector<mfp> vs;
	vs.push_back(mfp(mp(-2,-2),d.my.p));
	
	fpos mmyp = d.my.p;
	
	/*
	beam search �݂����Ȋ����ŁA
	���ȂȂ��p�^�[����񋓂��Ă����B
	*/
	rep(i,20){ //�[��15�ŁB��������߂�B
		//ods("depth %d vssize %d",i,vs.size()); 
		vector<mfp> tv;
		
		rep(j,vs.size()){
			mfp pa = vs[j];
			vector<mfp> tps = enummypos(d,pa.sec);
			rep(k,tps.size()){
				mfp pb = tps[k];
				d.my.p = pb.sec;
				d.my.normalize();
				if(d.isdie())continue;
				{
					if(pa.fir.fir>=-1)pb.fir = pa.fir;
					pb.sec = d.my.p;
					tv.push_back(pb);
				}
			}
		}
		
		swap(tv,vs);
		if(vs.size()>=500)break;
		d.step();
	}
	
	//sort(vs.begin(),vs.end());
	//return;
	
	typedef pair<double,mp> dmp;
	vector<dmp> han;
	{
		int pan[15]={};
		rep(i,vs.size()){
			mfp pa = vs[i];
			int p = pa.fir.fir * 3 + pa.fir.sec + 4;
			pan[p]++;
		}
		d.my.p = mmyp;
		vector<mfp> env = enummypos(d,d.my.p);
		rep(i,9){
			int dy = i/3-1 , dx = i%3-1;
			//ods("dy,dx %d %d -> %d pat",dy,dx,pan[i]);
			if(pan[i]>25){ //�Ƃ�܁A25�p�^�[�����炢����΂�����ˁB
				/*
				9���ʂ̂����A������x�������� && �{�X�ɋ߂��A�Ȃ��̂�I��ł���B
				�ǂ��牓���������D�悵�܂����B
				*/
				rep(j,env.size()){
					if(env[j].fir!=mp(dy,dx))continue;
					d.my.p = env[j].sec;
					d.my.normalize();
					double tbd = d.my.p.dist(d.bos.p);
					
					
					if(tbd<100.0){
						//�߂�����̂��l��������Ȃ�...
						tbd = (100.0-tbd) * 10.0;
					}
					
					double wd = d.my.distfromwall();
					if(wd<50.0){
						tbd += (50.0-wd) * 1000.0;
					}
					ods("dy,dx %d %d -> %d pat %f dist",dy,dx,pan[i],tbd);
					han.push_back(dmp( tbd , mp(dy,dx)));
					break;
				}
			}
		}
	}

	{
		sort(han.begin(),han.end());
		ods("candsize %d ",han.size());
		if(han.size()>0){
			//�{�X�Ɉ�ԋ߂����I�ԁB
			//ods("ress %f , %d %d",han[0].fir,han[0].sec.fir,han[0].sec.sec);
			mp pa = han[0].sec;
			ry = pa.fir * 300.0;
			rx = pa.sec * 300.0;
		}	
	}
}


void ai_conduction(replstate* re,joydata* jo,revdata& rd){
	//ods("conduct %d",nstate);
	jo->x=jo->y=500;
	jo->b=0;
	
	//ods("myp .. y: %f x: %f\n",rd.my.p.y,rd.my.p.x);
	
	if(nstate==-1){
		if(false && !isokinit(*re)){ //�Ƃ�܁A�O���Ƃ��B
			ods("cantstart");
			return;
		}
		else{
			jo->b = 1;
			//������
			if(re->ms<0){
				nstate = -1;
				//�܂��܂�
			}
			else{
				nstate = 0;
				mms = re->ms;
			}
		}
	}
	else{
		jo->b = 0;
		if(nstate<-1)return;
		if(re->ms<0){ //�Ȃ�炩�̃G���[���������Ă���B
			ods("errorocced");
			nstate = -2;
			return;
		}
	}
	
	
	switch(nstate){
	case -1:
		break;
	case 0:
		//���܂���Z�������ċN��������B
		jo->b = 1;
		nstate = 1;
		break;
	case 1:
		jo->b = 0;
		nstate = 2;
		break;
	case 2:
		//�N������܂ő҂�
		if(re->ms>=100){
			nstate = 3;
			//�N�������B
		}
		break;
	case 3:
		if(re->ms!=mms){
			//�����ɓ����Ă���
			//�Ƃ�܁A�����݂̂ŁB

			int y=0,x=0,b=0;
			getd(rd,y,x,b);
			jo->y = 500 + y;
			jo->x = 500 + x;
			jo->b = b;
		}
		else{
			//���ꂽ�B
			jo->x=jo->y=500;
			jo->b=0;
			nstate = 4;
		}
		break;
	case 4:
		//���ʏ���������B
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
	mms=re->ms;
	
	/*
	jo->x=jo->y=500;
	jo->b=0;
	*/
}

