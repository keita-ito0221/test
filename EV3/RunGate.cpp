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
}

/**
*�����J�n
*/
void RunGate::run() {
	//fputs("gate\r\n",bt);
	move(10);
	// 20cm�ŏ�Q�������m
	/*
	if(sonarsensor.getDetection() <= 20){

		fputs("sonor\r\n",bt);
	}
	*/
	//50cm�̎��̃��[�^�̉�]�����擾
	motor.setMovedistance(50);
	if((motor.getAngle(motor.left_motor)+motor.getAngle(motor.right_motor)) / 2 >= motor.getMovedistance()){
		fprintf(bt,"getdis:%d\r\n",motor.getMovedistance());
		motor.stop();
	}
}

