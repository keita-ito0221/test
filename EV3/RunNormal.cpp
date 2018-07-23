/**
 * RunNormal.cpp
 * 通常走行クラス
 */

#include "RunNormal.h"
#include "RunMain.h"
#include "ev3api.h"
#include "app.h"
extern FILE *bt;

#define R_COURSE 1 //Rコース
#define L_COURSE 2 //Lコース

/**
 * コンストラクタ
 */
RunNormal::RunNormal(int bt_cmd)
{
	sts = 0;
	speed = 70;
	
	if(bt_cmd == 3){
		course = R_COURSE;
	}
	else if(bt_cmd == 4){
		course = L_COURSE;
	}else{
		is_error = 1;
	}
}

/**
*処理開始
*/
void RunNormal::run() {
	if(course == R_COURSE){
		R_Course_Run();
	}
	else if(course == L_COURSE){
		L_Course_Run();
	}
}

void RunNormal::R_Course_Run(){
	while(1){
		move(speed);
		switch(sts){
		case 1:
			//スタートしてから左に曲がり始めたら
			sts = 2;
			break;
		case 2:
			//真っ直ぐに戻り始めたら
			sts = 3;
			break;
		case 3:
			//真っ直ぐ進んでいる途中で左に曲がり始めたら
			sts = 4;
			break;
		case 4:
			//真っ直ぐに戻り始めたら
			sts = 5;
			break;
		case 5:
			//真っ直ぐ進んでいる途中で右に曲がり始めたら
			sts = 6;
			break;
		case 6:
			//真っ直ぐに戻り始めたら
			sts = 7;
			break;
		case 7:
			//距離、秒数が一定の値に達し、左に曲がり始めたら(左カーブのみ)
			sts = 8;
			break;
		case 8:
			//真っ直ぐに戻り始め、灰色を検知したら
			sts = 9;
			break;
		case 9:
			//シーソーモードに切り替わる
			runmode =  SEESAW_RUNMODE;
			break;
		}
	}
}

void RunNormal::L_Course_Run(){
	while(1){
		switch(sts){
		case 1:
			//スタートしてから左に曲がり始めたら 
			sts = 2;
			break;
		case 2:
			//真っ直ぐに戻り始めたら
			sts = 3;
			break;
		case 3:
			//真っ直ぐ進んでいる途中で右に曲がり始めたら
			sts = 4;
			break;
		case 4:
			//真っ直ぐに戻り始めたら
			sts = 5;
			break;
		case 5:
			//真っ直ぐ進んでいる途中で右に曲がり始めたら
			sts = 6;
			break;
		case 6:
			//真っ直ぐに戻り始めたら
			sts = 7;
			break;
		case 7:
			//真っ直ぐ進んでいる途中出に左に曲がり始めたら
			sts = 8;
			break;
		case 8:
			//真っ直ぐに戻り始め、灰色を検知したら
			sts = 9;
			break;
		case 9:
			//ゲートモードに切り替わる
			runmode = GATE_RUNMODE;
		break;
		}
	}
}
