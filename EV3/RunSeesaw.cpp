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
#include "BalancerCpp.h"
extern Balancer balancer;

#define INIT			0 //�V�[�\�[�ɓ���ׂɏ�����
#define TRACE			1 //�V�[�\�[�p�̃��C���g���[�X���[�h
#define STEP_COLLISION	2 //�i���Փ˃��[�h
#define ST_COLLISION	3 //�^�������Փ˂��郂�[�h
#define COLLISION_BACK	4 //��x�o�b�N����
#define STOP			5 //�~�܂�
#define SPEED			6
#define SPEED_UP		7
#define BACK			8
#define FOWARD			9
#define TAIL_DOWN		10
/**
 * �R���X�g���N�^
 */
RunSeesaw::RunSeesaw()
{	
	left_motor_count = 0;
	right_motor_count = 0;
	for(int i = 0; i <= 1;i++){
		array[i] = 0;
		left_count[i] = 0;
		right_count[i] = 0;
	}
	left_angle = 0;
	right_angle = 0;
	power = 0;
}

/**
*�����J�n
*/
void RunSeesaw::run() {
	
	
	//motor.tail_down(-10);
	//motor.tail_control();
	//move(-30);
	
	//int speed = 10;
	//int count = 0;
	
	//int left_diff;
	//int right_diff;

	int state = INIT;
	ter_tsk(BLN_TASK);

	while(1){
		switch(state){
			
			//����������
			case INIT:
				motor.setMovedistance(20);
				ev3_motor_set_power(motor.left_motor, 8);
				ev3_motor_set_power(motor.right_motor, 8);
				if(motor.getAveAngle() >= motor.getMovedistance()){
					state = STEP_COLLISION;
				}
				break;
			//�i���ɏՓ˂������s�̂�^�������ɂ��鏈��
			case STEP_COLLISION:
				
				//���^�C������~������
				if(motor.getPower(motor.left_motor) == 0 && motor.getPower(motor.right_motor) == 0){
					motor.reset(motor.left_motor);
					motor.reset(motor.right_motor);
					motor.setMovedistance(-20);
					state = BACK;
					
				//�E�^�C�����i���ɏՓ˂��Ă����
				}else if(motor.getPower(motor.right_motor) == 0){
					ev3_motor_set_power(motor.right_motor, 0);
					ev3_motor_set_power(motor.left_motor, 8);
					
				//���^�C�����i���ɏՓ˂��Ă����
				}else if(motor.getPower(motor.left_motor) == 0){
					ev3_motor_set_power(motor.right_motor, 8);
					ev3_motor_set_power(motor.left_motor, 0);
					
				//�Փ˂��Ă��Ȃ���
				}else{
					ev3_motor_set_power(motor.left_motor, 8);
					ev3_motor_set_power(motor.right_motor, 8);
				}
				break;
			
			//�o�b�N���鏈��
			case BACK:
				ev3_motor_set_power(motor.left_motor, -9);
				ev3_motor_set_power(motor.right_motor, -9);
				if(motor.getAveAngle() <= motor.getMovedistance()){
					motor.reset(motor.left_motor);
					motor.reset(motor.right_motor);
					motor.setMovedistance(90);
					//act_tsk(BLN_TASK);
					//balancer.setOffset(16);
					state = TAIL_DOWN;
				}
				break;
			
			//��x��~���|����������s��Ԃɂ��鏈��
			case TAIL_DOWN:
				ev3_motor_set_power(motor.left_motor, 0);
				ev3_motor_set_power(motor.right_motor, 0);
				motor.tail_down(4);
				tslp_tsk(1000);
				act_tsk(BLN_TASK);
				move(80);
				//balancer.setOffset(15);
				state = SPEED_UP;
				break;
				
			//�i��������Ă��鏈��
			case SPEED_UP:
				//move(3);
				motor.tail_control();
				if(motor.getAveAngle() >= motor.getMovedistance()){
					state = STOP;
				}
				break;
			
			//���S��~���鏈��
			case STOP:
				motor.stop();
				ter_tsk(BLN_TASK);
				motor.tail_down(60);
				break;
				
		}
		
	}
	
	
	
	
	
	
#if 0
	while(1){
		switch(state){
			
			//�w�苗���܂Œʏ푖�s
			case INIT:
				
				balancer.setOffset(16);
				motor.tail_control();
				motor.setMovedistance(20);
				if(motor.getAveAngle() >= motor.getMovedistance()){
					state = SPEED;
					
				}
				break;
				
			//�i�������ׂɃX�s�[�h��������
			case SPEED:
				balancer.setOffset(16);
				array[0] = motor.getPower(motor.left_motor);
				state = TRACE;
				break;
			
			//�i������������ǂ������肷��
			case TRACE:
				motor.tail_control();
				array[1] = motor.getPower(motor.left_motor);
				if(array[0] - array[1] >= 10){
					fprintf(bt, "%s\r\n", "TRACE");
					left_angle = motor.getAngle(motor.left_motor);
					right_angle = motor.getAngle(motor.right_motor);
					motor.setMovedistance(33);
					//motor.tail_down(47);
					state = STEP_COLLISION;
				}
				array[0] = array[1];
				break;
			
			//�i��������Ă��鏈��
			case STEP_COLLISION:
				fprintf(bt, "%s\r\n", "STEP_COLLISION");
				motor.tail_control();
				left_diff = motor.getAngle(motor.left_motor) - left_angle;
				right_diff = motor.getAngle(motor.right_motor) - right_angle;
				if(((left_diff + right_diff) / 2) >= motor.getMovedistance()){
					//balancer.setOffset(-20);
					motor.setMovedistance(10);
					state = BACK;
				}
				break;
				
			
			case BACK:
				fprintf(bt, "%s\r\n", "BACK");
				//move(-20);
				balancer.setOffset(-11);
				motor.tail_control();
				left_diff = motor.getAngle(motor.left_motor) - left_angle;
				right_diff = motor.getAngle(motor.right_motor) - right_angle;
				if((left_diff + right_diff) / 2 <= motor.getMovedistance()){
					
					state = FOWARD;
				}
				break;
			
			case FOWARD:
				fprintf(bt, "%s\r\n", "FOWARD");
				balancer.setOffset(16);
				motor.tail_control();
				break;
			
			case STOP:
				fprintf(bt, "%s\r\n", "STOP");
				motor.stop();
				ter_tsk(BLN_TASK);
				motor.tail_down(5);
				break;
		}
	}
#endif
		
#if 0	
	while(1){
		
		switch(state){
			
			//�V�[�\�[�ŏ��������Ȃ���΂Ȃ�Ȃ����̂����������鏈��
			case INIT:
				//fprintf(bt, "%s\r\n", "INIT");
				//motor.reset(motor.left_motor);
				//motor.reset(motor.right_motor);
				state = TRACE;
				//motor.setMovedistance(20);
				//state = SPEED;
				break;
			
			//�V�[�\�[�̍ۂ̃��C���g���[�X�̏���
			case TRACE:
				fprintf(bt, "%s\r\n", "TRACE");
				balancer.setOffset(16);
				motor.tail_control();
				array[count] = motor.getPower(motor.left_motor);
				count++;
				//�i���ɂԂ�������
				if(array[]){
				//if(motor.getAveAngle() >= motor.getMovedistance()){
					motor.reset(motor.left_motor);
					motor.reset(motor.right_motor);
					motor.setMovedistance(-20);
					state = SPEED_UP;
				}
				
				break;
				
				
			case BACK:
				fprintf(bt, "%s\r\n", "BACK");
				move(-30);
				motor.tail_control();
				if(motor.getAveAngle() <= motor.getMovedistance()){
					motor.reset(motor.left_motor);
					motor.reset(motor.right_motor);
					left_motor_count = motor.getAngle(motor.left_motor);
					right_motor_count = motor.getAngle(motor.right_motor);
					motor.setMovedistance(30);
					state = SPEED_UP;
				}
				break;
				
			//case SPEED:
				//move(speed + count++ / 200);
				//fprintf(bt, "%d\r\n", speed + count / 200);
				//motor.tail_control();
				//break;
			
			case SPEED_UP:
				fprintf(bt, "%s\r\n", "SPEED_UP");
				balancer.setOffset(8);
				motor.tail_control();
				if(motor.getAveAngle() >= motor.getMovedistance()){
					balancer.setOffset(0);
					left_motor_count = motor.getAngle(motor.left_motor);
					right_motor_count = motor.getAngle(motor.right_motor);
					motor.setMovedistance(40);
					state = STEP_COLLISION;
				}
				break;
			
			
			case STEP_COLLISION:
				fprintf(bt, "%s\r\n", "COLLISION");
				move(8);
				//fprintf(bt, "%d\r\n", speed + count / 1000);
				motor.tail_control();
				//�w�肵�������ɓ��B
				if(((motor.getAngle(motor.left_motor) - left_motor_count) + (motor.getAngle(motor.right_motor) - right_motor_count)) / 2 >= motor.getMovedistance()){
					motor.reset(motor.left_motor);
					motor.reset(motor.right_motor);
					motor.setMovedistance(-20);
					state = COLLISION_BACK;
				}
				
				break;
				
			//�o�b�N���鏈��
			case COLLISION_BACK:
				fprintf(bt, "%s\r\n", "COLLISION_BACK");
				balancer.setOffset(-15);
				motor.tail_control();
				//ev3_motor_set_power(motor.left_motor, -20);
				//ev3_motor_set_power(motor.right_motor, -20);
				//�w�肵�������ɓ��B�����ۂ̏���
				if(motor.getAveAngle() <= motor.getMovedistance()){
					state = STOP;
				}
				break;
			
			//�~�܂鏈��
			case STOP:
				fprintf(bt, "%s\r\n", "STOP");
				motor.stop();
				break;
		}
		
	}
#endif
	
	
#if 0
	move(30);
	motor.tail_control();
	
	//�i���ɏՓ˂����ۂ̏���
	if(gyrosensor.getRate() >= 150){
		fprintf(bt, "%s\r\n", "collision");
		motor.reset(motor.left_motor);
		motor.reset(motor.right_motor);
		motor.setMovedistance(80);
		while(1){
			//�w�肵�������ɓ��B�����ۂ̏���
			if(motor.getAveAngle() >= motor.getMovedistance()){
				while(1){
					motor.stop();
					ter_tsk(BLN_TASK);
					motor.tail_down(60);
				}
			}
			move(8);
			motor.tail_control();
		}
	}
#endif
	
#if 0
	move(30);
	motor.tail_control();
	motor.setMovedistance(20);
	if(motor.getAveAngle() >= motor.getMovedistance()){
	motor.reset(motor.left_motor);
	motor.reset(motor.right_motor);
	balancer.setOffset(0.05);
	motor.setMovedistance(20);
	fprintf(bt, "%s\r\n", "offset1");
		while(1){
			move(30);
			motor.tail_control();
			if(motor.getAveAngle() >= motor.getMovedistance()){
				fprintf(bt, "%s\r\n", "offset2");
				balancer.setOffset(0);
				while(1){
					move(8);
					motor.tail_control();
				}
			}
		}
	}
#endif
#if 0
	move(30);
	motor.tail_control();
	motor.setMovedistance(20);
	if(motor.getAveAngle() >= motor.getMovedistance()){
		motor.reset(motor.left_motor);
		motor.reset(motor.right_motor);
		motor.setMovedistance(80);
		while(1){
			balancer.setOffset(10);
			move(8);
			motor.tail_control();
			if(motor.getAveAngle() >= motor.getMovedistance()){
				while(1){
					motor.stop();
					ter_tsk(BLN_TASK);
					motor.tail_down(60);
				}
			}
		}
	}
#endif
	
	/*�W���C���̃I�t�Z�b�g�l��ݒ肵���s�̂��X���Ă���悤�ɔF�������X�s�[�h�������Ă���*/
#if 0
	//move(30);
	motor.tail_control();
	balancer.setOffset(16);
	motor.setMovedistance(70);
	fprintf(bt, "%d,%d\r\n", motor.getPower(motor.left_motor), motor.getPower(motor.right_motor));
	array[count] = motor.getPower(motor.left_motor);
	count++;
	if(motor.getAveAngle() >= motor.getMovedistance()){
		motor.reset(motor.left_motor);
		motor.reset(motor.right_motor);
		motor.setMovedistance(-15);
		move(-30);
		
		while(1){
			if(motor.getAveAngle() <= motor.getMovedistance()){
				motor.stop();
				ter_tsk(BLN_TASK);
				motor.tail_down(60);
			}
			
			motor.tail_down(10);
			balancer.setOffset(-20);
			
		}
	}
#endif
	//ter_tsk(BLN_TASK);
	//ev3_motor_set_power(motor.left_motor, 2);
	//ev3_motor_set_power(motor.right_motor, 2);
	
	
	
	
}


