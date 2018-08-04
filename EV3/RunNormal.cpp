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
	for(int i = 0;i<10;i++){
		tA_r[i] = 0;
		tA_l[i] = 0;
	}
	for(int i = 0;i<8;i++){
		tAve[i] = 0;
	}
	i = k = Tcnt = 0;
	
	sts = 1;
	speed = 30;

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
		if(TurnDetection(13.5,14.0)){   //�E�^�C�������^�C���̃��[�^�̉�]�����P�O�O���炢�̍����o����
			//fputs("sts:1\r\n",bt);
			sts = 2;
		}
		break;
	case 2:
		//�^�������ɖ߂�n�߂���
		//if(getTurn()  <= �^�������̂Ƃ���turn) break;
		sts = 3;
		break;
	case 3:
		if(motor.getTurnAngle(motor.right_motor,motor.left_motor) <= 100){   //�^�������i��ł���r���ō��ɋȂ���n�߂���
			//fputs("sts:3\r\n",bt);
		sts = 4;
		}
		break;
	case 4:
		//�^�������ɖ߂�n�߂���
		sts = 5;
		break;
	case 5:
		if(motor.getTurnAngle(motor.left_motor,motor.right_motor) <= 100){	//�^�������i��ł���r���ŉE�ɋȂ���n�߂���
			//fputs("sts:5\r\n",bt);
		sts = 6;
		}
		break;
	case 6:
		//�^�������ɖ߂�n�߂���
		sts = 7;
		break;
	case 7:
		if(motor.getTurnAngle(motor.right_motor,motor.left_motor) <= 100){	//�����A�b�������̒l�ɒB���A���ɋȂ���n�߂���(���J�[�u�̂�)
			//fputs("sts:7\r\n",bt);
		sts = 8;
		}
		break;
	#if 0
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
			//�V�[�\�[���[�h�ɐ؂�ւ��
			runmode =  SEESAW_RUNMODE;
		}
		break;
	#endif
	}
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

int RunNormal::TurnDetection(float min, float max) {

  	
     if(i == 10){
       i=0;
     }
     if( k == 8){
       k=0;
     }
     AR = 0.0;
     AL = 0.0;
     tA_r[i] = pwm_R;
     tA_l[i] = pwm_L;
     for(int j = 0;j<10;j++){
       AR = AR + tA_r[j];
       AL = AL + tA_l[j];
     }
   AR = (AR/10);
   AL = (AL/10);
   printf("AR:%f\n",AR);
   
   tAve[k] = AR - AL;
   printf("Ave:[%f]\n",tAve[k]);
   for(int j=0;j<8;j++){
       if((tAve[j]>=min) && (tAve[j]<=max)){
         Tcnt++;
       }
    }
    if(Tcnt >= 8){
       return 1;
    }else{
      Tcnt = 0;
    }
    i++;
    k++;
	return 0;
  
}