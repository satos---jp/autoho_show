#include "conductor.h"

#include "ai_tes.h"


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

void ai_conduction(joydata* jo,revdata& rd){
	//ods("conduct %d",nstate);
	jo->x=jo->y=500;
	jo->b=0;
	
	//ods("myp .. y: %f x: %f\n",rd.my.p.y,rd.my.p.x);
	
	if(nstate==-1){
		if(false && !(rd.state.ms==-1)){ //�Ƃ�܁A�O���Ƃ��B
			ods("cantstart");
			return;
		}
		else{
			jo->b = 1;
			//������
			if(rd.state.ms<0){
				nstate = -1;
				//�܂��܂�
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
		if(rd.state.ms<0){ //�Ȃ�炩�̃G���[���������Ă���B
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
		if(rd.state.ms>=100){
			nstate = 3;
			//�N�������B
		}
		break;
	case 3:
		if(rd.state.ms!=mms){
			//�����ɓ����Ă���
			//�Ƃ�܁A�����݂̂ŁB

			int y=0,x=0,b=0;
			getd(rd,y,x,b);
			
			
			//8-2�̂��߂̗����[�u�B200ms
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
	mms=rd.state.ms;
	
	/*
	jo->x=jo->y=500;
	jo->b=0;
	*/
}



