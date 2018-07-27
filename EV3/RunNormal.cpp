/**
 * RunNormal.cpp
 * �ʏ푖�s�N���X
 */

#include "RunNormal.h"
#include "RunMain.h"
#include "ev3api.h"
#include "app.h"
extern FILE *bt;

#include "Motor.h"
extern Motor motor;

/**
 * �R���X�g���N�^
 */
RunNormal::RunNormal(int course)
{
	sts = 1;
	speed = 60;

	if( (course != R_COURSE) && (course != L_COURSE)){
		is_error = 1;
	}else{
		m_course = course;
	}
	
}

/**
 * �f�X�g���N�^
 */
RunNormal::~RunNormal()
{
}

/**
*�����J�n
*/
void RunNormal::run() {
	//fprintf(bt,"m_course:%d\r\n",m_course);
	if(m_course == R_COURSE){
		R_Course_Run();
	}
	else if(m_course == L_COURSE){
		L_Course_Run();
	}
}

void RunNormal::R_Course_Run(){
	
	//fprintf(bt,"speed:%d\r\n",speed);
	move(speed);
	//fprintf(bt,"sts:%d\r\n",sts);
	switch(sts){
	case 1:
		//�X�^�[�g���Ă��獶�ɋȂ���n�߂���
		//if(motor.getTurnAngle(motor.left_motor,motor.right_motor) <= 100){   //�E�^�C�������^�C���̃��[�^�̉�]�����P�O�O���炢�̍����o����
			//sts = 2;
		//}
		sts = 2;
		break;
	case 2:
		//�^�������ɖ߂�n�߂���
		//if(getTurn()) break;
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
		if(line_color == MODE_GRAY){
			sts = 9;
		}
		break;
	case 9:
		if(line_color == MODE_BLACK){
			move(0);
			tslp_tsk(1000);
			ev3_motor_rotate(motor.tail_motor, 80, 60, true);
			ter_tsk(BLN_TASK);
			fputs("ter_tsk\r\n",bt);
			stop();
			ev3_motor_set_power(motor.left_motor, 8);
			ev3_motor_set_power(motor.right_motor, 8);
			fputs("Running\r\n",bt);
			//�V�[�\�[���[�h�ɐ؂�ւ��
			runmode =  SEESAW_RUNMODE;
			fputs("runmode_chg\r\n",bt);
		}
		break;
	}
	fputs("R_course\r\n",bt);
}

void RunNormal::L_Course_Run(){
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
		if(line_color == MODE_GRAY){
			sts = 9;
		}
		break;
	case 9:
		//�Q�[�g���[�h�ɐ؂�ւ��
		runmode = GATE_RUNMODE;
	break;
	}
	
}
