#include <Arduino.h>
#include "Button.h"
#include "ServoMotor.h"
#include "Potentiometer.h"
#include "MsgService.h"
#include "Lcd.h"
#include "MemoryFree.h"

#define button_pin 2
#define pot_pin A0
#define servo_pin 10

int lastPercentage;
int percentage;
int temperature ;
int lastPotPercentage;

Button* btn;
ServoMotor* window;
Potentiometer* potentiometer;
Lcd* lcd;

enum {automatic, manual} mode ;

String tag;
String value;
String percentMsg;

void setup() {
  // put your setup code here, to run once:
  MsgService.init();
  
  Serial.begin(9600);

  tag.reserve(10);
  value.reserve(50);
  percentMsg.reserve(30);
  
  btn = new Button(button_pin,20);
  window = new ServoMotor(servo_pin);
  potentiometer = new Potentiometer(pot_pin);
  lcd = new Lcd(0x27,20,4);

  lcd->init();
  window->openPercentage(0);

  mode = automatic;
  lcd->updateMode("Automatic");
}

void controlWindow(){
   if(percentage != lastPercentage){
      lastPercentage = percentage;
      window->openPercentage(lastPercentage); 
    }
    lcd->updatePercentage(lastPercentage);
    
}


void loop() {
  
  // put your main code here, to run repeatedly:
  bool buttonIsPressed = btn->isPressed();
  if(MsgService.isMsgAvailable()){
    
    Msg* msg = MsgService.receiveMsg();
    
    tag = msg->getContent().substring(0,1);
    value = msg->getContent().substring(2);
    Serial.print("From arduino: ");
    Serial.print(tag);
    Serial.println(value);
   

    if(tag == "T"){
      temperature = value.toInt();
    }else if(tag == "M"){
      if(value == "automatic"){
        mode = automatic;
      }else if(value == "manual"){
        mode = manual;
      }else{
        //Serial.println("Mode is not set properly");
      }
    }else if(tag == "P"){
      percentage = value.toInt();
    }else{
      //Serial.println("I didn't understand!");
    } 
    delete msg;  
    tag = "";
    value = ""; 
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
      lcd->updateMode("Automatic");
      lcd->updatePercentage(percentage);
      controlWindow();
    }
    break;

    case manual:
    
    if(buttonIsPressed){
      mode = automatic;
      MsgService.sendMsg("M:automatic");
      lcd->clearScreen();
    }else{
     lcd->updateMode("Manual");
     lcd->updateTemp(temperature);
     
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
    }
    break;
  }
  lcd->setCursorTo(1,1);
  lcd->print(freeMemory());
  delay(10);
}