/**
 * Motor.cpp
 * モータクラス
 */
#include "ev3api.h"
#include "Motor.h"


#define TAIL_ANGLE_DRIVE      3 /* バランス走行時の角度[度] */
#define P_GAIN             2.5F /* 完全停止用モータ制御比例係数 */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */
/**
 * コンストラクタ
 */
Motor::Motor()
{
}

/*
 * 初期化
 */
void Motor::init(){
	/* モーター出力ポートの設定 */
	ev3_motor_config(left_motor, LARGE_MOTOR);
	ev3_motor_config(right_motor, LARGE_MOTOR);
	ev3_motor_config(tail_motor, LARGE_MOTOR);
	ev3_motor_reset_counts(tail_motor);
}

/*
* モータリセット
*/
void Motor::reset(motor_port_t motor) {
	ev3_motor_reset_counts(motor);
}

/*
* モータを停止する
*/
void Motor::stop() {
	ev3_motor_stop(left_motor, false);
	ev3_motor_stop(right_motor, false);
}

/*
* モータの角位置を取得する
*/
int32_t Motor::getAngle(motor_port_t motor) {
	return ev3_motor_get_counts(motor);
}

/*
 * モータのパワーを取得する
 */
int Motor::getPower(motor_port_t motor){
	return ev3_motor_get_power(motor);
}

/**
*両方のタイヤモータの角位置の平均を取得する
*/
int Motor::getAveAngle(){
	return (getAngle(left_motor) + getAngle(right_motor)) / 2;
}

/**
*両方のタイヤモータのターンの角位置を取得する
*/
int Motor::getTurnAngle(motor_port_t f_motor, motor_port_t b_motor){
	return (getAngle(f_motor) - getAngle(b_motor));
}

//*****************************************************************************
// 関数名 : tail_control
// 引数 : angle (モータ目標角度[度])
// 返り値 : 無し
// 概要 : 走行体完全停止用モータの角度制御
//*****************************************************************************
void Motor::tail_control(){
	float pwm = (float)(TAIL_ANGLE_DRIVE - getAngle(tail_motor))*P_GAIN; /* 比例制御 */
	/* PWM出力飽和処理 */
	if (pwm > PWM_ABS_MAX){
		pwm = PWM_ABS_MAX;
	}
	else if (pwm < -PWM_ABS_MAX){
		pwm = -PWM_ABS_MAX;
	}

	if (pwm == 0){
		ev3_motor_stop(tail_motor, true);
	}
	else{
		ev3_motor_set_power(tail_motor, (signed char)pwm);
	}
}

/*
* 尻尾を上げる
*/
void Motor::tail_up(int up) {
	ev3_motor_rotate(tail_motor, up, PWM_ABS_MAX, true);
}

/*
* 尻尾を下げる
*/
void Motor::tail_down(int down) {
	ev3_motor_rotate(tail_motor, down, PWM_ABS_MAX, true);
}

/*
* 尻尾を固定
*/
void Motor::tail_save() {
	ev3_motor_rotate(tail_motor, 0, PWM_ABS_MAX, true);
}

/*
 *タイヤを指定した角度回転させる
 */
void Motor::rotate(motor_port_t motor ,uint32_t degrees, int power) {
	ev3_motor_rotate(motor, degrees, power, true);
}

/*
* 移動距離
*/
void Motor::setMovedistance(int _movedistance) {
	//移動距離の計算
	movedistance = int((360 / (9.8 * 3.141592)) * _movedistance);
}

int Motor::getMovedistance() {
	return movedistance;
}
