/**
 *RunGate.h
 */
#ifndef RUNGATE_H_
#define RUNGATE_H_

#include "ev3api.h"
#include "RunMain.h"
	
class RunGate : public RunMain
{
public:
	RunGate();
	void run();
	int ObstacleDetection(int _distance);

private:
	int flg;
	int sts;
};

#endif
