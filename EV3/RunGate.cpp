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
		case 0:                                                 //��Q��
			if(ObstacleDetection(20)){                          //20cm�ȓ��ɏ�Q��������Ώ���
				fputs("sonar\r\n",bt);
				ter_tsk(BLN_TASK);                              //�o�����T��~
				ev3_motor_set_power(motor.left_motor, 40);      //�ԑ̂��|��Ȃ��悤�Ƀo�����T���~�߂ď����O�ɐi��
				ev3_motor_set_power(motor.right_motor, 40);
				motor.tail_down(80);                            //�K�������낷
				stop();                                         //���s�̒�~
				flg = 1;
			}
			break;
		case 1:                                                 //���s�̂��X����
			while(int(motor.getAngle(motor.tail_motor)) >= 65){ //�K����65�ɂȂ�܂ŏグ��
				motor.tail_up(-1);                              //�K�����グ��
				tslp_tsk(10);
			}
			flg = 2;
			break;
		case 2:                                                 //���[�^�̃��Z�b�g
			fputs("reset\r\n",bt);
			motor.reset(motor.left_motor);                      //���[�^�̉�]���̏�����
			motor.reset(motor.right_motor);
			ev3_motor_set_power(motor.left_motor, 40);          //�O�ɐi��
			ev3_motor_set_power(motor.right_motor, 40);
			flg = 3;
			break;
		case 3:                                                 //�Q�[�g�̒ʉ�
			motor.setMovedistance(35);                          //35cm�̎��̃��[�^�̉�]�����擾
			if(motor.getAveAngle() >= motor.getMovedistance()){ //�w�肳�ꂽ��]���Ƃ𒴂���Ǝ~�܂�
				stop();                                         //���s�̒�~
				tslp_tsk(1000);
				flg = 4;
			}
			break;
		case 4:                                                 //���s�̂𐂒��ɋ߂Â���
			ev3_motor_set_power(motor.left_motor, -40);         //�K�����������悤�Ɏ~�ߏ������ɉ�����
			ev3_motor_set_power(motor.right_motor, -40);
			tslp_tsk(125);
			stop();                                             //���s�̒�~
			while(int(motor.getAngle(motor.tail_motor)) <= 80){ //�K����80�ɂȂ�܂ŉ�����
				fprintf(bt,"down:%d\r\n",i);
				motor.tail_down(1);                             //�K����������艺����
				tslp_tsk(10);
			}
			tslp_tsk(1000);
			flg = 5;
			break;
		case 5:                                                 //���s�̂𔼉�]
			ev3_motor_set_power(motor.left_motor, 10);          //��]
			ev3_motor_set_power(motor.right_motor, -10);
			tslp_tsk(2800);
			flg = 1;
			break;
		case 6:                                                 //�I��
			stop_flg = 1;                                       //stop_flg��app.cpp�ɓn�� 1:�I��
			break;
		default:
			break;
		}
		
		if(flg >= 6){                                           //�I��
			stop_flg = 1;                                       //stop_flg��app.cpp�ɓn�� 1:�I��
			break;
		}
	}
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
