/**
 ******************************************************************************
 ** ファイル名 : app.c
 **
 ** 概要 : 2輪倒立振子ライントレースロボットのTOPPERS/HRP2用Cサンプルプログラム
 **
 ** 注記 : sample_c4 (sample_c3にBluetooth通信リモートスタート機能を追加)
 ******************************************************************************
 **/

#include "ev3api.h"
#include "app.h"
#include "balancer.h"


#if defined(BUILD_MODULE)
#include "module_cfg.h"
#else
#include "kernel_cfg.h"
#endif

#define DEBUG

#ifdef DEBUG
#define _debug(x) (x)
#else
#define _debug(x)
#endif

static int bt_cmd = 0;     /* Bluetoothコマンド 1:リモートスタート */
FILE     *bt = NULL;     /* Bluetoothファイルハンドル */

/* 下記のマクロは個体/環境に合わせて変更する必要があります */
/* sample_c3マクロ */
#define TAIL_ANGLE_STAND_UP  87 /* 完全停止時の角度[度] */
#define PWM_ABS_MAX          60 /* 完全停止用モータ制御PWM絶対最大値 */
#define GYRO_OFFSET  0       /* ジャイロセンサオフセット値(角速度0[deg/sec]時) */
/* sample_c4マクロ */
//#define DEVICE_NAME     "ET0"  /* Bluetooth名 SD:\ev3rt\etc\rc.conf.ini で設定 */
//#define PASS_KEY        "1234" /* パスキー    SD:\ev3rt\etc\rc.conf.ini で設定 */
#define CMD_START         '1'    /* リモートスタートコマンド */

/* LCDフォントサイズ */
#define CALIB_FONT (EV3_FONT_SMALL)
#define CALIB_FONT_WIDTH (6/*TODO: magic number*/)
#define CALIB_FONT_HEIGHT (8/*TODO: magic number*/)

#include "RunMain.h"
#include "RunNormal.h"
#include "RunGarage.h"
#include "RunGate.h"
#include "RunSeesaw.h"
RunMain *runmain = NULL;
#include "Motor.h"
Motor *_motor;
#include "GyroSensor.h"
GyroSensor *_gyrosensor;
#include "ColorSensor.h"
ColorSensor *_colorsensor;
#include "TouchSensor.h"
TouchSensor *_touchsensor;
#include "SonarSensor.h"
SonarSensor *_sonarsensor;
#include "BalancerCpp.h" 
Balancer balancer;
#include "LinetraceCpp.h" 
Linetrace linetrace;

//走行を終了する判定フラグ
bool is_finished = false;   //シーソー、ガレージが終了すればtrueになる。
bool is_error    = false;   //続行不可

int runmode = NORMAL_RUNMODE;

