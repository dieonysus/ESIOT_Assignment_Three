#include "Arduino.h"
#include "Potentiometer.h"

Potentiometer::Potentiometer(int pin){
  this->pin = pin;
}

Potentiometer::percentageValue(){
  int potValue = analogRead(pin);
  return map(potValue,0,1023,0,100);
}