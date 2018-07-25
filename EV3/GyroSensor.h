/**
 *GyroSensor.h
 */
#ifndef GYROSENSOR_H_
#define GYROSENSOR_H_

#include "ev3api.h"
	
class GyroSensor
{
public:
	GyroSensor();
	void init();
	int getRate();
	int getAngle();
	void reset();

	static const sensor_port_t
		gyro_sensor = EV3_PORT_4;

private:

};

#endif
