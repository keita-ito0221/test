/**
 * RunGate.cpp
 * ルックアップゲート攻略クラス
 */

#include "RunGate.h"
#include "RunMain.h"
#include "ev3api.h"

#include "SonarSensor.h"
SonarSensor sonarsensor;

#include "Motor.h"
extern Motor motor;

/**
 * コンストラクタ
 */
RunGate::RunGate()
{
	flg = 0;
}

/**
*処理開始
*/
void RunGate::run() {
	
	while(1){
		/*
		move(10);
		//50cmの時のモータの回転数を取得
		motor.setMovedistance(50);
		if(getAveAngle() >= motor.getMovedistance() && !flg){
			ter_tsk(BLN_TASK);
			ev3_motor_set_power(motor.left_motor, 40);
			ev3_motor_set_power(motor.right_motor, 40);
			motor.tail_down(80);//尻尾を下ろす
			//motor.stop();//50cm進んだら止まる
			fputs("2:",bt);
			flg = 1;
			break;
		}else if(!flg){
			if(ObstacleDetection(20)){ // 20cmで障害物を検知
				move(-60);//検知したら下がる
				fputs("1",bt);
			}else{
				move(60);//検知しなかったら進む
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
			//motor.tail_down(80);//尻尾を下ろす
			stop();
			flg = 1;
			break;
		}
		move(60);
	}
	
	//while(1){
	//	motor.tail_save();//尻尾を固定する
	//}
}

/**
*両方のタイヤモータの角位置の平均
*/
int RunGate::getAveAngle(){
	return (motor.getAngle(motor.left_motor) + motor.getAngle(motor.right_motor)) / 2;
}

/**
*障害物検知判定
*/
int RunGate::ObstacleDetection(int _distance){
	if(sonarsensor.getDetection() <= _distance){
		return 1;
	}
	return 0;
}
