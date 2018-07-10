/**
 * TouchSensor.cpp
 * タッチセンサクラス
 */

#include "ev3api.h"
#include "TouchSensor.h"

/**
 * コンストラクタ
 */
TouchSensor::TouchSensor()
{
}

/*
 * 初期化
 */
void TouchSensor::init(){
	/* センサー入力ポートの設定 */
	ev3_sensor_config(touch_sensor, TOUCH_SENSOR);
}

