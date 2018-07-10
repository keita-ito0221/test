/**
 * RunGate.cpp
 * ルックアップゲート攻略クラス
 */

#include "RunGate.h"
#include "RunMain.h"
#include "ev3api.h"
#include "app.h"

#include "SonarSensor.h"
SonarSensor sonarsensor;

#include "Motor.h"
extern Motor motor;

/**
 * コンストラクタ
 */
RunGate::RunGate()
{
	flg = 0;
}

/**
*処理開始
*/
/*
やりたい事
1.超音波で障害物を検知する     成功

2.自立制御を切って尻尾を出す   成功

3.ゲートを通過する             成功

4.自立制御を起動する

5.走行体を半回転させる

6.2->3->4->5                2セット
*/
void RunGate::run() {
	while(1){
		move(60);
		switch(flg){
		case 0:													//障害物
			if(ObstacleDetection(20)){							//20cm以内に障害物があれば処理
				fputs("sonar\r\n",bt);
				ter_tsk(BLN_TASK);								//バランサ停止
				ev3_motor_set_power(motor.left_motor, 40); 		//車体が倒れないようにバランサを止めて少し前に進む
				ev3_motor_set_power(motor.right_motor, 40);
				motor.tail_down(80);							//尻尾を下ろす
				flg = 1;
			}
			break;
		case 1:													//モータのリセット
			fputs("reset\r\n",bt);
			motor.reset(motor.left_motor);						//モータの回転数の初期化
			motor.reset(motor.right_motor);										
			flg = 2;
			break;			
		case 2:													//ゲートの通過
			motor.setMovedistance(35);							//50cmの時のモータの回転数を取得
			if(motor.getAveAngle() >= motor.getMovedistance()){	//指定された回転数とを超えると止まる
				stop();											//走行体停止
				tslp_tsk(1000);
				flg = 3;
			}
			break;
		case 3:													//自立制御起動
			ev3_motor_set_power(motor.tail_motor, 100);
			ev3_motor_set_power(motor.left_motor, -40); 		//尻尾を下げれるように止め少し後ろに下がる
			ev3_motor_set_power(motor.right_motor, -40);
			tslp_tsk(100);
			stop();
			for(int i = 0;i <= 7;i++){							//尻尾をゆっくり上げる
				fprintf(bt,"up:%d\r\n",i);
				motor.tail_down(3);
				tslp_tsk(100);
			}

			/////////////
			//    バランサ起動させる act_tsk(BLN_TASK); //バランサ起動
			//   motor.tail_control();/* バランス走行用角度に制御 */
			/////////////
			flg = 4;
			break;
		case 4:													//走行体を半回転
			flg = 5;
			break;
		default:
			break;
		}
		if(flg <= 5) break;

	}
	//stop();
	ter_tsk(BT_LOG);
}

/**
*障害物検知判定
*/
int RunGate::ObstacleDetection(int _distance){
	if(sonarsensor.getDetection() <= _distance){
		return 1;
	}
	return 0;
}
