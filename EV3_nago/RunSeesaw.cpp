/**
 * RunSeesaw.cpp
 * �V�[�\�U���N���X
 */

//****************************************************/
//switch���ŏ�Ԃ𔻒f���A���̏󋵂ɍ��킹���������s��
//�V�[�\�[���N���A����
//****************************************************/

#include "RunSeesaw.h"
#include "RunMain.h"
#include "ev3api.h"
#include "Motor.h"
extern Motor motor;
#include "GyroSensor.h"
extern GyroSensor gyrosensor;

#define INIT	0 //�V�[�\�[�ɓ���ׂɏ�����
#define TRACE	1 //�V�[�\�[�p�̃��C���g���[�X���[�h
#define STEP_COLLISION	2 //�i���Փ˃��[�h
#define ST_COLLISION	3 //�^�������Փ˂��郂�[�h


/**
 * �R���X�g���N�^
 */
RunSeesaw::RunSeesaw()
{	
}

/**
*�����J�n
*/
void RunSeesaw::run() {
	
	
	//int state = INIT;
	
	//while(1){
	//	
	//	switch(state){
	//		case INIT:
	//			/************************************************
	//			  �V�[�\�[�ŏ��������Ȃ���΂Ȃ�Ȃ����̂�������
	//			************************************************/
	//			state = TRACE;
	//			break;
	//		case TRACE:
	//			/**********************************
	//			 �V�[�\�[�̍ۂ̃��C���g���[�X�̏���
	//			 **********************************/
	//			move(20);
	//			
	//			
	//			/**************************
	//			 �i���ɂԂ������ۂ̏�����
	//			***************************/
	//			
	//			if(gyrosensor.getAngle() >= 100){
	//				state = STEP_COLLISION;
	//			}
	//			
	//			break;
	//		case STEP_COLLISION:
	//			/************************
	//			 �i���ɂԂ������ۂ̏���
	//			*************************/
	//			break;
	//	}
	//	
	//}
	
	motor.tail_control();
	move(20);
}

void collision(){
	
	ter_tsk(BLN_TASK);
	
	
}

