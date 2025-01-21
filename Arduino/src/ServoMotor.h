#ifndef _SERVOMOTOR_
#define _SERVOMOTOR_

#include <Servo.h>

class ServoMotor{
  public:
  ServoMotor(int pin);
  openPercentage(int percent);

  private:
  int pin;
  Servo motor;
};

#endif