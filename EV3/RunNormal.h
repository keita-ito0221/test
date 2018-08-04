/**
 *RunNormal.h
 */
#ifndef RUNNORMAL_H_
#define RUNNORMAL_H_

#include "ev3api.h"
#include "RunMain.h"

extern FILE *bt;

#define R_COURSE 1 //Rコース
#define L_COURSE 2 //Lコース

class RunNormal : public RunMain
{
public:
	RunNormal(int course);
	virtual ~RunNormal();
	void run();
	void R_Course_Run();
	void L_Course_Run();
	int TurnDetection(float min,float max);
	
private:
	
	float tA_r[10];
	float tA_l[10];
	float tAve[8];
	float AR,AL;
	int i,k;
	int Tcnt;
	
	int sts;
	int speed;
	int m_course;
	int is_error;
};

#endif