/* メインタスク */
void main_task(intptr_t unused){

	int runmode_backup = NO_RUNMODE;     
	int course_mode = R_COURSE;
	
	/* LCD画面表示 */
	ev3_lcd_fill_rect(0, 0, EV3_LCD_WIDTH, EV3_LCD_HEIGHT, EV3_LCD_WHITE);
	ev3_lcd_draw_string("EV3way-ET sample_cpp4", 0, CALIB_FONT_HEIGHT*1);

	/* 各センサー類の初期化 */
	_motor = new Motor;
	_motor->init();
	_gyrosensor = new GyroSensor;
	_gyrosensor->init();
	_colorsensor = new ColorSensor;
	_colorsensor->init();
	_touchsensor = new TouchSensor;
	_touchsensor->init();
	_sonarsensor = new SonarSensor;
	_sonarsensor->init();

	/* Open Bluetooth file */
	bt = ev3_serial_open_file(EV3_SERIAL_BT);
	assert(bt != NULL);

	/* Bluetooth通信タスクの起動 */
	act_tsk(BT_TASK);

	ev3_led_set_color(LED_ORANGE); /* 初期化完了通知 */
	ev3_motor_rotate(_motor->tail_motor, TAIL_ANGLE_STAND_UP, PWM_ABS_MAX, true);
	//ev3_lcd_draw_string("尻尾処理前", 0, CALIB_FONT_HEIGHT*1);
	/* スタート待機 */
	while(1){
		ev3_lcd_draw_string("                       ", 0, CALIB_FONT_HEIGHT*1);
		ev3_lcd_draw_string("ready", 0, CALIB_FONT_HEIGHT*1);
		//tail_control(TAIL_ANGLE_STAND_UP); /* 完全停止用角度に制御 */

		if (bt_cmd == 3){
			course_mode = R_COURSE;
			ev3_lcd_draw_string(" COURSE:Right     ", 5, CALIB_FONT_HEIGHT*1);
		} else if (bt_cmd == 4){
			course_mode = L_COURSE;
			ev3_lcd_draw_string(" COURSE:Left      ", 5, CALIB_FONT_HEIGHT*1);
		} else if (bt_cmd == 1){
			break; /* リモートスタート */
		}

		tslp_tsk(10); /* 10msecウェイト */
	}

	/* 走行モーターエンコーダーリセット */
	_motor->reset(_motor->left_motor);
	_motor->reset(_motor->right_motor);

	/* ジャイロセンサーリセット */
	_gyrosensor->reset();
	balancer.init(GYRO_OFFSET);
	linetrace.init();
	
	
	ev3_led_set_color(LED_GREEN); /* スタート通知 */
	
	_motor->tail_control();/* バランス走行用角度に制御 */
	act_tsk(BLN_TASK); //バランサ起動
	act_tsk(BT_LOG);   //ログ起動
	/**
	* Main loop for the self-balance control algorithm
	*/
	while(!is_finished && !is_error){
		if(runmode_backup != runmode){
			//走行処理
			switch (runmode) {
				case NORMAL_RUNMODE:
					fprintf(bt, "%s\r\n", "RunNormal");
					runmain = new RunNormal(course_mode);
					break;
				case SEESAW_RUNMODE:
					fprintf(bt, "%s\r\n", "RunSeesaw");
					runmain = new RunSeesaw;
					break;
				case GATE_RUNMODE:
					fprintf(bt, "%s\r\n", "RunGate");
					runmain = new RunGate;
					break;
				case GARAGE_RUNMODE:
					fprintf(bt, "%s\r\n", "RunGarage");
					runmain = new RunGarage;
					break;
				default:
					fprintf(bt, "%s\r\n", "RunMain");
					runmain = new RunMain;
					break;
			}
			runmode_backup = runmode;
		}
			
		runmain->run();
		fputs("runmain_run\r\n",bt);
		if(runmode != runmode_backup){
			fputs("delete runmain\r\n",bt);
			delete runmain;
			runmain = NULL;
		}
		fprintf(bt,"runmode:%d runmode_backup:%d\r\n",runmode ,runmode_backup);
//		if (bt_cmd == 2 || runmain->stop_flg == 1) break;
	}
  
//	runmain->stop();

	ter_tsk(BT_TASK);
	ter_tsk(BLN_TASK);
	ter_tsk(BT_LOG);
	fclose(bt);

	ext_tsk();
}

//*****************************************************************************
// 関数名 : bt_task
// 引数 : unused
// 返り値 : なし
// 概要 : Bluetooth通信によるリモートスタート。 Tera Termなどのターミナルソフトから、
//       ASCIIコードで1を送信すると、リモートスタートする。
//*****************************************************************************
void bt_task(intptr_t unused){
	while(1){
		uint8_t c = fgetc(bt); /* 受信 */
		switch(c){
			case '1':
				bt_cmd = 1;
				break;
			case '2':
				bt_cmd = 2;
				break;
			case '3':       //r
				bt_cmd = 3;
				break;
			case '4':       //l
				bt_cmd = 4;
				break;
		default:
				break;
		}
	}
}

