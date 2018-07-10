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
	move(10);
	// 20cmで障害物を検知
	/*
	//障害物との距離を取得
	if(sonarsensor.getDetection() <= 20){

		fputs("sonor\r\n",bt);
	}
	*/
	//while(1){
		/*
		//50cmの時のモータの回転数を取得
		motor.setMovedistance(50);
		if(getAveAngle() >= motor.getMovedistance()){
			motor.stop();//50cm進んだら止まる
		}
		*/
		if(ObstacleDetection(20)){
			move(60);
			fputs("1",bt);
		}else{
			move(-60);
			fputs("0",bt);
		}
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
