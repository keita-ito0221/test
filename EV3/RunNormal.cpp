/**
 * RunNormal.cpp
 * �ʏ푖�s�N���X
 */

#include "RunNormal.h"
#include "RunMain.h"
#include "ev3api.h"
#include "app.h"
extern FILE *bt;

#define R_COURSE 1 //R�R�[�X
#define L_COURSE 2 //L�R�[�X

/**
 * �R���X�g���N�^
 */
RunNormal::RunNormal(int bt_cmd)
{
	sts = 0;
	speed = 70;
	
	if(bt_cmd == 3){
		course = R_COURSE;
	}
	else if(bt_cmd == 4){
		course = L_COURSE;
	}else{
		is_error = 1;
	}
}

/**
*�����J�n
*/
void RunNormal::run() {
	if(course == R_COURSE){
		R_Course_Run();
	}
	else if(course == L_COURSE){
		L_Course_Run();
	}
}

void RunNormal::R_Course_Run(){
	while(1){
		move(speed);
		switch(sts){
		case 1:
			//�X�^�[�g���Ă��獶�ɋȂ���n�߂���
			sts = 2;
			break;
		case 2:
			//�^�������ɖ߂�n�߂���
			sts = 3;
			break;
		case 3:
			//�^�������i��ł���r���ō��ɋȂ���n�߂���
			sts = 4;
			break;
		case 4:
			//�^�������ɖ߂�n�߂���
			sts = 5;
			break;
		case 5:
			//�^�������i��ł���r���ŉE�ɋȂ���n�߂���
			sts = 6;
			break;
		case 6:
			//�^�������ɖ߂�n�߂���
			sts = 7;
			break;
		case 7:
			//�����A�b�������̒l�ɒB���A���ɋȂ���n�߂���(���J�[�u�̂�)
			sts = 8;
			break;
		case 8:
			//�^�������ɖ߂�n�߁A�D�F�����m������
			sts = 9;
			break;
		case 9:
			//�V�[�\�[���[�h�ɐ؂�ւ��
			runmode =  SEESAW_RUNMODE;
			break;
		}
	}
}

void RunNormal::L_Course_Run(){
	while(1){
		switch(sts){
		case 1:
			//�X�^�[�g���Ă��獶�ɋȂ���n�߂��� 
			sts = 2;
			break;
		case 2:
			//�^�������ɖ߂�n�߂���
			sts = 3;
			break;
		case 3:
			//�^�������i��ł���r���ŉE�ɋȂ���n�߂���
			sts = 4;
			break;
		case 4:
			//�^�������ɖ߂�n�߂���
			sts = 5;
			break;
		case 5:
			//�^�������i��ł���r���ŉE�ɋȂ���n�߂���
			sts = 6;
			break;
		case 6:
			//�^�������ɖ߂�n�߂���
			sts = 7;
			break;
		case 7:
			//�^�������i��ł���r���o�ɍ��ɋȂ���n�߂���
			sts = 8;
			break;
		case 8:
			//�^�������ɖ߂�n�߁A�D�F�����m������
			sts = 9;
			break;
		case 9:
			//�Q�[�g���[�h�ɐ؂�ւ��
			runmode = GATE_RUNMODE;
		break;
		}
	}
}
