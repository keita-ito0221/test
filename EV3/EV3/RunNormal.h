/**
 *RunNormal.h
 */
#ifndef RUNNORMAL_H_
#define RUNNORMAL_H_

#include "ev3api.h"
#include "RunMain.h"

extern FILE *bt;

#define R_COURSE 1 //R�R�[�X
#define L_COURSE 2 //L�R�[�X

class RunNormal : public RunMain
{
public:
	RunNormal(int course);
	virtual ~RunNormal();
	void run();
	void R_Course_Run();
	void L_Course_Run();
	
private:
	int sts;
	int speed;
	int m_course;
	int is_error;
};

#endif
