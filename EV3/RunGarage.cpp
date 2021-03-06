/**
 * RunGarage.cpp
 * 車庫入れ攻略クラス
 */

#define STS_GARAGE_INIT       1
#define STS_GARAGE_RUN_STOP   2
#define STS_GARAGE_END        3

#include "RunGarage.h"
#include "RunMain.h"
#include "ev3api.h"

/**
 * コンストラクタ
 */
RunGarage::RunGarage()
{
	sts = 1;
}

/**
*処理開始
*/
void RunGarage::run() {
	move(50);
	
	while(1){
	switch(sts){
		case INIT: //初期化
			tail_control(TAIL_ANGLE_DRIVE); /* バランス走行用角度に制御 */
			void Motor::reset(motor_port_t motor)
			sts = 2;
			break;
		
		case RUN_STOP: //停止位置まで走る
			if ((int(ev3_motor_get_counts(left_motor))+int(ev3_motor_get_counts(right_motor)))/2 >= 470) {
			/*ter_tsk(BLN_TASK);
			ev3_motor_set_power(left_motor, forward);
			ev3_motor_set_power(right_motor, forward);
			ev3_motor_rotate(tail_motor, 70, PWM_ABS_MAX, true);
			for(int i = 1; i >= 10; i++){
				fputs("for\r\n", bt);
				ev3_motor_rotate(tail_motor, i, PWM_ABS_MAX, true);
				tslp_tsk(2000);
			}
				break;*/
		
			ev3_motor_stop(left_motor, true);
			ev3_motor_stop(right_motor, true);
			sts = 3;
			}
		
			break;
		
		case END:
			ter_tsk(BT_TASK);
			fclose(bt);
			ext_tsk();
			break;
	}
}

/*
	#define GOAL 50
	motor.setMovedistance(GOAL);
	if(motor.getAveAngle() < motor.getMoveDistance()){
		ev3_motor_set_power(motor.left_motor,30);
		ev3_motor_set_power(motor.right_motor,30);
	}else{
		ev3_motor_set_power(motor.left_motor,0);
		ev3_motor_set_power(motor.right_motor,0);
	}











int RunGarage::getAveAngle(){
	return (motor.getAngle(motor.left_motor) + motor.getAngle(motor.right_motor)) / 2;
}





*/