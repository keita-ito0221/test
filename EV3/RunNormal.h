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
	RunNormal(int bt_cmd);
	void run();
	void R_Course_Run();
	void L_Course_Run();
	
private:
	int sts;
	int speed;
	int course;
	int is_error;
};

#endif
