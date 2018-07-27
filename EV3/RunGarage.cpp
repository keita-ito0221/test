/**
 * RunGarage.cpp
 * �Ԍɓ���U���N���X
 */
#define GOAL 40

#define STS_GARAGE_INIT       1
#define STS_GARAGE_RUN_STOP   2
#define STS_GARAGE_END        3

#include "RunGarage.h"
#include "RunMain.h"
#include "ev3api.h"

#include "Motor.h"
extern Motor motor;
/* �R���X�g���N�^*/
RunGarage::RunGarage()
{
	sts = 1;
}

/**
 * �f�X�g���N�^
 */
RunGarage::~RunGarage()
{
}

/**
*�����J�n
*/
void RunGarage::run() {
	move(50);
	
	while(1){
		switch(sts){
			case STS_GARAGE_INIT: //������
				motor.tail_control(); /* �o�����X���s�p�p�x�ɐ��� */
				//�e���[�^�̊p�ʒu�����Z�b�g
				motor.reset(motor.left_motor);
				motor.reset(motor.right_motor);
				sts = STS_GARAGE_RUN_STOP;
				break;
			
			case STS_GARAGE_RUN_STOP: //��~�ʒu�܂ő���
				//40cm�̎��̃��[�^�̉�]�����擾
				motor.setMovedistance(GOAL);
				//�w�肳�ꂽ��]���Ƃ𒴂���Ǝ~�܂�
				if(motor.getAveAngle() >= motor.getMovedistance()){
				//�o�����T��؂�
				ter_tsk(BLN_TASK);
				ev3_motor_set_power(motor.left_motor,30);
				ev3_motor_set_power(motor.right_motor,30);
				tslp_tsk(150);
				stop();
				//�K����������
				motor.tail_down(95);
			
				sts = STS_GARAGE_END;
				}
			
				break;
			
			case STS_GARAGE_END:
				ter_tsk(BT_TASK);
				fclose(bt);
				ext_tsk();
				break;
		}
	}
}