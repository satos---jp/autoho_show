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



vector<pair<mp,fpos> > enummypos(revdata d,fpos nmp){
	//次状態としてありうる自機位置を列挙する。
	//mpにはレバー入れ方が入る。
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
		//死ぬ場合は間に合わないっぽい。
		//5-7で確認可能。
		if(d.my.fil>=1.0){
			if(abs(d.my.p.dist(d.bos.p)) < 120.0){
				//通常120だったが、8-7のために50にした。
				//通常120だったが、10-4のために145にした。
				//撮れるので打つ
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
	beam search みたいな感じで、
	死なないパターンを列挙していく。
	*/
	rep(i,20){ //深さ15で。多けりゃやめる。
		//ods("depth %d vssize %d",i,vs.size()); 
		vector<mfp> tv;
		
		rep(j,vs.size()){
			mfp pa = vs[j];
			vector<mfp> tps = enummypos(d,pa.sec);
			rep(k,tps.size()){
				mfp pb = tps[k];
				d.my.p = pb.sec;
				d.my.normalize();
				if(d.isdie())continue; //iscolのチェック時にはここをコメントアウト。
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
			if(pan[i]>25){ //とりま、25パターンくらいあればいいよね。
				/*
				9方位のうち、ある程度いけそう && ボスに近い、なものを選んでいる。
				壁から遠ざかる方を優先しませう。
				*/
				rep(j,env.size()){
					if(env[j].fir!=mp(dy,dx))continue;
					d.my.p = env[j].sec;
					d.my.normalize();
					double tbd = d.my.p.dist(d.bos.p);
					
					//通常100だったが、10-4のために150にした。
					if(tbd<100.0){
						//近すぎるのも考え物だよなあ...
						tbd = (100.0-tbd) * 10.0;
					}
					
					double wd = d.my.distfromwall();
					if(wd<50.0){
						tbd += (50.0-wd) * 1000.0;
					}
					//ods("dy,dx %d %d -> %d pat %f dist",dy,dx,pan[i],tbd);
					han.push_back(dmp( tbd , mp(dy,dx)));
					break;
				}
			}
		}
	}

	{
		sort(han.begin(),han.end());
		//ods("candsize %d ",han.size());
		if(han.size()>0){
			//ボスに一番近いやつを選ぶ。
			//ods("ress %f , %d %d",han[0].fir,han[0].sec.fir,han[0].sec.sec);
			mp pa = han[0].sec;
			ry = pa.fir * 300.0;
			rx = pa.sec * 300.0;
		}	
	}
	
}
