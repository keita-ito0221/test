/**
 *RunMain.h
 */
#ifndef RUNMAIN_H_
#define RUNMAIN_H_

#include "ev3api.h"

//#define GYRO_OFFSET  0       /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
#define LIGHT_WHITE  55         /* 白色の光センサ値 */
#define LIGHT_BLACK  0          /* 黒色の光センサ値 */
#define LIGHT_GRAY   40         /* 灰色の光センサ値 */

#define MODE_BLACK	0		/* 黒色のライントレース */
#define MODE_GRAY	1		/* 灰色のライントレース */


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
	
	int colorlist[10];
	int cnt;
	bool flg;
	
	void find_gray(int color);
	int trace_mode = MODE_BLACK;	//スタート直後のモード
	int line_color;					//どの色のライン上をトレースするか
	float setTarget();				//トレース目標値設定

private:

};

#endif
