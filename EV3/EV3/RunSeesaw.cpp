/**
 * RunSeesaw.cpp
 * �V�[�\�U���N���X
 */

#include "RunSeesaw.h"
#include "RunMain.h"
#include "ev3api.h"

#include "Motor.h"
extern Motor motor;
#include "GyroSensor.h"
extern GyroSensor gyrosensor;

/**
 * �R���X�g���N�^
 */
RunSeesaw::RunSeesaw()
{
}

/**
*�����J�n
*/
void RunSeesaw::run() {
	move(10);
	motor.setMovedistance(45);
	if((motor.getAngle(motor.left_motor)+motor.getAngle(motor.right_motor)) / 2 >= motor.getMovedistance()){
		ter_tsk(BLN_TASK);
		fprintf(bt, "%s\r\n", "power");
	}
	//motor.setMovedistance(40);
	//if((motor.getAngle(motor.left_motor)+motor.getAngle(motor.right_motor)) / 2 >= motor.getMovedistance()){
	//	move(80);
	//
	//}
	
}

