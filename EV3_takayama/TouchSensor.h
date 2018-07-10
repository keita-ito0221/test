/**
 *TouchSensor.h
 */
#ifndef TOUCHSENSOR_H_
#define TOUCHSENSOR_H_

#include "ev3api.h"
	
class TouchSensor
{
public:
	TouchSensor();
	void init();

	static const sensor_port_t
		touch_sensor = EV3_PORT_1;

private:

};

#endif
