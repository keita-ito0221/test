/**
 * SonarSensor.cpp
 * 超音波センサクラス
 */

#include "ev3api.h"
#include "SonarSensor.h"

/**
 * コンストラクタ
 */
SonarSensor::SonarSensor()
{
}

/*
 * 初期化
 */
void SonarSensor::init(){
	/* センサー入力ポートの設定 */
	ev3_sensor_config(sonar_sensor, ULTRASONIC_SENSOR);
}

/*
* 障害物との距離を取得する
*/
int SonarSensor::getDetection() {
	return ev3_ultrasonic_sensor_get_distance(sonar_sensor);
}

