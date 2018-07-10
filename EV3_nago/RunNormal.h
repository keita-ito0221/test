/**
 *RunNormal.h
 */
#ifndef RUNNORMAL_H_
#define RUNNORMAL_H_

#include "ev3api.h"
#include "RunMain.h"

extern FILE *bt;
	
class RunNormal : public RunMain
{
public:
	RunNormal();
	void run();

private:

};

#endif
