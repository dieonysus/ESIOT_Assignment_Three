#include "Button.h"
#include <Arduino.h>

Button::Button(int pin,unsigned long debounceTime = 50){
  this->pin = pin;
  buttonState = LOW;
  lastButtonState = LOW;
  lastPressTime = 0;
  this->debounceTime = debounceTime ;
  pinMode(pin, INPUT);
}

bool Button::isPressed(){
  int reading = digitalRead(pin);
  unsigned long press_time = millis();

  if (reading != lastButtonState) {
        lastPressTime = press_time;
      }

  if ((press_time - lastPressTime) > debounceTime) {
        if (reading != buttonState) {
          buttonState = reading;
          if (buttonState == HIGH) {
            return true;
          }
        }
        lastButtonState = reading;
        return false;
      } 
}