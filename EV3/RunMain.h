/**
 *RunMain.h
 */
#ifndef RUNMAIN_H_
#define RUNMAIN_H_

#include "ev3api.h"

//#define GYRO_OFFSET  0       /* �W���C���Z���T�I�t�Z�b�g�l(�p���x0[deg/sec]��) */
#define LIGHT_WHITE  60         /* ���F�̌��Z���T�l */
#define LIGHT_BLACK  0          /* ���F�̌��Z���T�l */
#define LIGHT_GRAY   40         /* �D�F�̌��Z���T�l */

#define MODE_BLACK	0		/* ���F�̃��C���g���[�X */
#define MODE_GRAY	1		/* �D�F�̃��C���g���[�X */


extern FILE *bt;

class RunMain
{
public:
	RunMain();
	virtual ~RunMain();
	void init();
	virtual void run();
	int getTurn();
	int getForward();
	void move(int);
	void stop();
	void find_gray(int color);
	float setTarget();				//�g���[�X�ڕW�l�ݒ�
	int line_color;					//�ǂ̐F�̃��C������g���[�X���邩
	int color;


private:
	int forward = 0;      /* �O��i���� */
	int turn = 0;         /* ���񖽗� */
	int pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int gyro, volt;
	int32_t motor_ang_l, motor_ang_r;
	float target;
	int stop_flg = 0;
	int colorlist[10];
	int cnt;
	bool flg;
	int trace_mode = MODE_BLACK;	//�X�^�[�g����̃��[�h

};

#endif
