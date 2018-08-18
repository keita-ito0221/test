/**
 * PID.h
 */
#ifndef PID_H_
#define PID_H_
	
class PID
{
public:
	PID();
	int calcControllValue(int,int);

private:
	int math_limit(int,int,int);
	int diff;
	int m_d_now;
	int m_d_pre;
	int m_i_list[13];
	int m_num;
};

#endif
