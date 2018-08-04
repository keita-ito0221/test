/**
 * RunGarage.cpp
 * 車庫入れ攻略クラス
 */
#define GOAL 40

#define STS_GARAGE_INIT       1
#define STS_GARAGE_RUN_STOP   2
#define STS_GARAGE_END        3

#include "RunGarage.h"
#include "RunMain.h"
#include "ev3api.h"

#include "Motor.h"
extern Motor motor;
/* コンストラクタ*/
RunGarage::RunGarage()
{
	sts = 1;
}

/**
 * デストラクタ
 */
RunGarage::~RunGarage()
{
}

/**
*処理開始
*/
void RunGarage::run() {
	move(50);
	
	while(1){
		switch(sts){
			case STS_GARAGE_INIT: //初期化
				motor.tail_control(); /* バランス走行用角度に制御 */
				//各モータの角位置をリセット
				motor.reset(motor.left_motor);
				motor.reset(motor.right_motor);
				sts = STS_GARAGE_RUN_STOP;
				break;
			
			case STS_GARAGE_RUN_STOP: //停止位置まで走る
				//40cmの時のモータの回転数を取得
				motor.setMovedistance(GOAL);
				//指定された回転数とを超えると止まる
				if(motor.getAveAngle() >= motor.getMovedistance()){
				//バランサを切る
				ter_tsk(BLN_TASK);
				ev3_motor_set_power(motor.left_motor,30);
				ev3_motor_set_power(motor.right_motor,30);
				tslp_tsk(150);
				stop();
				//尻尾を下げる
				motor.tail_down(95);
			
				sts = STS_GARAGE_END;
				}
			
				break;
			
			case STS_GARAGE_END:
				ter_tsk(BT_TASK);
				fclose(bt);
				ext_tsk();
				break;
		}
	}
}