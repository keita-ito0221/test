/**
 * RunGate.cpp
 * ���b�N�A�b�v�Q�[�g�U���N���X
 */

#include "RunGate.h"
#include "RunMain.h"
#include "ev3api.h"

#include "SonarSensor.h"
SonarSensor sonarsensor;

#include "Motor.h"
extern Motor motor;

/**
 * �R���X�g���N�^
 */
RunGate::RunGate()
{
	flg = 0;
}

/**
*�����J�n
*/
/*
��肽����
1.�����g�ŏ�Q�������m����

2.���������؂��ĐK�����o��

3.�Q�[�g�����

4.����������N�����đ��s�̂𔼉�]������

5.2->3->4                2�Z�b�g
*/
void RunGate::run() {
	while(1){
		move(60);
		//motor.tail_save();//�K�����Œ�
		switch(flg){
		case 0:
			if(ObstacleDetection(20)){//20cm�ȓ��ɏ�Q��������Ώ���
				fputs("sonar\r\n",bt);
				ter_tsk(BLN_TASK);//�o�����T��~
				//�ԑ̂��|��Ȃ��悤�Ƀo�����T���~�߂ď����O�ɐi��
				ev3_motor_set_power(motor.left_motor, 40);
				ev3_motor_set_power(motor.right_motor, 40);
				motor.tail_down(75);//�K�������낷
				//stop();
				flg = 1;
			}
			break;
		case 1:
			fputs("reset\r\n",bt);
			//���[�^�̉�]���̏�����
			motor.reset(motor.left_motor);
			motor.reset(motor.right_motor);
			tslp_tsk(5000);
			stop();
			flg = 4;
			break;			
		case 2:
			//50cm�̎��̃��[�^�̉�]�����擾
			motor.setMovedistance(50);
			//�w�肳�ꂽ��]�����Ǝ~�܂�
			if(motor.getAveAngle() >= motor.getMovedistance()){
				stop();
				//tslp_tsk(5000);
				/////////////
				//    �o�����T�N�������� act_tsk(BLN_TASK); //�o�����T�N��
				//   motor.tail_control();/* �o�����X���s�p�p�x�ɐ��� */
				/////////////
				flg = 4;
			}
			break;
		case 3:
			fputs("back\r\n",bt);
			//50cm�̎��̃��[�^�̉�]�����擾
			motor.setMovedistance(50);
			//�w�肳�ꂽ��]�����Ǝ~�܂�
			if(motor.getAveAngle() >= motor.getMovedistance()){
				//stop();
				flg = 4;
			}
			break;
		default:
			break;
		}
	}
	stop();
	fputs("END\r\n",bt);
	ter_tsk(BT_LOG);
}

/**
*��Q�����m����
*/
int RunGate::ObstacleDetection(int _distance){
	if(sonarsensor.getDetection() <= _distance){
		return 1;
	}
	return 0;
}
