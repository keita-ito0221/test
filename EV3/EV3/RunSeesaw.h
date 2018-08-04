/**
 *RunSeesaw.h
 */
#ifndef RUNSEESAW_H_
#define RUNSEESAW_H_

#include "ev3api.h"
#include "RunMain.h"
	
class RunSeesaw : public RunMain
{
public:
	RunSeesaw();
	virtual ~RunSeesaw();
	void run();
	int left_motor_count;
	int right_motor_count;
	int array[2];
	int left_angle;
	int right_angle;
	int power;
	int left_count[2];
	int right_count[2];

private:

};

#endif
