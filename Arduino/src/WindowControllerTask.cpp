#include "Arduino.h"
#include "WindowControllerTask.h"

WindowControllerTask::WindowControllerTask(int button_pin, int pot_pin, int servo_pin){
  this->button_pin = button_pin;
  this->pot_pin = pot_pin;
  this->servo_pin = servo_pin;
}

void WindowControllerTask::init(){  
  MsgService.init();
  tag.reserve(10);
  value.reserve(50);
  percentMsg.reserve(30);
  percentage = 0;

  btn = new Button(button_pin);
  window = new ServoMotor(servo_pin);
  potentiometer = new Potentiometer(pot_pin);
  lcd = new Lcd(0x27,20,4);

  lcd->init();
  window->openPercentage(0);
  goInAutomaticMode();

  isButtonAlreadyPressed = false;
}


void WindowControllerTask::tick(){
 changeModeOnButtonPress();
 getDataFromSerial();

 switch(mode){

  case automatic:
    updateWindowFromSerial();
    break;

  case manual:
    updateTemperature();

    int potPercentage = potentiometer->getPercentageValue();
    if(potPercentage != lastPotPercentage){
     lastPotPercentage = potPercentage;
     updateWindowFromPotentiometer();
     sendPercentageToSerial();
    }else{
     updateWindowFromSerial();
    }
    break;
  }
}

void WindowControllerTask::updateWindowFromSerial(){
 if(percentage != lastPercentage){
  lastPercentage = percentage;
  window->openPercentage(lastPercentage); 
  lcd->updatePercentage(lastPercentage);
 }     
}

void WindowControllerTask::sendPercentageToSerial(){
  percentMsg = "P:";
  percentMsg.concat(lastPotPercentage);
  MsgService.sendMsg(percentMsg);
  percentMsg = "";
}

void WindowControllerTask::getDataFromSerial(){
 if(MsgService.isMsgAvailable()){   
   Msg* msg = MsgService.receiveMsg();
   tag = msg->getContent().substring(0,1);
   value = msg->getContent().substring(2);

  if(tag == "T"){
   temperature = value.toInt();
  }else if(tag == "M"){
    if(value == "automatic"){
      goInAutomaticMode();
    }else{
      goInManualMode();
    }
    }else if(tag == "P"){
      percentage = value.toInt();
    } 
  delete msg;  
  tag = "";
  value = ""; 
  }
}

void WindowControllerTask::changeModeOnButtonPress(){
 bool buttonIsPressed = btn->isPressed();
 if(buttonIsPressed && !isButtonAlreadyPressed){
  if(mode == automatic){
   goInManualMode();
  }else{
   goInAutomaticMode();
  }
   isButtonAlreadyPressed = true;
  }else if(!buttonIsPressed){
   isButtonAlreadyPressed = false;
  }
}

void WindowControllerTask::updateTemperature(){
 if(temperature != lastTemperature){
  lastTemperature = temperature;
  lcd->updateTemp(temperature);
 }
}

void WindowControllerTask::updateWindowFromPotentiometer(){
  lcd->updatePercentage(lastPotPercentage);
  window->openPercentage(lastPotPercentage);
  percentage = lastPotPercentage;
}
 
void WindowControllerTask::goInManualMode(){
  lcd->updateMode("Manual");
  lcd->updatePercentage(percentage);
  lcd->updateTemp(temperature);
  MsgService.sendMsg("M:manual");
  mode = manual;
}

void WindowControllerTask::goInAutomaticMode(){
  lcd->updateMode("Automatic");
  lcd->updatePercentage(percentage);
  MsgService.sendMsg("M:automatic");
  mode=automatic;
}