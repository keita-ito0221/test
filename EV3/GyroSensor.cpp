/**
 * GyroSensor.cpp
 * ジャイロクラス
 */

#include "ev3api.h"
#include "GyroSensor.h"

/**
 * コンストラクタ
 */
GyroSensor::GyroSensor()
{
}

/*
 * 初期化
 */
void GyroSensor::init(){
	/* センサー入力ポートの設定 */
	ev3_sensor_config(gyro_sensor, GYRO_SENSOR);
}

/*
* 角速度を取得する
*/
int GyroSensor::getRate() {
	return ev3_gyro_sensor_get_rate(gyro_sensor);
}

/*
* 角位置を取得する
*/
int GyroSensor::getAngle() {
	return ev3_gyro_sensor_get_angle(gyro_sensor);
}

/*
* リセット
*/
void GyroSensor::reset() {
	ev3_gyro_sensor_reset(gyro_sensor);
}

