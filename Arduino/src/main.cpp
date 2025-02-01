#include <Arduino.h>
#include "Button.h"
#include "ServoMotor.h"
#include "Potentiometer.h"
#include "MsgService.h"
#include "Lcd.h"

#define button_pin 2
#define pot_pin A0
#define servo_pin 10

int lastPercentage;
int percentage;
int temperature ;
int lastPotPercentage;
int lastTemperature;

Button* btn;
ServoMotor* window;
Potentiometer* potentiometer;
Lcd* lcd;

enum {automatic, manual} mode ;
enum {Automatic , Manual} lastMode;

String tag;
String value;
String percentMsg;

void setup() {
  // put your setup code here, to run once:
  MsgService.init();

  tag.reserve(10);
  value.reserve(50);
  percentMsg.reserve(30);

  percentage = 0;
  
  btn = new Button(button_pin,50);
  window = new ServoMotor(servo_pin);
  potentiometer = new Potentiometer(pot_pin);
  lcd = new Lcd(0x27,20,4);

  lcd->init();
  window->openPercentage(0);

  mode = automatic;
  lcd->updateMode("Automatic");
  lastMode = Automatic ;
  lcd->updatePercentage(percentage);
}

void controlWindow(){
   if(percentage != lastPercentage){
      lastPercentage = percentage;
      window->openPercentage(lastPercentage); 
      lcd->updatePercentage(lastPercentage);
    }
       
}


void loop() {
  
  // put your main code here, to run repeatedly:
  bool buttonIsPressed = btn->isPressed();

  if(MsgService.isMsgAvailable()){
    
    Msg* msg = MsgService.receiveMsg();
    
    tag = msg->getContent().substring(0,1);
    value = msg->getContent().substring(2);

    if(tag == "T"){
      temperature = value.toInt();
    }else if(tag == "M"){
      if(value == "automatic"){
        mode = automatic;
      }else{
        mode = manual;
      }
    }else if(tag == "P"){
      percentage = value.toInt();
    } 
    delete msg;  
    tag = "";
    value = ""; 
  }

    if(buttonIsPressed){
     if(mode == automatic){
      mode = manual;
     }else{
      mode = automatic;
    }
  }
      
  switch(mode){

    case automatic:
      //lcd->setCursorTo(0, 2);
      //lcd->print(" ");
      if(lastMode == Manual ){
        lastMode = Automatic ;
        lcd->updateMode("Automatic");
        lcd->updatePercentage(percentage);
        MsgService.sendMsg("M:automatic");
      }
      controlWindow();
      
    break;

    case manual:
    
     if(lastMode == Automatic){
      lastMode = Manual ;
      lcd->updateMode("Manual");
      lcd->updatePercentage(percentage);
      lcd->updateTemp(temperature);
      MsgService.sendMsg("M:manual");
     }
     if(temperature != lastTemperature){
       lastTemperature = temperature;
       lcd->updateTemp(temperature);
     }

      int potPercentage = potentiometer->percentageValue();

      if(potPercentage != lastPotPercentage){
        lastPotPercentage = potPercentage;
        lcd->updatePercentage(lastPotPercentage);
        window->openPercentage(lastPotPercentage);
        percentMsg = "P:";
        percentMsg.concat(lastPotPercentage);
        MsgService.sendMsg(percentMsg);
        percentMsg = "";
        percentage = lastPotPercentage;
       }else{
        controlWindow();
      }
    break;
  }
  delay(50);
}