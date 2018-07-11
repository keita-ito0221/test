/**
 * RunMain.cpp
 * 走行クラス
 */

#include "RunMain.h"
#include "ev3api.h"
#include "PID.h"
PID *pid;
#include "Motor.h"
Motor motor;
#include "GyroSensor.h"
GyroSensor gyrosensor;
#include "ColorSensor.h"
ColorSensor colorsensor;

/**
 * コンストラクタ
 */
RunMain::RunMain()
{
	target = (LIGHT_WHITE + LIGHT_BLACK)/2;
	pid = new PID();
	stop_flg = 0;
}



/**
*処理開始
*/
void RunMain::run() {
	move(30);
}

/*
 *回転量を取得する
 */
int RunMain::getTurn(){
	int diff = target - colorsensor.getReflect();
	return pid->calcControllValue(diff);
}

/*
 *速さを取得する
 */
int RunMain::getForward(){
	return forward;
}

/*
 *前進
 */
void RunMain::move(int _forward){
	forward = _forward;
}

/*
 *停止
 */
void RunMain::stop(){
	fputs("stop\r\n",bt);
	forward = 0;
	turn = 0;
	motor.stop();
}

