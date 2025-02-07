#include "ServoMotor.h"
#include "Arduino.h"

ServoMotor::ServoMotor(int pin){
 this->pin = pin;
}

void ServoMotor::openPercentage(int percent){
  int angle = map(percent,0,100,0,90);
  motor.attach(pin);
  motor.write(angle);
  delay(500);
  motor.detach();
}