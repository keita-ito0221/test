/**
 *SonarSensor.h
 */
#ifndef SONARSENSOR_H_
#define SONARSENSOR_H_

#include "ev3api.h"
	
class SonarSensor
{
public:
	SonarSensor();
	void init();
	int getDetection();

	static const sensor_port_t
		sonar_sensor = EV3_PORT_2;

private:

};

#endif
