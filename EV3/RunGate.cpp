/**
 * RunGate.cpp
 * ���b�N�A�b�v�Q�[�g�U���N���X
 */

#include "RunGate.h"
#include "RunMain.h"
#include "ev3api.h"
#include "app.h"

#include "SonarSensor.h"
SonarSensor sonarsensor;

#include "Motor.h"
extern Motor motor;

#define PASS            35//�Q�[�g�̒ʉߋ���
#define TURN            51//�^�[���̋���

#define STS_GATE_DETE	0 //��Q���̃Q�[�g�����m
#define STS_GATE_TILT	1 //�Q�[�g�����鍂���܂ő��s�̂�������
#define STS_GATE_RESE	2 //�^�C�����[�^�̊p�ʒu�����Z�b�g
#define STS_GATE_PASS	3 //�Q�[�g�ʉ�
#define STS_GATE_VERT	4 //���s�̂𐂒��ɋ߂Â���
#define STS_GATE_TURN	5 //���s�̂���]������
#define STS_GATE_FINI	6 //���s�̂��~������

/**
 * �R���X�g���N�^
 */
RunGate::RunGate()
{
	flg = 0;
	sts = 0;
}

/**
*�����J�n
*/
/*
��肽����
1.�����g�ŏ�Q�������m����     ����

2.���������؂��ĐK�����o��   ����

3.�Q�[�g��ʉ߂���             ����  �����v����

4.���s�̂𐂒��ɂ���           ����

5.���s�̂𔼉�]������         ����

6.2->3->4->5                2�Z�b�g
*/
void RunGate::run() {
	while(1){
		switch(sts){
		case STS_GATE_DETE:                                     /**��Q��**/
			move(60);
			if(ObstacleDetection(10)){                          //20cm�ȓ��ɏ�Q��������Ώ���
				fputs("sonar\r\n",bt);
				ter_tsk(BLN_TASK);                              //�o�����T��~
				ev3_motor_set_power(motor.left_motor, 40);      //�ԑ̂��|��Ȃ��悤�Ƀo�����T���~�߂ď����O�ɐi��
				ev3_motor_set_power(motor.right_motor, 40);
				motor.tail_down(80);                            //�K�������낷
				stop();                                         //���s�̒�~
				sts = STS_GATE_TILT;
			}
			break;
			
		case STS_GATE_TILT:                                     /**���s�̂��X����**/
			fputs("tilt\r\n",bt);
			while(motor.getAngle(motor.tail_motor) >= 65){      //�K����65�ɂȂ�܂ŏグ��
				motor.tail_up(-1);                              //�K�����グ��
				tslp_tsk(10);
			}
			sts = STS_GATE_RESE;
			break;
			
		case STS_GATE_RESE:                                     /**���[�^�̃��Z�b�g**/
			fputs("reset\r\n",bt);
			motor.reset(motor.left_motor);                      //���[�^�̉�]���̏�����
			motor.reset(motor.right_motor);
			ev3_motor_set_power(motor.left_motor, 20);          //�O�ɐi��
			ev3_motor_set_power(motor.right_motor, 20);
			sts = STS_GATE_PASS;
			break;
			
		case STS_GATE_PASS:                                     /**�Q�[�g�̒ʉ�**/
			fputs("pass\r\n",bt);
			motor.setMovedistance(PASS);                        //35cm�̎��̃��[�^�̉�]�����擾
			if(motor.getAveAngle() >= motor.getMovedistance()){ //�w�肳�ꂽ��]���Ƃ𒴂���Ǝ~�܂�
				stop();                                         //���s�̒�~
				tslp_tsk(1000);
				sts = STS_GATE_VERT;
			}
			break;
			
		case STS_GATE_VERT:                                     /**���s�̂𐂒��ɋ߂Â���**/
			fputs("vert\r\n",bt);
			ev3_motor_set_power(motor.left_motor, -40);         //�K�����������悤�Ɏ~�ߏ������ɉ�����
			ev3_motor_set_power(motor.right_motor, -40);
			tslp_tsk(120);
			stop();                                             //���s�̒�~
			while(motor.getAngle(motor.tail_motor) <= 80){      //�K����80�ɂȂ�܂ŉ�����
				motor.tail_down(1);                             //�K����������艺����
				tslp_tsk(10);
			}
			tslp_tsk(1000);
			flg++;
			if(flg == 3){
				sts = STS_GATE_FINI;                            //3��A�������I��
			}else{
				sts = STS_GATE_TURN;
				motor.reset(motor.left_motor);                  //���[�^�̉�]���̏�����
				motor.reset(motor.right_motor);
			}
			break;
			
		case STS_GATE_TURN:                                     /**���s�̂𔼉�]**/
			fputs("turn\r\n",bt);
			ev3_motor_set_power(motor.left_motor, 10);          //����]
			ev3_motor_set_power(motor.right_motor, -10);
			motor.setMovedistance(TURN);                        //�^�[������Ƃ��̉�]�����擾
			if(motor.getTurnAngle(motor.left_motor,motor.right_motor) >= motor.getMovedistance()){//����]���������
				stop();                                         //���s�̒�~
				sts = STS_GATE_TILT;                            //���s�̂��X����
			}
			break;
		}
		if(sts == STS_GATE_FINI){                               //�I��
			fputs("finish\r\n",bt);
			stop_flg = 1;                                       //stop_flg��app.cpp�ɓn�� 1:�I��
			break;
		}
	}
}

/**
*��Q�����m����
*/
int RunGate::ObstacleDetection(int _distance){
	if(sonarsensor.getDetection() <= _distance){
		return 1;
	}
	return 0;
}
