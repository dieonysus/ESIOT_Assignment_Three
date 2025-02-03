#include "Button.h"
#include <Arduino.h>

Button::Button(int pin, unsigned long debounceTime){
  this->pin = pin;
  pinMode(pin, INPUT);
  buttonState = LOW;
  lastButtonState = LOW;
  lastPressTime = 0;
  this->debounceTime = debounceTime ;
  
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