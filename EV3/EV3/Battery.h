/**
 *Battery.h
 */
#ifndef BATTERY_H_
#define BATTERY_H_

#include "ev3api.h"
	
class Battery
{
public:
	Battery();
	void init();
	void getVoltage();
	void getCurrent();

private:

};

#endif
