#ifndef __WINDOWCONTROLLERTASK__
#define __WINDOWCONTROLLERTASK__

#include <Arduino.h>
#include "Button.h"
#include "ServoMotor.h"
#include "Potentiometer.h"
#include "MsgService.h"
#include "Lcd.h"

class WindowControllerTask{
  int button_pin;
  int pot_pin;
  int servo_pin;

  enum {automatic, manual} mode;
  enum {Automatic, Manual} lastMode;

  Button* btn;
  ServoMotor* window;
  Potentiometer* potentiometer;
  Lcd* lcd;

  int lastPercentage;
  int percentage;
  int temperature ;
  int lastPotPercentage;
  int lastTemperature;
  bool isButtonAlreadyPressed;

  String tag;
  String value;
  String percentMsg;

  void updateWindowFromSerial();
  void sendPercentageToSerial();
  void getDataFromSerial();
  void changeModeOnButtonPress();
  void updateTemperature();
  void updateWindowFromPotentiometer();
  void goInManualMode();
  void goInAutomaticMode();
  

 public:
  WindowControllerTask(int button_pin, int pot_pin, int servo_pin);
  void init();
  void tick();
};

#endif