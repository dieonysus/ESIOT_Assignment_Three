#include <Arduino.h>
#include "Button.h"
#include "ServoMotor.h"
#include "Potentiometer.h"
#include "MsgService.h"
#include "Lcd.h"

#define button_pin 2
#define pot_pin A0
#define servo_pin 10

int lastPercentage = 0;
int percentage = 0;
int temperature = 0;

Button* btn;
ServoMotor* window;
Potentiometer* potentiometer;
Lcd* lcd;

enum {automatic, manual} mode ;

void setup() {
  // put your setup code here, to run once:
  MsgService.init();
  
  btn = new Button(button_pin);
  window = new ServoMotor(servo_pin);
  potentiometer = new Potentiometer(pot_pin);
  lcd = new Lcd(0x27,20,4);
  lcd->init();

  window->openPercentage(0);
  mode = automatic;

  lcd->setCursorTo(3,0);
  lcd->print("Mode:");
  lcd->print(mode);
}


void loop() {
  
  // put your main code here, to run repeatedly:
  bool buttonIsPressed = btn->isPressed();
  if(MsgService.isMsgAvailable()){
    Msg* msg = MsgService.receiveMsg();
    String tag = msg->getContent().substring(0,1);
    String value = msg->getContent().substring(2);

    if(tag == "T"){
      temperature = value.toInt();

    }else if(tag == "M"){
      if(value == "automatic"){
        mode = automatic;
      }else if(value == "manual"){
        mode = manual;
      }else{
        lcd->print("Mode is not set properly");
      }

    }else if(tag == "P"){
      percentage = value.toInt();
    }else{
      Serial.println("I didn't understand!");
    } 
    delete msg;   
  }
      
  switch(mode){

    case automatic:
  
    if(buttonIsPressed){
      mode = manual;
      MsgService.sendMsg("M:manual");
      lcd->clearScreen();
    }else{
      lcd->setCursorTo(0, 2);
      lcd->print(" ");
      lcd->updateMode("Automatic"
      if(percentage != lastPercentage){
      lastPercentage = percentage;
      lcd->updatePercentage(lastPercentage);
      window->openPercentage(lastPercentage); 
    }
    }
    break;

    case manual:
    
    if(buttonIsPressed){
      mode = automatic;
      MsgService.sendMsg("M:automatic");
      lcd->clearScreen();
    }else{
      lcd->updateMode("Manual");
      lcd->updatePercentage(lastPercentage);
      lcd->updateTemp(temperature);
     
      int potPercentage = potentiometer->percentageValue();

      if(potPercentage != lastPercentage){
        lastPercentage = potPercentage;
        lcd->updatePercentage(lastPercentage);
        window->openPercentage(lastPercentage);
        String percentMsg = "P:" + String(lastPercentage);
        MsgService.sendMsg(percentMsg);
    }  
    }
    break;
  }
 delay(200);
}