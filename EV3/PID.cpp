/**
 * PID.cpp
 * PID制御を行うクラス
 */

#include "PID.h"
#include <cstdlib>

struct Linetrace { // 配列番号を速さとする。
	float KP;
	float KI;
	float KD;
};

/**
 * コンストラクタ
 */
PID::PID()
{
}

int PID::calcControllValue(int diff, int speed) {
	float d_control = 0;
	float p_control = 0;
	float i_control = 0;
	int pid_total = 0;
	int total_i = 0;
	
	//moveにマイナスの値が入ったとき絶対値をとる
	speed = abs(speed);
	
Linetrace linetrace[101] =  // 配列番号を速さとする。
{   {0, 0, 0},		// 0 
	{0, 0, 0},		// 1
	{0, 0, 0},		// 2 
	{0, 0, 0},		// 3 
	{0, 0, 0},		// 4 
	{0, 0, 0},		// 5 
	{0, 0, 0},		// 6 
	{0, 0, 0},		// 7 
	{0, 0, 0},		// 8 
	{0, 0, 0},		// 9 
	{0, 0, 0},		// 10
	{0, 0, 0},		// 11 
	{0, 0, 0},		// 12
	{0, 0, 0},		// 13
	{0, 0, 0},		// 14
	{0, 0, 0},		// 15
	{0, 0, 0},		// 16
	{0, 0, 0},		// 17
	{0, 0, 0},		// 18
	{0, 0, 0},		// 19
	{0, 0, 0},		// 20
	{0, 0, 0},		// 21
	{0, 0, 0},		// 22 
	{0, 0, 0},		// 23 
	{0, 0, 0},		// 24 
	{0, 0, 0},		// 25 
	{0, 0, 0},		// 26 
	{0, 0, 0},		// 27 
	{0, 0, 0},		// 28 
	{0, 0, 0},		// 29 
	{0.03, 0.025, 0.0175}, // 30 (調整済み）
	{0, 0, 0},		// 31
	{0, 0, 0},		// 32 
	{0, 0, 0},		// 33 
	{0, 0, 0},		// 34 
	{0, 0, 0},		// 35 
	{0, 0, 0},		// 36 
	{0, 0, 0},		// 37 
	{0, 0, 0},		// 38 
	{0, 0, 0},		// 39 
	{0, 0, 0},		// 40
	{0, 0, 0},		// 41
	{0, 0, 0},		// 42 
	{0, 0, 0},		// 43 
	{0, 0, 0},		// 44 
	{0, 0, 0},		// 45 
	{0, 0, 0},		// 46 
	{0, 0, 0},		// 47 
	{0, 0, 0},		// 48 
	{0, 0, 0},		// 49 
	{0.7, 0, 0.0005},		// 50(調整済み)
	{0, 0, 0},		// 51
	{0, 0, 0},		// 52 
	{0, 0, 0},		// 53 
	{0, 0, 0},		// 54 
	{0, 0, 0},		// 55 
	{0, 0, 0},		// 56 
	{0, 0, 0},		// 57 
	{0, 0, 0},		// 58 
	{0, 0, 0},		// 59 
	{0.73, 0.0005, 0.003}, // 60(調整済み)0.73, 0.0005, 0.003
	{0, 0, 0},		// 61
	{0, 0, 0},		// 62 
	{0, 0, 0},		// 63 
	{0, 0, 0},		// 64 
	{0, 0, 0},		// 65 
	{0, 0, 0},		// 66 
	{0, 0, 0},		// 67 
	{0, 0, 0},		// 68 
	{0, 0, 0},		// 69 
	{0.7, 0, 0},		// 70
	{0, 0, 0},		// 71
	{0, 0, 0},		// 72 
	{0, 0, 0},		// 73 
	{0, 0, 0},		// 74 
	{0, 0, 0},		// 75 
	{0, 0, 0},		// 76 
	{0, 0, 0},		// 77 
	{0, 0, 0},		// 78 
	{0, 0, 0},		// 79 
	{0, 0, 0},		// 80
	{0, 0, 0},		// 81
	{0, 0, 0},		// 82 
	{0, 0, 0},		// 83 
	{0, 0, 0},		// 84 
	{0, 0, 0},		// 85 
	{0, 0, 0},		// 86 
	{0, 0, 0},		// 87 
	{0, 0, 0},		// 88 
	{0, 0, 0},		// 89 
	{0, 0, 0},		// 90
	{0, 0, 0},		// 91
	{0, 0, 0},		// 92 
	{0, 0, 0},		// 93 
	{0, 0, 0},		// 94 
	{0, 0, 0},		// 95 
	{0, 0, 0},		// 96 
	{0, 0, 0},		// 97 
	{0, 0, 0},		// 98 
	{0, 0, 0},		// 99 
	{0.5, 0, 0},		// 100 
};


	//P制御計算
//	p_control = diff * KP;
	p_control = diff * linetrace[speed].KP;
	
	//D制御計算
	m_d_pre = m_d_now;
	m_d_now = diff;
	//d_control = (m_d_pre - m_d_now) * KD;
	d_control = (m_d_pre - m_d_now) * linetrace[speed].KD;

	//I制御計算
	if(m_num>=13) {
		m_num = 0;
	}
	m_i_list[m_num] = diff;
	for(int i=0; i<13; i++) {
		total_i += m_i_list[i];
	}
	m_num ++;
	//i_control = total_i * KI;
	i_control = total_i * linetrace[speed].KI;

	//PID算出値
	pid_total = static_cast<int>(p_control + i_control + d_control);
	pid_total = math_limit(pid_total,-100,100);
	return pid_total;
}

int PID::math_limit(int pid,int min,int max){
	int result;
	result = pid;
	if (max < pid) {
		result = max;
	} else if (pid < min) {
		result = min;
	}
	return result;
}

