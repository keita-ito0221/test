/******************************************************************************
 *  LinetraceCpp.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class Linetrace
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_LinetraceCPP_H_
#define EV3_UNIT_LinetraceCPP_H_

#include "ev3api.h"

class Linetrace {
public:
  Linetrace();

  void init();
  void update_linetrace(int rwEnc, int lwEnc, int battery);
  void setCommand(int forward, int turn);
  int8_t getPwmRight();
  int8_t getPwmLeft();

private:
  int mForward;
  int mTurn;
  int8_t mRightPwm;
  int8_t mLeftPwm;

  void backlashCanceler(int& rwEnc, int& lwEnc);
};

#endif  // EV3_UNIT_LinetraceCPP_H_
