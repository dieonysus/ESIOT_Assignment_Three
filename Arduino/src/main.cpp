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

enum {automatic, manual} state ;

void setup() {
  // put your setup code here, to run once:
  MsgService.init();
  
  btn = new Button(button_pin);
  window = new ServoMotor(servo_pin);
  potentiometer = new Potentiometer(pot_pin);
  lcd = new Lcd(0x27,20,4);
  lcd->init();

  window->openPercentage(0);
  state = automatic;

  lcd->setCursorTo(3,0);
  lcd->print("state:Automatic");
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
        state = automatic;
      }else if(value == "manual"){
        state = manual;
      }else{
        lcd->print("State is not set properly");
      }

    }else if(tag == "P"){
      percentage = value.toInt();
    }else{
      Serial.println("I didn't understand!");
    }    
  }
      
  switch(state){

    case automatic:
    lcd->updateState("Automatic"); 
    lcd->updatePercentage(percentage); 
    MsgService.sendMsg("S:automatic");

    if(buttonIsPressed){
      state = manual;
    }else{
      lcd->setCursorTo(0, 2);
      lcd->print(" ");
      if(percentage != lastPercentage){
      lastPercentage = percentage;
      lcd->updatePercentage(lastPercentage);
      window->openPercentage(lastPercentage);
    }
    }
    break;

    case manual:
    
    if(buttonIsPressed){
      state = automatic;
      lcd->clearScreen();
    }else{
      lcd->updateState("Manual");
      lcd->updatePercentage(lastPercentage);
      lcd->updateTemp(temperature);
      MsgService.sendMsg("S:manual");
      
      int potPercentage = potentiometer->percentageValue();

      if(potPercentage != lastPercentage){
        lastPercentage = potPercentage;
        lcd->updatePercentage(lastPercentage);
        window->openPercentage(lastPercentage);
        String percentMsg = "P:" + String(lastPercentage);
        MsgService.sendMsg(percentMsg);
    }   
  }
  }
 delay(200);
}