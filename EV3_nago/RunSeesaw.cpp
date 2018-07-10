/**
 * RunSeesaw.cpp
 * シーソ攻略クラス
 */

//****************************************************/
//switch文で状態を判断し、その状況に合わせた処理を行い
//シーソーをクリアする
//****************************************************/

#include "RunSeesaw.h"
#include "RunMain.h"
#include "ev3api.h"
#include "Motor.h"
extern Motor motor;
#include "GyroSensor.h"
extern GyroSensor gyrosensor;

#define INIT	0 //シーソーに入る為に初期化
#define TRACE	1 //シーソー用のライントレースモード
#define STEP_COLLISION	2 //段差衝突モード
#define ST_COLLISION	3 //真っ直ぐ衝突するモード


/**
 * コンストラクタ
 */
RunSeesaw::RunSeesaw()
{	
}

/**
*処理開始
*/
void RunSeesaw::run() {
	
	
	//int state = INIT;
	
	//while(1){
	//	
	//	switch(state){
	//		case INIT:
	//			/************************************************
	//			  シーソーで初期化しなければならないものを初期化
	//			************************************************/
	//			state = TRACE;
	//			break;
	//		case TRACE:
	//			/**********************************
	//			 シーソーの際のライントレースの処理
	//			 **********************************/
	//			move(20);
	//			
	//			
	//			/**************************
	//			 段差にぶつかった際の条件式
	//			***************************/
	//			
	//			if(gyrosensor.getAngle() >= 100){
	//				state = STEP_COLLISION;
	//			}
	//			
	//			break;
	//		case STEP_COLLISION:
	//			/************************
	//			 段差にぶつかった際の処理
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