//*****************************************************************************
// 関数名 : bln_task
// 引数 : unused
// 返り値 : なし
// 概要 : スタートと同時に自立制御をする。
//*****************************************************************************
void bln_task(intptr_t unused){
	//セットする値の取得
	while(1){
		signed char pwm_L, pwm_R;
		int32_t motor_ang_l = _motor->getAngle(_motor->left_motor);
		int32_t motor_ang_r = _motor->getAngle(_motor->right_motor);
		int gyro = _gyrosensor->getRate();
		int volt = ev3_battery_voltage_mV();
		int turn = 0;
		int forward = 0;
		
		if(runmain != NULL){
			turn = runmain->getTurn();
			forward = runmain->getForward();
			//バランサーに値のセット。
			balancer.setCommand(forward, turn);
			balancer.update(gyro, motor_ang_r, motor_ang_l, volt);
		}
		
		//回転量の取得
		pwm_L = balancer.getPwmRight();
		pwm_R = balancer.getPwmLeft();
		
		//左右のモータにセット
		/* EV3ではモーター停止時のブレーキ設定が事前にできないため */
		/* 出力0時に、その都度設定する */
		if (pwm_L == 0){
			ev3_motor_stop(_motor->left_motor, true);
		}
		else{
			ev3_motor_set_power(_motor->left_motor, (int)pwm_L);
		}
		
		if (pwm_R == 0){
			ev3_motor_stop(_motor->right_motor, true);
		}
		else{
			ev3_motor_set_power(_motor->right_motor, (int)pwm_R);
		}
		
		tslp_tsk(4);
	}
}

//*****************************************************************************
// 関数名 : ltr_task
// 引数 : unused
// 返り値 : なし
// 概要 : ライントレースだけを行う。
//*****************************************************************************
void ltr_task(intptr_t unused){
	//セットする値の取得
	while(1){
		signed char pwm_L, pwm_R;
		int32_t motor_ang_l = _motor->getAngle(_motor->left_motor);
		int32_t motor_ang_r = _motor->getAngle(_motor->right_motor);
		int volt = ev3_battery_voltage_mV();
		int turn = 0;
		int forward = 0;
		
		if(runmain != NULL){
			turn = runmain->getTurn();
			forward = runmain->getForward();
			//バランサーに値のセット。
			linetrace.setCommand(forward, turn);
			linetrace.update_linetrace(motor_ang_r, motor_ang_l, volt);
		}
		
		//回転量の取得
		pwm_L = linetrace.getPwmRight();
		pwm_R = linetrace.getPwmLeft();
		
		//左右のモータにセット
		/* EV3ではモーター停止時のブレーキ設定が事前にできないため */
		/* 出力0時に、その都度設定する */
		if (pwm_L == 0){
			ev3_motor_stop(_motor->left_motor, true);
		}
		else{
			ev3_motor_set_power(_motor->left_motor, (int)pwm_L);
		}
		
		if (pwm_R == 0){
			ev3_motor_stop(_motor->right_motor, true);
		}
		else{
			ev3_motor_set_power(_motor->right_motor, (int)pwm_R);
		}
		
		tslp_tsk(4);
	}
}

//*****************************************************************************
// 関数名 : bt_log
// 引数 : unused
// 返り値 : なし
// 概要 : Bluetooth通信によるログを取得する。
//*****************************************************************************
void bt_log(intptr_t unused){
	
	fputs("left,right,tail\r\n",bt);
	while(1){
		//fprintf(bt, "%d,%d,%d\r\n", int(_motor->getAngle(_motor->left_motor)),int(_motor->getAngle(_motor->right_motor)),int(_motor->getAngle(_motor->tail_motor)));
		//fprintf(bt,"%d,gyro:%d\r\n",_colorsensor->getReflect(),gyro);
		
		if(!_colorsensor->getReflect()){
			fprintf(bt, "%s\r\n", "stop");
			_motor->stop();
			ter_tsk(BT_TASK);
			ter_tsk(MAIN_TASK);
			ter_tsk(BLN_TASK);
			ext_tsk();
		}
		
		
		tslp_tsk(250);
	}
}
