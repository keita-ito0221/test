/**
 *Motor.h
 */
#ifndef MOTOR_H_
#define MOTOR_H_

#include "ev3api.h"
	
class Motor
{
public:
	Motor();
	void init();
	void reset(motor_port_t);
	void stop();
	int32_t getAngle(motor_port_t);
	int getAveAngle();
	void setMovedistance(int _movedistance);
	int getMovedistance();
	void tail_control();
	void tail_up(int up);
	void tail_down(int down);
	void tail_save();
	

	static const motor_port_t
		left_motor = EV3_PORT_C,
		right_motor = EV3_PORT_B,
		tail_motor = EV3_PORT_A;

private:
	int movedistance; //ˆÚ“®‹——£

};

#endif
