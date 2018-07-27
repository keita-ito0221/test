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

#define LOOP_CNT 10
#define LOOP_SPAN 3

/**
 * コンストラクタ
 */
RunMain::RunMain()
{
	//gray_flg = 0;                       /////////////////////////////
	target = setTarget();
	pid = new PID();
	
	for(int i=0;i<LOOP_CNT;i++){
	   colorlist[i]=0;
	}
	cnt = -1000;
	flg = false;
	ev3_speaker_set_volume(1);
}


/**
 * デストラクタ
 */
RunMain::~RunMain()
{
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
	//fprintf(bt, "%d\r\n", line_color);
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
	
	//fprintf(bt, "%d,%d\r\n", color,cnt);
	if(cnt>=LOOP_CNT * LOOP_SPAN){
		cnt = 0;
	}
	
	if((cnt > 0) && (cnt % LOOP_SPAN == 0)){
		colorlist[cnt/LOOP_SPAN] = color;
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
	if((color >= 25) && (color <= 35)){
		//fprintf(bt,"color:%d\r\n",color);
	}
	//灰色の閾値を設定
	//取得した色の値を配列に格納する
	for(int i=0;i<LOOP_CNT;i++){
		if((colorlist[i]>=25) && (colorlist[i]<=35)){
			graycnt++;
		}
//		else{
//			line_color = MODE_BLACK;
//		}
	}

	if(color < 20){
		line_color = MODE_BLACK;
		graycnt = 0;
		for(int i = 0; i<LOOP_CNT; i++){
			colorlist[i] = 0;	// 初期化
		}
	}	
	//配列の中に8つ灰色の値があれば灰色のライン上にいるとする
	if(graycnt >= 8){
		//gray_flg++;
		//if(gray_flg >= 15){                                     /////////////////////////////////////////
			ev3_speaker_play_tone(NOTE_C4, 250);
			//fprintf(bt,"gray_flg:%d\r\n",gray_flg);
		//}
		line_color = MODE_GRAY;
	}
	else{
		line_color = MODE_BLACK;
		//gray_flg = 0;
	}
	
	//黒色を検知したらMODE_BLACKに切り替える
	/*if((color > 0) && (color < 10)){
		line_color = MODE_BLACK;
	}*/
	
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

