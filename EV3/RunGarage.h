/**
 *RunGarage.h
 */
#ifndef RUNGARAGE_H_
#define RUNGARAGE_H_

#include "ev3api.h"
#include "RunMain.h"
	
class RunGarage : public RunMain
{
public:
	RunGarage();
	void run();
	int sts;

private:

};

#endif
