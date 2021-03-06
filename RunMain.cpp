/**
 * RunMain.cpp
 * 走行クラス
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
 * コンストラクタ
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
targetの値を取得する
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
*処理開始
*/
void RunMain::run() {
	move(30);
}

/*
 *回転量を取得する
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
	//灰色を検知したらMODE_GRAYに切り替える
	int graycnt = 0;
	
	//灰色の閾値を設定
	//取得した色の値を配列に格納する
	for(int i=0;i<10;i++){
		if((colorlist[i]>=20) && (colorlist[i]<=40)){
			graycnt++;
		}
	}
	
	//配列の中に8つ灰色の値があれば灰色のライン上にいるとする
	if(graycnt >= 8){
		ev3_speaker_play_tone(NOTE_F6, 1000);
		line_color = MODE_GRAY;
	}
	
	//黒色を検知したらMODE_BLACKに切り替える
	if((color > 0) && (color < 10)){
		line_color = MODE_BLACK;
	}
	
	//fprintf(bt, "%s\r\n", "graycnt");
	//fprintf(bt, "%d\r\n", graycnt);
	
}

/*
 *速さを取得する
 */
int RunMain::getForward(){
	return forward;
}

/*
 *前進
 */
void RunMain::move(int _forward){
	forward = _forward;
}

/*
 *停止
 */
void RunMain::stop(){
	forward = 0;
	turn = 0;
	motor.stop();
}

