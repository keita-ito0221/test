/**
 *ColorSensor.h
 */
#ifndef COLORSENSOR_H_
#define COLORSENSOR_H_

#include "ev3api.h"
	
class ColorSensor
{
public:
	ColorSensor();
	void init();
	int getReflect();

	static const sensor_port_t
		color_sensor = EV3_PORT_3;

private:

};

#endif
