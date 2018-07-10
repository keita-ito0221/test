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
void RunGate::run() {
	
	while(1){
		/*
		move(10);
		//50cm�̎��̃��[�^�̉�]�����擾
		motor.setMovedistance(50);
		if(getAveAngle() >= motor.getMovedistance() && !flg){
			ter_tsk(BLN_TASK);
			ev3_motor_set_power(motor.left_motor, 40);
			ev3_motor_set_power(motor.right_motor, 40);
			motor.tail_down(80);//�K�������낷
			//motor.stop();//50cm�i�񂾂�~�܂�
			fputs("2:",bt);
			flg = 1;
			break;
		}else if(!flg){
			if(ObstacleDetection(20)){ // 20cm�ŏ�Q�������m
				move(-60);//���m�����牺����
				fputs("1",bt);
			}else{
				move(60);//���m���Ȃ�������i��
				fputs("0",bt);
			}
		}else{
			break;
		}
		*/
		
		if(ObstacleDetection(20) && !flg){
			fputs("1\r\n",bt);
			ter_tsk(BLN_TASK);
			tslp_tsk(100);
			ev3_motor_set_power(motor.left_motor, 40);
			ev3_motor_set_power(motor.right_motor, 40);
			ev3_motor_rotate(motor.tail_motor, 70, 60, true);
			//motor.tail_down(80);//�K�������낷
			stop();
			flg = 1;
			break;
		}
		move(60);
	}
	
	//while(1){
	//	motor.tail_save();//�K�����Œ肷��
	//}
}

/**
*�����̃^�C�����[�^�̊p�ʒu�̕���
*/
int RunGate::getAveAngle(){
	return (motor.getAngle(motor.left_motor) + motor.getAngle(motor.right_motor)) / 2;
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
