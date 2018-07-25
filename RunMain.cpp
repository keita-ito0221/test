/**
 * RunMain.cpp
 * ���s�N���X
 */

#include "RunMain.h"
#include "ev3api.h"
#include "PID.h"
PID *pid;
#include "Motor.h"
Motor motor;
#include "GyroSensor.h"
GyroSensor gyrosensor;
#include "ColorSensor.h"
ColorSensor colorsensor;

extern FILE *bt;

/**
 * �R���X�g���N�^
 */
RunMain::RunMain()
{
	target = setTarget();
	pid = new PID();
	
	for(int i=0;i<10;i++){
	   colorlist[i]=0;
	}
	cnt = -1000;
	flg = false;
	ev3_speaker_set_volume(20);
}

/*
target�̒l���擾����
*/
float RunMain::setTarget(){
	if(line_color == MODE_BLACK){
		target = (LIGHT_WHITE + LIGHT_BLACK)/2;
	}
	else{
		target = (LIGHT_WHITE + LIGHT_GRAY)/2;
	}
	fprintf(bt, "%d\r\n", line_color);
	return target;
}

/**
*�����J�n
*/
void RunMain::run() {
	move(30);
}

/*
 *��]�ʂ��擾����
 */
int RunMain::getTurn(){
	int color = colorsensor.getReflect();
	/*fprintf(bt, "%s\r\n", "color");
	fprintf(bt, "%d\r\n", color);
	fprintf(bt, "%s\r\n", "cnt");
	fprintf(bt, "%d\r\n", cnt);*/
	
	fprintf(bt, "%d,%d\r\n", color,cnt);
	if(cnt>=30){
		cnt = 0;
	}
	
	if((cnt > 0) && (cnt % 3 == 0)){
		colorlist[cnt/3] = color;
	    find_gray(color);
	}
	
	cnt++;
	
	target = setTarget();
	int diff = target - color;
	//int diff = target - colorsensor.getReflect();
	return pid->calcControllValue(diff, getForward());
}

void RunMain::find_gray(int color){
	//�D�F�����m������MODE_GRAY�ɐ؂�ւ���
	int graycnt = 0;
	
	//�D�F��臒l��ݒ�
	//�擾�����F�̒l��z��Ɋi�[����
	for(int i=0;i<10;i++){
		if((colorlist[i]>=20) && (colorlist[i]<=40)){
			graycnt++;
		}
	}
	
	//�z��̒���8�D�F�̒l������ΊD�F�̃��C����ɂ���Ƃ���
	if(graycnt >= 8){
		ev3_speaker_play_tone(NOTE_F6, 1000);
		line_color = MODE_GRAY;
	}
	
	//���F�����m������MODE_BLACK�ɐ؂�ւ���
	if((color > 0) && (color < 10)){
		line_color = MODE_BLACK;
	}
	
	//fprintf(bt, "%s\r\n", "graycnt");
	//fprintf(bt, "%d\r\n", graycnt);
	
}

/*
 *�������擾����
 */
int RunMain::getForward(){
	return forward;
}

/*
 *�O�i
 */
void RunMain::move(int _forward){
	forward = _forward;
}

/*
 *��~
 */
void RunMain::stop(){
	forward = 0;
	turn = 0;
	motor.stop();
}

