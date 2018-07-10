/**
 * RunGate.cpp
 * ���b�N�A�b�v�Q�[�g�U���N���X
 */

#include "RunGate.h"
#include "RunMain.h"
#include "ev3api.h"
#include "app.h"

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
1.�����g�ŏ�Q�������m����     ����

2.���������؂��ĐK�����o��   ����

3.�Q�[�g��ʉ߂���             ����

4.����������N������

5.���s�̂𔼉�]������

6.2->3->4->5                2�Z�b�g
*/
void RunGate::run() {
	while(1){
		move(60);
		switch(flg){
		case 0:													//��Q��
			if(ObstacleDetection(20)){							//20cm�ȓ��ɏ�Q��������Ώ���
				fputs("sonar\r\n",bt);
				ter_tsk(BLN_TASK);								//�o�����T��~
				ev3_motor_set_power(motor.left_motor, 40); 		//�ԑ̂��|��Ȃ��悤�Ƀo�����T���~�߂ď����O�ɐi��
				ev3_motor_set_power(motor.right_motor, 40);
				motor.tail_down(75);							//�K�������낷
				flg = 1;
			}
			break;
		case 1:													//���[�^�̃��Z�b�g
			fputs("reset\r\n",bt);
			motor.reset(motor.left_motor);						//���[�^�̉�]���̏�����
			motor.reset(motor.right_motor);										
			flg = 2;
			break;			
		case 2:													//�Q�[�g�̒ʉ�
			motor.setMovedistance(50);							//50cm�̎��̃��[�^�̉�]�����擾
			if(motor.getAveAngle() >= motor.getMovedistance()){	//�w�肳�ꂽ��]���Ƃ𒴂���Ǝ~�܂�
				stop();											//���s�̒�~
				flg = 5;
			}
			break;
		case 3:													//��������N��
				//tslp_tsk(5000);
				/////////////
				//    �o�����T�N�������� act_tsk(BLN_TASK); //�o�����T�N��
				//   motor.tail_control();/* �o�����X���s�p�p�x�ɐ��� */
				/////////////
			break;
		case 4:
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
		if(flg <= 5) break;

	}
	stop();
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
