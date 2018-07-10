/**
 * ColorSensor.cpp
 * カラーセンサクラス
 */

#include "ev3api.h"
#include "ColorSensor.h"

/**
 * コンストラクタ
 */
ColorSensor::ColorSensor()
{
}

/*
 * 初期化
 */
void ColorSensor::init(){
	/* センサー入力ポートの設定 */
	ev3_sensor_config(color_sensor, COLOR_SENSOR);
	ev3_color_sensor_get_reflect(color_sensor); /* 反射率モード */
}

/*
* 反射光の強さを取得する
*/
int ColorSensor::getReflect() {
	return ev3_color_sensor_get_reflect(color_sensor);
}

