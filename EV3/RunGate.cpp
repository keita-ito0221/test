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

#define PASS            35//ゲートの通過距離
#define TURN            51//ターンの距離

#define STS_GATE_DETE	0 //障害物のゲートを検知
#define STS_GATE_TILT	1 //ゲートを潜れる高さまで走行体を下げる
#define STS_GATE_RESE	2 //タイヤモータの角位置をリセット
#define STS_GATE_PASS	3 //ゲート通過
#define STS_GATE_VERT	4 //走行体を垂直に近づける
#define STS_GATE_TURN	5 //走行体を回転させる
#define STS_GATE_FINI	6 //走行体を停止させる

/**
 * コンストラクタ
 */
RunGate::RunGate()
{
	flg = 0;
	sts = 0;
}

/**
*処理開始
*/
/*
やりたい事
1.超音波で障害物を検知する     成功

2.自立制御を切って尻尾を出す   成功

3.ゲートを通過する             成功  距離要調整

4.走行体を垂直にする           成功

5.走行体を半回転させる         成功

6.2->3->4->5                2セット
*/
void RunGate::run() {
	while(1){
		switch(sts){
		case STS_GATE_DETE:                                     /**障害物**/
			move(60);
			if(ObstacleDetection(10)){                          //20cm以内に障害物があれば処理
				fputs("sonar\r\n",bt);
				ter_tsk(BLN_TASK);                              //バランサ停止
				ev3_motor_set_power(motor.left_motor, 40);      //車体が倒れないようにバランサを止めて少し前に進む
				ev3_motor_set_power(motor.right_motor, 40);
				motor.tail_down(80);                            //尻尾を下ろす
				stop();                                         //走行体停止
				sts = STS_GATE_TILT;
			}
			break;
			
		case STS_GATE_TILT:                                     /**走行体を傾ける**/
			fputs("tilt\r\n",bt);
			while(motor.getAngle(motor.tail_motor) >= 65){      //尻尾を65になるまで上げる
				motor.tail_up(-1);                              //尻尾を上げる
				tslp_tsk(10);
			}
			sts = STS_GATE_RESE;
			break;
			
		case STS_GATE_RESE:                                     /**モータのリセット**/
			fputs("reset\r\n",bt);
			motor.reset(motor.left_motor);                      //モータの回転数の初期化
			motor.reset(motor.right_motor);
			ev3_motor_set_power(motor.left_motor, 20);          //前に進む
			ev3_motor_set_power(motor.right_motor, 20);
			sts = STS_GATE_PASS;
			break;
			
		case STS_GATE_PASS:                                     /**ゲートの通過**/
			fputs("pass\r\n",bt);
			motor.setMovedistance(PASS);                        //35cmの時のモータの回転数を取得
			if(motor.getAveAngle() >= motor.getMovedistance()){ //指定された回転数とを超えると止まる
				stop();                                         //走行体停止
				tslp_tsk(1000);
				sts = STS_GATE_VERT;
			}
			break;
			
		case STS_GATE_VERT:                                     /**走行体を垂直に近づける**/
			fputs("vert\r\n",bt);
			ev3_motor_set_power(motor.left_motor, -40);         //尻尾を下げれるように止め少し後ろに下がる
			ev3_motor_set_power(motor.right_motor, -40);
			tslp_tsk(120);
			stop();                                             //走行体停止
			while(motor.getAngle(motor.tail_motor) <= 80){      //尻尾を80になるまで下げる
				motor.tail_down(1);                             //尻尾をゆっくり下げる
				tslp_tsk(10);
			}
			tslp_tsk(1000);
			flg++;
			if(flg == 3){
				sts = STS_GATE_FINI;                            //3回、回ったら終了
			}else{
				sts = STS_GATE_TURN;
				motor.reset(motor.left_motor);                  //モータの回転数の初期化
				motor.reset(motor.right_motor);
			}
			break;
			
		case STS_GATE_TURN:                                     /**走行体を半回転**/
			fputs("turn\r\n",bt);
			ev3_motor_set_power(motor.left_motor, 10);          //半回転
			ev3_motor_set_power(motor.right_motor, -10);
			motor.setMovedistance(TURN);                        //ターンするときの回転数を取得
			if(motor.getTurnAngle(motor.left_motor,motor.right_motor) >= motor.getMovedistance()){//半回転したら入る
				stop();                                         //走行体停止
				sts = STS_GATE_TILT;                            //走行体を傾ける
			}
			break;
		}
		if(sts == STS_GATE_FINI){                               //終了
			fputs("finish\r\n",bt);
			stop_flg = 1;                                       //stop_flgをapp.cppに渡す 1:終了
			break;
		}
	}
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
