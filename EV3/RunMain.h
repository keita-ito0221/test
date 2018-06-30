/**
 *RunMain.h
 */
#ifndef RUNMAIN_H_
#define RUNMAIN_H_

#include "ev3api.h"
	
class RunMain
{
public:
	RunMain();
	void init();
	void run();
	int getTurn();
	void move(int);
	void stop();

	int runmode = 0;

	typedef enum {
		NORMAL_RUNMODE = 0, //�ʏ푖�s�i���C���g���[�X�j
		SEESAW_RUNMODE = 1, //�V�[�\�[
		GATE_RUNMODE = 2,   //���b�N�A�b�v�Q�[�g
		GARAGE_RUNMODE = 3, //�Ԍɓ���
	} run_mode_t;
	
	int forward = 0;      /* �O��i���� */
	int turn = 0;         /* ���񖽗� */
	int pwm_L, pwm_R; /* ���E���[�^PWM�o�� */
	int gyro, volt;
	int32_t motor_ang_l, motor_ang_r;
	float target;

private:

};

#endif
