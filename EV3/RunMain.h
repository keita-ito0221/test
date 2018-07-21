/**
 *RunMain.h
 */
#ifndef RUNMAIN_H_
#define RUNMAIN_H_

#include "ev3api.h"

//#define GYRO_OFFSET  0       /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define LIGHT_WHITE  55         /* 白色の光センサ値 */
#define LIGHT_BLACK  0          /* 黒色の光センサ値 */

extern FILE *bt;

class RunMain
{
public:
	RunMain();
	void init();
	virtual void run();
	int getTurn();
	int getForward();
	void move(int);
	void stop();
	
	int forward = 0;      /* 前後進命令 */
	int turn = 0;         /* 旋回命令 */
	int pwm_L, pwm_R; /* 左右モータPWM出力 */
	int gyro, volt;
	int32_t motor_ang_l, motor_ang_r;
	float target;
	int stop_flg;

private:

};

#endif
