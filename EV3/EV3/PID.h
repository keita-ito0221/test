/**
 * PID.h
 */
#ifndef PID_H_
#define PID_H_

#define KP  0.80
#define KI  0.0405 
#define KD  0.038
	
class PID
{
public:
	PID();
	int calcControllValue(int);

private:
	int math_limit(int,int,int);
	int diff;
	int m_d_now;
	int m_d_pre;
	int m_i_list[13];
	int m_num;
};

#endif
